import QtQuick
import QtQuick.Layouts

Item {

    id: root

    property string object_name
    property alias x_coordinate: x.text
    property alias y_coordinate: y.text
    property string object_type
    property alias object_creation_time: obj_creation_time.text
    property int pixelSize: 16
    property string back_color: "transparent"
    property int _index
    Rectangle {
        id:container
        color: back_color
        anchors.fill: parent

        ColumnLayout {

            anchors.fill: parent
            spacing: 0
            RowLayout {

                Text {
                    id: num
                    font.pixelSize: root.pixelSize
                    text: root._index + 1 +  ". "
                    font.bold: true
                }
                Text {
                    id: obj_name
                    font.pixelSize: root.pixelSize
                    text: root.object_name
                }

                Text {
                    id :x
                    font.pixelSize: root.pixelSize
                }


                Text {
                    id: y
                    font.pixelSize: root.pixelSize
                }

                Text {
                    id: obj_type
                    font.pixelSize: root.pixelSize
                    text: root.object_type
                }

                Text {
                    id: obj_creation_time
                    font.pixelSize: root.pixelSize
                }
            }
        }
    }
}
