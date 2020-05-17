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
  this->thor_ = other.thor_;
}

UserDefinedTCPModel &UserDefinedTCPModel::operator=(const UserDefinedTCPModel &other)
{
  this->thor_ = other.thor_;
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

void UserDefinedTCPModel::setTesseract(tesseract::Tesseract::Ptr thor)
{
  this->clear();
  thor_ = thor;
  QStandardItem *parent_item = this->invisibleRootItem();
  Eigen::IOFormat eigen_format(Eigen::StreamPrecision, 0, ",", ",");
  for (const auto& group : thor_->getGroupTCPs())
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

  auto& group_tcps = thor_->getGroupTCPs();
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

    group->second[tcp_name.toStdString()] = tcp;
  }
  else
  {
    group_tcps[group_name.toStdString()][tcp_name_trimmed.toStdString()] = tcp;
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
    setTesseract(thor_);
  }
}

bool UserDefinedTCPModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString group_name = row_item->data(UserDefinedTCPRoles::GroupNameRole).toString();
    QString tcp_name = row_item->data(UserDefinedTCPRoles::TCPNameRole).toString();
    auto& group_tcps = thor_->getGroupTCPs();
    group_tcps[group_name.toStdString()].erase(tcp_name.toStdString());
    if (group_tcps[group_name.toStdString()].empty())
      group_tcps.erase(group_name.toStdString());

    return QStandardItemModel::removeRows(row, count, parent);
  }
  return false;
}

}
