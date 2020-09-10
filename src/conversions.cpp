/**
 * @file conversions.cpp
 * @brief A set of conversion between Tesseract and Ignition Robotics objects
 *
 * @author Levi Armstrong
 * @date May 14, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ignition/msgs/Utility.hh>
#include <ignition/math/eigen3/Conversions.hh>
#include <ignition/common/Console.hh>
#include <ignition/common/MeshManager.hh>
#include <console_bridge/console.h>

#include <tesseract_ignition/conversions.h>
#include <tesseract_geometry/geometries.h>
#include <tesseract_visualization/ignition/conversions.h>

namespace tesseract_ignition
{

bool toScene(ignition::rendering::Scene& scene,
             tesseract_visualization::EntityManager& entity_manager,
             const tesseract_scene_graph::SceneGraph& scene_graph,
             const tesseract_common::TransformMap& link_transforms)
{
  ignition::rendering::VisualPtr root = scene.RootVisual();

  for (const auto& link : scene_graph.getLinks())
  {
    unsigned id = static_cast<unsigned>(entity_manager.addLink(link->getName()));
    ignition::rendering::VisualPtr ign_link = scene.CreateVisual(id, link->getName());
    ign_link->SetWorldPose(ignition::math::eigen3::convert(link_transforms.at(link->getName())));
    int cnt = 0;
    for (const auto& vs : link->visual)
    {
      std::string gv_name = link->getName() + std::to_string(++cnt);
      switch (vs->geometry->getType())
      {
        case tesseract_geometry::GeometryType::BOX:
        {
          unsigned gv_id = static_cast<unsigned>(entity_manager.addVisual(gv_name));
          ignition::rendering::VisualPtr box = scene.CreateVisual(gv_id, gv_name);
          box->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
          box->AddGeometry(scene.CreateBox());

          auto shape = std::static_pointer_cast<const tesseract_geometry::Box>(vs->geometry);
          box->Scale(shape->getX(), shape->getY(), shape->getZ());

          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&  vs->material->texture_filename.empty())
          {
            const Eigen::Vector4d& rgba = vs->material->color;
            box->Material()->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
          }

          ign_link->AddChild(box);
          break;
        }
        case tesseract_geometry::GeometryType::SPHERE:
        {
          unsigned gv_id = static_cast<unsigned>(entity_manager.addVisual(gv_name));
          ignition::rendering::VisualPtr sphere = scene.CreateVisual(gv_id, gv_name);
          sphere->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
          sphere->AddGeometry(scene.CreateSphere());

          auto shape = std::static_pointer_cast<const tesseract_geometry::Sphere>(vs->geometry);
          sphere->Scale(shape->getRadius(), shape->getRadius(), shape->getRadius());

          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&  vs->material->texture_filename.empty())
          {
            const Eigen::Vector4d& rgba = vs->material->color;
            sphere->Material()->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
          }

          ign_link->AddChild(sphere);
          break;
        }
        case tesseract_geometry::GeometryType::CYLINDER:
        {
          unsigned gv_id = static_cast<unsigned>(entity_manager.addVisual(gv_name));
          ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_id, gv_name);
          cylinder->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
          cylinder->AddGeometry(scene.CreateCylinder());

          auto shape = std::static_pointer_cast<const tesseract_geometry::Cylinder>(vs->geometry);
          cylinder->Scale(shape->getRadius(), shape->getRadius(), shape->getLength());

          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&  vs->material->texture_filename.empty())
          {
            const Eigen::Vector4d& rgba = vs->material->color;
            cylinder->Material()->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
          }

          ign_link->AddChild(cylinder);
          break;
        }
        case tesseract_geometry::GeometryType::CONE:
        {
          unsigned gv_id = static_cast<unsigned>(entity_manager.addVisual(gv_name));
          ignition::rendering::VisualPtr cone = scene.CreateVisual(gv_id, gv_name);
          cone->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
          cone->AddGeometry(scene.CreateCone());

          auto shape = std::static_pointer_cast<const tesseract_geometry::Cone>(vs->geometry);
          cone->Scale(shape->getRadius(), shape->getRadius(), shape->getLength());

          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&  vs->material->texture_filename.empty())
          {
            const Eigen::Vector4d& rgba = vs->material->color;
            cone->Material()->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
          }

          ign_link->AddChild(cone);
          break;
        }
        case tesseract_geometry::GeometryType::CAPSULE:
        {
          //          unsigned gv_id = static_cast<unsigned>(entity_manager.addVisual(gv_name));
          //          VisualPtr capsule = scene.CreateVisual(gv_id, gv_name);
          //          capsule->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
          //          capsule->AddGeometry(scene.CreateCapsule());
          //
          //          const Eigen::Vector4d& rgba = vs->material->color;
          //          capsule->Material()->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
          //
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
            unsigned gv_id = static_cast<unsigned>(entity_manager.addVisual(gv_name));
            ignition::rendering::VisualPtr mesh = scene.CreateVisual(gv_id, gv_name);
            mesh->SetLocalPose(ignition::math::eigen3::convert(vs->origin));

            ignition::rendering::MeshDescriptor descriptor;
            descriptor.meshName = resource->getFilePath();
            ignition::common::MeshManager* mesh_manager = ignition::common::MeshManager::Instance();
            descriptor.mesh = mesh_manager->Load(descriptor.meshName);
            ignition::rendering::MeshPtr mesh_geom = scene.CreateMesh(descriptor);

            if (!tesseract_visualization::isMeshWithColor(resource->getFilePath()) && vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&  vs->material->texture_filename.empty())
            {
              const Eigen::Vector4d& rgba = vs->material->color;
              mesh->Material()->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
            }

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
            unsigned gv_id = static_cast<unsigned>(entity_manager.addVisual(gv_name));
            ignition::rendering::VisualPtr mesh = scene.CreateVisual(gv_id, gv_name);
            mesh->SetLocalPose(ignition::math::eigen3::convert(vs->origin));

            ignition::rendering::MeshDescriptor descriptor;
            descriptor.meshName = resource->getFilePath();
            ignition::common::MeshManager* mesh_manager = ignition::common::MeshManager::Instance();
            descriptor.mesh = mesh_manager->Load(descriptor.meshName);
            ignition::rendering::MeshPtr mesh_geom = scene.CreateMesh(descriptor);

            if (!tesseract_visualization::isMeshWithColor(resource->getFilePath()) && vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&  vs->material->texture_filename.empty())
            {
              const Eigen::Vector4d& rgba = vs->material->color;
              mesh->Material()->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
            }

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
  return true;
}


}
