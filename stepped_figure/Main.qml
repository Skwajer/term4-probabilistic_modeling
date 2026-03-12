import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 1000
    height: 700
    title: "Step Figures Simulation"

    Material.theme: Material.Dark
    Material.accent: "#bb86fc"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Левая панель - информация
        Rectangle {
            Layout.preferredWidth: 250
            Layout.fillHeight: true
            color: "#2a1b3d"
            radius: 8

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Text {
                    text: "📊 STEP FIGURES"
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

                        Text { text: "M:"; color: "#bb86fc" }
                        Text { text: adapter ? adapter.M.toFixed(1) : "10.0"; color: "#fbbf24" }

                        Text { text: "h:"; color: "#bb86fc" }
                        Text { text: adapter ? adapter.h.toFixed(2) : "1.0"; color: "#fbbf24" }

                        Text { text: "τ:"; color: "#bb86fc" }
                        Text { text: adapter ? adapter.tau.toFixed(2) : "1.0"; color: "#fbbf24" }

                        Text { text: "n:"; color: "#bb86fc" }
                        Text { text: adapter ? adapter.n : "5"; color: "#fbbf24" }

                        Text { text: "Distribution:"; color: "#bb86fc" }
                        Text {
                            text: adapter ? adapter.distType : "uniform"
                            color: "#8bc34a"
                            font.family: "Courier New"
                        }
                    }
                }

                // Статистика
                GroupBox {
                    title: "Statistics"
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            Text { text: "Total figures:"; color: "#bb86fc" }
                            Text {
                                text: adapter ? adapter.totalFigures : "0"
                                color: "#8bc34a"
                                font.bold: true
                            }
                        }

                        RowLayout {
                            Text { text: "Increasing:"; color: "#bb86fc" }
                            Text {
                                text: adapter ? adapter.increasingCount : "0"
                                color: "#8bc34a"
                                font.bold: true
                            }
                        }

                        RowLayout {
                            Text { text: "Probability:"; color: "#bb86fc" }
                            Text {
                                text: adapter && adapter.totalFigures > 0 ?
                                      (adapter.increasingCount / adapter.totalFigures * 100).toFixed(1) + "%" : "0%"
                                color: "#fbbf24"
                                font.bold: true
                                font.pixelSize: 16
                            }
                        }

                        ProgressBar {
                            Layout.fillWidth: true
                            value: adapter && adapter.totalFigures > 0 ?
                                   adapter.increasingCount / adapter.totalFigures : 0
                        }
                    }
                }

                // Пагинация
                GroupBox {
                    title: "Navigation"
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        RowLayout {
                            Text { text: "Showing:"; color: "#bb86fc" }
                            Text {
                                text: (adapter ? adapter.currentIndex + 1 : 0) + " - " +
                                      Math.min(adapter ? adapter.currentIndex + 10 : 0, adapter ? adapter.totalFigures : 0)
                                color: "#fbbf24"
                            }
                            Text {
                                text: "of " + (adapter ? adapter.totalFigures : 0)
                                color: "#bb86fc"
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 5

                            Button {
                                text: "◀ Prev"
                                Layout.fillWidth: true
                                onClicked: adapter.prevPage()
                                enabled: adapter && adapter.currentIndex > 0
                            }

                            Button {
                                text: "Next ▶"
                                Layout.fillWidth: true
                                onClicked: adapter.nextPage()
                                enabled: adapter && adapter.currentIndex + 10 < adapter.totalFigures
                            }
                        }

                        Text {
                            text: "Page " + (adapter ? Math.floor(adapter.currentIndex / 10) + 1 : 1) +
                                  " of " + (adapter ? adapter.totalPages : 1)
                            color: "#bb86fc"
                            font.pixelSize: 12
                            Layout.alignment: Qt.AlignHCenter
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
                    }

                    Button {
                        text: "🎲 Generate 100"
                        Layout.fillWidth: true
                        highlighted: true
                        onClicked: adapter.generateMultiple(100)
                    }

                    Button {
                        text: "🎲 Generate 1000"
                        Layout.fillWidth: true
                        onClicked: adapter.generateMultiple(1000)
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // Правая панель - график
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1a142b"
            radius: 8

            StepFigureCanvas {
                anchors.fill: parent
                anchors.margins: 10
            }
        }
    }

    FileDialog {
        id: jsonDialog
        title: "Select JSON config"
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            var path = jsonDialog.selectedFile.toString()
            path = path.replace("file://", "")
            adapter.loadFromFile(path)
        }
    }
}
