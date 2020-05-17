#include <tesseract_environment/kdl/kdl_env.h>
#include <tesseract_ignition/tesseract_ignition_visualization.h>
#include <tesseract_scene_graph/resource_locator.h>
#include <ignition/common/Console.hh>
#include <tesseract_urdf/urdf_parser.h>

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

  return mod_url;
}

tesseract_scene_graph::SceneGraph::Ptr getSceneGraph()
{
  std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";

  tesseract_scene_graph::ResourceLocator::Ptr locator =
      std::make_shared<tesseract_scene_graph::SimpleResourceLocator>(locateResource);
  return tesseract_urdf::parseURDFFile(path, locator);
}

int main(int /*_argc*/, char** /*_argv*/)
{
  auto env = std::make_shared<tesseract_environment::KDLEnv>();

  tesseract_ignition::TesseractIgnitionVisualization::Ptr plotting;
  if (env->init(getSceneGraph()))
  {
    plotting = std::make_shared<tesseract_ignition::TesseractIgnitionVisualization>(env);
  }

  long num_steps = 100;
  std::vector<std::string> joint_names = {"joint_a1", "joint_a2", "joint_a3", "joint_a4", "joint_a5", "joint_a6", "joint_a7"};
  long num_joints = static_cast<long>(joint_names.size());
  tesseract_common::TrajArray traj;
  traj.resize(num_steps, num_joints);
  for (long i = 0; i < num_steps; ++i)
  {
    traj.row(i) = Eigen::VectorXd::Zero(num_joints);
    traj(i, 0) = static_cast<double>(i) * 0.01;
    traj(i, 1) = 0.5;
  }

  plotting->waitForInput();
  plotting->plotTrajectory(joint_names, traj);

  ignition::transport::waitForShutdown();
  return 0;
}
