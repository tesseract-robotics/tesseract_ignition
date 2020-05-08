#ifndef TESSERACT_IGNITION_KINEMATIC_GROUPS_MODEL_H
#define TESSERACT_IGNITION_KINEMATIC_GROUPS_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract/tesseract.h>
#include <QStandardItemModel>
#include <QMetaType>
#endif

namespace tesseract_ignition
{

class KinematicGroupsModel : public QStandardItemModel
{
    Q_OBJECT
public:

  enum KinematicGroupsRoles {
      NameRole = Qt::UserRole + 1,
      TypeRole = Qt::UserRole + 2,
      DataRole = Qt::UserRole + 3
  };

  KinematicGroupsModel(QObject *parent = nullptr);
  KinematicGroupsModel(const KinematicGroupsModel &other);
  KinematicGroupsModel &operator=(const KinematicGroupsModel &other);
  ~KinematicGroupsModel() override = default;

  Q_INVOKABLE void setTesseract(tesseract::Tesseract::Ptr thor);
  Q_INVOKABLE void add(const QString& group_name, const QString& type, const QStringList& data);

  QHash<int, QByteArray> roleNames() const override;

private:
  tesseract::Tesseract::Ptr thor_ {nullptr};
};
}
#endif // TESSERACT_IGNITION_KINEMATIC_GROUPS_MODEL_H
