#include <tesseract_ignition/utils.h>
#include <unordered_map>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <ignition/common/Console.hh>
#include <QDir>

static std::unordered_map<std::string, std::string> cache_package_paths;

namespace tesseract_ignition
{

std::string locateResource(const std::string& url)
{
  std::string mod_url = url;
  if (url.find("file:///") == 0)
  {
    mod_url.erase(0, strlen("file://"));
    size_t pos = mod_url.find('/');
    if (pos == std::string::npos)
    {
      return std::string();
    }
  }
  else if (url.find("package://") == 0)
  {
    mod_url.erase(0, strlen("package://"));
    size_t pos = mod_url.find('/');
    if (pos == std::string::npos)
    {
      return std::string();
    }

    std::string package = mod_url.substr(0, pos);
    mod_url.erase(0, pos);

    if (cache_package_paths.empty())
    {
      // This was added to allow user defined resource path
      // When using this within a snap you can map host ros package paths to this environment variable
      char* tsw_resource_paths = std::getenv("TSW_RESOURCE_PATH");
      if (tsw_resource_paths != nullptr)
      {
        std::vector<std::string> tokens;
        boost::split(tokens, tsw_resource_paths, boost::is_any_of(":"), boost::token_compress_on);
        for (const auto& token : tokens)
        {
          QDir d(QString::fromStdString(token));
          if (d.exists())
          {
            if (cache_package_paths.find(d.dirName().toStdString()) == cache_package_paths.end())
              cache_package_paths[d.dirName().toStdString()] = token;
          }
          else
          {
            ignwarn << "ROS Package Path does not exist: " << token << std::endl;
          }
        }
      }

      char* ros_package_paths = std::getenv("ROS_PACKAGE_PATH");
      if (ros_package_paths != nullptr)
      {
        std::vector<std::string> tokens;
        boost::split(tokens, ros_package_paths, boost::is_any_of(":"), boost::token_compress_on);
        for (const auto& token : tokens)
        {
          QDir d(QString::fromStdString(token));
          if (d.exists())
          {
            if (cache_package_paths.find(d.dirName().toStdString()) == cache_package_paths.end())
              cache_package_paths[d.dirName().toStdString()] = token;
          }
          else
          {
            ignwarn << "ROS Package Path does not exist: " << token << std::endl;
          }
        }
      }
    }
    auto find_package = cache_package_paths.find(package);

    if (find_package != cache_package_paths.end())
    {
      mod_url = find_package->second + mod_url;
    }
    else
    {
      ignerr << "Failed to find package resource " << package << " for " << url << std::endl;
      return std::string();
    }
  }
  else
  {
    ignwarn << "Resource not handled: " << mod_url << std::endl;
  }

  return mod_url;
}
}
