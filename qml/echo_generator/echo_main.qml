import QtQuick 2.1
import QtQuick.Controls 1.0
import Capture 1.0

Rectangle {
    property int echo_size: 512
    property int controls_size: 200
    width: echo_size+controls_size
    height: echo_size

    Rectangle {
        id: controls_rect
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        //anchors.left: echo_rect.right
        //x: echo_rect.width
        x: parent.echo_size
        width: controls_size
        color: "steelblue"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                Qt.quit();
            }
        }
        Slider {
            id: wobbleSlider
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 40
        }
    }

    ScreenCapture {
        id: echo_rect
        width : parent.echo_size
        height: parent.echo_size
        anchors.right: controls_rect.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}
