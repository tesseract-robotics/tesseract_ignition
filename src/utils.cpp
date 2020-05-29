#include <tesseract_ignition/utils.h>
#include <unordered_map>

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
      char* ros_package_paths = std::getenv("ROS_PACKAGE_PATH");
      std::vector<std::string> tokens;
      boost::split(tokens, ros_package_paths, boost::is_any_of(":"), boost::token_compress_on);
      for (const auto& token : tokens)
      {
        QDir d(QString::fromStdString(token));
        if (d.exists())
          cache_package_paths[d.dirName().toStdString()] = token;
      }
    }
    auto find_package = cache_package_paths.find(package);

    if (find_package != cache_package_paths.end())
      mod_url = find_package->second + mod_url;
    else
      return std::string();
  }

  return mod_url;
}
}
