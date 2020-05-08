#ifndef TESSERACT_IGNITION_JOINT_LIST_MODEL_H
#define TESSERACT_IGNITION_JOINT_LIST_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#include <QStandardItemModel>
#include <QMetaType>
#endif

namespace tesseract_ignition
{
class JointListModel : public QStandardItemModel
{
    Q_OBJECT
public:
  enum JointRoles {
      NameRole = Qt::UserRole + 1,
      MinRole = Qt::UserRole + 2,
      MaxRole = Qt::UserRole + 3,
      ValueRole = Qt::UserRole + 4
  };

  JointListModel(QObject *parent = nullptr);
  JointListModel(const JointListModel &other);
  JointListModel &operator=(const JointListModel &other);
  ~JointListModel() override = default;

  Q_INVOKABLE void add(tesseract_scene_graph::Joint::ConstPtr joint);
  Q_INVOKABLE void clear();

  QHash<int, QByteArray> roleNames() const override;
//  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
//  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
  std::vector<tesseract_scene_graph::Joint::ConstPtr> joints_;
};
}
Q_DECLARE_METATYPE(tesseract_ignition::JointListModel)

#endif // TESSERACT_IGNITION_JOINT_LIST_MODEL_H
