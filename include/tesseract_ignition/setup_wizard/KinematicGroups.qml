import QtQuick 2.4
import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 2.2

KinematicGroupsForm {

    function setLinkModel(model)
    {
        linkNamesComboBox.model = model;
        baselinkComboBox.model = model;
        tiplinkComboBox.model = model;
    }

    function setJointModel(model)
    {
        jointNamesComboBox.model = model;
    }

    function setKinematicGroupsModel(model)
    {
        groupsTableView.model = model;
    }

    function setLinkListViewModel(model)
    {
        linkListView.model = model;
    }

    function setJointListViewModel(model)
    {
        jointListView.model = model;
    }

    linkListView.delegate: ItemDelegate {
        text: display
        width: parent.width
        onClicked: linkListView.currentIndex = index
    }

    jointListView.delegate: ItemDelegate {
        text: display
        width: parent.width
        onClicked: jointListView.currentIndex = index
    }
}
