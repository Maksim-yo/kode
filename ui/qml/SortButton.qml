import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


Menu {
    id: root
    property int _selected_index: 0
    property int num
    signal click(string data)
    property string clickedName
    ListModel {
        id: nameModel

        ListElement {
            name: "Отсутствует"
            test: true
        }
        ListElement {
            name: "По расстоянию"
            test: false
        }
        ListElement {
            name: "По имени"
            test: false
        }
        ListElement {
            name: "По времени создания"
            test: false
        }
        ListElement {
            name: "По типу"
            test:false
        }
    }


    Connections {

        target: dlg
        function onAccepted() {
            root.num = parseInt(dlg.text)
            handler.sortMethodChanged(root.clickedName, root.num)

        }

    }
    Connections {
        target: root
        function onClick(data){
            clickedName = data
        }
    }
    title: qsTr("&Группировка")

    contentItem: ColumnLayout {
        spacing: 0
        Repeater {
            id: rep
              model: nameModel
              delegate: CustomPopupElement {
                    id: item
                    text: model.name
                    isSelected: model.test
                    Layout.preferredWidth: 140
                    Layout.fillWidth: true
                    Layout.preferredHeight: 15
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            item.back_color = "grey"
                        }
                        onExited: {
                             item.back_color = "transparent"
                        }
                        onClicked: {
                            nameModel.setProperty(root._selected_index,"test", false)
                            nameModel.setProperty(index,"test", true)
                            root._selected_index = index
                            root.click(model.name)
                            root.close()

                            if (model.name != "По типу")
                                handler.sortMethodChanged(model.name)
                            else
                                dlg.open()
                    }
                }
            }
        }
    }


}
