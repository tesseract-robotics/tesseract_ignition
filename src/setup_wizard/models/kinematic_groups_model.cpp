#include <tesseract_ignition/setup_wizard/models/kinematic_groups_model.h>

namespace tesseract_ignition
{

KinematicGroupsModel::KinematicGroupsModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

KinematicGroupsModel::KinematicGroupsModel(const KinematicGroupsModel &other)
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
  for (const auto& group : thor_->getSRDFModelConst()->getChainGroups())
  {
    if (!group.second.empty())
    {
      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), this->roleNames().key("name"));
      item->setData(QString::fromStdString("Chain"), this->roleNames().key("type"));
      QString data = QString::fromStdString(group.second[0].first) + "," + QString::fromStdString(group.second[0].second);
      item->setData(data, this->roleNames().key("data"));
      parent_item->appendRow(item);
    }
  }

  for (const auto& group : thor_->getSRDFModelConst()->getJointGroups())
  {
    if (!group.second.empty())
    {
      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), this->roleNames().key("name"));
      item->setData(QString::fromStdString("Joint List"), this->roleNames().key("type"));
      QString data = QString::fromStdString(group.second[0]);
      for (std::size_t i = 1; i < group.second.size(); ++i)
        data += ("," + QString::fromStdString(group.second[i]));
      item->setData(data, this->roleNames().key("data"));
      parent_item->appendRow(item);
    }
  }

  for (const auto& group : thor_->getSRDFModelConst()->getLinkGroups())
  {
    if (!group.second.empty())
    {
      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), this->roleNames().key("name"));
      item->setData(QString::fromStdString("Link List"), this->roleNames().key("type"));
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
  if (type == "Chain")
  {
    std::vector<std::string> solver_names = this->thor_->getFwdKinematicsManager()->getAvailableFwdKinematicsSolvers(tesseract_kinematics::ForwardKinematicsFactoryType::CHAIN);
    if (!solver_names.empty())
    {
      auto factory = this->thor_->getFwdKinematicsManager()->getFwdKinematicFactory(solver_names[0]);
      auto fwd_kin = factory->create(this->thor_->getEnvironmentConst()->getSceneGraph(), data[0].toStdString(), data[1].toStdString(), group_name.toStdString());
      if (fwd_kin != nullptr && this->thor_->getFwdKinematicsManager()->addFwdKinematicSolver(fwd_kin))
      {
        auto item = new QStandardItem();
        item->setData(group_name, this->roleNames().key("name"));
        item->setData("Chain", this->roleNames().key("type"));
        QString d = data[0] + "," + data[1];
        item->setData(d, this->roleNames().key("data"));
        parent_item->appendRow(item);
      }
    }
  }
  else if (type == "Joint List")
  {
    std::vector<std::string> solver_names = this->thor_->getFwdKinematicsManager()->getAvailableFwdKinematicsSolvers(tesseract_kinematics::ForwardKinematicsFactoryType::TREE);
    if (!solver_names.empty())
    {
      std::vector<std::string> joints;
      for (const auto& j : data)
        joints.push_back(j.toStdString());

      auto factory = this->thor_->getFwdKinematicsManager()->getFwdKinematicFactory(solver_names[0]);
      auto fwd_kin = factory->create(this->thor_->getEnvironmentConst()->getSceneGraph(), joints, group_name.toStdString());
      if (fwd_kin != nullptr && this->thor_->getFwdKinematicsManager()->addFwdKinematicSolver(fwd_kin))
      {
        auto item = new QStandardItem();
        item->setData(group_name, this->roleNames().key("name"));
        item->setData("Joint List", this->roleNames().key("type"));
        QString d = data[0];
        for (int i = 1; i < data.size(); ++i)
          d += ("," + data[i]);
        item->setData(d, this->roleNames().key("data"));
        parent_item->appendRow(item);
      }
    }
  }
  else if (type == "Link List")
  {
    auto item = new QStandardItem();
    item->setData(group_name, this->roleNames().key("name"));
    item->setData("Link List", this->roleNames().key("type"));
    QString d = data[0];
    for (int i = 1; i < data.size(); ++i)
      d += ("," + data[i]);
    item->setData(d, this->roleNames().key("data"));
    parent_item->appendRow(item);
  }
  else
  {
    CONSOLE_BRIDGE_logError("Tried to add unknown kinematic group type!");
  }
}

}
