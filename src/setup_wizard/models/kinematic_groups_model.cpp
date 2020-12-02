/**
 * @file kinematic_groups_model.cpp
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

#include <tesseract_ignition/setup_wizard/models/kinematic_groups_model.h>

namespace tesseract_ignition
{

KinematicGroupsModel::KinematicGroupsModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

KinematicGroupsModel::KinematicGroupsModel(const KinematicGroupsModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->thor_ = other.thor_;
}

KinematicGroupsModel &KinematicGroupsModel::operator=(const KinematicGroupsModel &other)
{
  this->thor_ = other.thor_;
  return *this;
}

QHash<int, QByteArray> KinematicGroupsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[DataRole] = "data";
    return roles;
}

void KinematicGroupsModel::setTesseract(tesseract::Tesseract::Ptr thor)
{
  this->clear();

  thor_ = thor;
  QStandardItem *parent_item = this->invisibleRootItem();
  for (const auto& group : thor_->getEnvironment()->getManipulatorManager()->getChainGroups())
  {
    if (!group.second.empty())
    {
      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), KinematicGroupsRoles::NameRole);
      item->setData(QString::fromStdString("Chain"), KinematicGroupsRoles::TypeRole);
      QString data = QString::fromStdString(group.second[0].first) + "," + QString::fromStdString(group.second[0].second);
      item->setData(data, this->roleNames().key("data"));
      parent_item->appendRow(item);
    }
  }

  for (const auto& group : thor_->getEnvironment()->getManipulatorManager()->getJointGroups())
  {
    if (!group.second.empty())
    {
      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), KinematicGroupsRoles::NameRole);
      item->setData(QString::fromStdString("Joint List"), KinematicGroupsRoles::TypeRole);
      QString data = QString::fromStdString(group.second[0]);
      for (std::size_t i = 1; i < group.second.size(); ++i)
        data += ("," + QString::fromStdString(group.second[i]));
      item->setData(data, this->roleNames().key("data"));
      parent_item->appendRow(item);
    }
  }

  for (const auto& group : thor_->getEnvironment()->getManipulatorManager()->getLinkGroups())
  {
    if (!group.second.empty())
    {
      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), KinematicGroupsRoles::NameRole);
      item->setData(QString::fromStdString("Link List"), KinematicGroupsRoles::TypeRole);
      QString data = QString::fromStdString(group.second[0]);
      for (std::size_t i = 1; i < group.second.size(); ++i)
        data += ("," + QString::fromStdString(group.second[i]));
      item->setData(data, this->roleNames().key("data"));
      parent_item->appendRow(item);
    }
  }
}

void KinematicGroupsModel::add(const QString& group_name, const QString& type, const QStringList& data)
{
  QStandardItem *parent_item = this->invisibleRootItem();
  QString group_name_trimmed = group_name.trimmed();

  if (type == "Chain")
  {
    std::vector<std::string> fwd_solver_names = this->thor_->getEnvironment()->getManipulatorManager()->getAvailableFwdKinematicsSolvers(tesseract_kinematics::ForwardKinematicsFactoryType::CHAIN);
    std::vector<std::string> inv_solver_names = this->thor_->getEnvironment()->getManipulatorManager()->getAvailableInvKinematicsSolvers(tesseract_kinematics::InverseKinematicsFactoryType::CHAIN);
    if (!fwd_solver_names.empty() || !inv_solver_names.empty())
    {
      this->thor_->getEnvironment()->getManipulatorManager()->removeFwdKinematicSolver(group_name_trimmed.toStdString());
      this->thor_->getEnvironment()->getManipulatorManager()->removeInvKinematicSolver(group_name_trimmed.toStdString());

      bool fwd_good { false };
      bool inv_good { false };

      if (!fwd_solver_names.empty())
      {
        auto fwd_factory = this->thor_->getEnvironment()->getManipulatorManager()->getFwdKinematicFactory(fwd_solver_names[0]);
        auto fwd_kin = fwd_factory->create(this->thor_->getEnvironment()->getSceneGraph(), data[0].toStdString(), data[1].toStdString(), group_name_trimmed.toStdString());
        fwd_good = fwd_kin != nullptr && this->thor_->getEnvironment()->getManipulatorManager()->addFwdKinematicSolver(fwd_kin);
      }
      if (!inv_solver_names.empty())
      {
        auto inv_factory = this->thor_->getEnvironment()->getManipulatorManager()->getInvKinematicFactory(inv_solver_names[0]);
        auto inv_kin = inv_factory->create(this->thor_->getEnvironment()->getSceneGraph(), data[0].toStdString(), data[1].toStdString(), group_name_trimmed.toStdString());
        inv_good = inv_kin != nullptr && this->thor_->getEnvironment()->getManipulatorManager()->addInvKinematicSolver(inv_kin);
      }

      if (fwd_good || inv_good)
      {
        this->thor_->getEnvironment()->getManipulatorManager()->addChainGroup(group_name_trimmed.toStdString(), {std::make_pair(data[0].toStdString(), data[1].toStdString())});

        auto item = new QStandardItem();
        item->setData(group_name_trimmed, KinematicGroupsRoles::NameRole);
        item->setData("Chain", KinematicGroupsRoles::TypeRole);
        QString d = data[0] + "," + data[1];
        item->setData(d, KinematicGroupsRoles::DataRole);
        parent_item->appendRow(item);
      }
    }
  }
  else if (type == "Joint List")
  {
    std::vector<std::string> fwd_solver_names = this->thor_->getEnvironment()->getManipulatorManager()->getAvailableFwdKinematicsSolvers(tesseract_kinematics::ForwardKinematicsFactoryType::TREE);
    std::vector<std::string> inv_solver_names = this->thor_->getEnvironment()->getManipulatorManager()->getAvailableInvKinematicsSolvers(tesseract_kinematics::InverseKinematicsFactoryType::TREE);
    if (!fwd_solver_names.empty() || !inv_solver_names.empty())
    {
      this->thor_->getEnvironment()->getManipulatorManager()->removeFwdKinematicSolver(group_name_trimmed.toStdString());
      this->thor_->getEnvironment()->getManipulatorManager()->removeInvKinematicSolver(group_name_trimmed.toStdString());

      bool fwd_good { false };
      bool inv_good { false };

      std::vector<std::string> joints;
      for (const auto& j : data)
        joints.push_back(j.toStdString());

      if (!fwd_solver_names.empty())
      {
        auto fwd_factory = this->thor_->getEnvironment()->getManipulatorManager()->getFwdKinematicFactory(fwd_solver_names[0]);
        auto fwd_kin = fwd_factory->create(this->thor_->getEnvironment()->getSceneGraph(), joints, group_name_trimmed.toStdString());
        fwd_good = fwd_kin != nullptr && this->thor_->getEnvironment()->getManipulatorManager()->addFwdKinematicSolver(fwd_kin);
      }
      if (!inv_solver_names.empty())
      {
        auto inv_factory = this->thor_->getEnvironment()->getManipulatorManager()->getInvKinematicFactory(inv_solver_names[0]);
        auto inv_kin = inv_factory->create(this->thor_->getEnvironment()->getSceneGraph(), joints, group_name_trimmed.toStdString());
        inv_good = inv_kin != nullptr && this->thor_->getEnvironment()->getManipulatorManager()->addInvKinematicSolver(inv_kin);
      }

      if (fwd_good || inv_good)
      {
        this->thor_->getEnvironment()->getManipulatorManager()->addJointGroup(group_name_trimmed.toStdString(), joints);

        auto item = new QStandardItem();
        item->setData(group_name_trimmed, KinematicGroupsRoles::NameRole);
        item->setData("Joint List", KinematicGroupsRoles::TypeRole);
        QString d = data[0];
        for (int i = 1; i < data.size(); ++i)
          d += ("," + data[i]);
        item->setData(d, KinematicGroupsRoles::DataRole);
        parent_item->appendRow(item);
      }
    }
  }
  else if (type == "Link List")
  {
    std::vector<std::string> fwd_solver_names = this->thor_->getEnvironment()->getManipulatorManager()->getAvailableFwdKinematicsSolvers(tesseract_kinematics::ForwardKinematicsFactoryType::TREE);
    std::vector<std::string> inv_solver_names = this->thor_->getEnvironment()->getManipulatorManager()->getAvailableInvKinematicsSolvers(tesseract_kinematics::InverseKinematicsFactoryType::TREE);

    if (!fwd_solver_names.empty() || !inv_solver_names.empty())
    {
      this->thor_->getEnvironment()->getManipulatorManager()->removeFwdKinematicSolver(group_name_trimmed.toStdString());
      this->thor_->getEnvironment()->getManipulatorManager()->removeInvKinematicSolver(group_name_trimmed.toStdString());

      bool fwd_good { false };
      bool inv_good { false };

      std::vector<std::string> links;
      for (const auto& j : data)
        links.push_back(j.toStdString());

      // TODO: Need to find links active parent joint
      if (!fwd_solver_names.empty())
      {
        auto fwd_factory = this->thor_->getEnvironment()->getManipulatorManager()->getFwdKinematicFactory(fwd_solver_names[0]);
        auto fwd_kin = nullptr; //fwd_factory->create(this->thor_->getEnvironmentConst()->getSceneGraph(), joints, group_name_trimmed.toStdString());
        fwd_good = fwd_kin != nullptr && this->thor_->getEnvironment()->getManipulatorManager()->addFwdKinematicSolver(fwd_kin);
      }
      if (!inv_solver_names.empty())
      {
        auto inv_factory = this->thor_->getEnvironment()->getManipulatorManager()->getInvKinematicFactory(inv_solver_names[0]);
        auto inv_kin = nullptr; //inv_factory->create(this->thor_->getEnvironmentConst()->getSceneGraph(), joints, group_name_trimmed.toStdString());
        inv_good = inv_kin != nullptr && this->thor_->getEnvironment()->getManipulatorManager()->addInvKinematicSolver(inv_kin);
      }

      if (fwd_good || inv_good)
      {
        this->thor_->getEnvironment()->getManipulatorManager()->addLinkGroup(group_name_trimmed.toStdString(), links);

        auto item = new QStandardItem();
        item->setData(group_name_trimmed, KinematicGroupsRoles::NameRole);
        item->setData("Link List", KinematicGroupsRoles::TypeRole);
        QString d = data[0];
        for (int i = 1; i < data.size(); ++i)
          d += ("," + data[i]);
        item->setData(d, KinematicGroupsRoles::DataRole);
        parent_item->appendRow(item);
      }
    }
  }
  else
  {
    CONSOLE_BRIDGE_logError("Tried to add unknown kinematic group type!");
  }

  // If the count does not match then it was a replace so rebuild model.
  std::size_t num_groups = this->thor_->getEnvironment()->getManipulatorManager()->getChainGroups().size() + this->thor_->getEnvironment()->getManipulatorManager()->getJointGroups().size() + this->thor_->getManipulatorManager()->getLinkGroups().size();
  if (parent_item->rowCount() != static_cast<int>(num_groups))
    setTesseract(thor_);
}

bool KinematicGroupsModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString group_name = row_item->data(KinematicGroupsRoles::NameRole).toString();
    QString type = row_item->data(KinematicGroupsRoles::TypeRole).toString();
    if (type == "Chain")
    {
      thor_->getEnvironment()->getManipulatorManager()->removeFwdKinematicSolver(group_name.toStdString());
      thor_->getEnvironment()->getManipulatorManager()->removeInvKinematicSolver(group_name.toStdString());
      this->thor_->getEnvironment()->getManipulatorManager()->removeChainGroup(group_name.toStdString());
    }
    else if (type == "Joint List")
    {
      thor_->getEnvironment()->getManipulatorManager()->removeFwdKinematicSolver(group_name.toStdString());
      thor_->getEnvironment()->getManipulatorManager()->removeInvKinematicSolver(group_name.toStdString());
      this->thor_->getEnvironment()->getManipulatorManager()->removeJointGroup(group_name.toStdString());
    }
    else if (type == "Link List")
    {
      thor_->getEnvironment()->getManipulatorManager()->removeFwdKinematicSolver(group_name.toStdString());
      thor_->getEnvironment()->getManipulatorManager()->removeInvKinematicSolver(group_name.toStdString());
      this->thor_->getEnvironment()->getManipulatorManager()->removeLinkGroup(group_name.toStdString());
    }

    return QStandardItemModel::removeRows(row, count, parent);
  }
  return false;
}
}
