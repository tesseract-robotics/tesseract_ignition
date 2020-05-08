#ifndef TESSERACT_IGNITION_USER_DEFINED_JOINT_STATES_MODEL_H
#define TESSERACT_IGNITION_USER_DEFINED_JOINT_STATES_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract/tesseract.h>
#include <QStandardItemModel>
#include <QMetaType>
#include <QMap>
#endif

namespace tesseract_ignition
{

class UserDefinedJointStatesModel : public QStandardItemModel
{
    Q_OBJECT
public:

  enum UserDefinedJointStatesRoles {
      GroupNameRole = Qt::UserRole + 1,
      StateNameRole = Qt::UserRole + 2,
      JointNamesRole = Qt::UserRole + 3,
      JointValuesRole = Qt::UserRole + 4
  };

  UserDefinedJointStatesModel(QObject *parent = nullptr);
  UserDefinedJointStatesModel(const UserDefinedJointStatesModel &other);
  UserDefinedJointStatesModel &operator=(const UserDefinedJointStatesModel &other);
  ~UserDefinedJointStatesModel() override = default;

  Q_INVOKABLE void setTesseract(tesseract::Tesseract::Ptr thor);
  Q_INVOKABLE void add(const QString& group_name,
                       const QString& state_name,
                       const QStringList &joint_names,
                       const QStringList &joint_values);

  QHash<int, QByteArray> roleNames() const override;

private:
  tesseract::Tesseract::Ptr thor_ {nullptr};
};

}
#endif // TESSERACT_IGNITION_USER_DEFINED_JOINT_STATES_MODEL_H
