import QtQuick
import QtQuick.Controls.Basic

ApplicationWindow {
    id: root
    width: 650
    height: 550
    flags: Qt.Window | Qt.FramelessWindowHint
    color: Colors.surface1

    menuBar: MyMenuBar {
        id: menuBar
        dragWindow: root
        implicitHeight: 30
        infoText: "About File System Explore"
    }

    Image {
        id: logo

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20

        source: "../icons/qt_logo.svg"
        sourceSize.width: 80
        sourceSize.height: 80
        fillMode: Image.PreserveAspectFit

        smooth: true
        antialiasing: true
        asynchronous: true
    }

    ScrollView {
      anchors.top: logo.bottom
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      anchors.margins: 20

      TextArea {
          selectedTextColor: Colors.textFile
          selectionColor: Colors.selection
          horizontalAlignment: Text.AlignHCenter
          textFormat: Text.RichText

          text: qsTr("<h3>--*--</h3>"
                   + "<p>This program uses Qt version %1.</p>"
                   + "<p>File Explorer is an intuitive file manager"
                   + "designed to simplify Browse and managing files on the Linux operating system"
                   + "This is version [1.1].</p>"
                   + "<p>The application is developed using Qt (QML and C++), offering a modern,"
                   + "user-friendly interface alongside reliable performance. Essential functionalities"
                   + "such as text file (.txt) editing, deleting, copying, and cutting are all integrated"
                   + "to meet your daily needs.</p>")
                   .arg(Application.version).arg("qt.io/licensing").arg("2024").arg("github.com/khoinguyenAdonis")
          color: Colors.textFile
          wrapMode: Text.WordWrap
          readOnly: true
          antialiasing: true
          background: null

          onLinkActivated: function(link) {
              Qt.openUrlExternally(link)
          }
      }
    }


}
