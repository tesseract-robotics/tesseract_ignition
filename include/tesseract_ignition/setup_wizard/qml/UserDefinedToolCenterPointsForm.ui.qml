import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 1.4 as QC1
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item {
    id: element
    property alias userDefinedToolCenterPointNameTextField: userDefinedToolCenterPointNameTextField
    property alias tcpGroupNameComboBox: tcpGroupNameComboBox
    property alias userDefinedPositionXTextField: userDefinedPositionXTextField
    property alias userDefinedPositionYTextField: userDefinedPositionYTextField
    property alias userDefinedPositionZTextField: userDefinedPositionZTextField
    property alias userDefinedOrientationRTextField: userDefinedOrientationRTextField
    property alias userDefinedOrientationPTextField: userDefinedOrientationPTextField
    property alias userDefinedOrientationYTextField: userDefinedOrientationYTextField
    property alias addUserDefinedToolCenterPointButton: addUserDefinedToolCenterPointButton
    property alias userDefinedToolCenterPointTableView: userDefinedToolCenterPointTableView

    Label {
        id: userDefinedToolCenterPointNameLabel
        width: 125
        height: 40
        text: qsTr("TCP Name:")
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        verticalAlignment: Text.AlignVCenter
    }

    TextField {
        id: userDefinedToolCenterPointNameTextField
        y: 5
        text: qsTr("")
        placeholderText: "Tool Center Point Name"
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: userDefinedToolCenterPointNameLabel.right
        anchors.leftMargin: 5
        anchors.verticalCenter: userDefinedToolCenterPointNameLabel.verticalCenter
        validator: RegExpValidator {
            regExp: /[0-9A-Za-z_ ]+/
        }
    }

    QC1.TableView {
        id: userDefinedToolCenterPointTableView
        anchors.top: addUserDefinedToolCenterPointButton.bottom
        anchors.topMargin: 10
        anchors.bottom: removeUserDefinedToolCenterPointButton.top
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        model: userDefinedTCPModel

        QC1.TableViewColumn {
            id: groupNameColumn
            role: "group_name"
            title: "Group Name"
            width: 125
        }
        QC1.TableViewColumn {
            id: tcpNameColumn
            role: "tcp_name"
            title: "TCP Name"
            width: 125
        }
        QC1.TableViewColumn {
            id: postionColumn
            role: "position"
            title: "Position (m)"
            width: 125
        }
        QC1.TableViewColumn {
            id: orientationColumn
            role: "orientation"
            title: "Orientation RPY (radians)"
            width: userDefinedToolCenterPointTableView.viewport.width
                   - groupNameColumn.width - tcpNameColumn.width - postionColumn.width
        }
    }

    Button {
        id: removeUserDefinedToolCenterPointButton
        text: qsTr("Remove")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    Label {
        id: userDefinedGroupNameLabel
        width: 125
        height: 40
        text: qsTr("Group Name:")
        anchors.top: userDefinedToolCenterPointNameLabel.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        verticalAlignment: Text.AlignVCenter
    }

    ComboBox {
        id: tcpGroupNameComboBox
        y: 55
        anchors.left: userDefinedGroupNameLabel.right
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        model: kinematicGroupsModel
        textRole: "name"
    }

    Button {
        id: addUserDefinedToolCenterPointButton
        width: 100
        text: qsTr("Add TCP")
        anchors.top: userDefinedOrientationNameLabel.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    Connections {
        target: addUserDefinedToolCenterPointButton
        onClicked: TesseractSetupWizard.onAddUserDefinedTCP(
                       tcpGroupNameComboBox.currentText,
                       userDefinedToolCenterPointNameTextField.text,
                       Qt.vector3d(userDefinedPositionXTextField.text,
                                   userDefinedPositionYTextField.text,
                                   userDefinedPositionZTextField.text),
                       Qt.vector3d(userDefinedOrientationRTextField.text,
                                   userDefinedOrientationPTextField.text,
                                   userDefinedOrientationYTextField.text))
    }

    Connections {
        target: removeUserDefinedToolCenterPointButton
        onClicked: TesseractSetupWizard.onRemoveUserDefinedJointState(
                       userDefinedToolCenterPointTableView.currentRow)
    }

    Label {
        id: userDefinedPositionNameLabel
        width: 125
        height: 40
        text: qsTr("Position:")
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: userDefinedGroupNameLabel.bottom
        anchors.topMargin: 10
        verticalAlignment: Text.AlignVCenter
    }
    RowLayout {
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: userDefinedPositionNameLabel.right
        anchors.leftMargin: 5
        anchors.verticalCenter: userDefinedPositionNameLabel.verticalCenter
        Label {
            id: userDefinedPositionXNameLabel
            text: qsTr("X:")
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            id: userDefinedPositionXTextField
            text: qsTr("")
            Layout.fillHeight: false
            Layout.fillWidth: true
            placeholderText: "x (m)"
            validator: DoubleValidator {
            }
        }

        Label {
            id: userDefinedPositionYNameLabel
            text: qsTr("Y:")
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            id: userDefinedPositionYTextField
            //            width: userDefinedPositionXTextField.width
            text: qsTr("")
            Layout.fillWidth: true
            placeholderText: "y (m)"
            validator: DoubleValidator {
            }
        }

        Label {
            id: userDefinedPositionZNameLabel
            text: qsTr("Z:")
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            id: userDefinedPositionZTextField
            //            width: userDefinedPositionXTextField.width
            text: qsTr("")
            Layout.fillWidth: true
            placeholderText: "z (m)"
            validator: DoubleValidator {
            }
        }
    }

    RowLayout {
        anchors.verticalCenterOffset: 0
        Label {
            id: userDefinedOrientationRNameLabel
            text: qsTr("R:")
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            id: userDefinedOrientationRTextField
            text: qsTr("")
            placeholderText: "roll (radians)"
            Layout.fillWidth: true
            Layout.fillHeight: false
            validator: DoubleValidator {
            }
        }

        Label {
            id: userDefinedOrientationPNameLabel
            text: qsTr("P:")
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            id: userDefinedOrientationPTextField
            text: qsTr("")
            placeholderText: "pitch (radians)"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: userDefinedOrientationYNameLabel
            text: qsTr("Y:")
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            id: userDefinedOrientationYTextField
            text: qsTr("")
            placeholderText: "yaw (radians)"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }
        anchors.leftMargin: 5
        anchors.verticalCenter: userDefinedOrientationNameLabel.verticalCenter
        anchors.right: parent.right
        anchors.left: userDefinedOrientationNameLabel.right
        anchors.rightMargin: 5
    }

    Label {
        id: userDefinedOrientationNameLabel
        width: 125
        height: 40
        text: qsTr("Orientation:")
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 10
        anchors.top: userDefinedPositionNameLabel.bottom
        anchors.leftMargin: 5
        anchors.left: parent.left
    }
}




/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:12;anchors_y:265}D{i:15;anchors_x:"-9";anchors_y:"-8"}
D{i:36;anchors_x:"-9";anchors_y:"-8"}
}
 ##^##*/
