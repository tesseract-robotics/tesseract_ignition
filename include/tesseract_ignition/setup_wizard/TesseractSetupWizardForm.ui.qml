import QtQuick 2.4
import QtQuick.Controls 1.4 as QC1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: tesseractSetupWizardForm
    width: 400
    height: 300
    property alias wizardSwipeView: wizardSwipeView
    property alias wizardPageIndicator: wizardPageIndicator
    property alias tesseractSetupWizardForm: tesseractSetupWizardForm
    property alias loadButton: loadButton
    property alias srdfTextField: srdfTextField
    property alias srdfBrowseButton: srdfBrowseButton
    property alias urdfTextField: urdfTextField
    property alias urdfBrowseButton: urdfBrowseButton

    SwipeView {
        id: wizardSwipeView
        clip: true
        anchors.fill: parent
        currentIndex: 2

        Page {
            id: loadURDFPage
            header: Label {
                text: qsTr("Load URDF")
                font.underline: true
                topPadding: 10
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 11
                height: 30
            }

            Label {
                id: urdfLabel
                width: 65
                height: 30
                text: qsTr("URDF File:")
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 11
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 8
            }

            Button {
                id: urdfBrowseButton
                y: 26
                width: 100
                height: 50
                text: qsTr("Browse")
                anchors.right: parent.right
                anchors.rightMargin: 10
                clip: false
                anchors.verticalCenter: urdfLabel.verticalCenter
            }

            TextField {
                id: urdfTextField
                y: 24
                height: 50
                text: ""
                anchors.right: urdfBrowseButton.left
                anchors.rightMargin: 10
                anchors.left: urdfLabel.right
                anchors.leftMargin: 10
                anchors.verticalCenterOffset: 0
                placeholderText: qsTr("URDF filepath")
                horizontalAlignment: Text.AlignLeft
                leftPadding: 10
                rightPadding: 0
                bottomPadding: 0
                topPadding: 0
                readOnly: false
                anchors.verticalCenter: urdfLabel.verticalCenter
            }

            Label {
                id: srdfLabel
                x: -9
                y: -9
                width: 65
                height: 30
                text: qsTr("SRDF File:")
                verticalAlignment: Text.AlignVCenter
                anchors.top: urdfLabel.bottom
                anchors.left: parent.left
                font.pointSize: 11
                anchors.topMargin: 26
                anchors.leftMargin: 8
            }

            Button {
                id: srdfBrowseButton
                x: 290
                y: 17
                width: 100
                height: 50
                text: qsTr("Browse")
                anchors.verticalCenterOffset: 0
                anchors.right: parent.right
                clip: false
                anchors.verticalCenter: srdfLabel.verticalCenter
                anchors.rightMargin: 10
            }

            TextField {
                id: srdfTextField
                x: -9
                y: 15
                height: 50
                text: ""
                anchors.verticalCenterOffset: 0
                leftPadding: 10
                rightPadding: 0
                bottomPadding: 0
                anchors.right: srdfBrowseButton.left
                anchors.left: srdfLabel.right
                topPadding: 0
                anchors.verticalCenter: srdfLabel.verticalCenter
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                readOnly: false
                placeholderText: qsTr("SRDF filepath")
                horizontalAlignment: Text.AlignLeft
            }

            Button {
                id: loadButton
                x: 290
                width: 100
                height: 50
                text: qsTr("Load")
                anchors.top: srdfBrowseButton.bottom
                anchors.topMargin: 10
                anchors.right: parent.right
                clip: false
                anchors.rightMargin: 10
            }
        }

        Page {
            id: defineAllowedCollisionMatrixPage
            header: Label {
                text: qsTr("Allowed Collision Matrix")
                font.underline: true
                topPadding: 10
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 11
                height: 30
            }

            Slider {
                id: slider
                anchors.right: generateButton.left
                anchors.rightMargin: 5
                anchors.left: label.right
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 0
            }

            Label {
                id: label
                y: 22
                text: qsTr("Resolution:")
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 11
                height: 30
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: slider.verticalCenter
            }

            Button {
                id: generateButton
                x: 310
                y: 0
                width: 100
                text: qsTr("Generate")
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: slider.verticalCenter
            }

            ListModel {
                id: acmModel
                ListElement {
                    name: 'item1'
                    link1: "link_1"
                    link2: "link_2"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item1'
                    link1: "link_5"
                    link2: "link_2"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item2'
                    link1: "link_2"
                    link2: "link_3"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item3'
                    link1: "link_3"
                    link2: "link_4"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item4'
                    link1: "link_4"
                    link2: "link_5"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item5'
                    link1: "link_5"
                    link2: "link_6"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item5'
                    link1: "link_5"
                    link2: "link_6"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item5'
                    link1: "link_5"
                    link2: "link_6"
                    reason: "Adjacent"
                }
                ListElement {
                    name: 'item5'
                    link1: "link_5"
                    link2: "link_6"
                    reason: "Adjacent"
                }
            }

            // https://stackoverflow.com/questions/45168702/canonical-way-to-make-custom-tableview-from-listview-in-qt-quick
            QC1.TableView {
                id: tableView
                sortIndicatorColumn: 1
                sortIndicatorVisible: false
                backgroundVisible: true
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.bottom: removeButton.top
                anchors.bottomMargin: 5
                anchors.top: slider.bottom
                anchors.topMargin: 10
                model: acmModel

                QC1.TableViewColumn {
                    id: nameColumn
                    role: "name"
                    title: "Item"
                    width: 100
                }
                QC1.TableViewColumn {
                    id: link1Column
                    role: "link1"
                    title: "Link 1"
                    width: 100
                }
                QC1.TableViewColumn {
                    id: link2Column
                    role: "link2"
                    title: "Link 2"
                    width: 100
                }
                QC1.TableViewColumn {
                    id: reasonColumn
                    role: "reason"
                    title: "Reason"
                    width: tableView.viewport.width - nameColumn.width
                           - link1Column.width - link2Column.width
                }
            }

            Button {
                id: removeButton
                x: 308
                y: 186
                text: qsTr("Remove")
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
            }
        }
        //        Page {
        //            id: defineKinematicGroups
        //            header: Label {
        //                text: qsTr("Kinematic Groups")
        //                font.underline: true
        //                topPadding: 10
        //                horizontalAlignment: Text.AlignHCenter
        //                font.pointSize: 11
        //                height: 30
        //            }
        //        }
    }

    PageIndicator {
        id: wizardPageIndicator
        count: wizardSwipeView.count
        currentIndex: wizardSwipeView.currentIndex
        interactive: true

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Connections {
        target: loadButton
        onClicked: TesseractSetupWizard.onLoad(urdfTextField.text,
                                               srdfTextField.text)
    }
}
