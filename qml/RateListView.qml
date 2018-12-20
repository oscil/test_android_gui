import QtQuick 2.3
import QtQuick.Window 2.0
import QtQml.Models 2.1

ListView {
    id: idRateView
    snapMode: ListView.SnapOneItem
    orientation: Qt.Horizontal
    highlightRangeMode: ListView.StrictlyEnforceRange
    highlightMoveVelocity: 200


    property bool ismoving: false
    property alias currentIndex: idRateView.currentIndex
    property bool skipbtnvisible: true
    property alias count: idRateView.count
    property var delegatemodel: NULL
    property bool isbtnsenable: true
//    property alias ismoving: idRateView.vi

    signal signalSetItemRate(int index, int itemId, double rateValue)
    signal signalSetSkipItem(int index, int itemId)
    signal signalStartMoving()
    signal signalStopMoving()

    function itemRateSetted(rateValue, itemId){
        print("Function Rate Setted")
        signalSetItemRate(currentIndex, itemId, rateValue);
        idRateView.incrementCurrentIndex()
    }

    function itemSkipped(itemId){
        print("Function Skip Setted")
        signalSetSkipItem(currentIndex, itemId, count);
        idRateView.incrementCurrentIndex()
    }


    MouseArea{
        id: idLeftSide
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100*main.ds
        width: parent.width/3
        onClicked: {
            if (idRateView.currentIndex >=1 )
                idRateView.currentIndex = idRateView.currentIndex - 1
        }
    }
    MouseArea{
        id: idRightSide
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 150*main.ds
        anchors.bottomMargin: 100*main.ds
        width: parent.width/3
        onClicked: {
            if (idRateView.currentIndex < idRateView.count-1 )
                idRateView.currentIndex = idRateView.currentIndex + 1
        }
    }

    model: DelegateModel{
        model: idRateView.delegatemodel
        delegate:
            RatePageButtons{
                width: main.width
                height: parent.height
                itemId: model.itemid.toString()
                description: model.description
                name: model.name
                imageSource: model.poster
                backGround: main.colorBackground
                Component.onCompleted: {
                    signalSetItemRate.connect(idRateView.itemRateSetted)
                    signalSetSkipItem.connect(idRateView.itemSkipped)
                }
            }
    }


//    onMovementStarted: {
//        print("Movement started")
//        ismoving = true
//    }

//    onMovementEnded: {
//        print("Movement ended")
//        ismoving = false
//    }


    onMovementStarted: {
        print("START MOVE moving = " + moving)
        signalStartMoving()
    }

    onMovementEnded: {
        print("STOP MOVE moving = " + moving)
        signalStopMoving()
    }

    onCurrentIndexChanged: {
        print("Cur index changed")
    }

    onFlickStarted: {
        print("FliCK STARTED")
    }

    onFlickingChanged: {
        print("Flicking changed! = " + flicking)
    }

    onMovingChanged: {
        print("MOVING changed! = " + moving)
    }

    onDraggingHorizontallyChanged: {
        print("Drag hor chan")
    }

}

