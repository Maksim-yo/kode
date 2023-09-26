import QtQuick
import QtQuick.Layouts


Item {
    id: root
    property bool isSelected: false
    property string text: ""
    property string back_color: "transparent"
    Rectangle {

        id: wrapper
        color: root.back_color
        anchors.fill: parent
    RowLayout {
        id: cont
        Layout.fillWidth: true
        anchors.fill: parent
        Text {
            text: root.text
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
        }
        Loader {

            id: img1
            sourceComponent: mark
            active: root.isSelected
            visible: root.isSelected
            Component {

                id: mark

                Image {

                    source: "qrc:/assets/check_mark.png"
                    width: 10
                    height: 10
                }
            }
        }
        }
    }

}
