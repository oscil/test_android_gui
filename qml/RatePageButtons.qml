import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0

Rectangle {
    property string imageSource: ""
    property string description: ""
    property color backGround: "red"
    property int itemId: 0
    property string name: ""
    property string year: ""
    property string intname: ""
    property string imdb: ""


    color: backGround
    id: mainItem
    visible: true

    Flickable{
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        contentHeight: idMainGrid.height
        contentWidth: parent.width
        anchors.bottomMargin: 100*main.ds+10


        GridLayout{
            id: idMainGrid
            width: parent.width -20
            x: 10
            rows: 7
            columns: 2
            Item{
                Layout.rowSpan: 3
                Layout.column: 0
                Layout.row: 0
                Layout.fillWidth: true
                Layout.preferredWidth: main.width*2/3
                Layout.minimumHeight: main.height*0.5
                Layout.alignment: Qt.AlignLeft
                Image{
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    fillMode: Image.PreserveAspectFit
                    source: mainItem.imageSource
                    cache: true
                }
            }
            Item{
                id: idTextImdb
                Layout.column: 1
                Layout.row: 0
                Layout.rowSpan: 3
                Layout.fillHeight: true
            }

            Text{
                id: idTextName
                Layout.row: 3
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.fillWidth:true
                wrapMode: Text.WordWrap
                horizontalAlignment: Qt.AlignLeft
                font.pointSize: 20
                font.bold: true
                text: mainItem.name
            }

            Text{
                id: idTextIntName
                Layout.row: 4
                Layout.column: 0
                Layout.fillWidth:true
                font.pointSize: 15
                wrapMode: Text.WordWrap
                horizontalAlignment: Qt.AlignLeft
                text: "Оригинальное название: " + mainItem.intname + "\nID = " + mainItem.itemId
            }

            Rectangle{
                Layout.row: 5
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.fillWidth:true
                color: Qt.darker(main.colorBackground)
                height: 2*main.ds
            }


            Text{
                id: descrText
                Layout.fillWidth:true
                Layout.fillHeight:true
                Layout.row: 6
                Layout.columnSpan: 2
                Layout.column: 0
                textFormat: Text.StyledText
                text:  mainItem.description
                wrapMode: Text.WordWrap
                horizontalAlignment: Qt.AlignJustify
                font.pointSize: 18
            }

        }
    }
}

