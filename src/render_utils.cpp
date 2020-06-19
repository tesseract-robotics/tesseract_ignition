/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <ignition/common/Profiler.hh>
#include <ignition/common/Skeleton.hh>
#include <ignition/common/SkeletonAnimation.hh>

#include <ignition/math/Color.hh>
#include <ignition/math/Helpers.hh>
#include <ignition/math/Matrix4.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/eigen3/Conversions.hh>

#include <ignition/rendering.hh>
#include <ignition/rendering/RenderEngine.hh>
#include <ignition/rendering/RenderingIface.hh>
#include <ignition/rendering/Scene.hh>

#include <tesseract_ignition/render_utils.h>
#include <tesseract_ignition/conversions.h>
#include <tesseract/tesseract.h>

namespace tesseract_ignition
{
  class RenderUtilPrivate
  {
  public:
      /** @brief True if the rendering component is initialized */
      bool initialized {false};

      /** @brief This indicates that the environment should be loaded */
      bool load_environment{false};

      /** @brief The Tesseract Entity Manager */
      EntityManager entity_manager;

      /** @brief The Tesseract Object */
      tesseract::Tesseract::Ptr thor;

      /** @brief Set Tesseract commands to be applied to the scene and tesseract environment */
      tesseract_environment::Commands commands;

      /** @brief Set Tesseract transforms to be applied to the scene and tesseract environment */
      tesseract_common::TransformMap transfroms;

      /** @brief This stores the tesseract revision number to determine if new objects should be added */
      int tesseract_revision {-1};

      /** @brief Total time elapsed in simulation. This will not increase while paused. */
      std::chrono::steady_clock::duration sim_time{0};

      /** @brief Name of rendering engine */
      std::string engine_name {"ogre"}; //{"ogre2"}

      /** @brief Name of scene */
      std::string scene_name {"scene"};

      /** @brief Scene background color */
      ignition::math::Color background_color {ignition::math::Color::Black};

      /** @brief Ambient color */
      ignition::math::Color ambient_light {ignition::math::Color(1.0, 1.0, 1.0, 1.0)};

      /** @brief Pointer to rendering engine. */
      ignition::rendering::RenderEngine *engine{nullptr};

      /** @brief rendering scene to be managed by the scene manager and used to generate sensor data */
      ignition::rendering::ScenePtr scene;

      /** @brief Flag to indicate if the current GL context should be used */
      bool use_current_gl_context {false};

//      /// \brief A map of entity ids and wire boxes
//      std::unordered_map<EntityID, ignition::rendering::WireBoxPtr> wireBoxes;

//      /// \brief A map of entity ids and trajectory pose updates.
//      std::unordered_map<EntityID, ignition::math::Pose3d> trajectoryPoses;

//      /// \brief A map of entity ids and actor animation info.
//      std::unordered_map<EntityID, AnimationUpdateData> actorAnimationData;

      /// \brief True to update skeletons manually using bone poses
      /// (see actorTransforms). False to let render engine update animation
      /// based on sim time.
      /// \todo(anyone) Let this be turned on from a component
      bool actor_manual_skeleton_update {false};

      /** @brief Mutex to protect updates */
      std::mutex update_mutex;

      /** @brief Flag to indicate whether to create sensors */
      bool enable_sensors {false};

      /** @brief A set containing all the entities with attached rendering sensors */
      std::unordered_set<EntityID> sensorEntities;

//      /// \brief Callback function for creating sensors.
//      /// The function args are: entity id, sensor sdf, and parent name.
//      /// The function returns the id of the rendering sensor created.
//      std::function<std::string(const EntityID &, const sdf::Sensor &, const std::string &)> createSensorCb;

//      /// \brief Callback function for removing sensors.
//      /// The function arg is the entity id
//      std::function<void(const EntityID &)> removeSensorCb;

      /** @brief Currently selected entities, organized by order of selection. */
      std::vector<EntityID> selectedEntities;

      /** @brief Map of original emissive colors for nodes currently highlighted. */
      std::map<std::string, ignition::math::Color> originalEmissive;

      /** @brief Whether the transform gizmo is being dragged. */
      bool transform_active{false};

      /** @brief Highlight a node and all its children.
       * param[in] _node Node to be highlighted
       */
      void highlightNode(const ignition::rendering::NodePtr &_node);

      /**
       * @brief Restore a highlighted node to normal.
       * param[in] _node Node to be restored.
       */
      void lowlightNode(const ignition::rendering::NodePtr &_node);
  };

  //////////////////////////////////////////////////
  RenderUtil::RenderUtil() : dataPtr(std::make_unique<RenderUtilPrivate>())
  {
  }

  //////////////////////////////////////////////////
  RenderUtil::~RenderUtil() = default;

  //////////////////////////////////////////////////
  ignition::rendering::ScenePtr RenderUtil::scene() const
  {
    return this->dataPtr->scene;
  }

  //////////////////////////////////////////////////
  void RenderUtil::init()
  {
    std::map<std::string, std::string> params;
    if (this->dataPtr->use_current_gl_context)
      params["useCurrentGLContext"] = "1";
    this->dataPtr->engine = ignition::rendering::engine(this->dataPtr->engine_name, params);
    if (!this->dataPtr->engine)
    {
      ignerr << "Engine [" << this->dataPtr->engine_name << "] is not supported" << std::endl;
      return;
    }

    // Scene
    if (!this->dataPtr->scene)
    {
      this->dataPtr->scene = this->dataPtr->engine->SceneByName(this->dataPtr->scene_name);
      if (!this->dataPtr->scene)
      {
        igndbg << "Create scene [" << this->dataPtr->scene_name << "]" << std::endl;
        this->dataPtr->scene = this->dataPtr->engine->CreateScene(this->dataPtr->scene_name);
        this->dataPtr->scene->SetAmbientLight(this->dataPtr->ambient_light);
        this->dataPtr->scene->SetBackgroundColor(this->dataPtr->background_color);
      }

      showGrid();
      showWorldAxis();
    }
//    this->dataPtr->sceneManager.SetScene(this->dataPtr->scene);
//    if (this->dataPtr->enableSensors)
//      this->dataPtr->markerManager.SetTopic("/sensors/marker");
//    this->dataPtr->markerManager.Init(this->dataPtr->scene);
    this->dataPtr->initialized = true;
  }

  //////////////////////////////////////////////////
  bool RenderUtil::isInitialized() const { return this->dataPtr->initialized; }

  //////////////////////////////////////////////////
  void RenderUtil::setTesseract(tesseract::Tesseract::Ptr tesseract)
  {
    this->dataPtr->thor = tesseract;
    this->dataPtr->tesseract_revision = this->dataPtr->thor->getEnvironment()->getRevision();
    this->dataPtr->load_environment = true;
  }

  //////////////////////////////////////////////////
  tesseract::Tesseract::Ptr RenderUtil::getTesseract() { return this->dataPtr->thor; }

  //////////////////////////////////////////////////
  tesseract::Tesseract::ConstPtr RenderUtil::getTesseractConst() const { return this->dataPtr->thor; }

  //////////////////////////////////////////////////
  void RenderUtil::setTesseractCommands(tesseract_environment::Commands commands)
  {
    this->dataPtr->update_mutex.lock();
    this->dataPtr->commands = std::move(commands);
    this->dataPtr->update_mutex.unlock();
  }

  //////////////////////////////////////////////////
  void RenderUtil::setTesseractState(const std::unordered_map<std::string, double>& joints)
  {
    this->dataPtr->update_mutex.lock();
    this->dataPtr->thor->getEnvironment()->setState(joints);
    this->dataPtr->transfroms = this->dataPtr->thor->getEnvironment()->getCurrentState()->link_transforms;
    this->dataPtr->update_mutex.unlock();
  }

  //////////////////////////////////////////////////
  void RenderUtil::setTesseractState(const std::vector<std::string>& joint_names,
                                     const std::vector<double>& joint_values)
  {
    this->dataPtr->update_mutex.lock();
    this->dataPtr->thor->getEnvironment()->setState(joint_names, joint_values);
    this->dataPtr->transfroms = this->dataPtr->thor->getEnvironment()->getCurrentState()->link_transforms;
    this->dataPtr->update_mutex.unlock();
  }

  //////////////////////////////////////////////////
  void RenderUtil::setTesseractState(const std::vector<std::string>& joint_names,
                         const Eigen::Ref<const Eigen::VectorXd>& joint_values)
  {
    this->dataPtr->update_mutex.lock();
    this->dataPtr->thor->getEnvironment()->setState(joint_names, joint_values);
    this->dataPtr->transfroms = this->dataPtr->thor->getEnvironment()->getCurrentState()->link_transforms;
    this->dataPtr->update_mutex.unlock();
  }

//  //////////////////////////////////////////////////
//  int RenderUtil::PendingSensors() const
//  {
//    if (!this->dataPtr->initialized)
//      return -1;

//    if (!this->dataPtr->scene)
//      return -1;

//    this->dataPtr->updateMutex.lock();
//    int nSensors = this->dataPtr->newSensors.size();
//    this->dataPtr->updateMutex.unlock();
//    return nSensors;
//  }

  //////////////////////////////////////////////////
  void RenderUtil::update()
  {
    if (!this->dataPtr->initialized || !this->dataPtr->scene || !this->dataPtr->thor)
      return;

    this->dataPtr->update_mutex.lock();
    tesseract_environment::Commands commands = std::move(this->dataPtr->commands);
    tesseract_common::TransformMap transforms = std::move(this->dataPtr->transfroms);

    this->dataPtr->commands.clear();
    this->dataPtr->transfroms.clear();
    this->dataPtr->update_mutex.unlock();

    if (this->dataPtr->load_environment)
    {      
//      this->dataPtr->scene->Clear(); This is causing issues but it is best to probably only remove tesseract entities
      for (const auto& pair : this->dataPtr->entity_manager.getLinks())
        this->dataPtr->scene->DestroyNodeById(static_cast<unsigned>(pair.second));

      for (const auto& pair : this->dataPtr->entity_manager.getModels())
        this->dataPtr->scene->DestroyNodeById(static_cast<unsigned>(pair.second));

      for (const auto& pair : this->dataPtr->entity_manager.getVisuals())
        this->dataPtr->scene->DestroyNodeById(static_cast<unsigned>(pair.second));

      for (const auto& pair : this->dataPtr->entity_manager.getSensors())
        this->dataPtr->scene->DestroyNodeById(static_cast<unsigned>(pair.second));

      this->dataPtr->entity_manager.clear();

      // Load Ignition Scene
      const tesseract_environment::EnvState::ConstPtr& state = this->dataPtr->thor->getEnvironment()->getCurrentState();
      toScene(*(this->dataPtr->scene), this->dataPtr->entity_manager, *(this->dataPtr->thor->getEnvironmentConst()->getSceneGraph()), state->link_transforms);

      showGrid();
      showWorldAxis();
      this->dataPtr->load_environment = false;
    }
    else if (this->dataPtr->thor)
    {
      int revision = this->dataPtr->thor->getEnvironmentConst()->getRevision();
      assert(this->dataPtr->tesseract_revision == revision);
      if (!commands.empty())
      {
//        if (revision > this->dataPtr->tesseract_revision)
//        {
//          auto history = this->dataPtr->thor->getEnvironment()->getCommandHistory();
//          /** @todo Neex to add utils for process the command history */

          // Store new revision number
//          this->dataPtr->tesseract_revision = revision;
//        }
      }

      if (!transforms.empty())
      {
        const tesseract_environment::EnvState::ConstPtr& state = this->dataPtr->thor->getEnvironment()->getCurrentState();
        for (const auto& link : state->link_transforms)
        {
          ignition::rendering::VisualPtr v = this->dataPtr->scene->VisualByName(link.first);
          v->SetWorldPose(ignition::math::eigen3::convert(link.second));
        }
      }

  //    if (this->data_->update_selections)
  //    {
  //      for (const auto& id : this->data_->selected_entities)
  //      {
  //        ignition::rendering::VisualPtr v = this->data_->scene->VisualById(id);
  //  //      if(v)
  //  //      {
  //  //        v->SetH
  //  //      }
  //      }
  //    }
    }
  }

//  //////////////////////////////////////////////////
//  void RenderUtil::UpdateFromECM(const UpdateInfo &_info,
//                                 const EntityComponentManager &_ecm)
//  {

//  }

  /////////////////////////////////////////////////
  void RenderUtil::setEngineName(const std::string &engine_name)
  {
    this->dataPtr->engine_name = engine_name;
  }

  /////////////////////////////////////////////////
  std::string RenderUtil::engineName() const
  {
    return this->dataPtr->engine_name;
  }

  /////////////////////////////////////////////////
  void RenderUtil::setSceneName(const std::string &scene_name)
  {
    this->dataPtr->scene_name = scene_name;
  }

  /////////////////////////////////////////////////
  std::string RenderUtil::sceneName() const
  {
    return this->dataPtr->scene_name;
  }

  /////////////////////////////////////////////////
  void RenderUtil::setBackgroundColor(const ignition::math::Color &color)
  {
    this->dataPtr->background_color = color;
  }

  /////////////////////////////////////////////////
  void RenderUtil::setAmbientLight(const ignition::math::Color &ambient)
  {
    this->dataPtr->ambient_light  = ambient;
  }

  /////////////////////////////////////////////////
  void RenderUtil::showGrid()
  {
    ignition::rendering::VisualPtr visual = this->dataPtr->scene->VisualByName("tesseract_grid");
    if (visual == nullptr)
    {
      ignition::rendering::VisualPtr root = this->dataPtr->scene->RootVisual();

      // create gray material
      ignition::rendering::MaterialPtr gray = this->dataPtr->scene->CreateMaterial();
      gray->SetAmbient(0.7, 0.7, 0.7);
      gray->SetDiffuse(0.7, 0.7, 0.7);
      gray->SetSpecular(0.7, 0.7, 0.7);

      // create grid visual
      unsigned id = static_cast<unsigned>(this->dataPtr->entity_manager.addVisual("tesseract_grid"));
      ignition::rendering::VisualPtr visual = this->dataPtr->scene->CreateVisual(id, "tesseract_grid");
      ignition::rendering::GridPtr gridGeom = this->dataPtr->scene->CreateGrid();
      if (!gridGeom)
      {
        ignwarn << "Failed to create grid for scene ["
          << this->dataPtr->scene->Name() << "] on engine ["
            << this->dataPtr->scene->Engine()->Name() << "]"
              << std::endl;
        return;
      }
      gridGeom->SetCellCount(20);
      gridGeom->SetCellLength(1);
      gridGeom->SetVerticalCellCount(0);
      visual->AddGeometry(gridGeom);
      visual->SetLocalPosition(0, 0, 0.015);
      visual->SetMaterial(gray);
      root->AddChild(visual);
    }
    else
    {
      visual->SetVisible(true);
    }
  }

  /////////////////////////////////////////////////
  void RenderUtil::hideGrid()
  {
    ignition::rendering::VisualPtr visual = this->dataPtr->scene->VisualByName("tesseract_grid");
    if (visual != nullptr)
      visual->SetVisible(false);
  }

  /////////////////////////////////////////////////
  void RenderUtil::showWorldAxis()
  {
    ignition::rendering::VisualPtr visual = this->dataPtr->scene->VisualByName("tesseract_world_axis");
    if (visual == nullptr)
    {
      ignition::rendering::VisualPtr root = this->dataPtr->scene->RootVisual();
      unsigned id = static_cast<unsigned>(this->dataPtr->entity_manager.addVisual("tesseract_world_axis"));
      ignition::rendering::AxisVisualPtr axis = this->dataPtr->scene->CreateAxisVisual(id, "tesseract_world_axis");
      axis->Scale(0.5, 0.5, 0.5);
      root->AddChild(axis);
    }
    else
    {
      visual->SetVisible(true);
    }
  }

  /////////////////////////////////////////////////
  void RenderUtil::hideWorldAxis()
  {
    ignition::rendering::VisualPtr visual = this->dataPtr->scene->VisualByName("tesseract_world_axis");
    if (visual != nullptr)
      visual->SetVisible(false);
  }

  /////////////////////////////////////////////////
  void RenderUtil::setUseCurrentGLContext(bool enable)
  {
    this->dataPtr->use_current_gl_context = enable;
  }

//  /////////////////////////////////////////////////
//  void RenderUtil::SetEnableSensors(bool _enable,
//      std::function<std::string(const EntityID &, const sdf::Sensor &,
//        const std::string &)> _createSensorCb)
//  {
//    this->dataPtr->enableSensors = _enable;
//    this->dataPtr->createSensorCb = std::move(_createSensorCb);
//  }

//  /////////////////////////////////////////////////
//  void RenderUtil::SetRemoveSensorCb(
//      std::function<void(const EntityID &)> _removeSensorCb)
//  {
//    this->dataPtr->removeSensorCb = std::move(_removeSensorCb);
//  }

  /////////////////////////////////////////////////
  void RenderUtil::setSelectedEntity(const ignition::rendering::NodePtr &node)
  {
    if (!node)
      return;

    auto vis = std::dynamic_pointer_cast<ignition::rendering::Visual>(node);
    EntityID entityId = NULL_ENTITY_ID;

    if (vis)
      entityId = static_cast<int>(vis->Id());

    if (entityId == NULL_ENTITY_ID)
      return;

    this->dataPtr->selectedEntities.push_back(entityId);
    this->dataPtr->highlightNode(node);
  }

  /////////////////////////////////////////////////
  void RenderUtil::deselectAllEntities()
  {
    for (const auto &entity_id : this->dataPtr->selectedEntities)
    {
      auto node = this->dataPtr->scene->NodeById(static_cast<unsigned>(entity_id));
      this->dataPtr->lowlightNode(node);
    }
    this->dataPtr->selectedEntities.clear();
    this->dataPtr->originalEmissive.clear();
  }

  /////////////////////////////////////////////////
  std::vector<EntityID> RenderUtil::selectedEntities() const
  {
    return this->dataPtr->selectedEntities;
  }

  /////////////////////////////////////////////////
  void RenderUtil::setTransformActive(bool active)
  {
    this->dataPtr->transform_active = active;
  }

  ////////////////////////////////////////////////
  void RenderUtilPrivate::highlightNode(const ignition::rendering::NodePtr &_node)
  {
    if (!_node)
      return;
//    auto vis = std::dynamic_pointer_cast<ignition::rendering::Visual>(_node);
//    EntityID entityId = NULL_ENTITY_ID;
//    if (vis)
//      entityId = static_cast<int>(vis->Id());
    // If the entity is not found in the existing map, create a wire box
//    auto wireBoxIt = this->wireBoxes.find(entityId);
//    if (wireBoxIt == this->wireBoxes.end())
//    {
//      auto white = this->scene->Material("highlight_material");
//      if (!white)
//      {
//        white = this->scene->CreateMaterial("highlight_material");
//        white->SetAmbient(1.0, 1.0, 1.0);
//        white->SetDiffuse(1.0, 1.0, 1.0);
//        white->SetSpecular(1.0, 1.0, 1.0);
//        white->SetEmissive(1.0, 1.0, 1.0);
//      }

//      ignition::rendering::WireBoxPtr wireBox = this->scene->CreateWireBox();
//      ignition::math::AxisAlignedBox aabb = vis->LocalBoundingBox();
//      wireBox->SetBox(aabb);

//      // Create visual and add wire box
//      ignition::rendering::VisualPtr wireBoxVis =
//        this->scene->CreateVisual();
//      wireBoxVis->SetInheritScale(false);
//      wireBoxVis->AddGeometry(wireBox);
//      wireBoxVis->SetMaterial(white, false);
//      vis->AddChild(wireBoxVis);

//      // Add wire box to map for setting visibility
//      this->wireBoxes.insert(std::pair<EntityID, ignition::rendering::WireBoxPtr>(entityId, wireBox));
//    }
//    else
//    {
//      ignition::rendering::WireBoxPtr wireBox = wireBoxIt->second;
//      auto visParent = wireBox->Parent();
//      if (visParent)
//        visParent->SetVisible(true);
//    }
  }

  ////////////////////////////////////////////////
  void RenderUtilPrivate::lowlightNode(const ignition::rendering::NodePtr &_node)
  {
    if (!_node)
      return;
//    auto vis = std::dynamic_pointer_cast<ignition::rendering::Visual>(_node);
//    EntityID entityId = NULL_ENTITY_ID;
//    if (vis)
//      entityId = static_cast<int>(vis->Id());

//    if (this->wireBoxes.find(entityId) != this->wireBoxes.end())
//    {
//      ignition::rendering::WireBoxPtr wireBox = this->wireBoxes[entityId];
//      auto visParent = wireBox->Parent();
//      if (visParent)
//        visParent->SetVisible(false);
//    }
  }
}
