import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout{
    Label { text: "Pub pos:" }
    SpinBox {
        id: mSpin
        from: 1
        to: 30
        value: simulation.B
        editable: true
        spacing: 100
        onValueModified: {
                if (simulation) {
                    simulation.setB(value)
                }
            }
    }


GroupBox {
    id: params
    title: "Parameters"
    Layout.fillWidth: true
    height: 1
    width: 1


    label: Text {
        text: "Parameters"
        color: "#bb86fc"
        font.bold: true
        font.pixelSize: 16
        leftPadding: 1
    }


    GridLayout {
        columns: 3
        anchors.fill: parent
        anchors.margins: 3

        Text {
            text: "p: "
            color: "#e57373"
            font.italic: true
            font.bold: true
            anchors.fill: params_rectangle
            font.pixelSize: parent
        }

        Slider {
            id: p1Slider
            from: 0
            to: 1
            value: simulation ? simulation.p : 0.3
            stepSize: 0.01
            Layout.fillWidth: true

            onValueChanged: {
                if (simulation && Math.abs(value - simulation.p) > 0.001) {
                    simulation.p = value
                }
            }

            background: Rectangle {
                x: p1Slider.leftPadding
                y: p1Slider.topPadding + p1Slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 6
                width: p1Slider.availableWidth
                height: implicitHeight
                radius: 3
                color: "#3d3d5c"

                Rectangle {
                    width: p1Slider.visualPosition * parent.width
                    height: parent.height
                    color: "#e57373"
                    radius: 3
                }
            }

            handle: Rectangle {
                x: p1Slider.leftPadding + p1Slider.visualPosition * (p1Slider.availableWidth - width)
                y: p1Slider.topPadding + p1Slider.availableHeight / 2 - height / 2
                implicitWidth: 20
                implicitHeight: 20
                radius: 10
                color: "#e57373"
                border.color: "white"
                border.width: 1
                scale: p1Slider.pressed ? 1.2 : 1.0

                Behavior on scale {
                    NumberAnimation { duration: 100 }
                }
            }
        }

        Text {
            text: p1Slider.value.toFixed(2)
            color: "#e57373"
            font.bold: true
            font.family: "Courier New"
        }

        Text {
            text: "q: "
            color: "#6495ED"
            font.italic: true
            font.bold: true
        }

        Slider {
            id: p2Slider
            from: 0
            to: 1
            value: simulation ? simulation.q : 0.1
            stepSize: 0.01
            Layout.fillWidth: true

            onValueChanged: {
                if (simulation && Math.abs(value - simulation.q) > 0.001) {
                    simulation.q = value
                }
            }

            background: Rectangle {
                x: p2Slider.leftPadding
                y: p2Slider.topPadding + p2Slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 6
                width: p2Slider.availableWidth
                height: implicitHeight
                radius: 3
                color: "#3d3d5c"

                Rectangle {
                    width: p2Slider.visualPosition * parent.width
                    height: parent.height
                    color: "#6495ED"
                    radius: 3
                }
            }

            handle: Rectangle {
                x: p2Slider.leftPadding + p2Slider.visualPosition * (p2Slider.availableWidth - width)
                y: p2Slider.topPadding + p2Slider.availableHeight / 2 - height / 2
                implicitWidth: 20
                implicitHeight: 20
                radius: 10
                color: "#6495ED"
                border.color: "white"
                border.width: 1
                scale: p2Slider.pressed ? 1.2 : 1.0

                Behavior on scale {
                    NumberAnimation { duration: 100 }
                }
            }
        }

        Text {
            text: p2Slider.value.toFixed(2)
            color: "#6495ED"
            font.bold: true
            font.family: "Courier New"
        }
    }
}
}
