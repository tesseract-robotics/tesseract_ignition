/**
 * @file kinematic_groups_model.h
 * @brief A Qt Standard Item Model for Tesseract Kinematic Groups
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
#ifndef TESSERACT_IGNITION_KINEMATIC_GROUPS_MODEL_H
#define TESSERACT_IGNITION_KINEMATIC_GROUPS_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract_environment/core/environment.h>
#include <QStandardItemModel>
#include <QMetaType>
#endif

namespace tesseract_ignition
{

class KinematicGroupsModel : public QStandardItemModel
{
    Q_OBJECT
public:

  enum KinematicGroupsRoles {
      NameRole = Qt::UserRole + 1,
      TypeRole = Qt::UserRole + 2,
      DataRole = Qt::UserRole + 3
  };

  KinematicGroupsModel(QObject *parent = nullptr);
  KinematicGroupsModel(const KinematicGroupsModel &other);
  KinematicGroupsModel &operator=(const KinematicGroupsModel &other);
  ~KinematicGroupsModel() override = default;

  Q_INVOKABLE void setEnvironment(tesseract_environment::Environment::Ptr env);
  Q_INVOKABLE void add(const QString& group_name, const QString& type, const QStringList& data);

  QHash<int, QByteArray> roleNames() const override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
  tesseract_environment::Environment::Ptr env_ {nullptr};
};
}
#endif // TESSERACT_IGNITION_KINEMATIC_GROUPS_MODEL_H
