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
            id: darkness_slider
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 40
            value: 0.2
            stepSize: 0.01
            minimumValue: 0.0
            maximumValue: 1.0
        }
        /*Slider {
            id: animation_mod_slider
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 40
        }
        Slider {
            id: animation_from_center_mod_slider
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 40
        }*/
    }

    ScreenCapture {        
        id: echo_rect
        width : parent.echo_size
        height: parent.echo_size
        anchors.right: controls_rect.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        property variant resolution: Qt.size(width,height)
        property real time: parent.elapsed_time

        property variant center_offset: Qt.size(0.0,0.0)
        property real darkness: darkness_slider.value //0.2
        property real animation_mod: 1.0
        property real animation_from_center_mod: 1.0/8.0
        property real contrast_str: 5.0
        property real object_density: 24.0
    }

    property real elapsed_time: 0
    Timer {
        interval: 10
        running: true
        repeat: true
        onTriggered: parent.elapsed_time += interval * 0.001
    }
}
