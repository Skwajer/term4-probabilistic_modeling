import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs // Changed from Qt.labs.platform 1.1

ApplicationWindow {
    visible: true
    width: 650
    height: 500
    title: "Disease Simulation"

    Dialog {
            id: resultDialog
            title: "Recovered People"
            standardButtons: Dialog.Ok
            width: 400
            height: 300
            modal: true

            property var resultIds: []

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Text {
                    text: "Found " + resultsDialog.resultIds + " recovered people:"
                    font.bold: true
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: resultsDialog.resultIds
                    delegate: Text {
                        text: "ID: " + modelData
                    }
                }
            }
        }

    FileDialog
    {
        id: fileDialog
        title: "Select data file"
        nameFilters: ["Text files (*.txt)", "All files (*)"]
        currentFolder: shortcuts.home  // Start in home directory

        onAccepted: {
            // For QtQuick.Dialogs 1.3 we use fileUrl
            var path = fileDialog.selectedFile

            // Remove file:/// prefix
            path = path.toString()
            path = path.replace("file://", "")
            path.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/, "")

            // For Windows - remove leading slash if it's a drive path (e.g. /C:/)
            if (Qt.platform.os === "windows") {
                path = path.replace(/^\/([a-zA-Z]:)/, "$1")
            }

            console.log("Loading file:", path)
            simulation.loadFromFile(path)
        }

        onRejected: {
            console.log("File selection canceled")
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // Statistics
        GroupBox {
            title: "Statistics"
            Layout.fillWidth: true

            GridLayout {
                columns: 2
                anchors.fill: parent

                Text { text: "Healthy:"
                       color: "green"}
                Text {
                    text: simulation ? simulation.healthyCount : "0"
                    color: "green"
                    font.bold: true
                }

                Text { text: "Infected:"
                       color: "red"}
                Text {
                    text: simulation ? simulation.infectiousCount : "0"
                    color: "red"
                    font.bold: true
                }

                Text { text: "Recovered:"
                       color: "#6495ED"}
                Text {
                    text: simulation ? simulation.recoveredCount : "0"
                    color: "#6495ED"
                    font.bold: true
                }

                Text { text: "Total people:"
                       color: "#D3D3D3"}
                Text {
                    text: simulation ? simulation.totalPeople : "0"
                    color: "#D3D3D3"
                    font.bold: true
                }

                Text { text: "Time step:"
                    font.family: "Courier New"
                        font.pixelSize: 20
                        font.bold: true
                        color: "green"
                        style: Text.Raised
                        styleColor: "#004400"}
                Text {
                    text: simulation ? simulation.TimeStep : "0"
                    font.family: "Courier New"
                        font.pixelSize: 20
                        font.bold: true
                        color: "green"
                        style: Text.Raised
                        styleColor: "#004400"
                }
            }
        }

        // Parameters
        GroupBox {
            title: "Parameters"
            Layout.fillWidth: true

            GridLayout {
                columns: 3
                anchors.fill: parent

                Text { text: "p1 (infection):"
                       color: "#8B0000"
                       font.italic: true}
                Slider {
                    id: p1Slider
                    from: 0
                    to: 1
                    value: simulation ? simulation.p1 : 0.3
                    stepSize: 0.01
                    onValueChanged: {
                        if (simulation && Math.abs(value - simulation.p1) > 0.001) {
                            simulation.p1 = value
                        }
                    }
                    background: Rectangle {
                            x: p1Slider.leftPadding
                            y: p1Slider.topPadding + p1Slider.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: p1Slider.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#8B0000"

                            Rectangle {
                                width: p1Slider.visualPosition * parent.width
                                height: parent.height
                                color: "#ffd1dc"
                                radius: 2
                            }
                        }
                }
                Text { text: p1Slider.value.toFixed(2)
                       color: "#8B0000"}

                Text { text: "p2 (recovery):"
                       color: "#6495ED"
                       font.italic: true}
                Slider {
                    id: p2Slider
                    from: 0
                    to: 1
                    value: simulation ? simulation.p2 : 0.1
                    stepSize: 0.01
                    onValueChanged: {
                        if (simulation && Math.abs(value - simulation.p2) > 0.001) {
                            simulation.p2 = value
                        }
                    }
                    background: Rectangle {
                            x: p2Slider.leftPadding
                            y: p2Slider.topPadding + p2Slider.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: p2Slider.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#6495ED"

                            Rectangle {
                                width: p2Slider.visualPosition * parent.width
                                height: parent.height
                                color: "#ffd1dc"
                                radius: 2
                            }
                        }
                }

                Text { text: p2Slider.value.toFixed(2)
                       color: "#6495ED"}
            }
        }

        // Control buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Load file"
                onClicked: fileDialog.open()
            }

            Button {
                id: infect
                text: "infect"
                background: Rectangle
                {
                    implicitWidth: 150
                            implicitHeight: 40
                            color: "dark red"
                            border.width: 2
                            radius: 10
                }

                contentItem:  Text {
                        text: infect.text
                        font: infect.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                onClicked: {
                    if (simulation) {
                        simulation.runSimulation()
                    }
                }
            }

            Button {
                text: simulation.running ? "pause" : "play"
                onClicked: {
                    if (simulation) {
                    simulation.pauseSimulation()
                    }
                }
            }

            Button {
                text: "Step"
                onClicked: {
                    if (simulation) {
                        simulation.stepSimulation()
                    }
                }
            }

            Button {
                text: "Reset"
                onClicked: {
                    if (simulation) {
                        simulation.resetSimulation()
                    }
                }
            }
        }

        // Search section
        GroupBox {
            title: "Search"
            Layout.fillWidth: true

            GridLayout {
                columns: 2
                anchors.fill: parent
                rowSpacing: 10

                Button {
                    text: "Never infected"
                    Layout.fillWidth: true
                    onClicked: {
                        if (simulation) {
                            var result = simulation.findNeverInfected()
                            console.log("Never infected:", result)
                            // You can display results in a popup or list
                        }
                    }
                }

                Button {
                        text: "Recovered"
                        Layout.fillWidth: true
                        onClicked: {
                            if (simulation) {
                                var res = simulation.findRecovered()
                                resultDialog.resultIds = res
                                resultDialog.open()
                            }
                        }
                    }

                Button {
                    text: "Recovered with unhealthy neighbors"
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    enabled: simulation && simulation.totalPeople > 0
                    onClicked: {
                        if (simulation) {
                            var result = simulation.findRecoveredWithUnrecoveredNeighbors()
                            console.log("Recovered with unhealthy neighbors:", result)
                        }
                    }
                }

                Button {
                    text: "Never infected (all neighbors infected)"
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    enabled: simulation && simulation.totalPeople > 0
                    onClicked: {
                        if (simulation) {
                            var result = simulation.findNeverInfectedWithAllNeighborsInfected()
                            console.log("Never infected (all neighbors infected):", result)
                        }
                    }
                }
            }
        }
    }

    // Simple results dialog
    Dialog {
        id: resultsDialog
        title: "Search Results"
        standardButtons: Dialog.Ok
        property var results: []

        ColumnLayout {
            anchors.fill: parent
            Text {
                text: "Found " + resultsDialog.results.length + " people:"
                font.bold: true
            }
            ListView {
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                model: resultsDialog.results
                delegate: Text {
                    text: "ID: " + modelData
                }
            }
        }
    }
}
