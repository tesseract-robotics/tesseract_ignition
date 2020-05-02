#include <tesseract_ignition/entity_manager.h>

namespace tesseract_ignition
{

int EntityManager::addModel(const std::string& name)
{
  model_id_map_[name] = ++entity_counter_;
  return model_id_map_[name];
}

int EntityManager::getModel(const std::string& name)
{
  auto it = model_id_map_.find(name);
  if (it == model_id_map_.end())
    return 0;

  return it->second;
}

int EntityManager::addLink(const std::string& name)
{
  link_id_map_[name] = ++entity_counter_;
  return link_id_map_[name];
}

int EntityManager::getLink(const std::string& name)
{
  auto it = link_id_map_.find(name);
  if (it == link_id_map_.end())
    return 0;

  return it->second;
}

int EntityManager::addVisual(const std::string& name)
{
  visual_id_map_[name] = ++entity_counter_;
  return visual_id_map_[name];
}

int EntityManager::getVisual(const std::string& name)
{
  auto it = visual_id_map_.find(name);
  if (it == visual_id_map_.end())
    return 0;

  return it->second;
}
}
