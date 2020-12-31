/**
 * @file user_defined_tcp_model.cpp
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
#include <tesseract_ignition/setup_wizard/models/user_defined_tcp_model.h>
#include <Eigen/Geometry>
#include <QVector3D>

namespace tesseract_ignition
{

UserDefinedTCPModel::UserDefinedTCPModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

UserDefinedTCPModel::UserDefinedTCPModel(const UserDefinedTCPModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->env_ = other.env_;
}

UserDefinedTCPModel &UserDefinedTCPModel::operator=(const UserDefinedTCPModel &other)
{
  this->env_ = other.env_;
  return *this;
}

QHash<int, QByteArray> UserDefinedTCPModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[GroupNameRole] = "group_name";
    roles[TCPNameRole] = "tcp_name";
    roles[PositionRole] = "position";
    roles[OrientationRole] = "orientation";
    return roles;
}

void UserDefinedTCPModel::setEnvironment(tesseract_environment::Environment::Ptr env)
{
  this->clear();
  env_ = env;
  QStandardItem *parent_item = this->invisibleRootItem();
  Eigen::IOFormat eigen_format(Eigen::StreamPrecision, 0, ",", ",");
  for (const auto& group : env_->getManipulatorManager()->getGroupTCPs())
  {
    for (const auto& tcp : group.second)
    {

      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), UserDefinedTCPRoles::GroupNameRole);
      item->setData(QString::fromStdString(tcp.first), UserDefinedTCPRoles::TCPNameRole);

      std::stringstream xyz_string, rpy_string;
      xyz_string << tcp.second.translation().format(eigen_format);
      rpy_string << tcp.second.rotation().eulerAngles(2, 1, 0).format(eigen_format);

      item->setData(QString::fromStdString(xyz_string.str()), UserDefinedTCPRoles::PositionRole);
      item->setData(QString::fromStdString(rpy_string.str()), UserDefinedTCPRoles::OrientationRole);
      parent_item->appendRow(item);
    }
  }
}

void UserDefinedTCPModel::add(const QString& group_name,
                              const QString& tcp_name,
                              const QVector3D &position,
                              const QVector3D &orientation)
{
  QString tcp_name_trimmed = tcp_name.trimmed();

  const auto& group_tcps = env_->getManipulatorManager()->getGroupTCPs();
  auto group = group_tcps.find(group_name.toStdString());
  bool add = true;

  Eigen::IOFormat eigen_format(Eigen::StreamPrecision, 0, ",", ",");
  Eigen::Isometry3d tcp = Eigen::Isometry3d::Identity();
  Eigen::Vector3d xyz = Eigen::Vector3f(position.x(), position.y(), position.z()).cast<double>();
  Eigen::Vector3d rpy = Eigen::Vector3f(orientation.x(), orientation.y(), orientation.z()).cast<double>();

  tcp.translation() =  xyz;

  Eigen::AngleAxisd rollAngle(rpy.x(), Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd pitchAngle(rpy.y(), Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd yawAngle(rpy.z(), Eigen::Vector3d::UnitZ());

  tcp.linear() = Eigen::Quaterniond(yawAngle * pitchAngle * rollAngle).toRotationMatrix();

  if (group != group_tcps.end())
  {
    auto s = group->second.find(tcp_name_trimmed.toStdString());
    if (s != group->second.end())
      add = false;

    env_->getManipulatorManager()->addGroupTCP(group_name.toStdString(), tcp_name.toStdString(), tcp);
  }
  else
  {
    env_->getManipulatorManager()->addGroupTCP(group_name.toStdString(), tcp_name.toStdString(), tcp);
  }

  QStandardItem *parent_item = this->invisibleRootItem();
  if (add)
  {
    auto item = new QStandardItem();
    item->setData(group_name, UserDefinedTCPRoles::GroupNameRole);
    item->setData(tcp_name_trimmed,  UserDefinedTCPRoles::TCPNameRole);

    std::stringstream xyz_string, rpy_string;
    xyz_string << xyz.format(eigen_format);
    rpy_string << rpy.format(eigen_format);

    item->setData(QString::fromStdString(xyz_string.str()), UserDefinedTCPRoles::PositionRole);
    item->setData(QString::fromStdString(rpy_string.str()), UserDefinedTCPRoles::OrientationRole);
    parent_item->appendRow(item);
  }
  else // replace
  {
    setEnvironment(env_);
  }
}

bool UserDefinedTCPModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString group_name = row_item->data(UserDefinedTCPRoles::GroupNameRole).toString();
    QString tcp_name = row_item->data(UserDefinedTCPRoles::TCPNameRole).toString();
    this->env_->getManipulatorManager()->removeGroupTCP(group_name.toStdString(), tcp_name.toStdString());

    return QStandardItemModel::removeRows(row, count, parent);
  }
  return false;
}

}
