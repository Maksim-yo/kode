import QtQuick 2.0
import QtQuick.Controls 2.15


Item {

    id: root

    property string text: ""
    property string back_color_default: "#47d147"
    property string border_color_default:"#47d147"
    property string back_on_hover: "#2eb82e"
    property string text_color: "white"
    property int border_width: 0
    property int textSize: 16
    signal clicked()
//    signal clicked()

    Button {

        id: btn
        anchors.fill: parent

        background: Rectangle {
            id: back

            border.color: border_color_default
            color: back_color_default
            border.width: border_width
            radius: 20
        }

        contentItem: Text {
            text: root.text
            color: text_color
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Times"
            font.bold: true
            font.pixelSize: root.textSize
            elide: Text.ElideRight


        }

        MouseArea{

            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                back.color = back_on_hover
            }

            onClicked:root.clicked()
            onExited: {
                back.color = back_color_default
            }
        }
    }
}
