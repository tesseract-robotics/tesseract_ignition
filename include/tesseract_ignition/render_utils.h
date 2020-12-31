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
#ifndef TESSERACT_IGNITION_RENDER_UTILS_H
#define TESSERACT_IGNITION_RENDER_UTILS_H

#include <memory>
#include <string>
#include <vector>

#include <tesseract_environment/core/environment.h>
#include <tesseract_visualization/ignition/entity_manager.h>

//#include <sdf/Sensor.hh>

//#include <ignition/gazebo/config.hh>
//#include <ignition/gazebo/Export.hh>
//#include <ignition/gazebo/System.hh>

//#include "ignition/gazebo/rendering/SceneManager.hh"
//#include "ignition/gazebo/rendering/MarkerManager.hh"

namespace tesseract_ignition
{
  // forward declaration
  class RenderUtilPrivate;

  /// \class RenderUtil RenderUtil.hh ignition/gazebo/gui/plugins/RenderUtil.hh
  class RenderUtil
  {
  public:
    /// \brief Constructor
    explicit RenderUtil();

    /// \brief Destructor
    ~RenderUtil();

    /// \brief Initialize the renderer. Must be called in the rendering thread.
    void init();

    /** @brief Check if it has been initialized */
    bool isInitialized() const;

    /**
     * @brief Set the render environment object
     *
     * This is not required because it starts with an empty environment.
     *
     * @param env The environment object to initialize the scene with
     */
    void setEnvironment(tesseract_environment::Environment::Ptr env);

    /**
     * @brief Get Tesseract Object
     *
     * Do not modify the environment directly, use setTesseractCommands and setTesseractState
     *
     * @return Tesseract Shared Pointer
     */
    tesseract_environment::Environment::Ptr getEnvironment();

    /**
     * @brief Get Tesseract Object
     * @return Tesseract Const Shared Pointer
     */
    tesseract_environment::Environment::ConstPtr getEnvironmentConst() const;

    /** @brief Set Tesseract commands to be applied to the scene and tesseract environment */
    void setEnvironmentCommands(tesseract_environment::Commands commands);

    /** @brief Set Tesseract joint values to be applied to the scene and tesseract environment */
    void setEnvironmentState(const std::unordered_map<std::string, double>& joints);
    void setEnvironmentState(const std::vector<std::string>& joint_names, const std::vector<double>& joint_values);
    void setEnvironmentState(const std::vector<std::string>& joint_names,
                           const Eigen::Ref<const Eigen::VectorXd>& joint_values);

//    /// \brief Count of pending sensors. Must be called in the rendering thread.
//    /// \return Number of sensors to be added on the next `Update` call
//    ///
//    /// In the case that RenderUtil has not been initialized, this method
//    /// will return -1.
//    int PendingSensors() const;

    /// \brief Main update function. Must be called in the rendering thread.
    void update();

    /// \brief Get a pointer to the scene
    /// \return Pointer to the scene
    ignition::rendering::ScenePtr scene() const;

    /// \brief Set the rendering engine to use
    /// \param[in] _engineName Name of the rendering engine.
    void setEngineName(const std::string &engine_name);

    /// \brief Get the name of the rendering engine used
    /// \return Name of the rendering engine
    std::string engineName() const;

    /// \brief Set the scene to use
    /// \param[in] _sceneName Name of the engine.
    void setSceneName(const std::string &scene_name);

    /// \brief Get the name of the rendering scene used
    /// \return Name of the rendering scene.
    std::string sceneName() const;

    /// \brief Set background color of render window
    /// \param[in] _color Color of render window background
    void setBackgroundColor(const ignition::math::Color &color);

    /// \brief Set ambient light of render window
    /// \param[in] _ambient Color of ambient light
    void setAmbientLight(const ignition::math::Color &ambient);

    /// \brief Show grid view in the scene
    void showGrid();

    /// \brief Hide grid view in the scene
    void hideGrid();

    /// \brief Show world axis in the scene
    void showWorldAxis();

    /// \brief Hide world axis in the scene
    void hideWorldAxis();

    /// \brief Set whether to use the current GL context
    /// \param[in] _enable True to use the current GL context
    void setUseCurrentGLContext(bool enable);

//    /// \brief Set whether to create rendering sensors
//    /// \param[in] _enable True to create rendering sensors
//    /// \param[in] _createSensorCb Callback function for creating the sensors
//    /// The callback function args are: sensor entity, sensor sdf
//    /// and parent name, it returns the name of the rendering sensor created.
//    void SetEnableSensors(bool _enable, std::function<std::string(const EntityID &, const sdf::Sensor &, const std::string &)> _createSensorCb = {});

//    /// \brief Set the callback function for removing the sensors
//    /// \param[in] _removeSensorCb Callback function for removing the sensors
//    /// The callback function arg is the sensor entity to remove
//    void SetRemoveSensorCb(std::function<void(const EntityID &)> _removeSensorCb);

    /// \brief Set the entity being selected
    /// \param[in] _node Node representing the selected entity
    void setSelectedEntity(const ignition::rendering::NodePtr &node);

    /// \brief Get the entities currently selected, in order of selection.
    /// \return Vector of currently selected entities
    std::vector<tesseract_visualization::EntityID> selectedEntities() const;

    /// \brief Clears the set of selected entities and lowlights all of them.
    void deselectAllEntities();

    /// \brief Set whether the transform controls are currently being dragged.
    /// \param[in] _active True if active.
    void setTransformActive(bool active);

    /** @brief Clear the environment */
    void clear();

  private:
    /// \brief Private data pointer.
    std::unique_ptr<RenderUtilPrivate> dataPtr;
  };
}

#endif // TESSERACT_IGNITION_RENDER_UTILS_H
