/**
 * @file joint_list_model.h
 * @brief A Qt Standard Item Model for simple joint list
 *
 * @author Levi Armstrong
 * @date May 14, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_IGNITION_JOINT_LIST_MODEL_H
#define TESSERACT_IGNITION_JOINT_LIST_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#include <QStandardItemModel>
#include <QMetaType>
#endif

namespace tesseract_ignition
{
class JointListModel : public QStandardItemModel
{
    Q_OBJECT
public:
  enum JointRoles {
      NameRole = Qt::UserRole + 1,
      MinRole = Qt::UserRole + 2,
      MaxRole = Qt::UserRole + 3,
      ValueRole = Qt::UserRole + 4
  };

  JointListModel(QObject *parent = nullptr);
  JointListModel(const JointListModel &other);
  JointListModel &operator=(const JointListModel &other);
  ~JointListModel() override = default;

  Q_INVOKABLE void add(tesseract_scene_graph::Joint::ConstPtr joint);
  Q_INVOKABLE void clear();

  QHash<int, QByteArray> roleNames() const override;

private:
  std::vector<tesseract_scene_graph::Joint::ConstPtr> joints_;
};
}
Q_DECLARE_METATYPE(tesseract_ignition::JointListModel)

#endif // TESSERACT_IGNITION_JOINT_LIST_MODEL_H
