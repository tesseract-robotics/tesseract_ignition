#include <tesseract_ignition/setup_wizard/models/allowed_collision_matrix_model.h>

namespace tesseract_ignition
{

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(const AllowedCollisionMatrixModel &other)
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
  for (const auto& ac : thor_->getSRDFModelConst()->getAllowedCollisionMatrix().getAllAllowedCollisions())
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
  thor_->getSRDFModel()->getAllowedCollisionMatrix().addAllowedCollision(link_name1.toStdString(), link_name2.toStdString(), reason.toStdString());
  QStandardItem *parent_item = this->invisibleRootItem();
  auto item = new QStandardItem();
  item->setData(link_name1, AllowedCollisionMatrixRoles::Link1Role);
  item->setData(link_name2, AllowedCollisionMatrixRoles::Link2Role);
  item->setData(reason, AllowedCollisionMatrixRoles::ReasonRole);
  parent_item->appendRow(item);
  sort(0);

  // If the count does not match then it was a replace so rebuild model.
  if (parent_item->rowCount() != static_cast<int>(thor_->getSRDFModel()->getAllowedCollisionMatrix().getAllAllowedCollisions().size()))
    setTesseract(thor_);
}

bool AllowedCollisionMatrixModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString link1_name = row_item->data(AllowedCollisionMatrixRoles::Link1Role).toString();
    QString link2_name = row_item->data(AllowedCollisionMatrixRoles::Link2Role).toString();
    auto& acm = thor_->getSRDFModel()->getAllowedCollisionMatrix();
    acm.removeAllowedCollision(link1_name.toStdString(), link2_name.toStdString());
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
    tesseract_scene_graph::AllowedCollisionMatrix acm(thor_->getSRDFModel()->getAllowedCollisionMatrix());
    thor_->getSRDFModel()->getAllowedCollisionMatrix().clearAllowedCollisions();
    for (const auto& entry : acm.getAllAllowedCollisions())
      thor_->getEnvironment()->removeAllowedCollision(entry.first.first, entry.first.second);
  }
}

}
