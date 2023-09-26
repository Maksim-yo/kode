import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {

    id: root

    property alias name_text: txt.text
    property alias default_text: input.text
    property var validator: RegularExpressionValidator {}
    RowLayout {
        id: container
        anchors.fill: parent
        spacing:10
    Text {
        id: txt
        text: "Имя объекта"
        height: 40

    }
    TextField {
        id: input
        validator: root.validator
        focus: true
        Layout.alignment: Qt.AlignRight
        }
    }
}
