#ifndef TESSERACT_IGNITION_CONVERSIONS_H
#define TESSERACT_IGNITION_CONVERSIONS_H

#include <ignition/msgs/geometry.pb.h>
#include <ignition/msgs/material.pb.h>
#include <ignition/msgs/inertial.pb.h>
#include <ignition/msgs/collision.pb.h>
#include <ignition/msgs/visual.pb.h>
#include <ignition/msgs/link.pb.h>
#include <ignition/msgs/scene.pb.h>
#include <ignition/rendering/Scene.hh>

#include <tesseract_geometry/geometries.h>
#include <tesseract_scene_graph/graph.h>
#include <tesseract_scene_graph/link.h>
#include <tesseract_scene_graph/joint.h>
#include <tesseract_ignition/entity_manager.h>

namespace tesseract_ignition
{

bool toMsg(ignition::msgs::Scene& scene_msg,
           EntityManager& entity_manager,
           const tesseract_scene_graph::SceneGraph& scene_graph,
           const tesseract_common::TransformMap& link_transforms);

bool toScene(ignition::rendering::Scene& scene,
             EntityManager& entity_manager,
             const tesseract_scene_graph::SceneGraph& scene_graph,
             const tesseract_common::TransformMap& link_transforms);

}

#endif // TESSERACT_IGNITION_CONVERSIONS_H
