import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root
    title: "Return Records"
    modal: true
    standardButtons: Dialog.Ok
    width: 600
    height: 400

    property var records: simulation ? simulation.getReturnRecords() : []

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            text: "Total returns: " + (simulation ? simulation.totalReturns : 0)
            color: "#bb86fc"
            font.bold: true
            font.pixelSize: 16
        }

        // Таблица с записями
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: root.records
            clip: true

            header: Rectangle {
                width: parent.width
                height: 30
                color: "#2a1b3d"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 5

                    Text { text: "Trial"; color: "#bb86fc"; font.bold: true; Layout.preferredWidth: 60 }
                    Text { text: "Steps"; color: "#bb86fc"; font.bold: true; Layout.preferredWidth: 60 }
                    Text { text: "Final X"; color: "#bb86fc"; font.bold: true; Layout.preferredWidth: 80 }
                    Text { text: "Final Y"; color: "#bb86fc"; font.bold: true; Layout.preferredWidth: 80 }
                }
            }

            delegate: Rectangle {
                width: parent.width
                height: 30
                color: index % 2 ? "#2d2d44" : "#1e1e2f"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 5

                    Text {
                        text: modelData.trial
                        color: "#fbbf24"
                        font.family: "Courier New"
                        Layout.preferredWidth: 60
                    }
                    Text {
                        text: modelData.steps
                        color: modelData.steps < 10 ? "#8bc34a" : "#fbbf24"
                        font.family: "Courier New"
                        Layout.preferredWidth: 60
                    }
                    Text {
                        text: modelData.x.toFixed(4)
                        color: "#6495ED"
                        font.family: "Courier New"
                        Layout.preferredWidth: 80
                    }
                    Text {
                        text: modelData.y.toFixed(4)
                        color: "#6495ED"
                        font.family: "Courier New"
                        Layout.preferredWidth: 80
                    }
                }
            }
        }

        // Статистика по шагам
        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "#2a1b3d"
            radius: 5

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5

                Text {
                    text: "Average steps: " + (() => {
                        if (root.records.length === 0) return "0"
                        var sum = 0
                        for (var i = 0; i < root.records.length; i++) {
                            sum += root.records[i].steps
                        }
                        return (sum / root.records.length).toFixed(2)
                    })()
                    color: "#8bc34a"
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: "Min: " + (() => {
                        if (root.records.length === 0) return "0"
                        var min = root.records[0].steps
                        for (var i = 1; i < root.records.length; i++) {
                            if (root.records[i].steps < min) min = root.records[i].steps
                        }
                        return min
                    })()
                    color: "#fbbf24"
                }

                Text {
                    text: "Max: " + (() => {
                        if (root.records.length === 0) return "0"
                        var max = root.records[0].steps
                        for (var i = 1; i < root.records.length; i++) {
                            if (root.records[i].steps > max) max = root.records[i].steps
                        }
                        return max
                    })()
                    color: "#fbbf24"
                }
            }
        }
    }

    // Обновление при изменении статистики
    Connections {
        target: simulation
        function onStatsUpdated() {
            root.records = simulation.getReturnRecords()
        }
    }
}
