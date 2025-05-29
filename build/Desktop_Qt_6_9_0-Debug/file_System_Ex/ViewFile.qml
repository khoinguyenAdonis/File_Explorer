import QtQuick

import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "white"
    border.color: "darkblue"
    border.width: 1

    property alias labelText: myLabel.text

    Label {
        id : myLabel
        font.bold: true
    }
    Rectangle{
        id : path
        anchors.top:  parent.top
        Layout.fillWidth: true
        height: 20
        border.color: "lightgray"
        border.width: 2
    }

    ScrollView {
        anchors{
            bottom: parent.bottom
            right: parent.right
            top: path.bottom
            left: parent.left
        }
        spacing: 5
        TextArea {
            id: pathDisplayArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            placeholderText: "Selected file path will appear here..."
            wrapMode: TextEdit.WordWrap
            ScrollIndicator.vertical: ScrollIndicator {
                implicitWidth: 12
                contentItem: Rectangle {
                    implicitWidth: 8
                    implicitHeight: 8
                    radius: 4
                    color: "lightgray"
                    opacity: fileContentDisplayArea.flickingVertically || fileContentDisplayArea.movingVertically ? 1.0 : 0.0
                    Behavior on opacity { OpacityAnimator { duration: 300 } }
                }
            }
            ScrollIndicator.horizontal: ScrollIndicator {
                implicitHeight: 12
                contentItem: Rectangle {
                    implicitWidth: 8
                    implicitHeight: 8
                    radius: 4
                    color: "lightgray"
                    opacity: fileContentDisplayArea.flickingHorizontally || fileContentDisplayArea.movingHorizontally ? 1.0 : 0.0
                    Behavior on opacity { OpacityAnimator { duration: 300 } }
                }
            }
        }

        Connections {
            target: fileEditor
            onFileContentReady : (content) => {
                pathDisplayArea.text = content
            }
        }

    }
}
