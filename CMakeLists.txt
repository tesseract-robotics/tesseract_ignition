cmake_minimum_required(VERSION 3.5.0)
project(tesseract_ignition VERSION 0.1.0 LANGUAGES CXX)

find_package(Eigen3 REQUIRED)
find_package(tesseract_collision REQUIRED)
find_package(tesseract_common REQUIRED)
find_package(tesseract_environment REQUIRED)
find_package(tesseract_support REQUIRED)
find_package(tesseract_urdf REQUIRED)
find_package(tesseract_visualization REQUIRED)
find_package(tesseract REQUIRED)

# Find ignition-gui
find_package(ignition-gui3 REQUIRED)
find_package(ignition-common3 COMPONENTS profiler events av REQUIRED)
find_package(ignition-rendering3 REQUIRED)
find_package(ignition-transport8 REQUIRED)
find_package(ignition-msgs5 REQUIRED)
find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

set(COVERAGE_EXCLUDE /usr/* /opt/* ${CMAKE_CURRENT_LIST_DIR}/test/* /*/gtest/*)

add_code_coverage_all_targets(EXCLUDE ${COVERAGE_EXCLUDE})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${IGNITION-GUI_CXX_FLAGS}")

# Need to add to target when upgraded to newer cmake version
link_directories(
  ${IGNITION-COMMON_LIBRARY_DIRS}
  ${IGNITION-GUI_LIBRARY_DIRS}
  ${IGNITION-RENDERING_LIBRARY_DIRS}
  ${IGNITION-TRANSPORT_LIBRARY_DIRS}
  ${IGNITION-MSGS_LIBRARY_DIRS}
)

add_library(${PROJECT_NAME} SHARED src/conversions.cpp src/entity_manager.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC
  tesseract::tesseract_environment_core
  tesseract::tesseract_visualization
  tesseract::tesseract_common
  ${IGNITION-COMMON_LIBRARIES}
  ${IGNITION-RENDERING_LIBRARIES}
  ${IGNITION-MSGS_LIBRARIES})
tesseract_target_compile_options(${PROJECT_NAME} PUBLIC)
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    ${IGNITION-COMMON_INCLUDE_DIRS}
    ${IGNITION-RENDERING_INCLUDE_DIRS}
    ${IGNITION-MSGS_INCLUDE_DIRS})

QT5_WRAP_CPP(TesseractScene3D_headers_MOC include/tesseract_ignition/scene3d/tesseract_scene3d.h)
QT5_ADD_RESOURCES(TesseractScene3D_resources_RCC include/tesseract_ignition/scene3d/TesseractScene3D.qrc)

add_library(TesseractScene3D SHARED ${TesseractScene3D_headers_MOC} src/tesseract_scene3d.cpp  ${TesseractScene3D_resources_RCC})
target_link_libraries(TesseractScene3D PUBLIC
  ${IGNITION-COMMON_LIBRARIES}
  ${IGNITION-GUI_LIBRARIES}
  ${IGNITION-RENDERING_LIBRARIES}
  ${IGNITION-TRANSPORT_LIBRARIES}
  ${IGNITION-MSGS_LIBRARIES}
  Qt5::Core Qt5::Quick Qt5::QuickControls2 )
target_include_directories(TesseractScene3D PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
target_include_directories(TesseractScene3D SYSTEM PUBLIC
    ${IGNITION-GUI_INCLUDE_DIRS}
    ${IGNITION-COMMON_INCLUDE_DIRS}
    ${IGNITION-RENDERING_INCLUDE_DIRS}
    ${IGNITION-TRANSPORT_INCLUDE_DIRS}
    ${IGNITION-MSGS_INCLUDE_DIRS})

QT5_WRAP_CPP(TesseractGridConfig_headers_MOC include/tesseract_ignition/grid/tesseract_grid_config.h)
QT5_ADD_RESOURCES(TesseractGridConfig_resources_RCC include/tesseract_ignition/grid/TesseractGridConfig.qrc)

add_library(TesseractGridConfig SHARED ${TesseractGridConfig_headers_MOC} src/tesseract_grid_config.cpp  ${TesseractGridConfig_resources_RCC})
target_link_libraries(TesseractGridConfig PUBLIC ${IGNITION-COMMON_LIBRARIES} ${IGNITION-GUI_LIBRARIES} ${IGNITION-RENDERING_LIBRARIES} Qt5::Core Qt5::Quick Qt5::QuickControls2 )
target_include_directories(TesseractGridConfig PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
target_include_directories(TesseractGridConfig SYSTEM PUBLIC
    ${IGNITION-GUI_INCLUDE_DIRS}
    ${IGNITION-COMMON_INCLUDE_DIRS}
    ${IGNITION-RENDERING_INCLUDE_DIRS})

QT5_WRAP_CPP(TesseractLoad_headers_MOC include/tesseract_ignition/load/tesseract_load.h)
QT5_ADD_RESOURCES(TesseractLoad_resources_RCC include/tesseract_ignition/load/TesseractLoad.qrc)

add_library(TesseractLoad SHARED ${TesseractLoad_headers_MOC} src/tesseract_load.cpp  ${TesseractLoad_resources_RCC})
target_link_libraries(TesseractLoad PUBLIC
  tesseract::tesseract_environment_kdl
  tesseract::tesseract_support
  tesseract::tesseract_urdf
  ${IGNITION-COMMON_LIBRARIES}
  ${IGNITION-GUI_LIBRARIES}
  ${IGNITION-RENDERING_LIBRARIES}
  Qt5::Core Qt5::Quick Qt5::QuickControls2 )
target_include_directories(TesseractLoad PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
target_include_directories(TesseractLoad SYSTEM PUBLIC
    ${IGNITION-GUI_INCLUDE_DIRS}
    ${IGNITION-COMMON_INCLUDE_DIRS}
    ${IGNITION-RENDERING_INCLUDE_DIRS})

QT5_WRAP_CPP(TesseractSetupWizard_headers_MOC
  include/tesseract_ignition/setup_wizard/tesseract_setup_wizard.h
  include/tesseract_ignition/setup_wizard/models/joint_list_model.h
  include/tesseract_ignition/setup_wizard/models/allowed_collision_matrix_model.h
  include/tesseract_ignition/setup_wizard/models/kinematic_groups_model.h
  include/tesseract_ignition/setup_wizard/models/user_defined_joint_states_model.h)
QT5_ADD_RESOURCES(TesseractSetupWizard_resources_RCC include/tesseract_ignition/setup_wizard/TesseractSetupWizard.qrc)

add_library(TesseractSetupWizard SHARED
  ${TesseractSetupWizard_headers_MOC}
  src/setup_wizard/tesseract_setup_wizard.cpp
  src/setup_wizard/models/joint_list_model.cpp
  src/setup_wizard/models/allowed_collision_matrix_model.cpp
  src/setup_wizard/models/kinematic_groups_model.cpp
  src/setup_wizard/models/user_defined_joint_states_model.cpp
  ${TesseractSetupWizard_resources_RCC})
target_link_libraries(TesseractSetupWizard PUBLIC
  ${PROJECT_NAME}
  tesseract::tesseract_environment_kdl
  tesseract::tesseract_support
  tesseract::tesseract_urdf
  tesseract::tesseract
  ${IGNITION-COMMON_LIBRARIES}
  ${IGNITION-GUI_LIBRARIES}
  ${IGNITION-RENDERING_LIBRARIES}
  Qt5::Core Qt5::Quick Qt5::QuickControls2 Qt5::Gui)
target_include_directories(TesseractSetupWizard PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
target_include_directories(TesseractSetupWizard SYSTEM PUBLIC
    ${IGNITION-GUI_INCLUDE_DIRS}
    ${IGNITION-COMMON_INCLUDE_DIRS}
    ${IGNITION-RENDERING_INCLUDE_DIRS})

QT5_WRAP_CPP(TesseractVideoRecorder_headers_MOC include/tesseract_ignition/recorder/tesseract_video_recorder.h)
QT5_ADD_RESOURCES(TesseractVideoRecorder_resources_RCC include/tesseract_ignition/recorder/TesseractVideoRecorder.qrc)

add_library(TesseractVideoRecorder SHARED ${TesseractVideoRecorder_headers_MOC} src/tesseract_video_recorder.cpp  ${TesseractVideoRecorder_resources_RCC})
target_link_libraries(TesseractVideoRecorder PUBLIC
  ${IGNITION-COMMON_LIBRARIES}
  ${IGNITION-GUI_LIBRARIES}
  ${IGNITION-RENDERING_LIBRARIES}
  ${IGNITION-TRANSPORT_LIBRARIES}
  ${IGNITION-MSGS_LIBRARIES}
  Qt5::Core Qt5::Quick Qt5::QuickControls2 )
target_include_directories(TesseractVideoRecorder PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
target_include_directories(TesseractVideoRecorder SYSTEM PUBLIC
    ${IGNITION-GUI_INCLUDE_DIRS}
    ${IGNITION-COMMON_INCLUDE_DIRS}
    ${IGNITION-RENDERING_INCLUDE_DIRS}
    ${IGNITION-TRANSPORT_INCLUDE_DIRS}
    ${IGNITION-MSGS_INCLUDE_DIRS})

add_library(TesseractIgnitionVisualization SHARED src/tesseract_ignition_visualization.cpp)
target_link_libraries(TesseractIgnitionVisualization PUBLIC
  ${PROJECT_NAME}
  tesseract::tesseract_environment_core
  tesseract::tesseract_visualization
  tesseract::tesseract_common
  ${IGNITION-COMMON_LIBRARIES}
  ${IGNITION-TRANSPORT_LIBRARIES}
  ${IGNITION-MSGS_LIBRARIES})
tesseract_target_compile_options(TesseractIgnitionVisualization PUBLIC)
target_include_directories(TesseractIgnitionVisualization PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
target_include_directories(TesseractIgnitionVisualization SYSTEM PUBLIC
    ${IGNITION-COMMON_INCLUDE_DIRS}
    ${IGNITION-TRANSPORT_INCLUDE_DIRS}
    ${IGNITION-MSGS_INCLUDE_DIRS})

add_executable(demo_window src/demo_window.cpp)
target_link_libraries(demo_window PUBLIC ${IGNITION-COMMON_LIBRARIES} ${IGNITION-GUI_LIBRARIES} ${IGNITION-RENDERING_LIBRARIES} Qt5::Core Qt5::Quick Qt5::QuickControls2)
target_include_directories(demo_window PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
  target_include_directories(demo_window SYSTEM PUBLIC
      ${IGNITION-GUI_INCLUDE_DIRS}
      ${IGNITION-COMMON_INCLUDE_DIRS}
      ${IGNITION-RENDERING_INCLUDE_DIRS})

add_executable(demo_dialog src/demo_dialog.cpp)
target_link_libraries(demo_dialog PUBLIC ${IGNITION-COMMON_LIBRARIES} ${IGNITION-GUI_LIBRARIES} ${IGNITION-RENDERING_LIBRARIES} Qt5::Core Qt5::Quick Qt5::QuickControls2)
target_include_directories(demo_dialog PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
  target_include_directories(demo_dialog SYSTEM PUBLIC
      ${IGNITION-GUI_INCLUDE_DIRS}
      ${IGNITION-COMMON_INCLUDE_DIRS}
      ${IGNITION-RENDERING_INCLUDE_DIRS})

add_executable(demo_tesseract src/demo_tesseract.cpp)
target_link_libraries(demo_tesseract PUBLIC
  TesseractIgnitionVisualization
  tesseract::tesseract_environment_kdl
  tesseract::tesseract_support
  tesseract::tesseract_scene_graph
  tesseract::tesseract_urdf)
target_include_directories(demo_tesseract PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

tesseract_configure_package(
  ${PROJECT_NAME}
  TesseractScene3D
  TesseractGridConfig
  TesseractLoad
  TesseractVideoRecorder
  TesseractIgnitionVisualization
  TesseractSetupWizard
  demo_window
  demo_dialog
  demo_tesseract)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)
