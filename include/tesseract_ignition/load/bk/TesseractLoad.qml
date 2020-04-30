import QtQuick 2.4
import QtQuick.Dialogs 1.1
import QtQuick.Controls 2.2
import Qt.labs.folderlistmodel 2.1

Column {
    width: 400
    height: 200

FileDialog{
    id: urdfFileDialog;
    title: "Please choose a URDF file";
    folder: shortcuts.home
    nameFilters: ["URDF Files (*.urdf)"];
    selectFolder: false
    selectMultiple: false
    onAccepted: {
        console.info("User has selected URDF:" + urdfFileDialog.fileUrl);
        urdfTextField.text = urdfFileDialog.fileUrl.toString();
        urdfFileDialog.close();
    }
}

FileDialog{
    id: srdfFileDialog;
    title: "Please choose a SRDF file";
    folder: shortcuts.home
    nameFilters: ["SRDF Files (*.srdf)"];
    selectFolder: false
    selectMultiple: false
    onAccepted: {
        console.info("User has selected SRDF:" + srdfFileDialog.fileUrl);
        srdfTextField.text = srdfFileDialog.fileUrl.toString();
        srdfFileDialog.close();
    }
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
    onClicked: {
          console.info("URDF browser button selected");
          urdfFileDialog.open();
      }
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
    onClicked: {
          console.info("SRDF browser button selected");
          srdfFileDialog.open();
      }
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
    anchors.topMargin: 6
    anchors.right: parent.right
    clip: false
    anchors.rightMargin: 10
    onClicked: {
          console.info("Load button selected");
          TesseractLoad.OnLoadTesseract(urdfTextField.text, srdfTextField.text);
      }
}

}
