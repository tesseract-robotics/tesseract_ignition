import QtQuick 2.4
import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 2.2

KinematicGroupsForm {
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
