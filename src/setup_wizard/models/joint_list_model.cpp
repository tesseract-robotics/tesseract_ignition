#include <tesseract_ignition/setup_wizard/models/joint_list_model.h>

namespace tesseract_ignition
{

JointListModel::JointListModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

JointListModel::JointListModel(const JointListModel &other)
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
    item->setData(joint_name, this->roleNames().key("name"));
    item->setData(QString::number(joint->limits->lower), this->roleNames().key("min"));
    item->setData(QString::number(joint->limits->upper), this->roleNames().key("max"));
    item->setData(QString::number(0), this->roleNames().key("value"));
    parent_item->appendRow(item);
  }
}

void JointListModel::clear()
{
  QStandardItemModel::clear();
  joints_.clear();
}

}
