import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 1.4 as QC1
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item {
    id: element
    anchors.fill: parent

    // Exposed objects
    property alias removeGroupButton: removeGroupButton
    property alias groupsTableView: groupsTableView
    property alias groupNameTextField: groupNameTextField
    property alias baselinkComboBox: baselinkComboBox
    property alias tiplinkComboBox: tiplinkComboBox
    property alias chainGroupAddButton: chainGroupAddButton

    property alias removeJointButton: removeJointButton
    property alias jointListView: jointListView
    property alias jointNamesComboBox: jointNamesComboBox
    property alias addJointButton: addJointButton
    property alias jointGroupAddButton: jointGroupAddButton

    property alias removeLinkButton: removeLinkButton
    property alias linkListView: linkListView
    property alias linkNamesComboBox: linkNamesComboBox
    property alias addLinkButton: addLinkButton
    property alias linkGroupAddButton: linkGroupAddButton

    Item {
        id: frame
        visible: true
        anchors.fill: parent

        TabBar {
            id: tabBar
            anchors.topMargin: 10
            currentIndex: 0
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: groupNameLabel.bottom
            TabButton {
                text: qsTr("Chain")
            }
            TabButton {
                text: qsTr("Joints")
            }
            TabButton {
                text: qsTr("Links")
            }
        }
        StackLayout {
            currentIndex: tabBar.currentIndex
            anchors.bottom: groupsTableView.top
            anchors.bottomMargin: 5
            anchors.top: tabBar.bottom
            anchors.right: parent.right
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            Item {
                id: chainTab

                Label {
                    id: label
                    width: 70
                    height: 40
                    text: qsTr("Base Link:")
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 10
                }

                Label {
                    id: label1
                    width: 70
                    height: 40
                    text: qsTr("Tip Link:")
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: label.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                }

                Button {
                    id: chainGroupAddButton
                    x: 192
                    text: qsTr("Add Group")
                    anchors.top: tiplinkComboBox.bottom
                    anchors.topMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                }

                ComboBox {
                    id: baselinkComboBox
                    textRole: "display"
                    anchors.left: label.right
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: label.verticalCenter
                    model: linkModel
                }

                ComboBox {
                    id: tiplinkComboBox
                    textRole: "display"
                    anchors.left: label1.right
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: label1.verticalCenter
                    model: linkModel
                }
                Connections {
                    target: chainGroupAddButton
                    onClicked: TesseractSetupWizard.onAddChainGroup(
                                   groupNameTextField.text,
                                   baselinkComboBox.currentText,
                                   tiplinkComboBox.currentText)
                }
            }
            Item {
                id: jointsTab

                Label {
                    id: label2
                    height: 40
                    text: qsTr("Joint Names:")
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: jointNamesComboBox
                    textRole: "name"
                    anchors.right: addJointButton.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: label2.verticalCenter
                    anchors.left: label2.right
                    anchors.leftMargin: 5
                    model: jointModel
                }
                Frame {
                    clip: true
                    padding: 5
                    anchors.right: removeJointButton.left
                    anchors.rightMargin: 5
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.top: jointNamesComboBox.bottom
                    anchors.topMargin: 5
                    anchors.left: jointNamesComboBox.left
                    anchors.leftMargin: 0
                    ListView {
                        id: jointListView
                        spacing: 5
                        anchors.fill: parent
                        highlight: Rectangle {
                            color: "lightsteelblue"
                            radius: 5
                        }
                        focus: true
                        model: jointListViewModel
                    }
                }

                Button {
                    id: addJointButton
                    width: 110
                    text: qsTr("Add")
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: label2.verticalCenter
                }

                Button {
                    id: removeJointButton
                    width: 110
                    text: qsTr("Remove")
                    anchors.top: addJointButton.bottom
                    anchors.topMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                }

                Button {
                    id: jointGroupAddButton
                    width: 110
                    text: qsTr("Add Group")
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                }
                Connections {
                    target: jointGroupAddButton
                    onClicked: TesseractSetupWizard.onAddJointGroup(
                                   groupNameTextField.text)
                }
                Connections {
                    target: addJointButton
                    onClicked: TesseractSetupWizard.onAddJointGroupJoint(
                                   jointNamesComboBox.currentText)
                }
                Connections {
                    target: removeJointButton
                    onClicked: TesseractSetupWizard.onRemoveJointGroupJoint(
                                   jointListView.currentRow)
                }
            }
            Item {
                id: linksTab

                Label {
                    id: label3
                    height: 40
                    text: qsTr("Link Names:")
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: linkNamesComboBox
                    textRole: "display"
                    anchors.right: addLinkButton.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: label3.verticalCenter
                    anchors.left: label3.right
                    anchors.leftMargin: 5
                    model: linkModel
                }
                Frame {
                    clip: true
                    padding: 5
                    anchors.right: removeLinkButton.left
                    anchors.rightMargin: 5
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.top: linkNamesComboBox.bottom
                    anchors.topMargin: 5
                    anchors.left: linkNamesComboBox.left
                    anchors.leftMargin: 0
                    ListView {
                        id: linkListView
                        spacing: 5
                        anchors.fill: parent
                        highlight: Rectangle {
                            color: "lightsteelblue"
                            radius: 5
                        }
                        focus: true
                        model: linkListViewModel
                    }
                }

                Button {
                    id: addLinkButton
                    width: 110
                    text: qsTr("Add")
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: label3.verticalCenter
                }

                Button {
                    id: removeLinkButton
                    width: 110
                    text: qsTr("Remove")
                    anchors.top: addLinkButton.bottom
                    anchors.topMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                }

                Button {
                    id: linkGroupAddButton
                    width: 110
                    text: qsTr("Add Group")
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                }
                Connections {
                    target: linkGroupAddButton
                    onClicked: TesseractSetupWizard.onAddLinkGroup(
                                   groupNameTextField.text)
                }
                Connections {
                    target: addLinkButton
                    onClicked: TesseractSetupWizard.onAddLinkGroupLink(
                                   linkNamesComboBox.currentText)
                }
                Connections {
                    target: removeLinkButton
                    onClicked: TesseractSetupWizard.onRemoveLinkGroupLink(
                                   linkListView.currentRow)
                }
            }
        }

        Label {
            id: groupNameLabel
            height: 40
            text: qsTr("Group Name:")
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
        }

        TextField {
            id: groupNameTextField
            y: 5
            text: qsTr("")
            placeholderText: "Group Name"
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.left: groupNameLabel.right
            anchors.leftMargin: 5
            anchors.verticalCenter: groupNameLabel.verticalCenter
            validator: RegExpValidator {
                regExp: /[0-9A-Za-z_]+/
            }
        }

        QC1.TableView {
            id: groupsTableView
            anchors.bottom: removeGroupButton.top
            anchors.bottomMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 274
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            model: kinematicGroupsModel

            QC1.TableViewColumn {
                id: nameColumn
                role: "name"
                title: "Group Name"
                width: 100
            }
            QC1.TableViewColumn {
                id: typeColumn
                role: "type"
                title: "Type"
                width: 100
            }
            QC1.TableViewColumn {
                id: dataColumn
                role: "data"
                title: "Data"
                width: groupsTableView.viewport.width - nameColumn.width - typeColumn.width
            }
        }

        Button {
            id: removeGroupButton
            x: 190
            text: qsTr("Remove")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
        }
        Connections {
            target: removeGroupButton
            onClicked: TesseractSetupWizard.onRemoveKinematicGroup(
                           groupsTableView.currentRow)
        }
    }
}




/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
