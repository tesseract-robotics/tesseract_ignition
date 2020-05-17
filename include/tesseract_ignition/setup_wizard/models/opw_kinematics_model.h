#ifndef TESSERACT_IGNITION_OPW_KINEMATICS_MODEL_H
#define TESSERACT_IGNITION_OPW_KINEMATICS_MODEL_H

#ifndef Q_MOC_RUN
#include <tesseract/tesseract.h>
#include <QStandardItemModel>
#include <QMetaType>
#include <QMap>
#endif

namespace tesseract_ignition
{

class OPWKinematicsModel : public QStandardItemModel
{
    Q_OBJECT
public:

  enum OPWKinematicsRoles {
      GroupNameRole = Qt::UserRole + 1,
      KinematicParametersRole = Qt::UserRole + 2,
      JointOffsetRole = Qt::UserRole + 3,
      JointCorrectionRole = Qt::UserRole + 4
  };

  OPWKinematicsModel(QObject *parent = nullptr);
  OPWKinematicsModel(const OPWKinematicsModel &other);
  OPWKinematicsModel &operator=(const OPWKinematicsModel &other);
  ~OPWKinematicsModel() override = default;

  Q_INVOKABLE void setTesseract(tesseract::Tesseract::Ptr thor);
  Q_INVOKABLE void add(const QString& group_name,
                       double a1, double a2, double b,
                       double c1, double c2, double c3, double c4,
                       double o1, double o2, double o3, double o4, double o5, double o6,
                       int sc1, int sc2, int sc3, int sc4, int sc5, int sc6);

  QHash<int, QByteArray> roleNames() const override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
  tesseract::Tesseract::Ptr thor_ {nullptr};

  /**
   * @brief A helper function to add the opw data to the model.
   * @param opw OPW Kinematics data to be added
   */
  void addItem(const QString &group_name, const tesseract_scene_graph::SRDFModel::OPWKinematicParameters& opw);
};

}
#endif // TESSERACT_IGNITION_OPW_KINEMATICS_MODEL_H
