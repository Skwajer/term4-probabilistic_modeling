import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    visible: true
    width: 900
    height: 600
    title: "DrunkPerson_simulation"

    Material.theme: Material.Dark
    Material.accent: "#bb86fc"

    ColumnLayout {
        anchors.fill: parent
                anchors.margins: 0
                spacing: 1

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    color: "#2a1b3d"
                    border.color: "#bb86fc"
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 20

                        Text {
                            text: "🍺 DRUNK PERSON SIMULATION"
                            font.bold: true
                            font.pixelSize: 18
                            color: "#bb86fc"
                        }

                        Item { Layout.fillWidth: true }

                        Text {
                        text: "Time step:"
                        font.family: "Courier New"
                        font.bold: true
                        color: successColor
                        }
                        Text {
                        text: simulation ? simulation.timeleft : "0"
                        font.family: "Courier New"
                        font.bold: true
                        color: successColor
                        }

                        Text {
                            text: "Position: " + (simulation ? simulation.current_pos : 0)
                            color: "#8bc34a"
                            font.family: "Courier New"
                            font.bold: true
                        }

                        Text {
                            text: "finish pos: " + (simulation ? simulation.max_steps_forward : 20)
                            color: "#6495ED"
                            font.family: "Courier New"
                            font.bold: true
                        }

                        Rectangle {
                            width: 16
                            height: 16
                            radius: 8
                            color: {
                                if (!simulation) return "gray"
                                if (simulation.current_pos <= 0) return "#e57373"
                                if (simulation.current_pos >= simulation.max_steps_forward) return "#8bc34a"
                                return "#bb86fc"
                            }
                        }
                    }
                }



    SplitView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true

        Pane {
            id: leftPanel
            SplitView.minimumWidth: 200
            SplitView.maximumWidth: 400
            SplitView.preferredWidth: 280

            ColumnLayout {
                anchors.fill: parent
                spacing: 15

                Label {
                    text: "MENU"
                    font.bold: true
                    font.pixelSize: 18
                    color: Style.accent
                    Layout.alignment: Qt.AlignHCenter
                }

                FileDialog {
                    id: fileDialog
                }
                Button {
                    text: "open file"
                    onClicked: fileDialog.open()
                }

                // Parameters
                ParametersPanel {
                }

                ControlButtons {

                }


                Button {
                    text: "📊 Показать графики"
                    Layout.fillWidth: true
                    onClicked: chartWindow.open()
                                }


                Item { Layout.fillHeight: true }
            }
        }

        // ========== ПРАВАЯ ПАНЕЛЬ (CANVAS) ==========
        Pane {
            id: canvasPanel
            SplitView.fillWidth: true

            Rectangle {
                id: root
                anchors.fill: parent
                //anchors.fill: parent
                color: "#2a1b3d"
                radius: 10

                Road {

                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Canvas clicked")
                }
            }
        }

    }
    Dialog {
            id: chartWindow
            width: 800
            height: 600
            title: "Статистика вероятностей"
            visible: false

            ProbabilityChart {
                anchors.fill: parent
                anchors.margins: 10
            }
        }
}

    // Диалоги
    ResultDialog { id: resultDialog }
    ResultsDialog { id: resultsDialog }
}
