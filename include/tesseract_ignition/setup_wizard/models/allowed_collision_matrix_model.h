/**
 * @file allowed_collision_matrix_model.h
 * @brief A Qt Standard Item Model for Tesseract Allowed Collision Matrix
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
#ifndef TESSERACT_IGNITION_ALLOWED_COLLISION_MATRIX_MODEL_H
#define TESSERACT_IGNITION_ALLOWED_COLLISION_MATRIX_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract/tesseract.h>
#include <QStandardItemModel>
#include <QMetaType>
#endif

namespace tesseract_ignition
{
class AllowedCollisionMatrixModel : public QStandardItemModel
{
    Q_OBJECT
public:
  enum AllowedCollisionMatrixRoles {
      Link1Role = Qt::UserRole + 1,
      Link2Role = Qt::UserRole + 2,
      ReasonRole = Qt::UserRole + 3
  };

  AllowedCollisionMatrixModel(QObject *parent = nullptr);
  AllowedCollisionMatrixModel(const AllowedCollisionMatrixModel &other);
  AllowedCollisionMatrixModel &operator=(const AllowedCollisionMatrixModel &other);
  ~AllowedCollisionMatrixModel() override = default;

  Q_INVOKABLE void setTesseract(tesseract::Tesseract::Ptr thor);
  Q_INVOKABLE void add(const QString& link_name1, const QString& link_name2, const QString& reason);
  Q_INVOKABLE void clear();

  QHash<int, QByteArray> roleNames() const override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
  tesseract::Tesseract::Ptr thor_ {nullptr};
};
}
Q_DECLARE_METATYPE(tesseract_ignition::AllowedCollisionMatrixModel)

#endif // TESSERACT_IGNITION_ALLOWED_COLLISION_MATRIX_MODEL_H
