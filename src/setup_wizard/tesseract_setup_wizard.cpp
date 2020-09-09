#include <iostream>
#include <ignition/plugin/Register.hh>
#include <ignition/common/Console.hh>
#include <ignition/plugin/Register.hh>
#include <ignition/math/Color.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/eigen3/Conversions.hh>
#include <ignition/common/MeshManager.hh>
#include <ignition/common/Mesh.hh>
#include <ignition/rendering.hh>
#include <ignition/gui/Application.hh>
#include <ignition/gui/MainWindow.hh>
#include <tesseract_ignition/setup_wizard/tesseract_setup_wizard.h>
#include <tesseract_ignition/setup_wizard/models/joint_list_model.h>
#include <tesseract_ignition/setup_wizard/models/allowed_collision_matrix_model.h>
#include <tesseract_ignition/setup_wizard/models/kinematic_groups_model.h>
#include <tesseract_ignition/setup_wizard/models/user_defined_joint_states_model.h>
#include <tesseract_ignition/setup_wizard/models/user_defined_tcp_model.h>
#include <tesseract_ignition/setup_wizard/models/opw_kinematics_model.h>
#include <tesseract_ignition/render_utils.h>
#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/conversions.h>
#include <tesseract_ignition/utils.h>
#include <tesseract/tesseract.h>
#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_scene_graph/resource_locator.h>
#include <tesseract_visualization/ignition/entity_manager.h>
#include <memory>
#include <QMetaObject>


Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr);

const QString CHAIN_GROUP = "Chain";
const QString JOINT_LIST_GROUP = "Joint List";
const QString LINK_LIST_GROUP = "Link List";

static std::unordered_map<std::string, std::string> cache_package_paths;

namespace tesseract_ignition::gui::plugins
{

class TesseractSetupWizardPrivate
{
public:

  /** @brief URDF file path */
  std::string urdf_filepath;

  /** @brief SRDF file path */
  std::string srdf_filepath;

  /** @brief The Tesseract resource locator */
  tesseract_scene_graph::ResourceLocator::Ptr locator;

  tesseract_ignition::RenderUtil render_util;

  QStringListModel link_model;

  JointListModel joint_model;

  AllowedCollisionMatrixModel acm_model;

  KinematicGroupsModel kin_groups_model;

  UserDefinedJointStatesModel user_joint_states_model;

  UserDefinedTCPModel user_tcp_model;

  OPWKinematicsModel opw_kinematics_model;

  JointListModel joint_group_model;

  QStringListModel group_link_list_model;

  QStringListModel group_joint_list_model;
};
}

using namespace tesseract_ignition;
using namespace gui;
using namespace plugins;

/////////////////////////////////////////////////
TesseractSetupWizard::TesseractSetupWizard()
  : ignition::gui::Plugin()
  , data_(new TesseractSetupWizardPrivate)
{
  // Connect model
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("linkModel", &this->data_->link_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("jointModel", &this->data_->joint_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("acmModel", &this->data_->acm_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("kinematicGroupsModel", &this->data_->kin_groups_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("userDefinedJointStatesModel", &this->data_->user_joint_states_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("userDefinedTCPModel", &this->data_->user_tcp_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("jointGroupModel", &this->data_->joint_group_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("linkListViewModel", &this->data_->group_link_list_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("jointListViewModel", &this->data_->group_joint_list_model);
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("opwKinematicsModel", &this->data_->opw_kinematics_model);
}

/////////////////////////////////////////////////
TesseractSetupWizard::~TesseractSetupWizard()
{
}

/////////////////////////////////////////////////
void TesseractSetupWizard::LoadConfig( const tinyxml2::XMLElement * /*_pluginElem*/)
{
  if (this->title.empty())
    this->title = "Tesseract Setup Wizard";

  ignition::gui::App()->findChild<ignition::gui::MainWindow *>()->installEventFilter(this);
}

/////////////////////////////////////////////////
void TesseractSetupWizard::onLoad(const QString &urdf_filepath, const QString& srdf_filepath)
{
  auto thor = std::make_shared<tesseract::Tesseract>();
  this->data_->locator = std::make_shared<tesseract_scene_graph::SimpleResourceLocator>(tesseract_ignition::locateResource);
  this->data_->urdf_filepath = this->data_->locator->locateResource(urdf_filepath.toStdString())->getFilePath();
  bool loaded = false;
  if (srdf_filepath.toStdString().empty())
  {
    loaded = thor->init(boost::filesystem::path(this->data_->urdf_filepath), this->data_->locator);
  }
  else
  {
    this->data_->srdf_filepath = this->data_->locator->locateResource(srdf_filepath.toStdString())->getFilePath();
    loaded = thor->init(boost::filesystem::path(this->data_->urdf_filepath), boost::filesystem::path(this->data_->srdf_filepath), this->data_->locator);
  }

  if (!loaded)
  {
    ignerr << "Failed to parse URDF/SRDF!" << std::endl;
    return;
  }

  this->data_->render_util.setTesseract(thor);

  if (this->data_->render_util.getTesseract())
  {
    // Clear Models
    this->data_->joint_model.clear();
    this->data_->link_model.setStringList(QStringList());
    this->data_->acm_model.clear();
    this->data_->kin_groups_model.clear();
    this->data_->group_link_list_model.setStringList(QStringList());
    this->data_->group_joint_list_model.setStringList(QStringList());
    this->data_->joint_group_model.clear();
    this->data_->user_joint_states_model.clear();
    this->data_->user_tcp_model.clear();
    this->data_->opw_kinematics_model.clear();

    // Build link list model
    const std::vector<std::string>& link_names = this->data_->render_util.getTesseractConst()->getEnvironment()->getLinkNames();
    QStringList links;
    links.reserve(static_cast<int>(link_names.size()));
    for (const auto& link_name : link_names)
      links.push_back(QString::fromStdString(link_name));

    this->data_->link_model.setStringList(links);
    this->data_->link_model.sort(0);

    // Build Joint list model
    for (const auto& joint : this->data_->render_util.getTesseractConst()->getEnvironment()->getSceneGraph()->getJoints())
    {
      if (joint->type == tesseract_scene_graph::JointType::REVOLUTE || joint->type == tesseract_scene_graph::JointType::PRISMATIC)
      {
        this->data_->joint_model.add(joint);
      }
    }
    this->data_->joint_model.sort(0);

    // Build ACM Model
    this->data_->acm_model.setTesseract(this->data_->render_util.getTesseract());

    // Build Kinematic Groups Model
    this->data_->kin_groups_model.setTesseract(this->data_->render_util.getTesseract());

    // Build Groups Joint States Model
    this->data_->user_joint_states_model.setTesseract(this->data_->render_util.getTesseract());

    // Build Groups TCPs Model
    this->data_->user_tcp_model.setTesseract(this->data_->render_util.getTesseract());

    // Build OPW kinematics Model
    this->data_->opw_kinematics_model.setTesseract(this->data_->render_util.getTesseract());
  }
}

void TesseractSetupWizard::onSave(const QString& srdf_filepath)
{
  std::string local_path = this->data_->locator->locateResource(srdf_filepath.toStdString())->getFilePath();
  this->data_->render_util.getTesseract()->getManipulatorManager()->getSRDFModel()->saveToFile(local_path);
}

void TesseractSetupWizard::onAddChainGroup(const QString &group_name, const QString& base_link, const QString& tip_link)
{
  // If group name is empty then return
  if (group_name.trimmed().isEmpty())
    return;

  QStringList list = {base_link, tip_link};
  std::vector<std::string> groups = this->data_->render_util.getTesseract()->getManipulatorManager()->getAvailableFwdKinematicsManipulators();
  this->data_->kin_groups_model.add(group_name, CHAIN_GROUP, list);
  if (!group_name.isEmpty())
  {
    if (std::find(groups.begin(), groups.end(), group_name.toStdString()) != groups.end())
    {
      // Remove Group States, TCPs and OPW Kinematics associated with the group
      removeGroupStates(group_name);
      removeGroupTCPs(group_name);
      removeGroupOPWKinematics(group_name);
    }
  }
}

void TesseractSetupWizard::onAddJointGroup(const QString &group_name)
{
  // If group name is empty then return
  if (group_name.trimmed().isEmpty())
    return;

  std::vector<std::string> groups = this->data_->render_util.getTesseract()->getManipulatorManager()->getAvailableFwdKinematicsManipulators();
  this->data_->kin_groups_model.add(group_name, JOINT_LIST_GROUP, this->data_->group_joint_list_model.stringList());
  if (!group_name.isEmpty())
  {
    if (std::find(groups.begin(), groups.end(), group_name.toStdString()) != groups.end())
    {
      // Remove Group States, TCPs and OPW Kinematics associated with the group
      removeGroupStates(group_name);
      removeGroupTCPs(group_name);
      removeGroupOPWKinematics(group_name);
    }
  }
}

void TesseractSetupWizard::onAddLinkGroup(const QString &group_name)
{
  // If group name is empty then return
  if (group_name.trimmed().isEmpty())
    return;

  std::vector<std::string> groups = this->data_->render_util.getTesseract()->getManipulatorManager()->getAvailableFwdKinematicsManipulators();
  this->data_->kin_groups_model.add(group_name, LINK_LIST_GROUP, this->data_->group_link_list_model.stringList());
  if (!group_name.isEmpty())
  {
    if (std::find(groups.begin(), groups.end(), group_name.toStdString()) != groups.end())
    {
      // Remove Group States, TCPs and OPW Kinematics associated with the group
      removeGroupStates(group_name);
      removeGroupTCPs(group_name);
      removeGroupOPWKinematics(group_name);
    }
  }
}

void TesseractSetupWizard::onAddJointGroupJoint(const QString &joint_name)
{
  QStringList l = this->data_->group_joint_list_model.stringList();
  if (!l.contains(joint_name) && !joint_name.isEmpty())
  {
    l.append(joint_name);
    this->data_->group_joint_list_model.setStringList(l);
  }
}

void TesseractSetupWizard::onRemoveJointGroupJoint(int index)
{
  this->data_->group_joint_list_model.removeRow(index);
}

void TesseractSetupWizard::onAddLinkGroupLink(const QString &link_name)
{
  QStringList l = this->data_->group_link_list_model.stringList();
  if (!l.contains(link_name) && !link_name.isEmpty())
  {
    l.append(link_name);
    this->data_->group_link_list_model.setStringList(l);
  }
}

void TesseractSetupWizard::onRemoveLinkGroupLink(int index)
{
  this->data_->group_link_list_model.removeRow(index);
}

void TesseractSetupWizard::onRemoveKinematicGroup(int index)
{
  if (index >= 0)
  {
    QString group_name = this->data_->kin_groups_model.item(index, 0)->data(this->data_->kin_groups_model.NameRole).toString();
    if (this->data_->kin_groups_model.removeRow(index))
    {
      // Remove Group States, TCPs and OPW Kinematics associated with the group
      removeGroupStates(group_name);
      removeGroupTCPs(group_name);
      removeGroupOPWKinematics(group_name);
    }
  }
}

void TesseractSetupWizard::onGenerateACM(long resolution)
{
  auto env = this->data_->render_util.getTesseract()->getEnvironment();
  auto manip_manager = this->data_->render_util.getTesseract()->getManipulatorManager();
  auto contact_manager = env->getDiscreteContactManager();
  auto state_solver = env->getStateSolver();

  // We want to disable the allowed contact function for this process so it is set null
  contact_manager->setIsContactAllowedFn(nullptr);
  tesseract_collision::ContactResultMap results;
  tesseract_collision::ContactRequest request;
  request.type = tesseract_collision::ContactTestType::ALL;

  for (long i = 0; i < resolution; ++i)
  {
    tesseract_environment::EnvState::Ptr state = state_solver->getRandomState();
    contact_manager->contactTest(results, request);
  }

  this->data_->acm_model.clear();
  for (const auto& pair : results)
  {
    double percent = double(pair.second.size()) / double(resolution);
    if (percent > 0.95)
    {
      std::vector<std::string> adj_first = env->getSceneGraph()->getAdjacentLinkNames(pair.first.first);
      std::vector<std::string> adj_second = env->getSceneGraph()->getAdjacentLinkNames(pair.first.second);
      if (std::find(adj_first.begin(), adj_first.end(), pair.first.second) != adj_first.end())
      {
        env->addAllowedCollision(pair.first.first, pair.first.second, "Adjacent");
        manip_manager->addAllowedCollision(pair.first.first, pair.first.second, "Adjacent");
      }
      else if (std::find(adj_second.begin(), adj_second.end(), pair.first.first) != adj_second.end())
      {
        env->addAllowedCollision(pair.first.second, pair.first.first, "Adjacent");
        manip_manager->addAllowedCollision(pair.first.first, pair.first.second, "Adjacent");
      }
      else
      {
        env->addAllowedCollision(pair.first.second, pair.first.first, "Allways");
        manip_manager->addAllowedCollision(pair.first.first, pair.first.second, "Allways");
      }
    }
  }

  std::vector<std::string> link_names = env->getLinkNames();
  for (std::size_t i = 0; i < link_names.size() - 1; ++i)
  {
    const auto& link1 = env->getLink(link_names[i]);
    if (link1->collision.empty())
      continue;

    for (std::size_t j = i + 1; j < link_names.size(); ++j)
    {
      const auto& link2 = env->getLink(link_names[j]);
      if (link2->collision.empty())
        continue;

      if (results.find(tesseract_collision::getObjectPairKey(link_names[i], link_names[j])) == results.end())
      {
        env->addAllowedCollision(link_names[i], link_names[j], "Never");
        manip_manager->addAllowedCollision(link_names[i], link_names[j], "Never");
      }
    }
  }

  this->data_->acm_model.setTesseract(this->data_->render_util.getTesseract());
}

void TesseractSetupWizard::onRemoveACMEntry(int index)
{
  this->data_->acm_model.removeRow(index);
}

void TesseractSetupWizard::onClickedACMEntry(int /*index*/)
{
//  this->data_->selected_entities.clear();
//  this->data_->selected_entities.push_back(this->data_->entity_manager.getLink("link_1"));
//  this->data_->update_selections = true;
}

void TesseractSetupWizard::onLoadJointGroup(const QString &group_name)
{
  this->data_->joint_group_model.clear();
  auto kin = this->data_->render_util.getTesseractConst()->getManipulatorManager()->getFwdKinematicSolver(group_name.toStdString());
  if (kin != nullptr)
    for (const auto& joint_name : kin->getJointNames())
      this->data_->joint_group_model.add(this->data_->render_util.getTesseractConst()->getEnvironment()->getSceneGraph()->getJoint(joint_name));
}

void TesseractSetupWizard::onJointValue(const QString &joint_name, double joint_value)
{
  for (int i = 0; i < this->data_->joint_group_model.rowCount(); ++i)
  {
    QStandardItem* item = this->data_->joint_group_model.item(i);
    if (item->data(this->data_->joint_group_model.NameRole).toString() == joint_name)
    {
      item->setData(QString::number(joint_value), this->data_->joint_group_model.ValueRole);
      break;
    }
  }
  this->data_->render_util.setTesseractState({joint_name.toStdString()}, {joint_value});
}

void TesseractSetupWizard::onAddUserDefinedJointState(const QString &group_name, const QString &state_name)
{
  if (!group_name.trimmed().isEmpty() && !state_name.trimmed().isEmpty())
  {
    QStringList joint_names;
    QStringList joint_values;
    for (int i = 0; i < this->data_->joint_group_model.rowCount(); ++i)
    {
      QStandardItem* item = this->data_->joint_group_model.item(i);
      joint_names.push_back(item->data(this->data_->joint_group_model.NameRole).toString());
      joint_values.push_back(item->data(this->data_->joint_group_model.ValueRole).toString());
    }
    this->data_->user_joint_states_model.add(group_name, state_name, joint_names, joint_values);
  }
}

void TesseractSetupWizard::onRemoveUserDefinedJointState(int index)
{
  this->data_->user_joint_states_model.removeRow(index);
}

void TesseractSetupWizard::onAddUserDefinedTCP(const QString &group_name,
                                               const QString &tcp_name,
                                               const QVector3D& position,
                                               const QVector3D& orientation)
{
  if (!group_name.trimmed().isEmpty() && !tcp_name.trimmed().isEmpty())
    this->data_->user_tcp_model.add(group_name, tcp_name, position, orientation);

}

void TesseractSetupWizard::onRemoveUserDefinedTCP(int index)
{
  this->data_->user_tcp_model.removeRow(index);
}

void TesseractSetupWizard::onAddGroupOPWKinematics(const QString& group_name,
                                                   double a1, double a2, double b,
                                                   double c1, double c2, double c3, double c4,
                                                   double o1, double o2, double o3, double o4, double o5, double o6,
                                                   int sc1, int sc2, int sc3, int sc4, int sc5, int sc6)
{
  if (!group_name.trimmed().isEmpty())
    this->data_->opw_kinematics_model.add(group_name, a1, a2, b, c1, c2, c3, c4, o1, o2, o3, o4, o5, o6, sc1, sc2, sc3, sc4, sc5, sc6);
}

void TesseractSetupWizard::onRemoveGroupOPWKinematics(int index)
{
  this->data_->opw_kinematics_model.removeRow(index);
}

bool TesseractSetupWizard::eventFilter(QObject *_obj, QEvent *_event)
{
  if (_event->type() == tesseract_ignition::gui::events::Render::Type)
  {
    // This event is called in Scene3d's RenderThread, so it's safe to make
    // rendering calls here
    if (!this->data_->render_util.isInitialized())
      this->data_->render_util.init();

    this->data_->render_util.update();
  }

  // Standard event processing
  return QObject::eventFilter(_obj, _event);
}

void TesseractSetupWizard::removeGroupStates(const QString& group_name)
{
  auto matches = this->data_->user_joint_states_model.match(this->data_->user_joint_states_model.index(0, 0), this->data_->user_joint_states_model.GroupNameRole, group_name, 1, Qt::MatchExactly);
  if (!matches.empty())
    this->data_->user_joint_states_model.removeRows(matches[0].row(), 1);
}

void TesseractSetupWizard::removeGroupTCPs(const QString& group_name)
{
  auto matches = this->data_->user_tcp_model.match(this->data_->user_tcp_model.index(0, 0), this->data_->user_tcp_model.GroupNameRole, group_name, 1, Qt::MatchExactly);
  if (!matches.empty())
    this->data_->user_tcp_model.removeRows(matches[0].row(), 1);
}

void TesseractSetupWizard::removeGroupOPWKinematics(const QString& group_name)
{
  auto matches = this->data_->opw_kinematics_model.match(this->data_->opw_kinematics_model.index(0, 0), this->data_->opw_kinematics_model.GroupNameRole, group_name, 1, Qt::MatchExactly);
  if (!matches.empty())
    this->data_->opw_kinematics_model.removeRows(matches[0].row(), 1);
}

// Register this plugin
IGNITION_ADD_PLUGIN(tesseract_ignition::gui::plugins::TesseractSetupWizard, ignition::gui::Plugin);
