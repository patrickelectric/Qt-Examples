import QtQuick 2.12
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.12

import VideoElement 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    color: "black"
    title: "Main Application"

    Item {
        anchors.fill: parent

        GridLayout {
            id: lay
            rows: 2
            columns: rows
            anchors.fill: parent

            Repeater {
                model: lay.rows*lay.columns
                VideoElement {
                    id: videoElement
                    description: "videotestsrc pattern=" + (18 + index)
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }

        Rectangle {
            id: circle
            color: "red"
            anchors.bottom: window.bottom
            anchors.right: window.right
            anchors.margins: 15
            width : 50
            height: width
            radius: width

            Timer {
                interval: 300; running: true; repeat: true
                onTriggered: circle.visible = !circle.visible
            }
        }
    }
}
