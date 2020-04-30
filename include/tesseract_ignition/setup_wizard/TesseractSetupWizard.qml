import QtQuick 2.4
import QtQuick.Dialogs 1.1
import QtQuick.Controls 2.2
import Qt.labs.folderlistmodel 2.1
import QtQuick.Layouts 1.3
import "qrc:/TesseractSetupWizard"

ColumnLayout {
  id: tesseractSetupWizardLayout
  width: 400
  Layout.minimumWidth: 400
  Layout.minimumHeight: 400

FileDialog{
    id: urdfFileDialog;
    title: "Please choose a URDF file";
    folder: shortcuts.home
    nameFilters: ["URDF Files (*.urdf)"];
    selectFolder: false
    selectMultiple: false
    onAccepted: {
        console.info("User has selected URDF:" + urdfFileDialog.fileUrl);
        tesseractSetupWizard.urdfTextField.text = urdfFileDialog.fileUrl.toString();
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
        tesseractSetupWizard.srdfTextField.text = srdfFileDialog.fileUrl.toString();
        srdfFileDialog.close();
    }
}

TesseractSetupWizardForm {
    id: tesseractSetupWizard;
    Layout.fillHeight: true;
    Layout.fillWidth: true;

    wizardPageIndicator.onCurrentIndexChanged: {
        wizardSwipeView.currentIndex = wizardPageIndicator.currentIndex;
    }

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
