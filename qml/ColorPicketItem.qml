import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

Item{
    property alias color: colordialog.color
    id: idItem
    anchors.fill: parent
    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
                colordialog.visible = true
        }
    }
    ColorDialog{
        id: colordialog
        visible: false
        color: parent.color
        onAccepted: {
            idItem.color = colordialog.color
            print("COLOR = " + colordialog.color)
            idItem.parent.color = color
        }
        onColorChanged: {
            idItem.color = colordialog.color
            print("COLOR CHANGED = " + colordialog.color)
        }
    }
    Component.objectName: {
        parent.color = color
    }

}

