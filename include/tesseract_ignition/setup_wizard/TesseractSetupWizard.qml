import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "qrc:/TesseractSetupWizard/qml"

Item {
    id: tsw
    Layout.minimumWidth: 400
    Layout.minimumHeight: 600
    anchors.fill: parent

    ToolBar {
        id: overlayHeader
        width: parent.width
//        parent: window.overlay

        RowLayout {
            anchors.fill: parent
            ToolButton {
                Image {
                    id: iconlogo
                    anchors.fill: parent
                    source: "qrc:/TesseractSetupWizard/logo.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Label {
                text: "Tesseract Setup Wizard"
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                text: qsTr("⋮")
                onClicked: wizardDrawer.open()
            }
        }
    }

    Drawer {
        id: wizardDrawer
        visible: false

        y: overlayHeader.height

        width: (parent.width / 4) > 300 ? 300 : (parent.width / 4)
        height: tsw.height

        ListView {
            id: drawerListView
            anchors.fill: parent
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            focus: true

            header: Pane {
                id: header
                z: 2
                width: parent.width

                contentHeight: logo.height

                Image {
                    id: logo
                    width: parent.width
                    source: "qrc:/TesseractSetupWizard/logo.png"
                    fillMode: implicitWidth > width ? Image.PreserveAspectFit : Image.Pad
                }

                MenuSeparator {
                    parent: header
                    width: parent.width
                    anchors.verticalCenter: parent.bottom
                    visible: !drawerListView.atYBeginning
                }
            }

            footer: ItemDelegate {
                id: footer
                text: qsTr("Southwest Research Institute")
                width: parent.width

                MenuSeparator {
                    parent: footer
                    width: parent.width
                    anchors.verticalCenter: parent.top
                }
            }

            ListModel {
                id: nameModel
                ListElement { name: "Load" }
                ListElement { name: "Allowed Collisions" }
                ListElement { name: "Kinematic Groups" }
                ListElement { name: "User States" }
            }

            model: nameModel

            delegate: ItemDelegate {
                text: name
                width: parent.width
                onClicked: {
                    drawerListView.currentIndex = index
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }

        }
        Connections{
            target: wizardSwipeView
            onCurrentIndexChanged: {
                drawerListView.currentIndex = wizardSwipeView.currentIndex
            }
        }
    }

    SwipeView {
        id: wizardSwipeView
        clip: true
        anchors.fill: parent
        anchors.topMargin: overlayHeader.height
        currentIndex: 0

        Page {
            y: overlayHeader.height
            header: Label {
                text: qsTr("Load URDF")
                font.underline: true
                topPadding: 10
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 11
                height: 30
            }
            LoadSetupWizardFiles {
                id: loadPage
                anchors.fill: parent
            }
        }

        Page {
            y: overlayHeader.height
            header: Label {
                text: qsTr("Allowed Collision Matrix")
                font.underline: true
                topPadding: 10
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 11
                height: 30
            }
            AllowedCollisionEditor {
                id: acmPage
                anchors.fill: parent
            }
        }

        Page {
            y: overlayHeader.height
            header: Label {
                text: qsTr("Kinematic Groups")
                font.underline: true
                topPadding: 10
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 11
                height: 30
            }
            KinematicGroups {
                id: kinGroupsPage
                anchors.fill: parent
            }
        }

        Page {
            y: overlayHeader.height
            header: Label {
                text: qsTr("User Defined Joint States")
                font.underline: true
                topPadding: 10
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 11
                height: 30
            }
            UserDefinedJointStates {
                id: userDefinedStatesPage
                anchors.fill: parent
            }
        }

        Connections{
            target: drawerListView
            onCurrentIndexChanged: {
                wizardSwipeView.currentIndex = drawerListView.currentIndex
            }
        }
    }

    PageIndicator {
        id: wizardPageIndicator
        count: wizardSwipeView.count
        currentIndex: wizardSwipeView.currentIndex
        interactive: true

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
