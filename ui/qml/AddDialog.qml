import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Dialog {

    id: addDialog
    height: 300
    width: 300
    title: ""
    standardButtons: Dialog.Save | Dialog.Cancel
    focus: true
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    ColumnLayout {
        anchors.fill: parent
        spacing: 7
            DialogLabel {
                id: name
                name_text: "Название объекта"
                default_text: ""
                Layout.fillHeight: true
                Layout.fillWidth: true

        }
            DialogLabel {
                id: x_data
                name_text: "Координата X"
                default_text: ""
                Layout.fillHeight: true
                Layout.fillWidth: true
                validator: DoubleValidator{}



        }

            DialogLabel {
                id: y_data
                name_text: "Координата Y"
                default_text: ""
                Layout.fillHeight: true
                Layout.fillWidth: true
                validator: DoubleValidator{}

        }
            DialogLabel {
                id: time_creation
                name_text: "Время создания"
                default_text: ""
                Layout.fillHeight: true
                Layout.fillWidth: true
                validator: DoubleValidator{}
        }
            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    text: "Тип объекта"
                    height: 40
                }

                Item {
                    Layout.fillWidth: true
                }

                ComboBox {
                    id: type
                    model: ["Человек","Машина","Здание","Дерево","..."]
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
    }

    onAccepted: {
        var a = {'name':name.default_text, 'x':x_data.default_text, 'y': y_data.default_text, 'time_creation':time_creation.default_text, 'type':type.currentText}
        handler.addSaved(a)
    }

}
