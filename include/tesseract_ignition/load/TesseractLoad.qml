import QtQuick 2.4
import QtQuick.Dialogs 1.1
import QtQuick.Controls 2.2
import Qt.labs.folderlistmodel 2.1
import QtQuick.Layouts 1.3
import "qrc:/TesseractLoad"

ColumnLayout {
  id: tesseractLoadLayout
  width: 400
  Layout.minimumWidth: 100
  Layout.minimumHeight: 50
  anchors.fill: parent
  anchors.leftMargin: 10
  anchors.rightMargin: 10

FileDialog{
    id: urdfFileDialog;
    title: "Please choose a URDF file";
    folder: shortcuts.home
    nameFilters: ["URDF Files (*.urdf)"];
    selectFolder: false
    selectMultiple: false
    onAccepted: {
        console.info("User has selected URDF:" + urdfFileDialog.fileUrl);
        loadTesseract.urdfTextField.text = urdfFileDialog.fileUrl.toString();
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
        loadTesseract.srdfTextField.text = srdfFileDialog.fileUrl.toString();
        srdfFileDialog.close();
    }
}

TesseractLoadForm
{
  id: loadTesseract

  urdfBrowseButton.onClicked: {
      console.info("URDF browser button selected");
      urdfFileDialog.open();
  }

  srdfBrowseButton.onClicked: {
      console.info("SRDF browser button selected");
      srdfFileDialog.open();
  }
}

}
