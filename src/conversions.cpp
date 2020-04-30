#include <tesseract_ignition/conversions.h>
#include <ignition/msgs/Utility.hh>
#include <ignition/math/eigen3/Conversions.hh>
#include <ignition/common/Console.hh>

namespace tesseract_ignition
{

bool toMsg(ignition::msgs::Scene& scene_msg,
           const tesseract_scene_graph::SceneGraph& scene_graph,
           const tesseract_common::TransformMap& link_transforms)
{
  scene_msg.set_name("scene");
  ignition::msgs::Model* model = scene_msg.add_model();
  model->set_name(scene_graph.getName());
  for (const auto& link : scene_graph.getLinks())
  {
    ignition::msgs::Link* link_msg = model->add_link();
    link_msg->set_name(link->getName());
    link_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(link_transforms.at(link->getName()))));
//    ignition::msgs::Visual* link_visual_msg = model->add_visual();
//    link_visual_msg->set_name(link->getName());
//    link_visual_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(link_transforms.at(link->getName()))));

    for (const auto& vs : link->visual)
    {
      switch (vs->geometry->getType())
      {
//        case tesseract_geometry::GeometryType::BOX:
//        {
//          ignition::msgs::Visual* gv_msg = model->add_visual();
//          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

//          ignition::msgs::Geometry geometry_msg;
//          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_BOX);

//          auto shape = std::static_pointer_cast<const tesseract_geometry::Box>(vs->geometry);
//          ignition::msgs::BoxGeom shape_geometry_msg;
//          ignition::msgs::Vector3d shape_size;
//          shape_size.set_x(shape->getX());
//          shape_size.set_y(shape->getY());
//          shape_size.set_z(shape->getZ());
//          shape_geometry_msg.set_allocated_size(&shape_size);

//          geometry_msg.set_allocated_box(&shape_geometry_msg);
//          gv_msg->set_allocated_geometry(&geometry_msg);
////          gv_msg.set_allocated_material()

//          gv_msg->set_parent_name(link->getName());
//          break;
//        }
//        case tesseract_geometry::GeometryType::SPHERE:
//        {
//          ignition::msgs::Visual* gv_msg = model->add_visual();
//          ignition::msgs::Pose gv_pose_msg = ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin));
//          gv_msg->set_allocated_pose(&gv_pose_msg);

//          ignition::msgs::Geometry geometry_msg;
//          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_SPHERE);

//          auto shape = std::static_pointer_cast<const tesseract_geometry::Sphere>(vs->geometry);
//          ignition::msgs::SphereGeom shape_geometry_msg;
//          shape_geometry_msg.set_radius(shape->getRadius());

//          geometry_msg.set_allocated_sphere(&shape_geometry_msg);
//          gv_msg->set_allocated_geometry(&geometry_msg);
//  //          gv_msg.set_allocated_material()

//          gv_msg->set_parent_name(link->getName());
//          break;
//        }
//        case tesseract_geometry::GeometryType::CYLINDER:
//        {
//          ignition::msgs::Visual* gv_msg = model->add_visual();
//          ignition::msgs::Pose gv_pose_msg = ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin));
//          gv_msg->set_allocated_pose(&gv_pose_msg);

//          ignition::msgs::Geometry geometry_msg;
//          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_CYLINDER);

//          auto shape = std::static_pointer_cast<const tesseract_geometry::Cylinder>(vs->geometry);
//          ignition::msgs::CylinderGeom shape_geometry_msg;
//          shape_geometry_msg.set_radius(shape->getRadius());
//          shape_geometry_msg.set_length(shape->getLength());

//          geometry_msg.set_allocated_cylinder(&shape_geometry_msg);
//          gv_msg->set_allocated_geometry(&geometry_msg);
//  //          gv_msg.set_allocated_material()

//          gv_msg->set_parent_name(link->getName());
//          break;
//        }
//        case tesseract_geometry::GeometryType::CONE:
//        {
////          ignition::msgs::Visual* gv_msg = model->add_visual();
////          ignition::msgs::Pose gv_pose_msg = ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin));
////          gv_msg->set_allocated_pose(&gv_pose_msg);

////          ignition::msgs::Geometry geometry_msg;
////          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_CONE);

////          auto shape = std::static_pointer_cast<const tesseract_geometry::Cone>(vs->geometry);
////          ignition::msgs::ConeGEOM shape_geometry_msg;
////          shape_geometry_msg.set_radius(shape->getRadius());
////          shape_geometry_msg.set_length(shape->getLength());

////          geometry_msg.set_allocated_cone(&shape_geometry_msg);
////          gv_msg->set_allocated_geometry(&geometry_msg);
////  //          gv_msg.set_allocated_material()

////          gv_msg->set_parent_name(link->getName());
//          break;
//        }
//        case tesseract_geometry::GeometryType::CAPSULE:
//        {
//          //          VisualPtr capsule = ign_scene_->CreateVisual();
//          //          capsule->SetLocalPose(ignition::math::eigen3::convert(vs->origin));
//          //          capsule->AddGeometry(ign_scene_->CreateCapsule());

//          //          auto shape = std::static_pointer_cast<const tesseract_geometry::Capsule>(vs->geometry);
//          //          capsule->Scale(shape->getRadius(), shape->getRadius(), shape->getLength());
//          //          ign_link->AddChild(capsule);
//          break;
//        }
        case tesseract_geometry::GeometryType::MESH:
        {
          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          ignition::msgs::Geometry geometry_msg;
          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_MESH);

          auto shape = std::static_pointer_cast<const tesseract_geometry::Mesh>(vs->geometry);
          auto resource = shape->getResource();
          if (resource)
          {
            ignition::msgs::MeshGeom shape_geometry_msg;
            shape_geometry_msg.set_filename(resource->getFilePath());
            shape_geometry_msg.mutable_scale()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(shape->getScale())));
            geometry_msg.mutable_mesh()->CopyFrom(shape_geometry_msg);
            gv_msg->mutable_geometry()->CopyFrom(geometry_msg);
    //          gv_msg.set_allocated_material()
            gv_msg->mutable_scale()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(shape->getScale())));
            gv_msg->set_parent_name(link->getName());
          }
          else
          {
            assert(false);
          }

          break;
        }
        case tesseract_geometry::GeometryType::CONVEX_MESH:
        {
          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          ignition::msgs::Geometry geometry_msg;
          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_MESH);

          auto shape = std::static_pointer_cast<const tesseract_geometry::ConvexMesh>(vs->geometry);
          auto resource = shape->getResource();
          if (resource)
          {
            ignition::msgs::MeshGeom shape_geometry_msg;
            shape_geometry_msg.set_filename(resource->getFilePath());
            geometry_msg.mutable_mesh()->CopyFrom(shape_geometry_msg);
            gv_msg->mutable_geometry()->CopyFrom(geometry_msg);
    //          gv_msg.set_allocated_material()

            gv_msg->set_parent_name(link->getName());
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
          ignerr << "This geometric shape type " << static_cast<int>(vs->geometry->getType()) << " is not supported";
          break;
        }
      }
    }
  }
}

bool toMsg(ignition::msgs::Geometry& geometry_msg, const tesseract_geometry::Geometry& geometry)
{

}

bool toMsg(ignition::msgs::Material& material_msg, const tesseract_scene_graph::Material::Ptr& material)
{

}

bool toMsg(ignition::msgs::Inertial& inertial_msg, const tesseract_scene_graph::Inertial::Ptr& inertial)
{

}

bool toMsg(ignition::msgs::Visual& visual_msg, const tesseract_scene_graph::Visual& visual)
{

}

bool toMsg(ignition::msgs::Collision& collision_msg, const tesseract_scene_graph::Collision& collision)
{

}

bool toMsg(ignition::msgs::Link& link_msg, const tesseract_scene_graph::Link& link)
{

}
}
