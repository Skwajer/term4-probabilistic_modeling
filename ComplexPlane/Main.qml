import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Dialogs
import QtCore

ApplicationWindow {
    visible: true
    width: 1200
    height: 700
    title: "Complex Plane Trajectory"

    Material.theme: Material.Dark
    Material.accent: "#bb86fc"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // ЛЕВАЯ ПАНЕЛЬ - ИНФОРМАЦИЯ
        Rectangle {
            Layout.preferredWidth: 250
            Layout.fillHeight: true
            color: "#2a1b3d"
            radius: 8

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                // Заголовок
                Text {
                    text: "ℹ️ STATISTICS"
                    font.bold: true
                    font.pixelSize: 18
                    color: "#bb86fc"
                    Layout.alignment: Qt.AlignHCenter
                }

                // Параметры
                GroupBox {
                    title: "Parameters"
                    Layout.fillWidth: true

                    GridLayout {
                        columns: 2
                        anchors.fill: parent
                        rowSpacing: 8
                        columnSpacing: 10

                        Text { text: "ρ (step):"; color: "#bb86fc" }
                        Text {
                            text: simulation ? simulation.rho.toFixed(2) : "1.0"
                            color: "#fbbf24"
                            font.bold: true
                        }

                        Text { text: "K (display):"; color: "#bb86fc" }
                        Text {
                            text: simulation ? simulation.K : "10"
                            color: "#fbbf24"
                        }

                        Text { text: "Distribution:"; color: "#bb86fc" }
                        Text {
                            text: simulation ? simulation.distType : "uniform"
                            color: "#8bc34a"
                            font.family: "Courier New"
                        }

                        Text { text: "ξ values:"; color: "#bb86fc" }
                        Text {
                            text: simulation ? simulation.xiCount : "0"
                            color: "#8bc34a"
                        }
                    }
                }

                // Статистика возвратов
                GroupBox {
                    title: "Returns"
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            Text { text: "Total returns:"; color: "#bb86fc" }
                            Text {
                                text: simulation ? simulation.totalReturns : "0"
                                color: "#8bc34a"
                                font.bold: true
                                font.pixelSize: 16
                            }
                        }

                        RowLayout {
                            Text { text: "Total trials:"; color: "#bb86fc" }
                            Text {
                                text: simulation ? simulation.totalTrials : "0"
                                color: "#8bc34a"
                                font.bold: true
                                font.pixelSize: 16
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 2
                            color: "#3d3d5c"
                        }

                        RowLayout {
                            Text {
                                text: "Return rate:"
                                color: "#bb86fc"
                                font.bold: true
                            }
                            Text {
                                text: {
                                    if (!simulation || simulation.totalTrials === 0) return "0%"
                                    var rate = (simulation.totalReturns / simulation.totalTrials * 100).toFixed(1)
                                    return rate + "%"
                                }
                                color: simulation && simulation.totalReturns > 0 ? "#8bc34a" : "#e57373"
                                font.bold: true
                                font.pixelSize: 18
                            }
                        }

                        ProgressBar {
                            Layout.fillWidth: true
                            value: simulation && simulation.totalTrials > 0 ?
                                   simulation.totalReturns / simulation.totalTrials : 0
                            background: Rectangle {
                                color: "#3d3d5c"
                                radius: 3
                            }
                            contentItem: Rectangle {
                                color: "#8bc34a"
                                radius: 3
                            }
                        }
                    }
                }

                GroupBox {
                    title: "Progress"
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            Text { text: "Trial:"; color: "#bb86fc" }
                            Text {
                                text: simulation ? simulation.currentTrial + "/" + simulation.maxTrials : "0/0"
                                color: "#fbbf24"
                                font.bold: true
                            }
                        }

                        ProgressBar {
                            Layout.fillWidth: true
                            value: simulation && simulation.maxTrials > 0 ?
                                   simulation.currentTrial / simulation.maxTrials : 0
                            background: Rectangle {
                                color: "#3d3d5c"
                                radius: 3
                            }
                            contentItem: Rectangle {
                                color: "#bb86fc"
                                radius: 3
                            }
                        }

                        Text {
                            text: "Building trajectory..."
                            color: "#8bc34a"
                            visible: simulation && simulation.currentTrial > 0 &&
                                     simulation.currentTrial < simulation.maxTrials
                            font.italic: true
                        }
                    }
                }

                // Текущая траектория
                GroupBox {
                    title: "Current Trajectory"
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 5

                        RowLayout {
                            Text { text: "Steps:"; color: "#bb86fc" }
                            Text {
                                text: simulation && simulation.trajectory ?
                                      (simulation.trajectory.length - 1) : "0"
                                color: "#fbbf24"
                                font.bold: true
                            }
                        }

                        RowLayout {
                            Text { text: "Returned:"; color: "#bb86fc" }
                            Text {
                                text: simulation && simulation.returned ? "✅ YES" : "❌ NO"
                                color: simulation && simulation.returned ? "#8bc34a" : "#e57373"
                                font.bold: true
                            }
                        }

                        RowLayout {
                            Text { text: "Steps to return:"; color: "#bb86fc" }
                            Text {
                                text: simulation && simulation.returned ?
                                      simulation.stepsToReturn : "—"
                                color: "#fbbf24"
                                font.bold: true
                            }
                        }
                    }
                }

                // Кнопки управления
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Button {
                        text: "📁 Load JSON"
                        Layout.fillWidth: true
                        onClicked: jsonDialog.open()
                        height: 40
                    }

                    Button {
                        text: "▶ Run Single"
                        Layout.fillWidth: true
                        onClicked: simulation.runSingleSimulation(1000000)
                        height: 40
                        highlighted: true
                    }

                    Button {
                        text: "🎲 Run Multiple (100)"
                        Layout.fillWidth: true
                        onClicked: simulation.runMultipleSimulations(100, 1000000)
                        height: 40
                    }

                    Button {
                        text: "🔄 Reset"
                        Layout.fillWidth: true
                        onClicked: simulation.reset()
                        height: 40
                    }
                    Button {
                        text: "📋 Show Returns"
                        Layout.fillWidth: true
                        onClicked: returnsDialog.open()
                        height: 40
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // ПРАВАЯ ПАНЕЛЬ - ГРАФИК
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1a142b"
            radius: 8

            TrajectoryCanvas {
                anchors.fill: parent
                anchors.margins: 10
            }
        }
    }

    // Диалог для JSON
    JsonDialog {
        id: jsonDialog
        title: "Select JSON configuration"
        nameFilters: ["JSON files (*.json)"]
        currentFolder: StandardPaths.writableLocation(StandardPaths.HomeLocation)

        onAccepted: {
            var path = jsonDialog.selectedFile.toString()
            path = path.replace("file://", "")
            console.log("Loading JSON:", path)
            simulation.loadFromFile(path)
        }
    }

    ReturnsDialog {
        id: returnsDialog
    }
}


