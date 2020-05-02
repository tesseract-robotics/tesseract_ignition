import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: window

    ToolBar {
        id: overlayHeader
        width: parent.width
//        parent: window.overlay

        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("‹")
                onClicked: wizardDrawer.close()
            }
            ToolButton {
                text: qsTr("›")
                onClicked: wizardDrawer.open()
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
        width: (window.width / 4) > 300 ? 300 : (window.width / 4)
        height: window.height - overlayHeader.height

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
                    source: "logo.png"
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
            Loader {
                id: loadPageLoader
                anchors.fill: parent
                source: "LoadSetupWizardFiles.qml"
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
            Loader {
                id: acmPageLoader
                anchors.fill: parent
                source: "AllowedCollisionEditor.qml"
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
            Loader {
                id: kinGroupsPageLoader
                anchors.fill: parent
                source: "KinematicGroups.qml"
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

//    Flickable {
//        id: flickable

//        anchors.fill: parent
//        anchors.topMargin: overlayHeader.height

//        topMargin: 20
//        bottomMargin: 20
//        contentHeight: pageLoader.height

//        Loader { id: pageLoader }

//        ScrollIndicator.vertical: ScrollIndicator { }

//        Connections{
//            target: drawerListView
//            onCurrentIndexChanged: {
//                if (drawerListView.currentIndex == 0) {
//                    pageLoader.source = "LoadSetupWizardFiles.qml"
//                }
//                else if (drawerListView.currentIndex == 1) {
//                    pageLoader.source = "AllowedCollisionEditor.qml"
//                }
//            }
//        }
//    }
}
