import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 1.4 as QC1
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item {
    id: element

    property alias opwKinematicsGroupNameComboBox: opwKinematicsGroupNameComboBox
    property alias addOPWKinematicsButton: addOPWKinematicsButton
    property alias opwKinematicsTableView: opwKinematicsTableView
    property alias removeOPWKinematicsButton: removeOPWKinematicsButton
    width: 400
    height: 800

    QC1.TableView {
        id: opwKinematicsTableView
        anchors.top: addOPWKinematicsButton.bottom
        anchors.topMargin: 10
        anchors.bottom: removeOPWKinematicsButton.top
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        model: opwKinematicsModel

        QC1.TableViewColumn {
            id: groupNameColumn
            role: "group_name"
            title: "Group Name"
            width: 125
        }
        QC1.TableViewColumn {
            id: kinematicParametersColumn
            role: "kinematic_parameters"
            title: "Kin Parameters"
            width: 125
        }
        QC1.TableViewColumn {
            id: offsetColumn
            role: "joint_offset"
            title: "Joint Offset"
            width: 125
        }
        QC1.TableViewColumn {
            id: signCorrectionColumn
            role: "joint_correction"
            title: "Joint Correction"
            width: opwKinematicsTableView.viewport.width - groupNameColumn.width
                   - kinematicParametersColumn.width - offsetColumn.width
        }
    }

    Button {
        id: removeOPWKinematicsButton
        width: 100
        text: qsTr("Remove")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    Label {
        id: groupNameLabel
        width: 125
        height: 40
        text: qsTr("Group Name:")
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        verticalAlignment: Text.AlignVCenter
    }

    ComboBox {
        id: opwKinematicsGroupNameComboBox
        anchors.verticalCenter: groupNameLabel.verticalCenter
        anchors.left: groupNameLabel.right
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        model: kinematicGroupsModel
        textRole: "name"
    }

    Button {
        id: addOPWKinematicsButton
        width: 100
        text: qsTr("Add")
        anchors.top: grid.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    Connections {
        target: opwKinematicsGroupNameComboBox
        onCurrentTextChanged: TesseractSetupWizard.onLoadJointGroup(
                                  opwKinematicsGroupNameComboBox.currentText)
    }

    Connections {
        target: addOPWKinematicsButton
        onClicked: TesseractSetupWizard.onAddUserDefinedJointState(
                       opwKinematicsGroupNameComboBox.currentText,
                       userDefinedJointStateNameTextField.text)
    }

    Connections {
        target: removeOPWKinematicsButton
        onClicked: TesseractSetupWizard.onRemoveUserDefinedJointState(
                       opwKinematicsTableView.currentRow)
    }

    GridLayout {
        id: grid
        height: 425
        anchors.top: groupNameLabel.bottom
        anchors.topMargin: 10
        columnSpacing: 5
        rows: 4
        columns: 6
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

        Label {
            id: kinematicsParameterLabel
            text: qsTr("Kinematic Parameters")
            Layout.columnSpan: 6
            Layout.fillWidth: false
            font.underline: true
            verticalAlignment: Text.AlignTop
        }

        Label {
            id: a1Label
            text: qsTr("a1:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: a1TextField
            text: qsTr("")
            placeholderText: "meters"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: a2Label
            text: qsTr("a2:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: a2TextField
            text: qsTr("")
            placeholderText: "meters"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: bLabel
            text: qsTr("b:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: bTextField
            text: qsTr("")
            placeholderText: "meters"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: c1Label
            text: qsTr("c1:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: c1TextField
            text: qsTr("")
            placeholderText: "meters"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: c2Label
            text: qsTr("c2:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: c2TextField
            text: qsTr("")
            placeholderText: "meters"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: c3Label
            text: qsTr("c3:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: c3TextField
            text: qsTr("")
            placeholderText: "meters"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: c4Label
            text: qsTr("c4:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: c4TextField
            text: qsTr("")
            placeholderText: "meters"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Item {
            id: element1
            Layout.columnSpan: 2
        }

        Label {
            id: jointOffsetsLabel
            text: qsTr("Joint Offsets")
            verticalAlignment: Text.AlignTop
            Layout.columnSpan: 6
            font.underline: true
            Layout.fillWidth: false
        }

        Label {
            id: jointOffsetLabel
            text: qsTr("J1:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: jointOffsetTextField
            text: qsTr("")
            placeholderText: "radians"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: jointOffsetLabel1
            text: qsTr("J2:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: jointOffsetTextField1
            text: qsTr("")
            placeholderText: "radians"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: jointOffsetLabel2
            text: qsTr("J3:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: jointOffsetTextField2
            text: qsTr("")
            placeholderText: "radians"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: jointOffsetLabel3
            text: qsTr("J4:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: jointOffsetTextField3
            text: qsTr("")
            placeholderText: "radians"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: jointOffsetLabel4
            text: qsTr("J5:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: jointOffsetTextField4
            text: qsTr("")
            placeholderText: "radians"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: jointOffsetLabel5
            text: qsTr("J6:")
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: false
        }

        TextField {
            id: jointOffsetTextField5
            text: qsTr("")
            placeholderText: "radians"
            Layout.fillWidth: true
            validator: DoubleValidator {
            }
        }

        Label {
            id: jointCorrectionsLabel
            text: qsTr("Joint Corrections")
            verticalAlignment: Text.AlignTop
            Layout.columnSpan: 6
            font.underline: true
            Layout.fillWidth: false
        }
        Switch {
            id: jointCorrectionSwitch
            text: qsTr("J1")
            Layout.columnSpan: 2
            Layout.fillWidth: false
        }

        Switch {
            id: jointCorrectionSwitch1
            text: qsTr("J2")
            Layout.columnSpan: 2
            Layout.fillWidth: false
        }

        Switch {
            id: jointCorrectionSwitch2
            text: qsTr("J3")
            Layout.columnSpan: 2
            Layout.fillWidth: false
        }

        Switch {
            id: jointCorrectionSwitch3
            text: qsTr("J4")
            Layout.columnSpan: 2
            Layout.fillWidth: false
        }

        Switch {
            id: jointCorrectionSwitch4
            text: qsTr("J5")
            Layout.columnSpan: 2
            Layout.fillWidth: false
        }

        Switch {
            id: jointCorrectionSwitch5
            text: qsTr("J6")
            Layout.columnSpan: 2
            Layout.fillWidth: false
        }
    }
}




/*##^## Designer {
    D{i:1;anchors_y:376}
}
 ##^##*/
