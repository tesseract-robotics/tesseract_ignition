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
#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/entity_manager.h>
#include <tesseract_ignition/conversions.h>
#include <tesseract/tesseract.h>
#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_scene_graph/resource_locator.h>
#include <tesseract_environment/kdl/kdl_env.h>
#include <memory>
#include <QMetaObject>
#include <boost/algorithm/string/split.hpp>

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr);

const QString CHAIN_GROUP = "Chain";
const QString JOINT_LIST_GROUP = "Joint List";
const QString LINK_LIST_GROUP = "Link List";

static std::unordered_map<std::string, std::string> cache_package_paths;

namespace tesseract_ignition::gui::plugins
{

// QML Data Model Names
// acmModel - Allowed Collision Matrix
// kinematicGroupsModel - KinematicGroupModel
//

class TesseractSetupWizardPrivate
{
  public:

  /// \brief Flag that indicates whether there are new updates to be rendered.
  bool load_environment{false};
  bool update_transforms {false};

  std::string urdf_filepath;

  std::string srdf_filepath;

  std::string engine_name {"ogre"};

  std::string scene_name {"scene"};

  EntityManager entity_manager;

  tesseract_scene_graph::ResourceLocator::Ptr locator;

  tesseract::Tesseract::Ptr thor;

  ignition::rendering::ScenePtr scene {nullptr};

  std::vector<ignition::rendering::AxisVisualPtr> axes;

  std::vector<ignition::rendering::VisualPtr> visual;

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

std::string locateResource(const std::string& url)
{
  std::string mod_url = url;
  if (url.find("package://tesseract_support") == 0)
  {
    mod_url.erase(0, strlen("package://tesseract_support"));
    size_t pos = mod_url.find('/');
    if (pos == std::string::npos)
    {
      return std::string();
    }

    std::string package = mod_url.substr(0, pos);
    mod_url.erase(0, pos);
    std::string package_path = std::string(TESSERACT_SUPPORT_DIR);

    if (package_path.empty())
    {
      return std::string();
    }

    mod_url = package_path + mod_url;
  }
  else if (url.find("file:///") == 0)
  {
    mod_url.erase(0, strlen("file://"));
    size_t pos = mod_url.find('/');
    if (pos == std::string::npos)
    {
      return std::string();
    }
  }
  else if (url.find("package://") == 0)
  {
    mod_url.erase(0, strlen("package://"));
    size_t pos = mod_url.find('/');
    if (pos == std::string::npos)
    {
      return std::string();
    }

    std::string package = mod_url.substr(0, pos);
    mod_url.erase(0, pos);

    if (cache_package_paths.empty())
    {
      char* ros_package_paths = std::getenv("ROS_PACKAGE_PATH");
      std::vector<std::string> tokens;
      boost::split(tokens, ros_package_paths, boost::is_any_of(":"), boost::token_compress_on);
      for (const auto& token : tokens)
      {
        QDir d(QString::fromStdString(token));
        if (d.exists())
          cache_package_paths[d.dirName().toStdString()] = token;
      }
    }
    auto find_package = cache_package_paths.find(package);

    if (find_package != cache_package_paths.end())
      mod_url = find_package->second + mod_url;
    else
      return std::string();
  }

  return mod_url;
}

tesseract_scene_graph::SceneGraph::Ptr getSceneGraph(std::string path)
{
  tesseract_scene_graph::ResourceLocator::Ptr locator =
      std::make_shared<tesseract_scene_graph::SimpleResourceLocator>(locateResource);
  return tesseract_urdf::parseURDFFile(locateResource(path), locator);
}


using namespace tesseract_ignition;
using namespace gui;
using namespace plugins;

/////////////////////////////////////////////////
TesseractSetupWizard::TesseractSetupWizard()
  : ignition::gui::Plugin()
  , data_(new TesseractSetupWizardPrivate)
{
  data_->thor = std::make_shared<tesseract::Tesseract>();

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
  this->data_->locator = std::make_shared<tesseract_scene_graph::SimpleResourceLocator>(locateResource);
  this->data_->urdf_filepath = this->data_->locator->locateResource(urdf_filepath.toStdString())->getFilePath();
  if (srdf_filepath.toStdString().empty())
  {
    this->data_->thor->init(boost::filesystem::path(this->data_->urdf_filepath), this->data_->locator);
  }
  else
  {
    this->data_->srdf_filepath = this->data_->locator->locateResource(srdf_filepath.toStdString())->getFilePath();
    this->data_->thor->init(boost::filesystem::path(this->data_->urdf_filepath),
                           boost::filesystem::path(this->data_->srdf_filepath),
                           this->data_->locator);
  }

  if (this->data_->thor->isInitialized())
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
    const std::vector<std::string>& link_names = this->data_->thor->getEnvironmentConst()->getLinkNames();
    QStringList links;
    links.reserve(static_cast<int>(link_names.size()));
    for (const auto& link_name : link_names)
      links.push_back(QString::fromStdString(link_name));

    this->data_->link_model.setStringList(links);
    this->data_->link_model.sort(0);

    // Build Joint list model
    for (const auto& joint : this->data_->thor->getEnvironmentConst()->getSceneGraph()->getJoints())
    {
      if (joint->type == tesseract_scene_graph::JointType::REVOLUTE || joint->type == tesseract_scene_graph::JointType::PRISMATIC)
      {
        this->data_->joint_model.add(joint);
      }
    }
    this->data_->joint_model.sort(0);

    // Build ACM Model
    this->data_->acm_model.setTesseract(this->data_->thor);

    // Build Kinematic Groups Model
    this->data_->kin_groups_model.setTesseract(this->data_->thor);

    // Build Groups Joint States Model
    this->data_->user_joint_states_model.setTesseract(this->data_->thor);

    // Build Groups TCPs Model
    this->data_->user_tcp_model.setTesseract(this->data_->thor);

    // Build OPW kinematics Model
    this->data_->opw_kinematics_model.setTesseract(this->data_->thor);
  }

  this->data_->load_environment = true;
}

Q_INVOKABLE void TesseractSetupWizard::onSave(const QString& srdf_filepath)
{
  std::string local_path = this->data_->locator->locateResource(srdf_filepath.toStdString())->getFilePath();
  this->data_->thor->getSRDFModelConst()->saveToFile(local_path);
}

void TesseractSetupWizard::onAddChainGroup(const QString &group_name, const QString& base_link, const QString& tip_link)
{
  // If group name is empty then return
  if (group_name.trimmed().isEmpty())
    return;

  QStringList list = {base_link, tip_link};
  std::vector<std::string> groups = this->data_->thor->getFwdKinematicsManager()->getAvailableFwdKinematicsManipulators();
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

  std::vector<std::string> groups = this->data_->thor->getFwdKinematicsManager()->getAvailableFwdKinematicsManipulators();
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

  std::vector<std::string> groups = this->data_->thor->getFwdKinematicsManager()->getAvailableFwdKinematicsManipulators();
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
  CONSOLE_BRIDGE_logError("onAddJointGroup");
  QStringList l = this->data_->group_joint_list_model.stringList();
  if (!l.contains(joint_name) && !joint_name.isEmpty())
  {
    l.append(joint_name);
    this->data_->group_joint_list_model.setStringList(l);
  }
}

void TesseractSetupWizard::onRemoveJointGroupJoint(int index)
{
  CONSOLE_BRIDGE_logError("onRemoveJointGroupJoint");
  this->data_->group_joint_list_model.removeRow(index);
}

void TesseractSetupWizard::onAddLinkGroupLink(const QString &link_name)
{
  CONSOLE_BRIDGE_logError("onAddLinkGroupLink");
  QStringList l = this->data_->group_link_list_model.stringList();
  if (!l.contains(link_name) && !link_name.isEmpty())
  {
    l.append(link_name);
    this->data_->group_link_list_model.setStringList(l);
  }
}

void TesseractSetupWizard::onRemoveLinkGroupLink(int index)
{
  CONSOLE_BRIDGE_logError("onRemoveLinkGroupLink");
  this->data_->group_link_list_model.removeRow(index);
}

void TesseractSetupWizard::onRemoveKinematicGroup(int index)
{
  CONSOLE_BRIDGE_logError("onRemoveKinematicGroup");
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
  CONSOLE_BRIDGE_logError("onGenerateACM");
  auto& env = this->data_->thor->getEnvironment();
  auto& srdf = this->data_->thor->getSRDFModel();
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
        srdf->getAllowedCollisionMatrix().addAllowedCollision(pair.first.first, pair.first.second, "Adjacent");
      }
      else if (std::find(adj_second.begin(), adj_second.end(), pair.first.first) != adj_second.end())
      {
        env->addAllowedCollision(pair.first.second, pair.first.first, "Adjacent");
        srdf->getAllowedCollisionMatrix().addAllowedCollision(pair.first.first, pair.first.second, "Adjacent");
      }
      else
      {
        env->addAllowedCollision(pair.first.second, pair.first.first, "Allways");
        srdf->getAllowedCollisionMatrix().addAllowedCollision(pair.first.first, pair.first.second, "Allways");
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
        srdf->getAllowedCollisionMatrix().addAllowedCollision(link_names[i], link_names[j], "Never");
      }
    }
  }

  this->data_->acm_model.setTesseract(this->data_->thor);
}

void TesseractSetupWizard::onRemoveACMEntry(int index)
{
  CONSOLE_BRIDGE_logError("onRemoveACMEntry");
  this->data_->acm_model.removeRow(index);
}

void TesseractSetupWizard::onLoadJointGroup(const QString &group_name)
{
  this->data_->joint_group_model.clear();
  auto kin = this->data_->thor->getFwdKinematicsManagerConst()->getFwdKinematicSolver(group_name.toStdString());
  if (kin != nullptr)
    for (const auto& joint_name : kin->getJointNames())
      this->data_->joint_group_model.add(this->data_->thor->getEnvironmentConst()->getSceneGraph()->getJoint(joint_name));
}

void TesseractSetupWizard::onJointValue(const QString &joint_name, double joint_value)
{
  CONSOLE_BRIDGE_logError("onJointValue");
  for (int i = 0; i < this->data_->joint_group_model.rowCount(); ++i)
  {
    QStandardItem* item = this->data_->joint_group_model.item(i);
    if (item->data(this->data_->joint_group_model.NameRole).toString() == joint_name)
    {
      item->setData(QString::number(joint_value), this->data_->joint_group_model.ValueRole);
      break;
    }
  }
  this->data_->thor->getEnvironment()->setState({joint_name.toStdString()}, {joint_value});
  this->data_->update_transforms = true;
}

void TesseractSetupWizard::onAddUserDefinedJointState(const QString &group_name, const QString &state_name)
{
  CONSOLE_BRIDGE_logError("onAddUserDefinedJointState");
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
  CONSOLE_BRIDGE_logError("onRemoveUserDefinedJointState");
  this->data_->user_joint_states_model.removeRow(index);
}

void TesseractSetupWizard::onAddUserDefinedTCP(const QString &group_name,
                                               const QString &tcp_name,
                                               const QVector3D& position,
                                               const QVector3D& orientation)
{
  CONSOLE_BRIDGE_logError("onAddUserDefinedTCP");
  if (!group_name.trimmed().isEmpty() && !tcp_name.trimmed().isEmpty())
    this->data_->user_tcp_model.add(group_name, tcp_name, position, orientation);

}

void TesseractSetupWizard::onRemoveUserDefinedTCP(int index)
{
  CONSOLE_BRIDGE_logError("onRemoveUserDefinedTCP");
  this->data_->user_tcp_model.removeRow(index);
}

void TesseractSetupWizard::onAddGroupOPWKinematics(const QString& group_name,
                                                   double a1, double a2, double b,
                                                   double c1, double c2, double c3, double c4,
                                                   double o1, double o2, double o3, double o4, double o5, double o6,
                                                   int sc1, int sc2, int sc3, int sc4, int sc5, int sc6)
{
  CONSOLE_BRIDGE_logError("onAddGroupOPWKinematics");
  if (!group_name.trimmed().isEmpty())
    this->data_->opw_kinematics_model.add(group_name, a1, a2, b, c1, c2, c3, c4, o1, o2, o3, o4, o5, o6, sc1, sc2, sc3, sc4, sc5, sc6);
}

void TesseractSetupWizard::onRemoveGroupOPWKinematics(int index)
{
  CONSOLE_BRIDGE_logError("onRemoveGroupOPWKinematics");
  this->data_->opw_kinematics_model.removeRow(index);
}

bool TesseractSetupWizard::eventFilter(QObject *_obj, QEvent *_event)
{
  if (_event->type() == tesseract_ignition::gui::events::Render::Type)
  {
    // This event is called in Scene3d's RenderThread, so it's safe to make
    // rendering calls here
    this->updateTesseract();
  }

  // Standard event processing
  return QObject::eventFilter(_obj, _event);
}

/////////////////////////////////////////////////
void TesseractSetupWizard::updateTesseract()
{
  if (this->data_->load_environment)
  {

    // create scene
    ignition::rendering::RenderEngine* engine = ignition::rendering::engine(this->data_->engine_name);
    if (!engine)
    {
      ignerr << "Engine " << this->data_->engine_name.c_str() << " is not supported" << std::endl;
      this->data_->load_environment = false;
      return;
    }

    // Scene
    if (!this->data_->scene)
    {
      this->data_->scene = engine->SceneByName(this->data_->scene_name);
      if (!this->data_->scene)
      {
        igndbg << "Failed to get scene [" << this->data_->scene_name << "]" << std::endl;
        if (engine->SceneCount() == 0)
        {
          this->data_->load_environment = false;
          return;
        }

        this->data_->scene = engine->SceneByIndex(0);
      }
    }
    else
    {
      this->data_->scene->Clear();
      this->data_->axes.clear();
    }

    this->data_->entity_manager.clear();

    // Load Ignition Scene
    const tesseract_environment::EnvState::ConstPtr& state = this->data_->thor->getEnvironment()->getCurrentState();
    toScene(*(this->data_->scene), this->data_->entity_manager, *(this->data_->thor->getEnvironmentConst()->getSceneGraph()), state->link_transforms);

    // Add axis to scene
    ignition::rendering::VisualPtr root = this->data_->scene->RootVisual();
    unsigned id = static_cast<unsigned>(this->data_->entity_manager.addVisual("world_axis"));
    ignition::rendering::AxisVisualPtr axis = this->data_->scene->CreateAxisVisual(id, "world_axis");
    axis->Scale(0.5, 0.5, 0.5);
    root->AddChild(axis);
    this->data_->axes.push_back(axis);

    this->data_->load_environment = false;
  }
  else if (this->data_->update_transforms)
  {
    const tesseract_environment::EnvState::ConstPtr& state = this->data_->thor->getEnvironment()->getCurrentState();
    for (const auto& link : state->link_transforms)
    {
      ignition::rendering::VisualPtr v = this->data_->scene->VisualByName(link.first);
      v->SetWorldPose(ignition::math::eigen3::convert(link.second));
    }
    this->data_->update_transforms = false;
  }
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
