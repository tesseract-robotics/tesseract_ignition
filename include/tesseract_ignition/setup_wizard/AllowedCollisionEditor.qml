import QtQuick 2.4

AllowedCollisionEditorForm {
    removeButton.onClicked: {
        acmModel.remove(tableView.currentRow)
    }
}
