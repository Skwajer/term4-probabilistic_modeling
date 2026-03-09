import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt5Compat.GraphicalEffects

RowLayout {
    spacing: 10
    Layout.fillWidth: true

    signal loadFileRequested

    Button {
        id: run
        text: "RUN"
        Layout.fillWidth: true
        onClicked: {
            if (simulation) {
                simulation.runSimulation()
            }
        }

        background: Rectangle {
            implicitHeight: 40
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#e57373" }
                GradientStop { position: 1.0; color: "#c62828" }
            }
            radius: 8

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                radius: 10
                samples: 10
                color: "#c62828"
                horizontalOffset: 2
                verticalOffset: 1
            }

            scale: parent.pressed ? 0.95 : 1.0

            Behavior on scale {
                NumberAnimation { duration: 100 }
            }
        }

        contentItem: Text {
            text: parent.text
            color: "white"
            font.bold: true
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Button {
        text: simulation ? (simulation.running ? "PAUSE" : "PLAY") : "PLAY"
        Layout.fillWidth: true
        onClicked: {
            if (simulation) {
                simulation.pauseSimulation()
            }
        }

        background: Rectangle {
            implicitHeight: 40
            color: parent.hovered ? "#bb86fc" : "#9f7aea"
            radius: 8

            Behavior on color {
                ColorAnimation { duration: 200 }
            }

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                radius: 8
                samples: 16
                color: "#409f7aea"
                horizontalOffset: 0
                verticalOffset: 3
            }
        }

        contentItem: Text {
            text: parent.text
            color: "white"
            font.bold: true
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    // Step button
    Button {
        text: "STEP"
        Layout.fillWidth: true
        onClicked: {
            if (simulation) {
                simulation.stepSimulation()
            }
        }

        background: Rectangle {
            implicitHeight: 40
            color: parent.hovered ? "#8bc34a" : "#6a994e"
            radius: 8

            Behavior on color {
                ColorAnimation { duration: 200 }
            }

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                radius: 8
                samples: 16
                color: "#406a994e"
                horizontalOffset: 0
                verticalOffset: 3
            }
        }

        contentItem: Text {
            text: parent.text
            color: "white"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    // Reset button
    Button {
        text: "RESET"
        Layout.fillWidth: true
        onClicked: {
            if (simulation) {
                simulation.resetSimulation()
            }
        }

        background: Rectangle {
            implicitHeight: 40
            color: parent.hovered ? "#ffb74d" : "#ff9800"
            radius: 8

            Behavior on color {
                ColorAnimation { duration: 200 }
            }

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                radius: 8
                samples: 16
                color: "#40ff9800"
                horizontalOffset: 0
                verticalOffset: 4
            }
        }

        contentItem: Text {
            text: parent.text
            color: "white"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
