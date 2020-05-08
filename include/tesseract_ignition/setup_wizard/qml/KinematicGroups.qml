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

    function setJointGroupModel(model)
    {
        jointGroupModel = model;
    }

    linkGroupAddButton.onClicked: {
        if (!!groupNameTextField.text)
        {
            if (linkListView.model.count > 0)
            {
                var data = linkListView.model.get(0).name;
                for (var i=1; i < linkListView.model.count; i++)
                {
                    data = data + ";" + linkListView.model.get(i).name;
                }
                var group ={'name': groupNameTextField.text, 'type' : 'Link List', 'data' : data};
                groupsTableView.model.append(group);
            }
        }
    }

    linkListView.delegate: ItemDelegate {
        text: name
        width: parent.width
        onClicked: linkListView.currentIndex = index
    }

    addLinkButton.onClicked: {
        var link ={'name': linkNamesComboBox.currentText}
        linkListView.model.append(link)
    }

    removeLinkButton.onClicked: {
        if (linkListView.currentIndex >= 0)
        {
            linkListView.model.remove(linkListView.currentRow);
        }
    }

    jointGroupAddButton.onClicked: {
        if (!!groupNameTextField.text)
        {
            if (jointListView.model.count > 0)
            {
                var data = jointListView.model.get(0).name;
                for (var i=1; i < jointListView.model.count; i++)
                {
                    data = data + ";" + jointListView.model.get(i).name;
                }
                var group ={'name': groupNameTextField.text, 'type' : 'Joint List', 'data' : data};
                groupsTableView.model.append(group);
            }
        }
    }

    jointListView.delegate: ItemDelegate {
        text: name
        width: parent.width
        onClicked: jointListView.currentIndex = index
    }

    addJointButton.onClicked: {
        var joint ={'name': jointNamesComboBox.currentText}
        jointListView.model.append(joint)
    }

    removeJointButton.onClicked: {
        if (jointListView.currentIndex >= 0)
        {
            jointListView.model.remove(jointListView.currentRow);
        }
    }

    chainGroupAddButton.onClicked: {
        if (!!groupNameTextField.text)
        {
            var group ={'name': groupNameTextField.text, 'type' : 'Chain', 'data' : baselinkComboBox.currentText + ';' + tiplinkComboBox.currentText};
            groupsTableView.model.append(group);
        }
    }

    removeGroupButton.onClicked: {
        if (groupsTableView.currentRow >= 0)
        {
            groupsTableView.model.remove(groupsTableView.currentRow);
        }
    }
}
