#include <tesseract_ignition/setup_wizard/models/allowed_collision_matrix_model.h>

namespace tesseract_ignition
{

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(const AllowedCollisionMatrixModel &other)
{
  this->env_ = other.env_;
}

AllowedCollisionMatrixModel &AllowedCollisionMatrixModel::operator=(const AllowedCollisionMatrixModel &other)
{
  this->env_ = other.env_;
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

void AllowedCollisionMatrixModel::setEnvironment(tesseract_environment::Environment::Ptr env)
{
  this->clear();
  env_ = env;
  QStandardItem *parent_item = this->invisibleRootItem();
  for (const auto& ac : env_->getAllowedCollisionMatrix()->getAllAllowedCollisions())
  {
    auto item = new QStandardItem();
    item->setData(QString::fromStdString(ac.first.first), this->roleNames().key("link1"));
    item->setData(QString::fromStdString(ac.first.second), this->roleNames().key("link2"));
    item->setData(QString::fromStdString(ac.second), this->roleNames().key("reason"));
    parent_item->appendRow(item);
  }
}

void AllowedCollisionMatrixModel::add(const QString& link_name1, const QString& link_name2, const QString& reason)
{
  env_->addAllowedCollision(link_name1.toStdString(), link_name2.toStdString(), reason.toStdString());
  QStandardItem *parent_item = this->invisibleRootItem();
  auto item = new QStandardItem();
  item->setData(link_name1, this->roleNames().key("link1"));
  item->setData(link_name2, this->roleNames().key("link2"));
  item->setData(reason, this->roleNames().key("reason"));
  parent_item->appendRow(item);
}

}
