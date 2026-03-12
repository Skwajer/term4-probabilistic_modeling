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
                    Layout.preferredHeight: 80
                    color: "#2a1b3d"
                    border.color: "#bb86fc"
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 4

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 20

                            Text {
                                text: "📈 xOy POINT MOVEMENT SIMULATION"
                                font.bold: true
                                font.pixelSize: 18
                                color: "#bb86fc"
                            }

                            Item { Layout.fillWidth: true }

                            // Текущий шаг
                            Text {
                                text: "Step:"
                                font.family: "Courier New"
                                font.bold: true
                                color: "#8bc34a"
                            }
                            Text {
                                text: simulation ? (simulation.trajectory.length - 1) : "0"
                                font.family: "Courier New"
                                font.bold: true
                                color: "#8bc34a"
                            }

                            // Текущая позиция Y
                            Text {
                                text: "Y:"
                                font.family: "Courier New"
                                font.bold: true
                                color: "#fbbf24"
                            }
                            Text {
                                text: simulation ? simulation.currentY.toFixed(3) : "0.000"
                                font.family: "Courier New"
                                font.bold: true
                                color: "#fbbf24"
                            }

                            // Количество пересечений
                            Text {
                                text: "Crossings:"
                                font.family: "Courier New"
                                font.bold: true
                                color: "#f87171"
                            }
                            Text {
                                text: simulation ? simulation.intersectionCount : 0
                                font.family: "Courier New"
                                font.bold: true
                                color: "#f87171"
                            }

                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 20

                            // Параметры h, n, K
                            Text {
                                text: "h=" + (simulation ? simulation.h : "0.5")
                                font.family: "Courier New"
                                font.bold: true
                                color: "#6495ED"
                            }

                            Text {
                                text: "n=" + (simulation ? simulation.n : "100")
                                font.family: "Courier New"
                                font.bold: true
                                color: "#6495ED"
                            }

                            Text {
                                text: "K=" + (simulation ? simulation.k : "10")
                                font.family: "Courier New"
                                font.bold: true
                                color: "#6495ED"
                            }

                            Text {
                                text: "l=" + (configManager ? configManager.l : "0")
                                font.family: "Courier New"
                                font.bold: true
                                color: "#6495ED"
                            }

                            // Разделитель
                            Text {
                                text: "|"
                                color: "#bb86fc"
                                font.pixelSize: 18
                            }

                            // Текущее распределение
                            Text {
                                text: "📊 " + (simulation ? simulation.distStr : "uniform")
                                font.family: "Courier New"
                                font.bold: true
                                color: "#bb86fc"
                            }

                            // Количество значений s
                            Text {
                                text: "| s: " + (configManager ? configManager.s.length : "0") + " vals"
                                font.family: "Courier New"
                                color: "#a5a1b8"
                            }

                            // Индикатор состояния
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 8
                                color: {
                                    if (!simulation) return "gray"
                                    if (simulation.intersectionCount >= (configManager ? configManager.l : 0))
                                        return "#fbbf24"  // Достигнуто l пересечений
                                    if (simulation.currentY > 0) return "#8bc34a"
                                    if (simulation.currentY < 0) return "#e57373"
                                    return "#bb86fc"
                                }
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
                    color: "#bb86fc"
                    Layout.alignment: Qt.AlignHCenter
                }

                /*FileDialog {
                    id: fileDialog
                }
                Button {
                    text: "open file"
                    onClicked: fileDialog.open()
                }*/
                JsonDialog {
                    id: fileDialog
                }
                Button {
                    text: "open JSON"
                    onClicked: fileDialog.open()
                }

                // Parameters


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

                PlaneView {

                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Canvas clicked")
                }
            }
        }

    }

}


}
