#ifndef TESSERACT_IGNITION_USER_DEFINED_TCP_MODEL_H
#define TESSERACT_IGNITION_USER_DEFINED_TCP_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract/tesseract.h>
#include <QStandardItemModel>
#include <QMetaType>
#include <QMap>
#endif

namespace tesseract_ignition
{

class UserDefinedTCPModel : public QStandardItemModel
{
    Q_OBJECT
public:

  enum UserDefinedTCPRoles {
      GroupNameRole = Qt::UserRole + 1,
      TCPNameRole = Qt::UserRole + 2,
      PositionRole = Qt::UserRole + 3,
      OrientationRole = Qt::UserRole + 4
  };

  UserDefinedTCPModel(QObject *parent = nullptr);
  UserDefinedTCPModel(const UserDefinedTCPModel &other);
  UserDefinedTCPModel &operator=(const UserDefinedTCPModel &other);
  ~UserDefinedTCPModel() override = default;

  Q_INVOKABLE void setTesseract(tesseract::Tesseract::Ptr thor);
  Q_INVOKABLE void add(const QString& group_name,
                       const QString& tcp_name,
                       const QVector3D &position,
                       const QVector3D &orientation);

  QHash<int, QByteArray> roleNames() const override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
  tesseract::Tesseract::Ptr thor_ {nullptr};
};

}
#endif // TESSERACT_IGNITION_USER_DEFINED_TCP_MODEL_H
