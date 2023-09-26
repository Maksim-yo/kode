import QtQuick
import QtQuick.Controls
import QtQuick.Layouts



Dialog {

    standardButtons: Dialog.Ok | Dialog.Cancel
    title: ""
    focus: true
    height: 150
    width: 300
    property alias text: num_count.text

        RowLayout {
            anchors.fill: parent
            Text {
                text: "Количество для отбора"
                height: 40
            }
            TextField {
                id: num_count
                validator: IntValidator {}
                focus: true
            }
        }
}
