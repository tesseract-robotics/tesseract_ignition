/**
 * @file user_defined_tcp_model.h
 * @brief A Qt Standard Item Model for User Defined Group Tool Center Points
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
#ifndef TESSERACT_IGNITION_USER_DEFINED_TCP_MODEL_H
#define TESSERACT_IGNITION_USER_DEFINED_TCP_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract/tesseract.h>
#include <QStandardItemModel>
#include <QMetaType>
#include <QMap>
#endif

namespace tesseract_ignition
{

class UserDefinedTCPModel : public QStandardItemModel
{
    Q_OBJECT
public:

  enum UserDefinedTCPRoles {
      GroupNameRole = Qt::UserRole + 1,
      TCPNameRole = Qt::UserRole + 2,
      PositionRole = Qt::UserRole + 3,
      OrientationRole = Qt::UserRole + 4
  };

  UserDefinedTCPModel(QObject *parent = nullptr);
  UserDefinedTCPModel(const UserDefinedTCPModel &other);
  UserDefinedTCPModel &operator=(const UserDefinedTCPModel &other);
  ~UserDefinedTCPModel() override = default;

  Q_INVOKABLE void setTesseract(tesseract::Tesseract::Ptr thor);
  Q_INVOKABLE void add(const QString& group_name,
                       const QString& tcp_name,
                       const QVector3D &position,
                       const QVector3D &orientation);

  QHash<int, QByteArray> roleNames() const override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
  tesseract::Tesseract::Ptr thor_ {nullptr};
};

}
#endif // TESSERACT_IGNITION_USER_DEFINED_TCP_MODEL_H
