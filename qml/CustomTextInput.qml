import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0

TextField{
    id: iDinputText
    style: TextFieldStyle {
        textColor: "black"
        background: Rectangle {
            anchors.fill: parent
            radius: 0*main.ds
            color: main.colorTextField
            border.color: main.colorBorder
            border.width: 2*main.ds
        }
    }
//    ColorPicketItem{
//        color: main.colorTextField
//    }
    horizontalAlignment: TextInput.AlignLeft
    font.family: main.fontName
    font.pointSize: main.fSize

    onAccepted: {
        GUIMgr.str = text
    }
}


