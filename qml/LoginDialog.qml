import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0

Rectangle{
    property alias statusText: iDstatusText.text
    property int minLoginLen: 4
    property alias loginStr: iDinputLogin.text
    property alias passwordStr: iDinputPassword.text

    signal signalRegistration(string loginDialog, string passwordDialog)

    id: iDrectDialog
    height: 280*main.ds
    width: 300*main.ds
    color: main.colorDlgBackground
    border.width: 1*main.ds
    border.color: main.colorBorder
    radius: 0*main.ds
    state: "login"

    function checkRegistrationInput(){
        if (iDrectDialog.state === "registration"){
            if( iDinputLogin.acceptableInput &&
                    iDinputPassword.text.length >= minLoginLen &&
                    iDinputPassword.text === iDinputConfirm.text){
                iDbtnRegistration.state = "normal"
            }
            else
                iDbtnRegistration.state = "disable"
        }
    }

    function checkLoginInput(){
        if (iDrectDialog.state === "login"){
            if( iDinputLogin.acceptableInput &&
                    iDinputPassword.text.length >= minLoginLen){
                iDbtnEnter.state = "normal"
            }
            else{
                iDbtnEnter.state = "disable"
            }
        }
    }

    function showText(){
        if(iDstatusAnimation.running)
            iDstatusAnimation.stop()
        iDstatusAnimation.start()
    }

//    ColorPicketItem{
//        color: main.colorDlgBackground
//    }

    GridLayout{
        id: idGridLO
        anchors.fill: parent
        anchors.leftMargin: 10*main.ds
        anchors.rightMargin: 10*main.ds
        anchors.topMargin: 10*main.ds
        anchors.bottomMargin: 10*main.ds
        rows: 6
        columns: 2

        Rectangle{
            property string fillLoginText: "Login"
            id: iDrectLogin
            Layout.preferredWidth: idlabelLogin.contentWidth
            Layout.preferredHeight: 30*main.ds
//            color: iDrectDialog.color
            Layout.row: 1
            Layout.column: 1
            Layout.alignment: Qt.AlignRight

            ColorPicketItem{
                color: iDrectDialog.color
            }
            Text{
                id: idlabelLogin
                font.pointSize: main.fSize
                font.family: main.fontName
                text: iDrectLogin.fillLoginText
            }
        }
        CustomTextInput{
            id: iDinputLogin
            focus: true
            Layout.fillWidth: true
            Layout.row: 1
            Layout.column: 2
            placeholderText: iDrectLogin.fillLoginText+"..."
            maximumLength: 16
            validator: RegExpValidator{
                regExp: /[a-z, A-Z, 0-9]{4,16}/
            }
            onTextChanged: {if (iDrectDialog.state=== "login")
                    checkLoginInput()
                else
                    checkRegistrationInput()
            }

        }
        Rectangle{
            property string fillPasswordText: "Password"
            id: iDrectPassword
            Layout.preferredWidth: idlabelLogin2.contentWidth
            Layout.preferredHeight: 30*main.ds
//            ColorPicketItem{
//                color: iDrectDialog.color
//            }
            color: iDrectDialog.color
            Layout.row: 2
            Layout.column: 1
            Text{
                id: idlabelLogin2
                font.pointSize: main.fSize
                font.family: main.fontName
                horizontalAlignment: Text.AlignRight
                text: iDrectPassword.fillPasswordText
            }
        }
        CustomTextInput{
            id: iDinputPassword
            focus: true
            Layout.fillWidth: true
            Layout.row: 2
            Layout.column: 2
            placeholderText: iDrectPassword.fillPasswordText+"..."
            echoMode: TextInput.Password
            onTextChanged: {if (iDrectDialog.state=== "login")
                    checkLoginInput()
                else
                    checkRegistrationInput()
            }
        }
        Rectangle{
            property string fillConfirmText: "Confirm"
            id: iDrectConfirm
            Layout.preferredWidth: idlabelConfirm.contentWidth
            Layout.preferredHeight: 30*main.ds
            color: iDrectDialog.color
//            ColorPicketItem{
//                color: iDrectDialog.color
//            }
            Layout.row: 3
            Layout.column: 1
            Layout.alignment: Qt.AlignRight
            visible: false
            Text{
                id: idlabelConfirm
                font.pointSize: main.fSize
                font.family: main.fontName
                text: iDrectConfirm.fillConfirmText
            }
        }
        CustomTextInput{
            id: iDinputConfirm
            focus: true
            Layout.fillWidth: true
            Layout.row: 3
            Layout.column: 2
            placeholderText: iDrectConfirm.fillConfirmText+"..."
            echoMode: TextInput.Password
            visible: false
            onTextChanged: checkRegistrationInput()
        }
        CustomButton{
            id: iDbtnEnter
            Layout.row: 4
            Layout.column: 1
            Layout.columnSpan: 2
            btnText: "Enter"
            state: "disable"
            onBtnClicked: {
                iDrectDialog.state = "waitlogin"
                iDwaiter.visible = true
                iDwaiter.running = true
//                iDwaiter.startAnimation()
                main.signalLogin(iDinputLogin.text, iDinputPassword.text)
            }
        }
        CustomButton{
            id: iDbtnRegistration
            Layout.row: 5
            Layout.column: 1
            Layout.columnSpan: 2
            btnText: "Registration"
            onBtnClicked: {
                if (iDrectDialog.state != "registration"){
                    iDrectDialog.state = "registration"
                }
                else{
                    iDrectDialog.state = "waitregistration"
//                    iDwaiter.startAnimation()
                    iDwaiter.visible = true
                    iDwaiter.running = true
                    iDrectDialog.signalRegistration(iDinputLogin.text, iDinputPassword.text)
                }
            }
        }
        Text{
            id: iDstatusText
            Layout.row: 6
            Layout.column: 1
            Layout.columnSpan: 2
            font.pointSize: main.fSize*0.9
            font.family: main.fontName
            text:""
            PropertyAnimation{
                id: iDstatusAnimation
                target:iDstatusText
                duration: 5000
                property: "opacity"
                from: 1.0
                to: 0.0
                easing.type: Easing.InExpo
            }
        }

    }

    Image{
        id: idSettingsButton

    }

    BusyIndicator{
        id: iDwaiter
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

//    Waiter{
//        id: iDwaiter
//        color1: main.colorDark
//        color2: main.colorDlgBackground
//        color3: main.colorTextField
//        color4: main.colorBorder
//        borderColor: main.colorBackground
//        anchors.centerIn: parent
//        visible: false
//    }
    states:[
        State{
            name: "login"
            PropertyChanges{
                target: iDbtnEnter
            }
            PropertyChanges{
                target: idlabelConfirm
                visible: false
            }
            PropertyChanges{
                target: iDrectConfirm
                visible: false
            }
            PropertyChanges {
                target: iDinputConfirm
                visible: false
            }
            PropertyChanges {
                target: iDwaiter
                visible: false
                running: false
            }
            //            PropertyChanges {
            //                target: GUIMgr
            //                isWaiting: false
            //            }
        },
        State{
            name: "registration"
            PropertyChanges{
                target: iDbtnEnter
                visible: false
            }
            PropertyChanges{
                target: idlabelConfirm
                visible: true
            }
            PropertyChanges{
                target: iDrectConfirm
                visible: true
            }
            PropertyChanges {
                target: iDinputConfirm
                visible: true
            }
            PropertyChanges {
                target: iDbtnRegistration
                visible: true
            }
            PropertyChanges {
                target: iDwaiter
                visible: false
                running: false
            }
            //            PropertyChanges {
            //                target: GUIMgr
            //                isWaiting: false
            //            }
        },
        State{
            name: "waitregistration"
            PropertyChanges {
                target: iDrectDialog
                enabled: false
            }
            PropertyChanges {
                target: iDwaiter
                visible: true
                running: true
            }
            //            PropertyChanges {
            //                target: GUIMgr
            //                isWaiting: true
            //            }
            PropertyChanges{
                target: iDbtnEnter
                visible: false
            }
            PropertyChanges{
                target: idlabelConfirm
                visible: true
            }
            PropertyChanges{
                target: iDrectConfirm
                visible: true
            }
            PropertyChanges {
                target: iDinputConfirm
                visible: true
            }
            PropertyChanges {
                target: iDbtnRegistration
                visible: true
            }
        },
        State{
            name: "waitlogin"
            PropertyChanges {
                target: iDrectDialog
                enabled: false
            }
            PropertyChanges {
                target: iDrectDialog
                enabled: false
            }
            PropertyChanges {
                target: iDwaiter
                visible: true
                running: true
            }
            //            PropertyChanges {
            //                target: GUIMgr
            //                isWaiting: true
            //            }
            PropertyChanges{
                target: iDbtnEnter
                visible: true
            }
            PropertyChanges{
                target: idlabelConfirm
                visible: false
            }
            PropertyChanges{
                target: iDrectConfirm
                visible: false
            }
            PropertyChanges {
                target: iDinputConfirm
                visible: false
            }
            PropertyChanges {
                target: iDbtnRegistration
                visible: true
            }
        }

    ]

    onStateChanged: {
        if (state === "login"){
//            iDwaiter.stopAnimation()
            iDwaiter.visible = false
            iDwaiter.running = false
            checkLoginInput()
        }
        if (state === "registration"){
//            iDwaiter.stopAnimation()
            iDwaiter.visible = false
            iDwaiter.running = false
            checkRegistrationInput()
        }
    }
    Keys.onBackPressed: {
        if (state === "registration"){
            state = "login"
        }
        else if (state === "login"){
            Qt.quit()
        }
    }
}



