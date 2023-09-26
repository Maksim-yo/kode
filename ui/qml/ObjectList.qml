import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {

    id: root
    ScrollView  {
        anchors.fill: parent

        contentWidth: -1

        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            anchors.fill: parent
            Repeater {
                model: objectList
                delegate: ObjectGroup {
                    group_data: model.group_data
                    group_name: model.group_name
//                    Layout.preferredHeight: 100
                    Layout.fillHeight: true
//                    implicitWidth: root.width
                    Layout.fillWidth: true
                }

            }
        }

    }

}
