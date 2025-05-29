import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ApplicationWindow {
    id : root
    width: 1100
    height: 600
    minimumWidth: 200
    minimumHeight: 100
    visible: true
    color: Colors.background
    flags: Qt.Window | Qt.FramelessWindowHint

    function getInfoText() : string {
        let out = root.currentFilePath
        if (!out)
            return qsTr("File System Explorer")
    }

    menuBar: MyMenuBar {
        dragWindow: root
        infoText: root.getInfoText()
        MyMenu {
            title: qsTr("File")
            Action {
                text: root.showLineNumbers ? qsTr("Toggle Line Numbers OFF")
                                           : qsTr("Toggle Line Numbers ON")
                shortcut: "Ctrl+L"
                onTriggered: root.showLineNumbers = !root.showLineNumbers
            }
            Action {
                text: root.expandPath ? qsTr("Toggle Short Path")
                                      : qsTr("Toggle Expand Path")
                enabled: root.currentFilePath
                onTriggered: root.expandPath = !root.expandPath
            }
            Action {
                text: qsTr("Reset Filesystem")
                enabled: sidebar.currentTabIndex === 1
                onTriggered: fileSystemView.rootIndex = undefined
            }
            Action {
                text: qsTr("Exit")
                onTriggered: Qt.exit(0)
                shortcut: StandardKey.Quit
            }
        }
        MyMenu{
            title: qsTr("Edit")
        }
        MyMenu{
            title: qsTr("Setting")
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Sidebar {
            id: sidebar
            dragWindow: root
            Layout.preferredWidth: 50
            Layout.fillHeight: true
        }

        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            handle: Rectangle {
                implicitWidth: 10
                color: SplitHandle.pressed ? Colors.color2 : Colors.background
                border.color: SplitHandle.hovered ? Colors.color2 : Colors.background
                opacity: SplitHandle.hovered || navigationView.width < 15 ? 1.0 : 0.0

                Behavior on opacity {
                    OpacityAnimator {
                        duration: 1400
                    }
                }
            }
            FileList {
                id: fileSystemView
                color: Colors.surface1
                SplitView.preferredWidth: 300
            }
            ViewFile{
                id : viewFile
                SplitView.preferredWidth: 800
            }
        }

        Editor {
            id: editor
            showLineNumbers: root.showLineNumbers
            currentFilePath: root.currentFilePath
            SplitView.fillWidth: true
            SplitView.fillHeight: true
        }

        Connections {
            target: fileSystemView
            onFileClicked:  (path) => {
                console.log("Receiver: Signal received with path:", path);
                viewFile.labelText = path
            }
        }
    }
}
