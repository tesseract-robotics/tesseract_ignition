import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 1.4 as QC1
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

UserDefinedJointStatesForm {

    function setKinematicGroupsModel(model)
    {
        jointStateGroupNameComboBox.model = model;
    }

    function setUserDefinedStatesModel(model)
    {
        userDefinedJointStateTableView.model = model;
    }

    function setJointGroupsModel(model)
    {
        jointGroupListView.model = model;
    }

//    jointGroupListView.delegate: Item {
//        id: element1
//        width: parent.width
//        height: 40

//        Label {
//            id: jointLabel1
//            width: 125
//            height: 40
//            text: model.name
//            anchors.left: parent.left
//            anchors.leftMargin: 5
//            verticalAlignment: Text.AlignVCenter
//        }
//        Slider {
//            id: slider1
//            stepSize: 0.01
//            anchors.right: spinBox1.left
//            anchors.rightMargin: 5
//            anchors.left: jointLabel1.right
//            anchors.leftMargin: 5
//            anchors.verticalCenter: jointLabel1.verticalCenter
//            from: model.min
//            to: model.max
//            value: model.data
//            Binding on value {
//                when: spinBox1.focus
//                value: spinBox1.value
//            }
//            onValueChanged: {
//                model.data = value;
//            }
//            Connections {
//                target: slider1
//                onValueChanged: TesseractSetupWizard.onJointValue(model.name, slider1.value)
//            }
//        }
//        QC1.SpinBox {
//            id: spinBox1
//            anchors.verticalCenter: jointLabel1.verticalCenter
//            anchors.right: parent.right
//            anchors.rightMargin: 5
//            style: SpinBoxStyle {
//                background: Rectangle {
//                    implicitWidth: 70
//                    implicitHeight: 40
//                    border.color: "gray"
//                }
//            }
//            decimals: 2
//            value: model.data
//            minimumValue: model.min
//            maximumValue: model.max
//            Binding on value {
//                when: slider1.pressed
//                value: slider1.value
//            }

//        }
//    }
}
