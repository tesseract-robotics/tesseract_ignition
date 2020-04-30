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
#include <tesseract_ignition/gui_events.h>
#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_scene_graph/resource_locator.h>
#include <tesseract_environment/kdl/kdl_env.h>

namespace tesseract_ignition::gui::plugins
{

class TesseractSetupWizardPrivate
{
  public:

  /// \brief Flag that indicates whether there are new updates to be rendered.
  bool dirty{false};

  std::string urdf_filepath;

  std::string srdf_filepath;

  std::string engine_name {"ogre"};

  std::string scene_name {"scene"};

  ignition::rendering::ScenePtr scene {nullptr};

  tesseract_environment::Environment::Ptr env;

  std::vector<ignition::rendering::AxisVisualPtr> axes;

  std::vector<ignition::rendering::VisualPtr> visual;
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
void TesseractSetupWizard::onLoad(const QString &_urdf_filepath, const QString& _srdf_filepath)
{
  this->data_->urdf_filepath = _urdf_filepath.toStdString();
  this->data_->srdf_filepath = _srdf_filepath.toStdString();
  this->data_->dirty = true;
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
  if (!(this->data_->dirty))
    return;

  // create scene
  ignition::rendering::RenderEngine* engine = ignition::rendering::engine(this->data_->engine_name);
  if (!engine)
  {
    ignerr << "Engine " << this->data_->engine_name.c_str() << " is not supported" << std::endl;
    this->data_->dirty = false;
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
        this->data_->dirty = false;
        return;
      }

      this->data_->scene = engine->SceneByIndex(0);
    }
  }

  ignition::rendering::ScenePtr& scene = this->data_->scene;
  ignition::rendering::VisualPtr root = scene->RootVisual();
  ignition::rendering::AxisVisualPtr axis = scene->CreateAxisVisual();
  axis->Scale(0.5, 0.5, 0.5);
  root->AddChild(axis);
  this->data_->axes.push_back(axis);

  auto env = std::make_shared<tesseract_environment::KDLEnv>();
  if (env->init(getSceneGraph(this->data_->urdf_filepath)))
  {
    this->data_->env = env;
    tesseract_environment::EnvState::ConstPtr state = env->getCurrentState();
    tesseract_scene_graph::SceneGraph::ConstPtr sg = env->getSceneGraph();
    for (const auto& link : sg->getLinks())
    {
      ignition::rendering::VisualPtr ign_link = scene->CreateVisual(link->getName());
      ign_link->SetWorldPose(ignition::math::eigen3::convert(state->link_transforms.at(link->getName())));
      for (const auto& vs : link->visual)
      {
        switch (vs->geometry->getType())
        {
          case tesseract_geometry::GeometryType::BOX:
          {
            ignition::rendering::VisualPtr box = scene->CreateVisual();
            box->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
            box->AddGeometry(scene->CreateBox());

            auto shape = std::static_pointer_cast<const tesseract_geometry::Box>(vs->geometry);
            box->Scale(shape->getX(), shape->getY(), shape->getZ());
            ign_link->AddChild(box);
            break;
          }
          case tesseract_geometry::GeometryType::SPHERE:
          {
            ignition::rendering::VisualPtr sphere = scene->CreateVisual();
            sphere->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
            sphere->AddGeometry(scene->CreateSphere());

            auto shape = std::static_pointer_cast<const tesseract_geometry::Sphere>(vs->geometry);
            sphere->Scale(shape->getRadius(), shape->getRadius(), shape->getRadius());
            ign_link->AddChild(sphere);
            break;
          }
          case tesseract_geometry::GeometryType::CYLINDER:
          {
            ignition::rendering::VisualPtr cylinder = scene->CreateVisual();
            cylinder->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
            cylinder->AddGeometry(scene->CreateCylinder());

            auto shape = std::static_pointer_cast<const tesseract_geometry::Cylinder>(vs->geometry);
            cylinder->Scale(shape->getRadius(), shape->getRadius(), shape->getLength());
            ign_link->AddChild(cylinder);
            break;
          }
          case tesseract_geometry::GeometryType::CONE:
          {
            ignition::rendering::VisualPtr cone = scene->CreateVisual();
            cone->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
            cone->AddGeometry(scene->CreateCone());

            auto shape = std::static_pointer_cast<const tesseract_geometry::Cone>(vs->geometry);
            cone->Scale(shape->getRadius(), shape->getRadius(), shape->getLength());
            ign_link->AddChild(cone);
            break;
          }
          case tesseract_geometry::GeometryType::CAPSULE:
          {
            //          VisualPtr capsule = scene->CreateVisual();
            //          capsule->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
            //          capsule->AddGeometry(scene->CreateCapsule());

            //          auto shape = std::static_pointer_cast<const tesseract_geometry::Capsule>(vs->geometry);
            //          capsule->Scale(shape->getRadius(), shape->getRadius(), shape->getLength());
            //          ign_link->AddChild(capsule);
            break;
          }
          case tesseract_geometry::GeometryType::MESH:
          {
            auto shape = std::static_pointer_cast<const tesseract_geometry::Mesh>(vs->geometry);
            auto resource = shape->getResource();
            if (resource)
            {
              ignition::rendering::VisualPtr mesh = scene->CreateVisual();
              mesh->SetLocalPose(ignition::math::eigen3::convert(vs->origin));

              ignition::rendering::MeshDescriptor descriptor;
              descriptor.meshName = resource->getFilePath();
              ignition::common::MeshManager* mesh_manager = ignition::common::MeshManager::Instance();
              descriptor.mesh = mesh_manager->Load(descriptor.meshName);
              ignition::rendering::MeshPtr mesh_geom = scene->CreateMesh(descriptor);
              mesh->AddGeometry(mesh_geom);
              ign_link->AddChild(mesh);
            }
            else
            {
              assert(false);
            }

            break;
          }
          case tesseract_geometry::GeometryType::CONVEX_MESH:
          {
            auto shape = std::static_pointer_cast<const tesseract_geometry::ConvexMesh>(vs->geometry);
            auto resource = shape->getResource();
            if (resource)
            {
              ignition::rendering::VisualPtr mesh = scene->CreateVisual();
              mesh->SetLocalPose(ignition::math::eigen3::convert(vs->origin));

              ignition::rendering::MeshDescriptor descriptor;
              descriptor.meshName = resource->getFilePath();
              ignition::common::MeshManager* mesh_manager = ignition::common::MeshManager::Instance();
              descriptor.mesh = mesh_manager->Load(descriptor.meshName);
              ignition::rendering::MeshPtr mesh_geom = scene->CreateMesh(descriptor);
              mesh->AddGeometry(mesh_geom);
              ign_link->AddChild(mesh);
            }
            else
            {
              assert(false);
            }

            break;
          }
          case tesseract_geometry::GeometryType::OCTREE:
          {
            auto shape = std::static_pointer_cast<const tesseract_geometry::Octree>(vs->geometry);

            // TODO: Need to implement
            assert(false);
            break;
          }
          default:
          {
            CONSOLE_BRIDGE_logError("This geometric shape type (%d) is not supported",
                                    static_cast<int>(vs->geometry->getType()));
            break;
          }
        }
      }
      root->AddChild(ign_link);
    }
  }
  this->data_->dirty = false;
}

// Register this plugin
IGNITION_ADD_PLUGIN(tesseract_ignition::gui::plugins::TesseractSetupWizard, ignition::gui::Plugin);
