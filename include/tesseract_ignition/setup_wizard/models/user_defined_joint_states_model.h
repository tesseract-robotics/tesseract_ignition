/**
 * @file user_defined_joint_states_model.h
 * @brief A Qt Standard Item Model for User Defined Group Joint States
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
#ifndef TESSERACT_IGNITION_USER_DEFINED_JOINT_STATES_MODEL_H
#define TESSERACT_IGNITION_USER_DEFINED_JOINT_STATES_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract/tesseract.h>
#include <QStandardItemModel>
#include <QMetaType>
#include <QMap>
#endif

namespace tesseract_ignition
{

class UserDefinedJointStatesModel : public QStandardItemModel
{
    Q_OBJECT
public:

  enum UserDefinedJointStatesRoles {
      GroupNameRole = Qt::UserRole + 1,
      StateNameRole = Qt::UserRole + 2,
      JointNamesRole = Qt::UserRole + 3,
      JointValuesRole = Qt::UserRole + 4
  };

  UserDefinedJointStatesModel(QObject *parent = nullptr);
  UserDefinedJointStatesModel(const UserDefinedJointStatesModel &other);
  UserDefinedJointStatesModel &operator=(const UserDefinedJointStatesModel &other);
  ~UserDefinedJointStatesModel() override = default;

  Q_INVOKABLE void setTesseract(tesseract::Tesseract::Ptr thor);
  Q_INVOKABLE void add(const QString& group_name,
                       const QString& state_name,
                       const QStringList &joint_names,
                       const QStringList &joint_values);

  QHash<int, QByteArray> roleNames() const override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
  tesseract::Tesseract::Ptr thor_ {nullptr};
};

}
#endif // TESSERACT_IGNITION_USER_DEFINED_JOINT_STATES_MODEL_H
