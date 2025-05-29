
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Rectangle {
    id: root

    property alias currentTabIndex: topBar.currentIndex
    required property ApplicationWindow dragWindow
    readonly property int tabBarSpacing: 10

    color: "white"

    component SidebarEntry: Button {
        id: sidebarButton

        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true

        icon.color: down || checked ? Colors.iconIndicator : Colors.icon
        icon.width: 25
        icon.height: 25

        topPadding: 0
        rightPadding: 0
        bottomPadding: 0
        leftPadding: 0
        background: null

        Rectangle {
            id: indicator

            anchors.verticalCenter: parent.verticalCenter
            x: 2
            width: 4
            height: sidebarButton.icon.height * 1.2

            visible: sidebarButton.checked
            color: Colors.color1
        }
    }
    component TabBar: Container {
        id: tabBarComponent

        Layout.fillWidth: true
        Layout.fillHeight: false

        ButtonGroup {
            buttons: tabBarComponent.contentChildren
            onCheckedButtonChanged: tabBarComponent.setCurrentIndex(
                Math.max(0, buttons.indexOf(checkedButton)))
        }

        contentItem: ColumnLayout {
            spacing: tabBarComponent.spacing
            Repeater {
                model: tabBarComponent.contentModel
            }
        }
    }

    ColumnLayout {
        anchors.fill: root
        anchors.topMargin: root.tabBarSpacing
        anchors.bottomMargin: root.tabBarSpacing

        spacing: root.tabBarSpacing
        TabBar {
            id: topBar

            spacing: root.tabBarSpacing
            // Shows help text when clicked.
            // SidebarEntry {
            //     id: infoTab
            //     icon.source: "icons/light_bulb.svg"
            //     checkable: true
            //     checked: true
            // }

            // // Shows the file system when clicked.
            // SidebarEntry {
            //     id: filesystemTab

            //     icon.source: "icons/read.svg"
            //     checkable: true
            // }
            SidebarEntry{
                id : copy
                checkable: false
                icon.source: "icons/copy.png"
            }
            SidebarEntry{
                id : paste
                checkable: false
                icon.source: "icons/paste.png"
            }
            SidebarEntry{
                id : cut
                checkable: false
                icon.source: "icons/cutting.png"
            }
            SidebarEntry{
                id : trash
                checkable: false
                icon.source: "icons/trash.png"
            }
        }

        // This item acts as a spacer to expand between the checkable and non-checkable buttons.
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            // Make the empty space drag our main window.
            WindowDragHandler {
                dragWindow: root.dragWindow
            }
        }

        TabBar {
            id: bottomBar

            spacing: root.tabBarSpacing
            // Opens the Qt website in the system's web browser.
            SidebarEntry {
                id: qtWebsiteButton
                icon.source: "icons/github-logo.png"
                checkable: false
                onClicked: Qt.openUrlExternally("https://github.com/khoinguyenAdonis")
            }

            SidebarEntry {
                id: aboutQtButton

                icon.source: "icons/information.png"
                checkable: false
                onClicked: aboutQtWindow.visible = !aboutQtWindow.visible
            }
        }
    }

    About {
        id: aboutQtWindow
        visible: false
    }
    Rectangle {
        id: bottomBorder
        height: parent.height // Chiều rộng bằng với Rectangle cha
        width: 1           // Độ dày của đường viền
        color: "darkblue"       // Màu của đường viền
        anchors.right: parent.right // Căn chỉnh xuống đáy của Rectangle cha
        //anchors.horizontalCenter: parent.horizontalCenter // Căn giữa theo chiều ngang
    }
}
