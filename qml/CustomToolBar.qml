import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

Rectangle {
    color: main.colorDlgBackground

    GridLayout{
        anchors.fill: parent
        anchors.leftMargin: 20*main.ds
        rows: 1
        columns:3
        columnSpacing: 20*main.ds


        Item{
            Layout.fillHeight: true
            Layout.row: 0
            Layout.column: 0
            Layout.fillWidth: false
            Layout.minimumWidth: parent.height
            Image{
                anchors.fill: parent
                anchors.topMargin: 5*main.ds
                anchors.bottomMargin: 5*main.ds
                source: "qrc:/images/images/like.png"
                fillMode: Image.PreserveAspectFit
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: false
            Layout.minimumWidth: parent.height
            Image{
                anchors.fill: parent
                anchors.topMargin: 5*main.ds
                anchors.bottomMargin: 5*main.ds
                source: "qrc:/images/images/heart.png"
                fillMode: Image.PreserveAspectFit
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.row: 0
            Layout.column: 2
            Layout.fillWidth: true
            Layout.minimumWidth: parent.height
        }


    }
}

