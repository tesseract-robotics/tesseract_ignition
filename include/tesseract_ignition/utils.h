#ifndef TESSERACT_IGNITION_UTILS_H
#define TESSERACT_IGNITION_UTILS_H

#include <tesseract_scene_graph/resource_locator.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <QDir>

#include <string>

namespace tesseract_ignition
{

std::string locateResource(const std::string& url);

}

#endif // TESSERACT_IGNITION_UTILS_H
