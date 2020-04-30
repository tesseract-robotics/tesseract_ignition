#ifndef TESSERACT_IGNITION_CONVERSIONS_H
#define TESSERACT_IGNITION_CONVERSIONS_H

#include <ignition/msgs/geometry.pb.h>
#include <ignition/msgs/material.pb.h>
#include <ignition/msgs/inertial.pb.h>
#include <ignition/msgs/collision.pb.h>
#include <ignition/msgs/visual.pb.h>
#include <ignition/msgs/link.pb.h>
#include <ignition/msgs/scene.pb.h>

#include <tesseract_geometry/geometries.h>
#include <tesseract_scene_graph/graph.h>
#include <tesseract_scene_graph/link.h>
#include <tesseract_scene_graph/joint.h>

namespace tesseract_ignition
{
bool toMsg(ignition::msgs::Scene& scene_msg,
           const tesseract_scene_graph::SceneGraph& scene_graph,
           const tesseract_common::TransformMap& link_transforms);

bool toMsg(ignition::msgs::Geometry& geometry_msg, const tesseract_geometry::Geometry& geometry);

bool toMsg(ignition::msgs::Material& material_msg, const tesseract_scene_graph::Material::Ptr& material);

bool toMsg(ignition::msgs::Inertial& inertial_msg, const tesseract_scene_graph::Inertial::Ptr& inertial);

bool toMsg(ignition::msgs::Visual& visual_msg, const tesseract_scene_graph::Visual& visual);

bool toMsg(ignition::msgs::Collision& collision_msg, const tesseract_scene_graph::Collision& collision);

bool toMsg(ignition::msgs::Link& link_msg, const tesseract_scene_graph::Link& link);

//bool toMsg(tesseract_msgs::JointCalibration& joint_calibration_msg,
//           const tesseract_scene_graph::JointCalibration::Ptr& joint_calibration);

//bool toMsg(tesseract_msgs::JointDynamics& joint_dynamics_msg,
//           const tesseract_scene_graph::JointDynamics::Ptr& joint_dynamics);

//bool toMsg(tesseract_msgs::JointLimits& joint_limits_msg, const tesseract_scene_graph::JointLimits::Ptr& joint_limits);

//bool toMsg(tesseract_msgs::JointMimic& joint_mimic_msg, const tesseract_scene_graph::JointMimic::Ptr& joint_mimic);

//bool toMsg(tesseract_msgs::JointSafety& joint_safety_msg, const tesseract_scene_graph::JointSafety::Ptr& joint_safety);

//bool toMsg(tesseract_msgs::Joint& joint_msg, const tesseract_scene_graph::Joint& joint);

//bool toMsg(tesseract_msgs::EnvironmentCommand& command_msg, const tesseract_environment::Command& command);

///**
// * @brief Generate a JointTrajectory Message that contains all joints in the environment
// * @param traj_msg The output JointTrajectory Message
// * @param start_state The Environment start/current state
// * @param joint_names The joint names corresponding to the trajectory
// * @param traj The joint trajectory
// */
//void toMsg(trajectory_msgs::JointTrajectory& traj_msg,
//           const tesseract_environment::EnvState& start_state,
//           const std::vector<std::string>& joint_names,
//           const Eigen::Ref<const tesseract_common::TrajArray>& traj);

///**
// * @brief Generate a JointTrajectory Message that contains all joints in the environment
// * @param traj_msg The output JointTrajectory Message
// * @param start_state The Environment start/current state
// * @param joint_names The joint names corresponding to the trajectory
// * @param traj The joint trajectory
// */
//void toMsg(const trajectory_msgs::JointTrajectoryPtr& traj_msg,
//           const tesseract_environment::EnvState& start_state,
//           const std::vector<std::string>& joint_names,
//           const Eigen::Ref<const tesseract_common::TrajArray>& traj);

///**
// * @brief Generate a JointTrajectory Message that contains only trajectory joints
// * @param traj_msg The output JointTrajectory Message
// * @param joint_names The joint names corresponding to the trajectory
// * @param traj The joint trajectory
// */
//void toMsg(trajectory_msgs::JointTrajectory& traj_msg, const tesseract_common::JointTrajectory& traj);

///**
// * @brief Generate a JointTrajectory Message that contains only trajectory joints
// * @param traj_msg The output JointTrajectory Message
// * @param joint_names The joint names corresponding to the trajectory
// * @param traj The joint trajectory
// */
//void toMsg(const trajectory_msgs::JointTrajectoryPtr& traj_msg, const tesseract_common::JointTrajectory& traj);

///**
// * @brief Convert Eigen to Geometry Pose Message
// * @param pose_msg Geometry Pose Message to filled out
// * @param pose The Eigen type to be converted
// * @return True if successful, otherwise false
// */
//bool toMsg(geometry_msgs::Pose& pose_msg, const Eigen::Isometry3d& pose);

//void toMsg(tesseract_msgs::ContactResult& contact_result_msg,
//           const tesseract_collision::ContactResult& contact_result,
//           const ros::Time& stamp = ros::Time::now());

//void toMsg(const tesseract_msgs::ContactResultPtr& contact_result_msg,
//           const tesseract_collision::ContactResult& contact_result,
//           const ros::Time& stamp = ros::Time::now());

//bool toMsg(sensor_msgs::JointState& joint_state, const tesseract_motion_planners::Waypoint& waypoint);

///**
// * @brief Convert a vector of waypoints into a pose array
// * @param Pose Array
// * @param waypoints A vector of waypoints
// * @return True if successful, otherwise false
// */
//bool toMsg(geometry_msgs::PoseArray& pose_array,
//           const std::vector<tesseract_motion_planners::Waypoint::Ptr>& waypoints);

///**
// * @brief Convert a vector of Eigen::Isometry3d into a pose array
// * @param Pose Array
// * @param transforms A vector of transforms
// * @return True if successful, otherwise false
// */
//bool toMsg(geometry_msgs::PoseArray& pose_array, const tesseract_common::VectorIsometry3d& transforms);

///**
// * @brief Convert a process definition into a single pose array
// * @param pose_array A pose array to load the process definition into.
// * @param process_definition A process definition
// * @return True if successful, otherwise false
// */
//bool toMsg(geometry_msgs::PoseArray& pose_array,
//           const tesseract_process_planners::ProcessDefinition& process_definition);

}

#endif // TESSERACT_IGNITION_CONVERSIONS_H
