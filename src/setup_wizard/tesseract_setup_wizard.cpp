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
#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/entity_manager.h>
#include <tesseract_ignition/conversions.h>
#include <tesseract/tesseract.h>
#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_scene_graph/resource_locator.h>
#include <tesseract_environment/kdl/kdl_env.h>
#include <memory>
#include <QMetaObject>

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr);

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

  JointListModel joint_group_model;
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
  ignition::gui::App()->Engine()->rootContext()->setContextProperty("jointGroupModel", &this->data_->joint_group_model);
}

/////////////////////////////////////////////////
TesseractSetupWizard::~TesseractSetupWizard()
{
}

/////////////////////////////////////////////////
void TesseractSetupWizard::LoadConfig( const tinyxml2::XMLElement * _pluginElem)
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
  this->data_->srdf_filepath = this->data_->locator->locateResource(srdf_filepath.toStdString())->getFilePath();
  this->data_->thor->init(boost::filesystem::path(this->data_->urdf_filepath),
                         boost::filesystem::path(this->data_->srdf_filepath),
                         this->data_->locator);

  if (this->data_->thor->isInitialized())
  {
    // Build link list model
    const std::vector<std::string>& link_names = this->data_->thor->getEnvironmentConst()->getLinkNames();
    QStringList links;
    links.reserve(static_cast<int>(link_names.size()));
    for (const auto& link_name : link_names)
      links.push_back(QString::fromStdString(link_name));

    this->data_->link_model.setStringList(links);

    // Build Joint list model
    for (const auto& joint : this->data_->thor->getEnvironmentConst()->getSceneGraph()->getJoints())
    {
      if (joint->type == tesseract_scene_graph::JointType::REVOLUTE || joint->type == tesseract_scene_graph::JointType::PRISMATIC)
      {
        this->data_->joint_model.add(joint);
      }
    }

    // Build ACM Model
    this->data_->acm_model.setEnvironment(this->data_->thor->getEnvironment());

    // Build Kinematic Groups Model
    this->data_->kin_groups_model.setTesseract(this->data_->thor);

    // Build Groups Joint States Model
    this->data_->user_joint_states_model.setTesseract(this->data_->thor);
  }

  this->data_->load_environment = true;
}

void TesseractSetupWizard::onAddChainGroup(const QString &group_name, const QString& base_link, const QString& tip_link)
{
  CONSOLE_BRIDGE_logError("onAddChainGroup");
}
void TesseractSetupWizard::onAddJointGroup(const QString &group_name, const QStringList& joint_list)
{
  CONSOLE_BRIDGE_logError("onAddJointGroup");
}
void TesseractSetupWizard::onAddLinkGroup(const QString &group_name, const QStringList& link_list)
{
  CONSOLE_BRIDGE_logError("onAddLinkGroup");
}
void TesseractSetupWizard::onGenerateACM(long resolution)
{
  CONSOLE_BRIDGE_logError("onGenerateACM");
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
  this->data_->thor->getEnvironment()->setState({joint_name.toStdString()}, {joint_value});
  this->data_->update_transforms = true;
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

// Register this plugin
IGNITION_ADD_PLUGIN(tesseract_ignition::gui::plugins::TesseractSetupWizard, ignition::gui::Plugin);
