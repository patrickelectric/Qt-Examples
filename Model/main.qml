import QtQuick 2.0
import Model 1.0

Item {
    width: 320
    height: 480

    ListView {
        anchors.fill: parent

        model: Model {
        }

        delegate: Text {
            text: modelData.value
        }
    }
}
