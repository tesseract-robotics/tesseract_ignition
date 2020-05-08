import QtQuick 2.4

AllowedCollisionEditorForm {
    generateButton.onClicked: {
    }
    function setAllowedCollisionMatrixModel(model)
    {
        acmTableView.model = model;
    }

    removeButton.onClicked: {
        acmTableView.model.remove(acmTableView.currentRow)
    }
}
