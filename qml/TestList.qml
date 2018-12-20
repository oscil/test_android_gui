import QtQuick 2.0

Rectangle {
    width: 360
    height: 360

    ListModel {
        id: dataModel

        ListElement {
            color: "orange"
            text: "first"
        }
        ListElement {
            color: "lightgreen"
            text: "second"
        }
        ListElement {
            color: "orchid"
            text: "third"
        }
        ListElement {
            color: "tomato"
            text: "fourth"
        }
    }

    ListView {
        id: view

        anchors.margins: 10
        anchors.fill: parent
        spacing: 10
        model: dataModel
        clip: true

        highlight: Rectangle {
            color: "skyblue"
        }
        highlightFollowsCurrentItem: true

        delegate: Item {
            property var view: ListView.view
            property var isCurrent: ListView.isCurrentItem

            width: view.width
            height: 40

            Rectangle {
                anchors.margins: 5
                anchors.fill: parent
                radius: height / 2
                color: model.color
                border {
                    color: "black"
                    width: 1
                }

                Text {
                    anchors.centerIn: parent
                    renderType: Text.NativeRendering
                    text: "%1%2".arg(model.text).arg(isCurrent ? " *" : "")
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: view.currentIndex = model.index
                }
            }
        }
    }
}

