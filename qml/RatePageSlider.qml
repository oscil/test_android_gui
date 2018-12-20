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
    property bool rateSliderVisible: true
    property alias value: idRateSlider.value
    property bool userChangeValue: false
    property bool skipbtnvisible: true

    property alias sliderHeight: idRateSlider.height

    signal signalSetItemRate(double rateValue, int itemId)
    signal signalSetSkipItem(int itemId)

    function setSliderVisible(){
        idRateSlider.visible = !idRateSlider.visible
    }

    function incrementSlider() {
        if (idRateSlider.value < 1.0){
            idRateSlider.value += 0.1;
            if (idRateSlider.value > 1.0)
                idRateSlider.value = 1.0
        }
    }

    function decrementSlider() {
        if (idRateSlider.value > 0.0){
            idRateSlider.value -= 0.1;
            if (idRateSlider.value < 0.0)
                idRateSlider.value = 0.0
        }
    }

    color: backGround
    id: mainItem
    visible: true

    Flickable{
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        contentHeight: idMainGrid.height
        contentWidth: parent.width
        anchors.bottomMargin: idRateSlider.size*main.ds+10


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
//                    anchors.fill: parent
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
//                Rectangle{
//                    id: idRateSkip
//                    anchors.fill: parent
//                    color: main.colorDark
//                    border.color: main.colorBorder
//                    border.width: 3*main.ds
//                    opacity: 0.6
//                    height: 150*main.ds
//                    width: height
//                    radius: height/10
//                    visible: (skipbtnvisible&&mainItem.rateSliderVisible)
//                    Text{
//                        anchors.centerIn: parent
//                        text: "Skip"
//                        font.pixelSize: main.ds*50
//                    }

//                    MouseArea{
//                        anchors.fill: parent
//                        onClicked: {
//                            print("SKIP PRESSED")
//                            mainItem.signalSetSkipItem(mainItem.itemId)
//                        }
//                    }

//                    onVisibleChanged: {
//                        if (visible)
//                            idRateSkipAnimation.start()
//                    }

//                    NumberAnimation {
//                        id: idRateSkipAnimation
//                        targets: [idRateSkip]
//                        property: "opacity"
//                        duration: 200
//                        from: 0
//                        to: 0.6
//                        onStarted: {
//                            idRateSkip.visible = skipbtnvisible;
//                            idRateSkip.opacity = 0
//                        }
//                    }
//                }

            }
//            Item{
//                id: idTextKP
//                Layout.column: 1
//                Layout.row: 1
//                Layout.fillWidth: true
////                text: "Kinopoisk: " + 12
////                font.pointSize: 14
////                font.bold: true
////                Layout.maximumWidth: contentWidth
////                horizontalAlignment: Qt.AlignLeft
//            }
//            Item{
//                id: idEmptyItem
//                Layout.column: 1
//                Layout.row: 2
//                Layout.rowSpan: 2
//                Layout.fillHeight: true
////                Layout.fillWidth: true
//            }

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



    Rectangle{
        property alias animation: idRateTextAnimation
        property alias text: idRateText.text
        property real value
        id: idRateItem
        visible: false
        anchors.centerIn: parent
        width: (main.width < main.height ?  main.width : main.height)/2
        height: (main.width < main.height ?  main.width : main.height)/2
        radius: (main.width < main.height ?  main.width : main.height)/4
        border.width: 5*main.ds
        border.color: "black"
        color: value < 0.25? "red" :
                             value < 0.5 ? "orange" :
                                           value < 0.75 ? "yellow" : "green"

        opacity: 0.7
        Text{
            id: idRateText
            font.pixelSize: (main.width < main.height ?  main.width : main.height)/4
            anchors.centerIn: parent
            text: Math.floor(idRateItem.value*10)
        }

        NumberAnimation {
            id: idRateTextAnimation
            target: idRateItem
            property: "opacity"
            duration: 1000
            from: idRateItem.opacity
            to: 0
            onStopped: {
                idRateItem.visible = false;
            }
            onStarted: {
                idRateItem.visible = true;
            }
        }
    }
    Rectangle{
        id: idSliderBackground
        anchors.bottom: parent.bottom
        anchors.bottomMargin: idRateSlider.height*0.1
        width: parent.width
        visible: idRateSlider.visible
        implicitHeight: idRateSlider.height - idRateSlider.height*0.2
        color: main.colorDlgBackground
//        ColorPicketItem{
//            color: "#cccccc"
//        }
        opacity: idRateSlider.opacity
    }

    Slider{
        property int size: 100

        signal signalSetRate(double value)

        id: idRateSlider
        visible: mainItem.rateSliderVisible
        anchors.bottom: parent.bottom
        width: parent.width
        height: size*main.ds
        minimumValue: 0.0
        maximumValue: 1.0
        value: 0.5
        style: SliderStyle {
            groove: Rectangle {
                implicitWidth: 200*main.ds
                implicitHeight: 8*main.ds
                color: main.colorBorder
                radius: 0
//                ColorPicketItem{
//                    color: "gray"
//                }
            }
            handle: Rectangle {
                anchors.centerIn: parent
                color: Qt.lighter(main.colorDark, control.pressed ? 1.0 : 0.8)
                border.color: "gray"
                border.width: 2*main.ds
                implicitWidth: idRateSlider.size*main.ds
                implicitHeight: idRateSlider.size*main.ds
                radius: idRateSlider.size*main.ds/2
//                ColorPicketItem{
//                    color: control.pressed ? "white" : "lightgray"
//                }
            }
        }

        onValueChanged: {
            idRateItem.value = value
        }

        onPressedChanged: {
            idRateItem.value = value
            if (!pressed){
                if (idRateItem.animation.running)
                    idRateItem.animation.stop()
                idRateItem.animation.start()
                print("Rate Page send signal val = " + value)
                mainItem.signalSetItemRate(value, mainItem.itemId)
            } else {
                idRateItem.animation.stop()
                idRateItem.opacity = 0.9
                idRateItem.visible = true
            }
        }


        onVisibleChanged: {
            if (visible)
                idSliderAnimation.start()
        }

        NumberAnimation {
            id: idSliderAnimation
            targets: [idRateSlider,idSliderBackground]
            property: "opacity"
            duration: 200
            from: 0
            to: 1
            onStarted: {
                idRateSlider.visible = true;
                idRateSlider.opacity = 0
            }
        }

//        Component.onCompleted: {
//            signalSetRate.connect(mainItem.signalSetItemRate)
//        }
    }

    Rectangle{
        id: idRateSkip
        anchors.top: parent.top
        anchors.right: parent.right
        height: 150*main.ds
        width: height
        color: main.colorDark
        border.color: main.colorBorder
        border.width: 3*main.ds
//        ColorPicketItem{
//            color: "red"
//        }
        opacity: 0.6
        radius: height/2
        visible: (skipbtnvisible&&mainItem.rateSliderVisible)
        Text{
            anchors.centerIn: parent
            text: "Skip"
            font.pixelSize: main.ds*50
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                mainItem.signalSetSkipItem(mainItem.itemId)
            }
        }

        onVisibleChanged: {
            if (visible)
                idRateSkipAnimation.start()
        }

        NumberAnimation {
            id: idRateSkipAnimation
            targets: [idRateSkip]
            property: "opacity"
            duration: 200
            from: 0
            to: 0.6
            onStarted: {
                idRateSkip.visible = skipbtnvisible;
                idRateSkip.opacity = 0
            }
        }
    }

//    onFocusChanged: {
//        idRateSlider.focus = true
//    }

//    ListView.onRemove: SequentialAnimation {
//        PropertyAction { target: mainItem; property: "ListView.delayRemove"; value: true }
//        NumberAnimation { target: mainItem; property: "scale"; to: 0; duration: 250; easing.type: Easing.InOutQuad }
//        PropertyAction { target: mainItem; property: "ListView.delayRemove"; value: false }
//    }

}

