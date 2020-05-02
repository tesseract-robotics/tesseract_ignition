
#include <tesseract_ignition/tesseract_ignition_visualization.h>
#include <tesseract_ignition/conversions.h>

#include <ignition/transport/Node.hh>
#include <ignition/msgs/MessageTypes.hh>
#include <ignition/common/Console.hh>
#include <ignition/math/eigen3/Conversions.hh>
#include <chrono>

static const std::string DEFAULT_SCENE_SERVICE_NAME =  "/tesseract_ignition/service/scene"; // ignition::msgs::Scene
static const std::string DEFAULT_SCENE_TOPIC_NAME =    "/tesseract_ignition/topic/scene"; // ignition::msgs::Scene
static const std::string DEFAULT_POSE_TOPIC_NAME =     "/tesseract_ignition/topic/pose"; // ignition::msgs::Pose_V
static const std::string DEFAULT_DELETION_TOPIC_NAME = "/tesseract_ignition/topic/deletion"; // ignition::msgs::UInt32_V
static const std::string COLLISION_RESULTS_MODEL_NAME = "tesseract_collision_results_model";
static const std::string AXES_MODEL_NAME = "tesseract_axes_model";
static const std::string ARROW_MODEL_NAME = "tesseract_arrow_model";

using namespace tesseract_ignition;

TesseractIgnitionVisualization::TesseractIgnitionVisualization(tesseract_environment::Environment::ConstPtr env)
  : env_(std::move(env))
{
  // Advertise a service call.
  if (!node_.Advertise(DEFAULT_SCENE_SERVICE_NAME, &TesseractIgnitionVisualization::OnSceneRequest, this))
  {
    ignerr << "Error advertising service [" << DEFAULT_SCENE_SERVICE_NAME << "]" << std::endl;
  }
  scene_pub_ = node_.Advertise<ignition::msgs::Scene>(DEFAULT_SCENE_TOPIC_NAME);
  pose_pub_  = node_.Advertise<ignition::msgs::Pose_V>(DEFAULT_POSE_TOPIC_NAME);
  deletion_pub_  = node_.Advertise<ignition::msgs::Pose_V>(DEFAULT_DELETION_TOPIC_NAME);
}

bool TesseractIgnitionVisualization::OnSceneRequest(ignition::msgs::Scene &msg)
{
  bool success = true;
  toMsg(msg, entity_manager_, *(env_->getSceneGraph()), env_->getCurrentState()->link_transforms);
  ignerr << "TesseractIgnitionVisualization::OnSceneRequest" << std::endl;
  return success;
}

void TesseractIgnitionVisualization::plotTrajectory(const std::vector<std::string>& joint_names,
                                                    const Eigen::Ref<const tesseract_common::TrajArray>& traj)
{
  tesseract_environment::StateSolver::Ptr state_solver = env_->getStateSolver();

  std::chrono::duration<double> fp_s(1.0/traj.rows());
  for (long i = 0; i < traj.rows(); ++i)
  {
    ignition::msgs::Pose_V pose_v;
    tesseract_environment::EnvState::Ptr state = state_solver->getState(joint_names, traj.row(i));
    for (const auto& pair : state->link_transforms)
    {
      ignition::msgs::Pose* pose = pose_v.add_pose();
      pose->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(pair.second)));
      pose->set_name(pair.first);
      pose->set_id(static_cast<unsigned>(entity_manager_.getLink(pair.first)));
    }

    std::this_thread::sleep_for(fp_s);
    if(!pose_pub_.Publish(pose_v))
    {
      ignerr << "Failed to publish pose vector!" << std::endl;
    }
  }
}

void addArrow(EntityManager& entity_manager,
              ignition::msgs::Link& link,
              long& sub_index,
              const std::string& parent_name,
              const Eigen::Ref<const Eigen::Vector3d>& pt1,
              const Eigen::Ref<const Eigen::Vector3d>& pt2,
              const Eigen::Ref<const Eigen::Vector4d>& rgba,
              double radius)
{
  std::string gv_name = parent_name + "_" + std::to_string(++sub_index);
  ignition::msgs::Visual* gv_msg = link.add_visual();
  gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
  gv_msg->set_name(gv_name);

  Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
  Eigen::Vector3d x, y, z;
  x = (pt2 - pt1).normalized();
  pose.translation() = x;
  y = x.unitOrthogonal();
  z = (x.cross(y)).normalized();
  Eigen::Matrix3d rot;
  rot.col(0) = x;
  rot.col(1) = y;
  rot.col(2) = z;
  pose.linear() = rot;

  gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(pose)));

  ignition::msgs::Geometry geometry_msg;
  geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_CYLINDER);
  ignition::msgs::CylinderGeom shape_geometry_msg;
  shape_geometry_msg.set_radius(radius);
  shape_geometry_msg.set_length((pt2 - pt1).norm());
  geometry_msg.mutable_sphere()->CopyFrom(shape_geometry_msg);
  gv_msg->mutable_geometry()->CopyFrom(geometry_msg);
//          gv_msg.mutable_material()
  gv_msg->set_parent_name(parent_name);

//    marker.color.r = static_cast<float>(rgba(0));
//    marker.color.g = static_cast<float>(rgba(1));
//    marker.color.b = static_cast<float>(rgba(2));
//    marker.color.a = static_cast<float>(rgba(3));

}

void addCylinder(EntityManager& entity_manager,
                 ignition::msgs::Link& link,
                 long& sub_index,
                 const std::string& parent_name,
                 const Eigen::Ref<const Eigen::Vector3d>& pt1,
                 const Eigen::Ref<const Eigen::Vector3d>& pt2,
                 const Eigen::Ref<const Eigen::Vector4d>& rgba,
                 double radius)
{
  std::string gv_name = parent_name + "_" + std::to_string(++sub_index);
  ignition::msgs::Visual* gv_msg = link.add_visual();
  gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
  gv_msg->set_name(gv_name);

  Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
  Eigen::Vector3d x, y, z;
  x = (pt2 - pt1).normalized();
  pose.translation() = x;
  y = x.unitOrthogonal();
  z = (x.cross(y)).normalized();
  Eigen::Matrix3d rot;
  rot.col(0) = x;
  rot.col(1) = y;
  rot.col(2) = z;
  pose.linear() = rot;

  gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(pose)));

  ignition::msgs::Geometry geometry_msg;
  geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_CYLINDER);
  ignition::msgs::CylinderGeom shape_geometry_msg;
  shape_geometry_msg.set_radius(radius);
  shape_geometry_msg.set_length((pt2 - pt1).norm());
  geometry_msg.mutable_sphere()->CopyFrom(shape_geometry_msg);
  gv_msg->mutable_geometry()->CopyFrom(geometry_msg);
//          gv_msg.mutable_material()
  gv_msg->set_parent_name(parent_name);

//    marker.color.r = static_cast<float>(rgba(0));
//    marker.color.g = static_cast<float>(rgba(1));
//    marker.color.b = static_cast<float>(rgba(2));
//    marker.color.a = static_cast<float>(rgba(3));
}

void TesseractIgnitionVisualization::plotContactResults(const std::vector<std::string>& link_names,
                                                        const tesseract_collision::ContactResultVector& dist_results,
                                                        const Eigen::Ref<const Eigen::VectorXd>& safety_distances)
{
  ignition::msgs::Scene scene_msg;
  scene_msg.set_name("scene");
  ignition::msgs::Model* model = scene_msg.add_model();
  std::string model_name = COLLISION_RESULTS_MODEL_NAME;
  model->set_name(model_name);
  model->set_id(static_cast<unsigned>(entity_manager_.addModel(model_name)));

  long cnt = 0;
  for (size_t i = 0; i < dist_results.size(); ++i)
  {
    const tesseract_collision::ContactResult& dist = dist_results[i];
    const double& safety_distance = safety_distances[static_cast<long>(i)];

    std::string link_name = model_name + std::to_string(++cnt);
    ignition::msgs::Link* link_msg = model->add_link();
    link_msg->set_id(static_cast<unsigned>(entity_manager_.addVisual(link_name)));
    link_msg->set_name(link_name);

    Eigen::Vector4d rgba;
    if (dist.distance < 0)
    {
      rgba << 1.0, 0.0, 0.0, 1.0;
    }
    else if (dist.distance < safety_distance)
    {
      rgba << 1.0, 1.0, 0.0, 1.0;
    }
    else
    {
      rgba << 0.0, 1.0, 0.0, 1.0;
    }

    if (dist.cc_type[0] == tesseract_collision::ContinuousCollisionType::CCType_Between)
    {
      Eigen::Vector4d cc_rgba;
      cc_rgba << 0.0, 0.0, 1.0, 1.0;
      addArrow(entity_manager_, *link_msg, cnt,
               link_name, dist.transform[0] * dist.nearest_points_local[0],
               dist.cc_transform[0] * dist.nearest_points_local[0], cc_rgba, 0.01);
    }

    if (dist.cc_type[1] == tesseract_collision::ContinuousCollisionType::CCType_Between)
    {
      Eigen::Vector4d cc_rgba;
      cc_rgba << 0.0, 0.0, 0.5, 1.0;
      addArrow(entity_manager_, *link_msg, cnt,
               link_name, dist.transform[1] * dist.nearest_points_local[1],
               dist.cc_transform[1] * dist.nearest_points_local[1], cc_rgba, 0.01);
    }

    auto it0 = std::find(link_names.begin(), link_names.end(), dist.link_names[0]);
    auto it1 = std::find(link_names.begin(), link_names.end(), dist.link_names[1]);

    if (it0 != link_names.end() && it1 != link_names.end())
    {
      addArrow(entity_manager_, *link_msg, cnt, link_name, dist.nearest_points[0], dist.nearest_points[1], rgba, 0.01);
      addArrow(entity_manager_, *link_msg, cnt, link_name, dist.nearest_points[1], dist.nearest_points[0], rgba, 0.01);
    }
    else if (it0 != link_names.end())
    {
      addArrow(entity_manager_, *link_msg, cnt, link_name, dist.nearest_points[1], dist.nearest_points[0], rgba, 0.01);
    }
    else
    {
      addArrow(entity_manager_, *link_msg, cnt, link_name, dist.nearest_points[0], dist.nearest_points[1], rgba, 0.01);
    }
  }
  scene_pub_.Publish(scene_msg);
}

void TesseractIgnitionVisualization::plotArrow(const Eigen::Ref<const Eigen::Vector3d>& pt1,
                                               const Eigen::Ref<const Eigen::Vector3d>& pt2,
                                               const Eigen::Ref<const Eigen::Vector4d>& rgba,
                                               double scale)
{
  ignition::msgs::Scene scene_msg;
  scene_msg.set_name("scene");
  ignition::msgs::Model* model = scene_msg.add_model();
  std::string model_name = ARROW_MODEL_NAME;
  model->set_name(model_name);
  model->set_id(static_cast<unsigned>(entity_manager_.addModel(model_name)));

  long cnt = 0;
  std::string link_name = model_name + std::to_string(++cnt);
  ignition::msgs::Link* link_msg = model->add_link();
  link_msg->set_id(static_cast<unsigned>(entity_manager_.addVisual(link_name)));
  link_msg->set_name(link_name);
  addArrow(entity_manager_, *link_msg, cnt, link_name, pt1, pt2, rgba, scale);
  scene_pub_.Publish(scene_msg);
}

void TesseractIgnitionVisualization::plotAxis(const Eigen::Isometry3d& axis, double scale)
{
//    visualization_msgs::MarkerArray msg;
    Eigen::Vector3d x_axis = axis.matrix().block<3, 1>(0, 0);
    Eigen::Vector3d y_axis = axis.matrix().block<3, 1>(0, 1);
    Eigen::Vector3d z_axis = axis.matrix().block<3, 1>(0, 2);
    Eigen::Vector3d position = axis.matrix().block<3, 1>(0, 3);

    ignition::msgs::Scene scene_msg;
    scene_msg.set_name("scene");
    ignition::msgs::Model* model = scene_msg.add_model();
    std::string model_name = AXES_MODEL_NAME;
    model->set_name(model_name);
    model->set_id(static_cast<unsigned>(entity_manager_.addModel(model_name)));

    long cnt = 0;
    std::string link_name = model_name + std::to_string(++cnt);
    ignition::msgs::Link* link_msg = model->add_link();
    link_msg->set_id(static_cast<unsigned>(entity_manager_.addVisual(link_name)));
    link_msg->set_name(link_name);
    addCylinder(entity_manager_, *link_msg, cnt, link_name, position, position + (scale * x_axis), Eigen::Vector4d(1, 0, 0, 1), scale);
    addCylinder(entity_manager_, *link_msg, cnt, link_name, position, position + (scale * y_axis), Eigen::Vector4d(0, 1, 0, 1), scale);
    addCylinder(entity_manager_, *link_msg, cnt, link_name, position, position + (scale * z_axis), Eigen::Vector4d(0, 0, 1, 1), scale);
    scene_pub_.Publish(scene_msg);
}

void TesseractIgnitionVisualization::clear()
{
  ignition::msgs::UInt32_V deletion_msg;
  long id = entity_manager_.getModel(COLLISION_RESULTS_MODEL_NAME);
  if (id >= 1000)
    deletion_msg.add_data(static_cast<unsigned>(id));

  id = entity_manager_.getModel(ARROW_MODEL_NAME);
  if (id >= 1000)
    deletion_msg.add_data(static_cast<unsigned>(id));

  id = entity_manager_.getModel(AXES_MODEL_NAME);
  if (id >= 1000)
    deletion_msg.add_data(static_cast<unsigned>(id));

  deletion_pub_.Publish(deletion_msg);
}

void TesseractIgnitionVisualization::waitForInput()
{
  ignwarn  << "Hit enter key to step optimization!" << std::endl;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

