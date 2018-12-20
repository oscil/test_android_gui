import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0

Item {
    property alias skipVisible: idBtnSkip.visible
    property bool isskipped: false
    property bool isratebad: false
    property bool isrategood: false
    property bool israteexcellent: false
    property bool isenable: true

    signal signalSetRate(double value)
    id: rootBtnsItem

    RowLayout{
        anchors.fill: parent
//        spacing: 15*main.ds
        id: idrowbtnslayout

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle{
            id: idBtnSkip
            Layout.fillHeight: true
            Layout.fillWidth: false
            Layout.preferredWidth: height
            color: isskipped ?  Qt.darker(main.colorDark,0.7) : main.colorDark
            border.color: main.colorBorder
            border.width: 1*main.ds
            opacity: 0.9
            radius: height/2
            scale: isskipped ? 1.2 : 1.0
            Text{
                anchors.centerIn: parent
                font.pointSize: 14
                text: "Не\nсмотрел"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    print("PRESS")
                    idBtnSkip.scale = 1.2
                }
                onReleased: {
                    print("RELEASE")
                    if(containsMouse){
                        idBtnSkip.height = rootBtnsItem.height
                        signalSetRate(-2.0)
                    }
                }
                onExited: {
                    print("EXITED")
                    idBtnSkip.scale = 1.0
                }
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
            visible: idBtnSkip.visible
        }

        Rectangle{
            id: idBtnBad
            Layout.fillHeight: true
            Layout.fillWidth: false
            Layout.preferredWidth: height
            width: height
            color: isratebad ?  Qt.darker(main.colorDark,0.7) : main.colorDark
            border.color: main.colorBorder
            border.width: 1*main.ds
            opacity: 0.9
            radius: height/2
            scale: isratebad ? 1.2 : 1.0
            Text{
                anchors.centerIn: parent
                font.pointSize: 18
                text: "Гэ"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    idBtnBad.scale = 1.2
                }
                onReleased: {
                    if(containsMouse){
                        idBtnBad.height = rootBtnsItem.height
                        signalSetRate(0.0)
                    }
                }
                onExited: {
                    print("EXITED")
                    idBtnBad.scale = 1.0
                }
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle{
            id: idBtnGood
            Layout.fillHeight: true
            Layout.fillWidth: false
            Layout.preferredWidth: height
            width: height
            color: isrategood ?  Qt.darker(main.colorDark,0.7):main.colorDark
            border.color: main.colorBorder
            border.width: 1*main.ds
            opacity: 0.9
            radius: height/2
            scale: isrategood ? 1.2 : 1.0
            Text{
                anchors.centerIn: parent
                font.pointSize: 20
                text: "Ничо"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    idBtnGood.scale = 1.2
                }
                onReleased: {
                    if(containsMouse){
                        idBtnGood.height = rootBtnsItem.height
                        signalSetRate(0.6)
                    }
                }
                onExited: {
                    print("EXITED")
                    idBtnGood.scale = 1.0
                }
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle{
            id: idBtnExcellent
            Layout.fillHeight: true
            Layout.fillWidth: false
            Layout.preferredWidth: height
            width: height
            color: israteexcellent ?  Qt.darker(main.colorDark,0.7):main.colorDark
            border.color: main.colorBorder
            border.width: 1*main.ds
            opacity: 0.9
            radius: height/2
            scale: israteexcellent ? 1.2 : 1.0
            Text{
                anchors.centerIn: parent
                font.pointSize: 20
                text: "5+"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    idBtnExcellent.scale = 1.2
                }
                onReleased: {
                    if(containsMouse){
                        idBtnExcellent.height = rootBtnsItem.height
                        signalSetRate(1.0)
                    }
                }
                onExited: {
                    print("EXITED")
                    idBtnExcellent.scale = 1.0
                }
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    onIsenableChanged: {
        if (idhideanimation.running)
            idhideanimation.stop()
        if (idshowanimation.running)
            idshowanimation.stop()
        if (isenable)
            idshowanimation.start()
        else
            idhideanimation.start()
    }

    PropertyAnimation{
        id: idhideanimation
        target: idrowbtnslayout
        property: "scale"
        to: 0
        duration: 100
    }
    PropertyAnimation{
        id: idshowanimation
        target: idrowbtnslayout
        property: "scale"
        to: 1
        duration: 100

    }

}

