import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import test2

Window {
    id: window
    width: 360
    height:  640
    visible: true
    title: qsTr("Wisp")

    Rectangle {
        anchors.fill: parent
        color: "black"

        ShaderEffect {
            id: effect
            anchors.fill: parent

            property real uu_time: 0.1
            property real uuu_time: 0.0009
            property point u_mouse: Qt.point(0, 0)
            property size  u_resolution: Qt.size(width, height)

            Timer
            {
               id: timer
               running: true
               repeat: true
               interval: 10
               onTriggered:
               {
                   effect.uu_time += effect.uuu_time;
               }
            }

            MouseArea{
                anchors.fill: parent
                onPressed: (mouse) => {
                        effect.u_mouse = Qt.point(mouse.x/parent.width, mouse.y/parent.height)
                }

                onPositionChanged: (mouse) => {
                    if (pressed) {
                        effect.u_mouse = Qt.point(mouse.x/parent.width, mouse.y/parent.height)
                    }
                }
            }

            fragmentShader: "qrc:/shaders/patterrneffect.frag.qsb"
            vertexShader: "qrc:/shaders/patterrneffect.vert.qsb"
        }
    }

    ShaderEffectSource {
        id: effectSource
        sourceItem: effect
        live: true
        hideSource: false
        sourceRect: Qt.rect(0, settingsPopup.y, settingsPopup.width, settingsPopup.height)
    }

    MouseArea{
        id: an
        anchors.fill:  parent

        hoverEnabled: true
        property real  startY: window.height

        drag.target: settingsPopup
        drag.axis: Drag.YAxis
        drag.minimumY: parent.height*0.01
        drag.maximumY: parent.height

        onReleased: (mouse) => {
            if(mouse.y < startY)
            {
                console.log(mouse.y + " " + startY)
                open.start()()
                startY = window.height
            }
        }

    }

    NumberAnimation {
        id: open
        property: "y";
        target: settingsPopup
        to:  window .height*0.01
        duration: 200
    }

    NumberAnimation {
        id: close
        property: "y";
        target: settingsPopup
        to: window.height
        duration: 200
    }

    Control{
        id: settingsPopup
        focus: true
        y: parent.height
        height: parent.height*0.99
        width: parent.width

        background:  ShaderEffect{
            id: effectSh

            property variant baseTex: effectSource
            property color topColor: Qt.rgba(1, 1, 1, 0.2)
            property color bottomColor: Qt.rgba(0.8, 0.8, 0.8, 0.2)
            property size  u_resolution: Qt.size(window.width, window.height)
            anchors.fill:  parent

            fragmentShader: "qrc:/shaders/blurandglass.frag.qsb"
            vertexShader: "qrc:/shaders/blurandglass.vert.qsb"
        }


        RowLayout{
            id: settingsLabel
            anchors.top: parent.top
            width: parent.width
            height: parent.height*0.025

            Control{
                id: closeButton
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.topMargin: 3
                height:  7
                width: 35

                background: Rectangle{
                    color:  "black"
                    radius:  15
                }

                MouseArea{
                    anchors.fill:  parent
                    onPressed:{
                        close.start()
                    }
                }
            }
        }

        Item{

            width: parent.width
            anchors{
                top: settingsLabel.bottom
                bottom: parent.bottom
            }

            Item{
                id: configButton
                anchors.top: parent.top
                width: parent.width
                height: 40

                Text{
                    color: "white"
                    anchors.centerIn: parent
                    text: "+ Add config"
                }

                MouseArea{
                    anchors.fill: parent
                     onPressed: {
                        chooseconfig.height = 120
                        configButton.visible = false
                    }
                }
            }

            Column{
                id: chooseconfig
                anchors.top: configButton.top
                width: parent.width
                height: 0

                Repeater{
                    model: [
                        "+ Add from link",
                        "+ Add from Qr",
                        "+ Add from file"
                    ]

                    delegate: Rectangle{
                        id: add
                        width: parent.width
                        height: parent.height/3
                        color: "transparent"

                        Text{
                            text: modelData
                            anchors.centerIn: parent
                            color: "white"
                        }

                        MouseArea{
                            id: area
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked:{
                                console.log(modelData)
                                chooseconfig.height = 0
                                configButton.visible = true
                                if (modelData == "+ Add from link"){
                                    inputDialog.open()
                                }
                            }
                            onEntered: add.color = "hotpink"
                            onExited: add.color = "transparent"
                        }
                    }
                }
            }

        }
    }

    Dialog {
        id: inputDialog
        title: "Введите текст"
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        anchors.centerIn:Overlay.overlay

        contentItem: Column {
            spacing: 10
            TextField {
                id: inputField
                placeholderText: "Ваш текст..."
            }
        }

        onAccepted: {
            console.log("Введено:", inputField.text)
            vpnservice.vlessurl = inputField.text
        }
    }

    Vpnservice {
        id: vpnservice
    }


}

