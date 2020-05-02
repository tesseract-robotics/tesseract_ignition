import QtQuick 2.4
import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 2.2

KinematicGroupsForm {
    jointGroupAddButton.onClicked: {
        if (!!groupNameTextField.text)
        {
            if (jointListViewModel.count > 0)
            {
                var data = jointListViewModel.get(0).name;
                for (var i=1; i < jointListViewModel.count; i++)
                {
                    data = data + ";" + jointListViewModel.get(i).name;
                }
                var group ={'name': groupNameTextField.text, 'type' : 'joint', 'data' : data};
                kinematicGroupsModel.append(group);
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
        jointListViewModel.append(joint)
    }

    removeJointButton.onClicked: {
        if (jointListView.currentIndex >= 0)
        {
            jointListViewModel.remove(jointListView.currentRow);
        }
    }

    chainGroupAddButton.onClicked: {
        if (!!groupNameTextField.text)
        {
            var group ={'name': groupNameTextField.text, 'type' : 'chain', 'data' : baselinkComboBox.currentText + ';' + tiplinkComboBox.currentText};
            kinematicGroupsModel.append(group);
        }
    }

    removeGroupButton.onClicked: {
        if (groupsTableView.currentRow >= 0)
        {
            kinematicGroupsModel.remove(groupsTableView.currentRow);
        }
    }
}
