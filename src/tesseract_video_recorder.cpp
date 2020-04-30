/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include <ignition/msgs/boolean.pb.h>
#include <ignition/msgs/video_record.pb.h>

#include <iostream>
#include <ignition/common/Console.hh>
#include <ignition/gui/Application.hh>
#include <ignition/plugin/Register.hh>
#include <ignition/transport/Node.hh>
#include <ignition/transport/Publisher.hh>

#include <tesseract_ignition/recorder/tesseract_video_recorder.h>

namespace tesseract_ignition::gui::plugins
{
  class TesseractVideoRecorderPrivate
  {
    /// \brief Ignition communication node.
    public: ignition::transport::Node node;

    /// \brief Video record service name
    public: std::string service;

    /// \brief Filename of the recorded video
    public: std::string filename;
  };
}

using namespace tesseract_ignition;
using namespace gui;
using namespace plugins;

/////////////////////////////////////////////////
TesseractVideoRecorder::TesseractVideoRecorder()
  : ignition::gui::Plugin(), dataPtr(std::make_unique<TesseractVideoRecorderPrivate>())
{
}

/////////////////////////////////////////////////
TesseractVideoRecorder::~TesseractVideoRecorder() = default;

/////////////////////////////////////////////////
void TesseractVideoRecorder::LoadConfig(const tinyxml2::XMLElement *)
{
  if (this->title.empty())
    this->title = "Video recorder";

  // For video record requests
  this->dataPtr->service = "/tesseract/gui/record_video";
}

/////////////////////////////////////////////////
void TesseractVideoRecorder::OnStart(const QString &_format)
{
  std::function<void(const ignition::msgs::Boolean &, const bool)> cb =
      [](const ignition::msgs::Boolean &/*_rep*/, const bool _result)
  {
    if (!_result)
      ignerr << "Error sending video record start request" << std::endl;
  };

  std::string format = _format.toStdString();
  std::string tmp_dir = (QStandardPaths::standardLocations(QStandardPaths::TempLocation)[0] + QDir::separator()).toStdString();
  igndbg << "Temp dir location: " << tmp_dir << std::endl;
  this->dataPtr->filename = tmp_dir + "ign_recording." + format;

  ignition::msgs::VideoRecord req;
  req.set_start(true);
  req.set_format(format);
  req.set_save_filename(this->dataPtr->filename);
  this->dataPtr->node.Request(this->dataPtr->service, req, cb);
}

/////////////////////////////////////////////////
void TesseractVideoRecorder::OnStop()
{
  std::function<void(const ignition::msgs::Boolean &, const bool)> cb =
      [](const ignition::msgs::Boolean &/*_rep*/, const bool _result)
  {
    if (!_result)
      ignerr << "Error sending video record stop request" << std::endl;
  };

  ignition::msgs::VideoRecord req;
  req.set_stop(true);
  this->dataPtr->node.Request(this->dataPtr->service, req, cb);
}

/////////////////////////////////////////////////
void TesseractVideoRecorder::OnSave(const QString &_url)
{
  std::string path = QUrl(_url).toLocalFile().toStdString();
  bool result = ignition::common::moveFile(this->dataPtr->filename, path);

  if (!result)
  {
    ignerr  << "Unable to rename file from[" << this->dataPtr->filename
      << "] to [" << path << "]" << std::endl;
  }
  else
  {
    ignmsg << "Video saved to: " << path << std::endl;
  }
}

/////////////////////////////////////////////////
void TesseractVideoRecorder::OnCancel()
{
  if (ignition::common::exists(this->dataPtr->filename))
    std::remove(this->dataPtr->filename.c_str());
}

// Register this plugin
IGNITION_ADD_PLUGIN(tesseract_ignition::gui::plugins::TesseractVideoRecorder, ignition::gui::Plugin)
