import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Rectangle {
    id: root
    signal fileClicked(string filePath)
    property alias rootIndex: fileSystemTreeView.rootIndex

    TreeView {
        id: fileSystemTreeView
        anchors.fill: parent

        model: fileTreeModel
        rootIndex: fileTreeModel.rootIndex
        clip: true

        delegate: Item {
            implicitWidth: padding + label.x + label.implicitWidth + padding
            implicitHeight: label.implicitHeight * 1.5

            readonly property real indentation: 20
            readonly property real padding: 2

            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property bool hasChildren
            required property int depth
            required property int row
            required property int column
            required property bool current

            Image {
                id: image
                source: "icons/folder.png"
                x: padding + (depth * indentation)
                anchors.verticalCenter: parent.verticalCenter
                visible: isTreeNode && hasChildren
                sourceSize.width: 17
                sourceSize.height: 17

                TapHandler {
                    onSingleTapped: {
                        let index = treeView.index(row, column)
                        treeView.selectionModel.setCurrentIndex(index, ItemSelectionModel.NoUpdate)
                        treeView.toggleExpanded(row)
                    }
                }
            }
            Label {
                id: label
                x: padding + (isTreeNode ? (depth + 1) * indentation : 0)
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - padding - x
                clip: true
                text: model.name
                TapHandler {
                    enabled: model.type === FileEntry
                    onSingleTapped: (tap) => {
                        fileEditor.readFileToRam(model.fullPath)
                        //console.log("Signal fileClicked emitted for:", model.fullPath);
                        fileClicked(model.name)
                    }
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                active: true
                implicitWidth: 15

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 6

                    color: Colors.color1
                    opacity: fileSystemTreeView.movingVertically ? 0.5 : 0.0

                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 500
                        }
                    }
                }
            }
        }
    }
}
