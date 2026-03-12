import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

GroupBox {
    title: "Search"
    Layout.fillWidth: true

    property var simulation
    signal showRecoveredDialog(var ids)
    signal showResultsDialog(string title, var ids)

    label: Text {
        text: "Search"
        color: "#bb86fc"
        font.bold: true
        font.pixelSize: 16
        leftPadding: 10
    }

    background: Rectangle {
        color: "#2a1b3d"
        radius: 10
        border.color: "#bb86fc"
        border.width: 1
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        anchors.margins: 10
        rowSpacing: 10

        Repeater {
            model: [
                "Never infected",
                "Recovered",
                "Recovered with unhealthy neighbors",
                "Never infected (all neighbors infected)"
            ]

            Button {
                text: modelData
                Layout.fillWidth: true
                Layout.columnSpan: index > 1 ? 2 : 1
                enabled: simulation && simulation.totalPeople > 0

                background: Rectangle {
                    implicitHeight: 36
                    color: parent.hovered ? "#3d2b5c" : "#2a1b3d"
                    border.color: "#bb86fc"
                    border.width: parent.enabled ? 1 : 0.5
                    radius: 6

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }

                contentItem: Text {
                    text: parent.text
                    color: parent.enabled ? "#bb86fc" : "#666666"
                    font.bold: true
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                }

                onClicked: {
                    if (!simulation) return

                    var result
                    switch(index) {
                        case 0:
                            result = simulation.findNeverInfected()
                            showResultsDialog(modelData, result)
                            break
                        case 1:
                            result = simulation.findRecovered()
                            showRecoveredDialog(result)
                            break
                        case 2:
                            result = simulation.findRecoveredWithUnrecoveredNeighbors()
                            showResultsDialog(modelData, result)
                            break
                        case 3:
                            result = simulation.findNeverInfectedWithAllNeighborsInfected()
                            showResultsDialog(modelData, result)
                            break
                    }
                }
            }
        }
    }
}
