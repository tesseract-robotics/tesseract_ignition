/**
 * @file allowed_collision_matrix_model.cpp
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
#include <tesseract_ignition/setup_wizard/models/allowed_collision_matrix_model.h>

namespace tesseract_ignition
{

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(const AllowedCollisionMatrixModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->thor_ = other.thor_;
}

AllowedCollisionMatrixModel &AllowedCollisionMatrixModel::operator=(const AllowedCollisionMatrixModel &other)
{
  this->thor_ = other.thor_;
  return *this;
}

QHash<int, QByteArray> AllowedCollisionMatrixModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Link1Role] = "link1";
    roles[Link2Role] = "link2";
    roles[ReasonRole] = "reason";
    return roles;
}

void AllowedCollisionMatrixModel::setTesseract(tesseract::Tesseract::Ptr thor)
{
  QStandardItemModel::clear();
  thor_ = thor;
  QStandardItem *parent_item = this->invisibleRootItem();
  for (const auto& ac : thor_->getManipulatorManager()->getSRDFModel()->getAllowedCollisionMatrix().getAllAllowedCollisions())
  {
    auto item = new QStandardItem();
    item->setData(QString::fromStdString(ac.first.first), AllowedCollisionMatrixRoles::Link1Role);
    item->setData(QString::fromStdString(ac.first.second), AllowedCollisionMatrixRoles::Link2Role);
    item->setData(QString::fromStdString(ac.second), AllowedCollisionMatrixRoles::ReasonRole);
    parent_item->appendRow(item);
  }
  sort(0);
}

void AllowedCollisionMatrixModel::add(const QString& link_name1, const QString& link_name2, const QString& reason)
{
  thor_->getEnvironment()->addAllowedCollision(link_name1.toStdString(), link_name2.toStdString(), reason.toStdString());
  thor_->getManipulatorManager()->addAllowedCollision(link_name1.toStdString(), link_name2.toStdString(), reason.toStdString());
  QStandardItem *parent_item = this->invisibleRootItem();
  auto item = new QStandardItem();
  item->setData(link_name1, AllowedCollisionMatrixRoles::Link1Role);
  item->setData(link_name2, AllowedCollisionMatrixRoles::Link2Role);
  item->setData(reason, AllowedCollisionMatrixRoles::ReasonRole);
  parent_item->appendRow(item);
  sort(0);

  // If the count does not match then it was a replace so rebuild model.
  if (parent_item->rowCount() != static_cast<int>(thor_->getManipulatorManager()->getAllowedCollisionMatrix().getAllAllowedCollisions().size()))
    setTesseract(thor_);
}

bool AllowedCollisionMatrixModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString link1_name = row_item->data(AllowedCollisionMatrixRoles::Link1Role).toString();
    QString link2_name = row_item->data(AllowedCollisionMatrixRoles::Link2Role).toString();
    thor_->getManipulatorManager()->removeAllowedCollision(link1_name.toStdString(), link2_name.toStdString());
    thor_->getEnvironment()->removeAllowedCollision(link1_name.toStdString(), link2_name.toStdString());

    return QStandardItemModel::removeRows(row, count, parent);
  }
  return false;
}

void AllowedCollisionMatrixModel::clear()
{
  QStandardItemModel::clear();
  if (thor_)
  {
    tesseract_scene_graph::AllowedCollisionMatrix acm(thor_->getManipulatorManager()->getAllowedCollisionMatrix());
    thor_->getManipulatorManager()->clearAllowedCollisions();
    for (const auto& entry : acm.getAllAllowedCollisions())
      thor_->getEnvironment()->removeAllowedCollision(entry.first.first, entry.first.second);
  }
}

}
