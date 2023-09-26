import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.15


Item {

    id: root
    property alias src: img.source
    property bool isHover: false
    property int hoverNum
    property double default_bright: 0
    signal clicked()
    signal hoverEnter()
    signal hoverExit()
    Connections {

        target: root

        function onHoverEnter(){
            if (default_bright == 0)
                myBright.brightness=0.5

        }

        function onHoverExit(){
            myBright.brightness=default_bright

        }
    }


    Image {
        id: img
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                if(default_bright == 0) {
                    myBright.brightness= 0.5
                }
            }
            onExited:{
                myBright.brightness= root.default_bright
            }
            onClicked:{
                root.clicked()
            }
        }


        BrightnessContrast {
            id: myBright
            anchors.fill: img
            source: img
            brightness: root.default_bright

        }
    }
}
