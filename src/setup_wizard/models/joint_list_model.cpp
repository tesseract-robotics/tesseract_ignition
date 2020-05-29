/**
 * @file joint_list_model.cpp
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

#include <tesseract_ignition/setup_wizard/models/joint_list_model.h>

namespace tesseract_ignition
{

JointListModel::JointListModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

JointListModel::JointListModel(const JointListModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->joints_ = other.joints_;
}

JointListModel &JointListModel::operator=(const JointListModel &other)
{
  this->joints_ = other.joints_;
  return *this;
}

QHash<int, QByteArray> JointListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[MinRole] = "min";
    roles[MaxRole] = "max";
    roles[ValueRole] = "value";
    return roles;
}

void JointListModel::add(tesseract_scene_graph::Joint::ConstPtr joint)
{
  if (joint->type == tesseract_scene_graph::JointType::REVOLUTE || joint->type == tesseract_scene_graph::JointType::PRISMATIC)
  {
    joints_.push_back(joint);
    QStandardItem *parent_item = this->invisibleRootItem();
    QString joint_name = QString::fromStdString(joint->getName());
    auto item = new QStandardItem(joint_name);
    item->setData(joint_name, JointRoles::NameRole);
    item->setData(QString::number(joint->limits->lower), JointRoles::MinRole);
    item->setData(QString::number(joint->limits->upper), JointRoles::MaxRole);
    item->setData(QString::number(0), JointRoles::ValueRole);
    parent_item->appendRow(item);
  }
}

void JointListModel::clear()
{
  QStandardItemModel::clear();
  joints_.clear();
}

}
