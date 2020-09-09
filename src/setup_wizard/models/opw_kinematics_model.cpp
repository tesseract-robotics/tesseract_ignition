/**
 * @file opw_kinematics_model.cpp
 * @brief A Qt Standard Item Model for OPW Kinematic
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

#include <tesseract_ignition/setup_wizard/models/opw_kinematics_model.h>
#include <Eigen/Geometry>
#include <QVector3D>

namespace tesseract_ignition
{

OPWKinematicsModel::OPWKinematicsModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

OPWKinematicsModel::OPWKinematicsModel(const OPWKinematicsModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->thor_ = other.thor_;
}

OPWKinematicsModel &OPWKinematicsModel::operator=(const OPWKinematicsModel &other)
{
  this->thor_ = other.thor_;
  return *this;
}

QHash<int, QByteArray> OPWKinematicsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[GroupNameRole] = "group_name";
    roles[KinematicParametersRole] = "parameters";
    roles[JointOffsetRole] = "offsets";
    roles[JointCorrectionRole] = "sign_corrections";
    return roles;
}

void OPWKinematicsModel::setTesseract(tesseract::Tesseract::Ptr thor)
{
  this->clear();
  thor_ = thor;

  for (const auto& group : thor_->getManipulatorManager()->getOPWKinematicsSolvers())
    addItem(QString::fromStdString(group.first), group.second);
}

void OPWKinematicsModel::add(const QString& group_name,
                             double a1, double a2, double b,
                             double c1, double c2, double c3, double c4,
                             double o1, double o2, double o3, double o4, double o5, double o6,
                             int sc1, int sc2, int sc3, int sc4, int sc5, int sc6)
{
  const auto& group_opw_kinematics = thor_->getManipulatorManager()->getOPWKinematicsSolvers();
  bool add = false;

  if (group_opw_kinematics.find(group_name.toStdString()) == group_opw_kinematics.end())
    add = true;

  tesseract_scene_graph::OPWKinematicParameters opw_params;
  opw_params.a1 = a1;
  opw_params.a2 = a2;
  opw_params.b = b;
  opw_params.c1 = c1;
  opw_params.c2 = c2;
  opw_params.c3 = c3;
  opw_params.c4 = c4;
  opw_params.offsets[0] = o1;
  opw_params.offsets[1] = o2;
  opw_params.offsets[2] = o3;
  opw_params.offsets[3] = o4;
  opw_params.offsets[4] = o5;
  opw_params.offsets[5] = o6;
  opw_params.sign_corrections[0] = static_cast<signed char>(sc1);
  opw_params.sign_corrections[1] = static_cast<signed char>(sc2);
  opw_params.sign_corrections[2] = static_cast<signed char>(sc3);
  opw_params.sign_corrections[3] = static_cast<signed char>(sc4);
  opw_params.sign_corrections[4] = static_cast<signed char>(sc5);
  opw_params.sign_corrections[5] = static_cast<signed char>(sc6);

  thor_->getManipulatorManager()->addOPWKinematicsSolver(group_name.toStdString(), opw_params);

  if (add)
    addItem(group_name, opw_params);
  else // replace
    setTesseract(thor_);
}

bool OPWKinematicsModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString group_name = row_item->data(OPWKinematicsRoles::GroupNameRole).toString();
    thor_->getManipulatorManager()->removeOPWKinematicsSovler(group_name.toStdString());
    return QStandardItemModel::removeRows(row, count, parent);
  }
  return false;
}

void OPWKinematicsModel::addItem(const QString& group_name, const tesseract_scene_graph::OPWKinematicParameters& opw)
{
  QStandardItem *parent_item = this->invisibleRootItem();
  auto item = new QStandardItem();
  item->setData(group_name, OPWKinematicsRoles::GroupNameRole);

  QString param_string = QString("a1: %1, a2: %2, b: %3, c1: %4, c2: %5, c3: %6, c4: %7").arg(opw.a1).arg(opw.a2).arg(opw.b).arg(opw.c1).arg(opw.c2).arg(opw.c3).arg(opw.c4);
  item->setData(param_string, OPWKinematicsRoles::KinematicParametersRole);

  QString offsets_string = QString::number(opw.offsets[0]);
  QString sign_corrections_string = QString::number(opw.sign_corrections[0]);
  for (std::size_t i = 0; i < 6; ++i)
  {
    offsets_string += "," + QString::number(opw.offsets[i]);
    sign_corrections_string += "," + QString::number(opw.sign_corrections[i]);
  }

  item->setData(offsets_string, OPWKinematicsRoles::JointOffsetRole);
  item->setData(sign_corrections_string, OPWKinematicsRoles::JointCorrectionRole);
  parent_item->appendRow(item);
}

}
