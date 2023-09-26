import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var group_data
    property string group_name
    property int pixelSize: 20
    property string group_color: "#D3D3D3"
    property string item_color: "#DCDCDC"
    height: temp1.height
    ColumnLayout {
        id: temp1
        implicitWidth: root.width
        Layout.fillWidth: true

        Loader {
            id: loader
            sourceComponent: grp_name
            active: root.group_name ? true : false
            Layout.fillWidth: true
            Component {
                id: grp_name
                Rectangle {
                    id: temp2
                    Layout.fillWidth: true
                    height: grp.height
                    color: root.group_color
                    RowLayout {

                        id: grp
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Text {
                            text: root.group_name
                            font.bold: true
                            font.pixelSize: root.pixelSize
                        }
                 }

            }
            }

        }

        Repeater {
            id: cont
            model: root.group_data.length
            delegate: ObjectListItem {

                id: itm
                Layout.leftMargin: 15
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                object_name: group_data[index].name
                x_coordinate: group_data[index].x
                y_coordinate: group_data[index].y
                object_type: group_data[index].type
                object_creation_time: group_data[index].time_creation
                _index: index
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: itm.back_color = "grey"
                    onExited: itm.back_color = "transparent"

                }
            }
        }
    }
}
