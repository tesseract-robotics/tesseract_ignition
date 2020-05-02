#ifndef TESSERACT_IGNITION_ENTITY_MANAGER_H
#define TESSERACT_IGNITION_ENTITY_MANAGER_H

#include <string>
#include <unordered_map>

namespace tesseract_ignition
{

class EntityManager
{
public:
  /**
   * @brief Add model name to manager and return id for model
   * @param name Name given to the model
   * @return The Entity ID
   */
  int addModel(const std::string& name);

  /**
   * @brief Given the models name return the ID
   * @param name Name of the model
   * @return The ID of the model (if < 1000 it was not found)
   */
  int getModel(const std::string& name);

  /**
   * @brief Add link name to manager and return id for model
   * @param name Name given to the link
   * @return The Entity ID
   */
  int addLink(const std::string& name);

  /**
   * @brief Given the link name return the ID
   * @param name Name of the link
   * @return The ID of the link (if < 1000 it was not found)
   */
  int getLink(const std::string& name);

  /**
   * @brief Add visual name to manager and return id for model
   * @param name Name given to the visual
   * @return The Entity ID
   */
  int addVisual(const std::string& name);

  /**
   * @brief Given the visual name return the ID
   * @param name Name of the visual
   * @return The ID of the visual (if < 1000 it was not found)
   */
  int getVisual(const std::string& name);

private:
  std::unordered_map<std::string, int> link_id_map_;  /**< Stores entity id for each link */
  std::unordered_map<std::string, int> model_id_map_;  /**< Stores entity id for each link */
  std::unordered_map<std::string, int> visual_id_map_;  /**< Stores entity id for each link */
  int entity_counter_ { 1000 }; /**< Start entity counter to avoid clashing with gazebo */
};
}

#endif // TESSERACT_IGNITION_ENTITY_MANAGER_H
