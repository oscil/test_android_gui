import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0

Rectangle {
    property alias btnText: iDbtnText.text


    signal btnClicked()
    id: iDbtnRect
    color: main.colorDark
//    ColorPicketItem{
//        color: main.colorDark
//    }
    Layout.fillWidth: true
    height: 40*main.ds
    border.width: 2*main.ds
    border.color: main.colorBorder
    radius: 1*main.ds
    state: "normal"
    Text{
        anchors.centerIn: parent
        id: iDbtnText
        font.family: main.fontName
        font.pointSize: main.fSize
        text: "Some Text"
    }
    MouseArea{
        id: iDmouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        onEntered: if (iDbtnRect.state != "disable"){
                       parent.state = "hover"
                   }
        onPressed: {
            if (iDbtnRect.state != "disable"){
                parent.state = "pressed"
                btnClicked()
            }
        }
        onReleased: {if (iDbtnRect.state != "disable"){
                        parent.state = "hover"
                    }
        }
        onExited: {if (iDbtnRect.state != "disable"){
                      parent.state = "normal"
                  }
        }
    }

    states:[
        State{
            name: "disable"
            PropertyChanges {
                target: iDbtnRect
                enabled: false
                opacity: 0.3
            }
        },
        State{
            name: "normal"
            PropertyChanges{
                target: iDbtnRect
                opacity: 0.9
                enabled: true
            }
        },
        State{
            name: "hover"
            PropertyChanges {
                target: iDbtnRect
                opacity: 0.8
                enabled: true
            }
        },
        State{
            name: "pressed"
            PropertyChanges {
                target: iDbtnRect
                opacity: 1.0
                enabled: true
            }
        }
    ]
}

