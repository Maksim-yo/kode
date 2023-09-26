import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as FD



ApplicationWindow {
    id: main
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&New...")
                onTriggered: fileDialog.open()

            }
        }

        SortButton{
            id: btn
        }

    }
    SortDialogHelper {
        id: dlg
        text: ""
    }
    FD.FileDialog {
        id: fileDialog
        currentFile: FD.StandardPaths.standardLocations(FD.StandardPaths.DocumentLocation)[0]
        onAccepted: handler.readFile(currentFile)
    }
    ColumnLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 30
            Rectangle {

                Layout.preferredWidth: 5 * main.width / 6
                Layout.preferredHeight: 3
                color:"grey"
                Layout.alignment: Qt.AlignHCenter
            }

        }

        ObjectList {

            Layout.topMargin: 20
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.leftMargin: 30
        }

        AddDialog{
            id:dialog

        }

        CustomButton {

            Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            Layout.preferredHeight: 35
            Layout.preferredWidth: 80
            Layout.rightMargin: 20
            Layout.bottomMargin: 15
            onClicked: dialog.open()
            text: "Add"
        }

    }

 }

