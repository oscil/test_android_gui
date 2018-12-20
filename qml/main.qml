import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Window {
    property color colorBackground: Qt.rgba(252/256,246/256,196/256,1) //brigthyellow 1
    property color colorDark: Qt.rgba(255/256,241/256,125/256,1) //darker brightyellow 1
    property color colorDlgBackground: Qt.rgba(255/256,243/256,135/256,1) //darker brightyellow 1
    property color colorBorder: Qt.rgba(32/256,22/256,0/256,1) //darkbrown 1
    property color colorTextField: Qt.rgba(252/256,246/256,196/256,1) //darkyellow

    property int fSize: 24
    property string fontName: "Calibri"

    property int dpi: Screen.pixelDensity * 25.4
    property real ds: dpi < 180 ? 1 :
                                  dpi < 270 ? 1.5 :
                                              dpi < 360 ? 2 : 3;

    ListModel{
        id: idRateModel
    }

    ListModel{
        id: idRecommendModel
    }


    function dp(x){
        if(dpi < 120) {
            return x; // Для обычного монитора компьютера
        } else {
            return x*(dpi/160);
        }
    }

    id: main
    title: qsTr("Sokos registration")
    width: 640
    height: 480
    visible: true

    signal signalRegistrationMain(string loginMain, string passwordMain)
    signal signalLogin(string login, string password);
    signal signalListIndexChanged(int currentIndex, int count);
    signal signalRecommendListIndexChanged(int currentIndex, int count);
    signal signalRateItem(int index, double rateValue)
    signal signalRateRecommendItem(int index, double rateValue)
    signal signalSkipItem(int index)

    Connections{
        target: GUIMgr
        onStatusStringChanged: iDloginDialog.showText()
        onSignalRegistrationStatusChanged:{
            if(isOK){
                iDloginDialog.state = "login"
            }
            else
                iDloginDialog.state = "registration"
        }
        onSignalLoginStatusChanged:{
            print("on login status changed")
            if (isOK){
                //тут мы прошли авторизацию!
//                iDMainwaiter.startAnimation()
                iDMainwaiter.visible = true
                iDMainwaiter.running = true
                iDloginDialog.visible = false
            } else {
                iDloginDialog.state = "login"
            }

        }
        onSignalReceiveNewRateDescription:{
            print("receive RATE description")
//            iDMainwaiter.stopAnimation()
            iDMainwaiter.visible = false
            iDMainwaiter.running = false
            idListsTabView.focus = true
            iDloginScreen.visible = false
//            main.recvNewDescription(description)
            idListsTabView.visible = true
        }
        onSignalReceiveNewRecommendDescription:{
            print("receive RECOMEND description")
//            iDMainwaiter.stopAnimation()
            idListsTabView.focus = true
            iDloginScreen.visible = false
            iDMainwaiter.visible = false
            iDMainwaiter.running = false
//            main.recvNewRecommendDescription(description)
            idListsTabView.visible = true
        }
    }

    Rectangle { //BackGround
        color: main.colorBackground
        anchors.fill: parent
    }


    Rectangle {
        id: iDloginScreen
        color: main.colorBackground

        anchors.fill: parent
        LoginDialog{
            id: iDloginDialog
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top : parent.top
            anchors.topMargin: 50*main.ds
            loginStr: GUIMgr.loginString
            passwordStr: GUIMgr.passwordString
            onSignalRegistration: {
                signalRegistrationMain(loginDialog, passwordDialog)
            }
            statusText: GUIMgr.statusString
        }
    }

    BusyIndicator{
        id: iDMainwaiter
        anchors.centerIn: parent
        height: Math.min(main.width/4, 100*main.ds)
        visible: false
        style: BusyIndicatorStyle {
                indicator: Image {
                    visible: control.running
                    source: "qrc:/images/images/recom_ico.png"
                    fillMode: Image.PreserveAspectFit
                    RotationAnimator on rotation {
                        running: control.running
                        loops: Animation.Infinite
                        duration: 2000
                        from: 0 ; to: 360

                    }
                }
            }
    }


    TabView{
        id: idListsTabView
        anchors.fill: parent
        visible: false

        style: TabViewStyle {
            frameOverlap: 0
            tab: Rectangle {
                color: Qt.darker(main.colorDark,styleData.selected ? 0.8 : 0.89)
                border.color:  colorBorder
                implicitWidth: main.width/control.count
                implicitHeight: 40*main.ds
                radius: 0
                Text {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title.split("|")[0]
                    color: styleData.selected ? "black" : "gray"
                    font.pointSize: 14
                }
                Image{
                    id: image
                    anchors.fill: parent
                    anchors.topMargin: (styleData.selected?5:13)*main.ds
                    anchors.bottomMargin: (styleData.selected?5:13)*main.ds
                    fillMode: Image.PreserveAspectFit
                    source: styleData.title.split("|")[1]
                }
            }
            frame: Rectangle { color: main.colorBackground }
        }


        Tab{
            id: idRateListTab
//            title: "|qrc:/images/images/like.png"
            title: "Оценки"
            Item{
                anchors.fill: parent
                RateListView{
                    id: idRateList
                    anchors.fill: parent
                    objectName: "ratelistview"
                    delegatemodel: CPPItemsRateModel
                    onCurrentIndexChanged: {
                        signalListIndexChanged(currentIndex, count)
                        idRateButtons.isskipped = idRateList.model.items.get(currentIndex).model.israte &&
                                                  idRateList.model.items.get(currentIndex).model.userskip
                        idRateButtons.isratebad = idRateList.model.items.get(currentIndex).model.israte &&
                                                  !idRateList.model.items.get(currentIndex).model.userskip&&
                                                  (idRateList.model.items.get(currentIndex).model.userrate<0.1)
                        idRateButtons.isrategood = idRateList.model.items.get(currentIndex).model.israte &&
                                                   !idRateList.model.items.get(currentIndex).model.userskip&&
                                                  (idRateList.model.items.get(currentIndex).model.userrate<=0.9) &&
                                                  (idRateList.model.items.get(currentIndex).model.userrate>=0.1)
                        idRateButtons.israteexcellent = idRateList.model.items.get(currentIndex).model.israte &&
                                                  (idRateList.model.items.get(currentIndex).model.userrate>0.9) &&
                                                   !idRateList.model.items.get(currentIndex).model.userskip

                    }
                    onSignalStartMoving: {
                        idRateButtons.isenable = false
                    }

                    onSignalStopMoving: {
                        idRateButtons.isenable = true
                    }
                }
                RateButtons{
                    id: idRateButtons
                    anchors.bottom: parent.bottom
                    height: Math.min(100*main.ds, main.width/5)
                    width: main.width
                    onSignalSetRate: {
                        idRateList.update()
                        print("List view cur index = " + idRateList.currentIndex)
                        if (value < -1.0)
                            main.signalSkipItem(idRateList.currentIndex)
                        else
                            main.signalRateItem(idRateList.currentIndex, value)
                        idRateList.incrementCurrentIndex()
                    }
                }
            }
        }

        Tab{
//            title: "|qrc:/images/images/heart.png"
            title: "Рекомендации"
            Item{
                anchors.fill: parent
                RateListView{
                    id: idRecomList
                    anchors.fill: parent
                    objectName: "recomlistview"
                    delegatemodel: CPPItemsRecommendModel
                    onCurrentIndexChanged: {
                        signalRecommendListIndexChanged(currentIndex, count)
                        idRecomButtons.isratebad = idRecomList.model.items.get(currentIndex).model.israte &&
                                                  !idRecomList.model.items.get(currentIndex).model.userskip&&
                                                  (idRecomList.model.items.get(currentIndex).model.userrate<0.1)
                        idRecomButtons.isrategood = idRecomList.model.items.get(currentIndex).model.israte &&
                                                   !idRecomList.model.items.get(currentIndex).model.userskip&&
                                                  (idRecomList.model.items.get(currentIndex).model.userrate<=0.9) &&
                                                  (idRecomList.model.items.get(currentIndex).model.userrate>=0.1)
                        idRecomButtons.israteexcellent = idRecomList.model.items.get(currentIndex).model.israte &&
                                                  (idRecomList.model.items.get(currentIndex).model.userrate>0.9) &&
                                                   !idRecomList.model.items.get(currentIndex).model.userskip
                    }

                }

                RateButtons{
                    id: idRecomButtons
                    anchors.bottom: parent.bottom
                    height: Math.min(100*main.ds, main.width/5)
                    width: main.width
                    skipVisible: false
                    onSignalSetRate: {
                        idRecomList.update()
                        print("List view cur index = " + idRecomList.currentIndex)
                        main.signalRateRecommendItem(idRecomList.currentIndex, value)
                        idRecomList.incrementCurrentIndex()
                    }
                }
            }
        }
    }


}
