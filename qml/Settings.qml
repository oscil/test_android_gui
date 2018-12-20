import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0

Rectangle {
    property alias serverAddress: iDinputServerAddress.text

    GridLayout{
        id: idSettingsLO
        anchors.fill: parent
        anchors.leftMargin: 10*main.ds
        anchors.rightMargin: 10*main.ds
        anchors.topMargin: 10*main.ds
        anchors.bottomMargin: 10*main.ds
        rows: 6
        columns: 1

        CustomTextInput{
            id: iDinputServerAddress
            focus: true
            Layout.fillWidth: true
            Layout.row: 1
            Layout.column: 1
            placeholderText: "Server address..."
        }
    }
}

