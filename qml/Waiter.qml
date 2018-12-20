import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3

Item {
    property color color1: "red"
    property color color2: "green"
    property color color3: "blue"
    property color color4: "yellow"
    property color borderColor: "lavender"
    property int borderWidth: 4
    property int itemWidth: 30

    function startAnimation(){
        iDanimation.start()
        visible = true
    }
    function stopAnimation(){
        iDanimation.stop()
        visible = false
    }

    id: iDmainWaiter
    width: 100*main.ds
    height: 100*main.ds

    GridLayout{
        rows: 3
        columns: 3
        anchors.fill: parent
        Rectangle{
            Layout.row: 1
            Layout.column: 1
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            width: itemWidth*main.ds
            height: width
            radius:width/2
            color: iDmainWaiter.color1
            border.width: borderWidth
            border.color: borderColor
        }
        Rectangle{
            Layout.row: 1
            Layout.column: 3
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            width: itemWidth*main.ds
            height: width
            radius:width/2
            color: iDmainWaiter.color2
            border.width: borderWidth
            border.color: borderColor
        }
        Rectangle{
            Layout.row: 3
            Layout.column: 1
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            width: itemWidth*main.ds
            height: width
            radius:width/2
            color: iDmainWaiter.color3
            border.width: borderWidth
            border.color: borderColor
        }
        Rectangle{
            Layout.row: 3
            Layout.column: 3
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            width: itemWidth*main.ds
            height: width
            radius:width/2
            color: iDmainWaiter.color4
            border.width: borderWidth
            border.color: borderColor
        }
    }

    ParallelAnimation{
        id: iDanimation
        PropertyAnimation {
            target: iDmainWaiter
            property: "rotation"
//            easing.type: Easing.OutInQuad
            from: 0
            to: 360
            duration: 1600
        }
        SequentialAnimation{
            PropertyAnimation {
                target: iDmainWaiter
                property: "opacity"
                from: 1
                to: 0.6
                duration: 1000
                easing.type: Easing.InOutBack
            }
            PropertyAnimation {
                target: iDmainWaiter
                property: "opacity"
                from: 0.6
                to: 1
                duration: 600
                easing.type: Easing.InOutBack
            }
        }
        loops: Animation.Infinite
    }
}

