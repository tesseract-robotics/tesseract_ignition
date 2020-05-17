#include <tesseract_ignition/setup_wizard/models/user_defined_joint_states_model.h>

namespace tesseract_ignition
{

UserDefinedJointStatesModel::UserDefinedJointStatesModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

UserDefinedJointStatesModel::UserDefinedJointStatesModel(const UserDefinedJointStatesModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->thor_ = other.thor_;
}

UserDefinedJointStatesModel &UserDefinedJointStatesModel::operator=(const UserDefinedJointStatesModel &other)
{
  this->thor_ = other.thor_;
  return *this;
}

QHash<int, QByteArray> UserDefinedJointStatesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[GroupNameRole] = "group_name";
    roles[StateNameRole] = "state_name";
    roles[JointNamesRole] = "joint_names";
    roles[JointValuesRole] = "joint_values";
    return roles;
}

void UserDefinedJointStatesModel::setTesseract(tesseract::Tesseract::Ptr thor)
{
  this->clear();
  thor_ = thor;
  QStandardItem *parent_item = this->invisibleRootItem();
  for (const auto& group : thor_->getGroupStatesConst())
  {
    for (const auto& state : group.second)
    {

      auto item = new QStandardItem();
      item->setData(QString::fromStdString(group.first), UserDefinedJointStatesRoles::GroupNameRole);
      item->setData(QString::fromStdString(state.first), UserDefinedJointStatesRoles::StateNameRole);

      QStringList joint_names;
      QStringList joint_values;
      for (const auto& v : state.second)
      {
        joint_names.push_back(QString::fromStdString(v.first));
        joint_values.push_back(QString::number(v.second));
      }

      item->setData(joint_names.join(","), UserDefinedJointStatesRoles::JointNamesRole);
      item->setData(joint_values.join(","), UserDefinedJointStatesRoles::JointValuesRole);
      parent_item->appendRow(item);
    }
  }
}

void UserDefinedJointStatesModel::add(const QString& group_name,
                                      const QString& state_name,
                                      const QStringList& joint_names,
                                      const QStringList& joint_values)
{
  QString state_name_trimmed = state_name.trimmed();

  auto& group_states = thor_->getGroupStates();
  auto group = group_states.find(group_name.toStdString());
  bool add = true;

  tesseract_scene_graph::SRDFModel::JointState state;
  for (int i = 0; i < joint_names.size(); ++i)
    state[joint_names[i].toStdString()] = joint_values[i].toDouble();

  if (group != group_states.end())
  {
    auto s = group->second.find(state_name_trimmed.toStdString());
    if (s != group->second.end())
      add = false;

    group->second[state_name_trimmed.toStdString()] = state;
  }
  else
  {
    tesseract_scene_graph::SRDFModel::JointStates states;
    states[state_name.toStdString()] = state;
    group_states[group_name.toStdString()] = states;
  }

  QStandardItem *parent_item = this->invisibleRootItem();
  if (add)
  {
    auto item = new QStandardItem();
    item->setData(group_name, UserDefinedJointStatesRoles::GroupNameRole );
    item->setData(state_name_trimmed, UserDefinedJointStatesRoles::StateNameRole);
    item->setData(joint_names.join(","), UserDefinedJointStatesRoles::JointNamesRole);
    item->setData(joint_values.join(","), UserDefinedJointStatesRoles::JointValuesRole);
    parent_item->appendRow(item);
  }
  else // replace
  {
    setTesseract(thor_);
  }
}

bool UserDefinedJointStatesModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString group_name = row_item->data(UserDefinedJointStatesRoles::GroupNameRole).toString();
    QString state_name = row_item->data(UserDefinedJointStatesRoles::StateNameRole).toString();
    auto& group_states = thor_->getGroupStates();
    group_states[group_name.toStdString()].erase(state_name.toStdString());
    if (group_states[group_name.toStdString()].empty())
      group_states.erase(group_name.toStdString());

    return QStandardItemModel::removeRows(row, count, parent);
  }
  return false;
}

}
