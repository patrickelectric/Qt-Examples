import QtQuick 2.0
import QQuickOpenGL 1.0

Item {
    width: 320
    height: 480

    QQuickOpenGL {
        anchors.fill: parent
        Timer {
            interval: 50; running: true; repeat: true
            onTriggered: parent.shift += 2
        }
    }
}
