import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {

    property alias loadButton: loadButton
    property alias srdfTextField: srdfTextField
    property alias srdfBrowseButton: srdfBrowseButton
    property alias urdfTextField: urdfTextField
    property alias urdfBrowseButton: urdfBrowseButton
    id: loadSetupWizardFilesPage

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

    Connections {
        target: loadButton
        onClicked: TesseractSetupWizard.onLoad(urdfTextField.text,
                                               srdfTextField.text)
    }
}




/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
