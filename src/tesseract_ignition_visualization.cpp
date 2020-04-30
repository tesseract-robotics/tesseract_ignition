
#include <tesseract_ignition/tesseract_ignition_visualization.h>
#include <tesseract_ignition/conversions.h>

#include <ignition/transport/Node.hh>
#include <ignition/msgs/MessageTypes.hh>
#include <ignition/common/Console.hh>

static const std::string DEFAULT_SCENE_SERVICE_NAME =  "/tesseract_ignition/service/scene"; // ignition::msgs::Scene
static const std::string DEFAULT_SCENE_TOPIC_NAME =    "/tesseract_ignition/topic/scene"; // ignition::msgs::Scene
static const std::string DEFAULT_POSE_TOPIC_NAME =     "/tesseract_ignition/topic/pose"; // ignition::msgs::Pose_V
static const std::string DEFAULT_DELETION_TOPIC_NAME = "/tesseract_ignition/topic/deletion"; // ignition::msgs::UInt32_V

using namespace tesseract_ignition;

TesseractIgnitionVisualization::TesseractIgnitionVisualization(tesseract_environment::Environment::ConstPtr env)
  : env_(std::move(env))
{
  ;
  // Advertise a service call.
  if (!node_.Advertise(DEFAULT_SCENE_SERVICE_NAME, &TesseractIgnitionVisualization::OnSceneRequest, this))
  {
    ignerr << "Error advertising service [" << DEFAULT_SCENE_SERVICE_NAME << "]" << std::endl;
  }
  scene_pub_ = node_.Advertise<ignition::msgs::Scene>(DEFAULT_SCENE_TOPIC_NAME);
}

bool TesseractIgnitionVisualization::OnSceneRequest(ignition::msgs::Scene &msg)
{
  bool success = true;
  toMsg(msg, *(env_->getSceneGraph()), env_->getCurrentState()->link_transforms);
  ignerr << "TesseractIgnitionVisualization::OnSceneRequest" << std::endl;
  return success;
}

void TesseractIgnitionVisualization::publishInitialScene()
{
  if (initial_scene_published_)
    return;

  ignition::msgs::Scene scene_msg;
  toMsg(scene_msg, *(env_->getSceneGraph()), env_->getCurrentState()->link_transforms);
  scene_pub_.Publish(scene_msg);
  initial_scene_published_ = true;
}

void TesseractIgnitionVisualization::plotTrajectory(const std::vector<std::string>& joint_names,
                                                    const Eigen::Ref<const tesseract_common::TrajArray>& traj)
{
//    tesseract_msgs::Trajectory msg;

//    // Set tesseract state information
//    toMsg(msg.tesseract_state, *env_);

//    // Set the joint trajectory message
//    toMsg(msg.joint_trajectory, *(env_->getCurrentState()), joint_names, traj);

//    trajectory_pub_.publish(msg);
}

//static visualization_msgs::MarkerArray
//getContactResultsMarkerArrayMsg(int& id_counter,
//                                const std::string& frame_id,
//                                const std::string& ns,
//                                const ros::Time& time_stamp,
//                                const std::vector<std::string>& link_names,
//                                const tesseract_collision::ContactResultVector& dist_results,
//                                const Eigen::Ref<const Eigen::VectorXd>& safety_distances)
//{
//    visualization_msgs::MarkerArray msg;
//    for (unsigned i = 0; i < dist_results.size(); ++i)
//    {
//      const tesseract_collision::ContactResult& dist = dist_results[i];
//      const double& safety_distance = safety_distances[i];

//      Eigen::Vector4d rgba;
//      if (dist.distance < 0)
//      {
//        rgba << 1.0, 0.0, 0.0, 1.0;
//      }
//      else if (dist.distance < safety_distance)
//      {
//        rgba << 1.0, 1.0, 0.0, 1.0;
//      }
//      else
//      {
//        rgba << 0.0, 1.0, 0.0, 1.0;
//      }

//      if (dist.cc_type[0] == tesseract_collision::ContinuousCollisionType::CCType_Between)
//      {
//        Eigen::Vector4d cc_rgba;
//        cc_rgba << 0.0, 0.0, 1.0, 1.0;
//        auto marker = getMarkerArrowMsg(id_counter,
//                                        frame_id,
//                                        ns,
//                                        time_stamp,
//                                        dist.transform[0] * dist.nearest_points_local[0],
//                                        dist.cc_transform[0] * dist.nearest_points_local[0],
//                                        cc_rgba,
//                                        0.01);
//        msg.markers.push_back(marker);
//      }

//      if (dist.cc_type[1] == tesseract_collision::ContinuousCollisionType::CCType_Between)
//      {
//        Eigen::Vector4d cc_rgba;
//        cc_rgba << 0.0, 0.0, 0.5, 1.0;
//        auto marker = getMarkerArrowMsg(id_counter,
//                                        frame_id,
//                                        ns,
//                                        time_stamp,
//                                        dist.transform[1] * dist.nearest_points_local[1],
//                                        dist.cc_transform[1] * dist.nearest_points_local[1],
//                                        cc_rgba,
//                                        0.01);
//        msg.markers.push_back(marker);
//      }

//      auto it0 = std::find(link_names.begin(), link_names.end(), dist.link_names[0]);
//      auto it1 = std::find(link_names.begin(), link_names.end(), dist.link_names[1]);

//      if (it0 != link_names.end() && it1 != link_names.end())
//      {
//        auto marker0 = getMarkerArrowMsg(
//            id_counter, frame_id, ns, time_stamp, dist.nearest_points[0], dist.nearest_points[1], rgba, 0.01);
//        msg.markers.push_back(marker0);

//        auto marker1 = getMarkerArrowMsg(
//            id_counter, frame_id, ns, time_stamp, dist.nearest_points[1], dist.nearest_points[0], rgba, 0.01);
//        msg.markers.push_back(marker1);
//      }
//      else if (it0 != link_names.end())
//      {
//        auto marker = getMarkerArrowMsg(
//            id_counter, frame_id, ns, time_stamp, dist.nearest_points[1], dist.nearest_points[0], rgba, 0.01);
//        msg.markers.push_back(marker);
//      }
//      else
//      {
//        auto marker = getMarkerArrowMsg(
//            id_counter, frame_id, ns, time_stamp, dist.nearest_points[0], dist.nearest_points[1], rgba, 0.01);
//        msg.markers.push_back(marker);
//      }
//    }

//    return msg;
//}

void TesseractIgnitionVisualization::plotContactResults(const std::vector<std::string>& link_names,
                                                        const tesseract_collision::ContactResultVector& dist_results,
                                                        const Eigen::Ref<const Eigen::VectorXd>& safety_distances)
{
//    visualization_msgs::MarkerArray msg = getContactResultsMarkerArrayMsg(marker_counter_,
//                                                                          env_->getSceneGraph()->getRoot(),
//                                                                          "trajopt",
//                                                                          ros::Time::now(),
//                                                                          link_names,
//                                                                          dist_results,
//                                                                          safety_distances);
//    if (!dist_results.empty())
//    {
//      collisions_pub_.publish(msg);
//    }
}

void TesseractIgnitionVisualization::plotArrow(const Eigen::Ref<const Eigen::Vector3d>& pt1,
                                               const Eigen::Ref<const Eigen::Vector3d>& pt2,
                                               const Eigen::Ref<const Eigen::Vector4d>& rgba,
                                               double scale)
{
//    visualization_msgs::MarkerArray msg;
//    auto marker = getMarkerArrowMsg(
//        marker_counter_, env_->getSceneGraph()->getRoot(), "trajopt", ros::Time::now(), pt1, pt2, rgba, scale);
//    msg.markers.push_back(marker);
//    arrows_pub_.publish(msg);
}

void TesseractIgnitionVisualization::plotAxis(const Eigen::Isometry3d& axis, double scale)
{
//    visualization_msgs::MarkerArray msg;
//    Eigen::Vector3d x_axis = axis.matrix().block<3, 1>(0, 0);
//    Eigen::Vector3d y_axis = axis.matrix().block<3, 1>(0, 1);
//    Eigen::Vector3d z_axis = axis.matrix().block<3, 1>(0, 2);
//    Eigen::Vector3d position = axis.matrix().block<3, 1>(0, 3);

//    auto marker = getMarkerCylinderMsg(marker_counter_,
//                                       env_->getSceneGraph()->getRoot(),
//                                       "trajopt",
//                                       ros::Time::now(),
//                                       position,
//                                       position + x_axis,
//                                       Eigen::Vector4d(1, 0, 0, 1),
//                                       scale);
//    msg.markers.push_back(marker);

//    marker = getMarkerCylinderMsg(marker_counter_,
//                                  env_->getSceneGraph()->getRoot(),
//                                  "trajopt",
//                                  ros::Time::now(),
//                                  position,
//                                  position + y_axis,
//                                  Eigen::Vector4d(0, 1, 0, 1),
//                                  scale);
//    msg.markers.push_back(marker);

//    marker = getMarkerCylinderMsg(marker_counter_,
//                                  env_->getSceneGraph()->getRoot(),
//                                  "trajopt",
//                                  ros::Time::now(),
//                                  position,
//                                  position + z_axis,
//                                  Eigen::Vector4d(0, 0, 1, 1),
//                                  scale);
//    msg.markers.push_back(marker);

//    axes_pub_.publish(msg);
}

void TesseractIgnitionVisualization::clear()
{
//    // Remove old arrows
//    marker_counter_ = 0;
//    visualization_msgs::MarkerArray msg;
//    visualization_msgs::Marker marker;
//    marker.header.frame_id = env_->getSceneGraph()->getRoot();
//    marker.header.stamp = ros::Time();
//    marker.ns = "trajopt";
//    marker.id = 0;
//    marker.type = visualization_msgs::Marker::ARROW;
//    marker.action = visualization_msgs::Marker::DELETEALL;
//    msg.markers.push_back(marker);
//    collisions_pub_.publish(msg);
//    arrows_pub_.publish(msg);
//    axes_pub_.publish(msg);

//    ros::Duration(0.5).sleep();
}

void TesseractIgnitionVisualization::waitForInput()
{
  ignwarn  << "Hit enter key to step optimization!" << std::endl;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//  static visualization_msgs::Marker getMarkerArrowMsg(int& id_counter,
//                                                      const std::string& frame_id,
//                                                      const std::string& ns,
//                                                      const ros::Time& time_stamp,
//                                                      const Eigen::Ref<const Eigen::Vector3d>& pt1,
//                                                      const Eigen::Ref<const Eigen::Vector3d>& pt2,
//                                                      const Eigen::Ref<const Eigen::Vector4d>& rgba,
//                                                      const double scale)
//  {
//    visualization_msgs::Marker marker;
//    marker.header.frame_id = frame_id;
//    marker.header.stamp = time_stamp;
//    marker.ns = ns;
//    marker.id = ++id_counter;
//    marker.type = visualization_msgs::Marker::ARROW;
//    marker.action = visualization_msgs::Marker::ADD;

//    Eigen::Vector3d x, y, z;
//    x = (pt2 - pt1).normalized();
//    marker.pose.position.x = pt1(0);
//    marker.pose.position.y = pt1(1);
//    marker.pose.position.z = pt1(2);

//    y = x.unitOrthogonal();
//    z = (x.cross(y)).normalized();
//    Eigen::Matrix3d rot;
//    rot.col(0) = x;
//    rot.col(1) = y;
//    rot.col(2) = z;
//    Eigen::Quaterniond q(rot);
//    q.normalize();
//    marker.pose.orientation.x = q.x();
//    marker.pose.orientation.y = q.y();
//    marker.pose.orientation.z = q.z();
//    marker.pose.orientation.w = q.w();

//    marker.scale.x = std::abs((pt2 - pt1).norm());
//    marker.scale.y = scale;
//    marker.scale.z = scale;

//    marker.color.r = static_cast<float>(rgba(0));
//    marker.color.g = static_cast<float>(rgba(1));
//    marker.color.b = static_cast<float>(rgba(2));
//    marker.color.a = static_cast<float>(rgba(3));

//    return marker;
//  }

//  static visualization_msgs::Marker getMarkerCylinderMsg(int& id_counter,
//                                                         const std::string& frame_id,
//                                                         const std::string& ns,
//                                                         const ros::Time& time_stamp,
//                                                         const Eigen::Ref<const Eigen::Vector3d>& pt1,
//                                                         const Eigen::Ref<const Eigen::Vector3d>& pt2,
//                                                         const Eigen::Ref<const Eigen::Vector4d>& rgba,
//                                                         const double scale)
//  {
//    visualization_msgs::Marker marker;
//    marker.header.frame_id = frame_id;
//    marker.header.stamp = time_stamp;
//    marker.ns = ns;
//    marker.id = ++id_counter;
//    marker.type = visualization_msgs::Marker::CYLINDER;
//    marker.action = visualization_msgs::Marker::ADD;

//    double length = scale * std::abs((pt2 - pt1).norm());
//    Eigen::Vector3d x, y, z, center;
//    z = (pt2 - pt1).normalized();
//    center = pt1 + (length / 2.0) * z;
//    marker.pose.position.x = center(0);
//    marker.pose.position.y = center(1);
//    marker.pose.position.z = center(2);

//    y = z.unitOrthogonal();
//    x = (y.cross(z)).normalized();
//    Eigen::Matrix3d rot;
//    rot.col(0) = x;
//    rot.col(1) = y;
//    rot.col(2) = z;
//    Eigen::Quaterniond q(rot);
//    q.normalize();
//    marker.pose.orientation.x = q.x();
//    marker.pose.orientation.y = q.y();
//    marker.pose.orientation.z = q.z();
//    marker.pose.orientation.w = q.w();

//    marker.scale.x = length / 20.0;
//    marker.scale.y = length / 20.0;
//    marker.scale.z = length;

//    marker.color.r = static_cast<float>(rgba(0));
//    marker.color.g = static_cast<float>(rgba(1));
//    marker.color.b = static_cast<float>(rgba(2));
//    marker.color.a = static_cast<float>(rgba(3));

//    return marker;
//  }
