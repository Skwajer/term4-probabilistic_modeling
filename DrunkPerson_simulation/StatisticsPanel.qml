import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


GroupBox {
    title: "Statistics"
    Layout.fillWidth: false
    width: parent.width * 0.3      // ширина 30% от родителя
        height: parent.height * 0.1

    property var simulation

    label: Text {
        text: "Statistics"
        color: "#bb86fc"
        font.bold: true
        font.pixelSize: 20
        leftPadding: 10
    }

    background: Rectangle {
        color: "#2a1b3d"
        radius: 14
        border.color: "#bb86fc"
        border.width: 1
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        anchors.margins: 10

        Text { text: "Healthy:"; color: "#8bc34a"; font.bold: true }
        Text {
            text: simulation ? simulation.healthyCount : "0"
            color: "#8bc34a"
            font.bold: true
            font.family: "Courier New"
            font.pixelSize: 20
        }

        Text { text: "Infected:"; color: "#e57373"; font.bold: true }
        Text {
            text: simulation ? simulation.infectiousCount : "0"
            color: "#e57373"
            font.bold: true
            font.family: "Courier New"
            font.pixelSize: 20
        }

        Text { text: "Recovered:"; color: "#6495ED"; font.bold: true }
        Text {
            text: simulation ? simulation.recoveredCount : "0"
            color: "#6495ED"
            font.bold: true
            font.family: "Courier New"
            font.pixelSize: 20
        }

        Text { text: "Total people:"; color: "#D3D3D3"; font.bold: true }
        Text
        {
            text: simulation ? simulation.totalPeople : "0"
            color: "#6495ED"
            font.bold: true
            font.family: "Courier New"
            font.pixelSize: 20
        }

        Text {
            text: "Time step:"
            font.family: "Courier New"
            font.pixelSize: 16
            font.bold: true
            color: "#8bc34a"
        }
        Text {
            text: simulation ? simulation.TimeStep : "0"
            font.family: "Courier New"
            font.pixelSize: 16
            font.bold: true
            color: "#8bc34a"
        }
    }
}
