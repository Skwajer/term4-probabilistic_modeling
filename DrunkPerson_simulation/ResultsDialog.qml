import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: resultsDialog
    title: "Search Results"
    standardButtons: Dialog.Ok
    width: 400
    height: 300
    modal: true

    property var results: []

    background: Rectangle {
        color: "#2a1b3d"
        radius: 12
        border.color: "#bb86fc"
        border.width: 1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            text: "Found " + resultsDialog.results.length + " people:"
            font.bold: true
            color: Material.foreground
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: resultsDialog.results
            delegate: Rectangle {
                width: parent.width
                height: 30
                color: index % 2 ? "#2d2d44" : "#1e1e2f"
                radius: 4

                Text {
                    anchors.centerIn: parent
                    text: "ID: " + modelData
                    color: "#bb86fc"
                    font.family: "Courier New"
                    font.bold: true
                }
            }

            ScrollBar.vertical: ScrollBar {
                active: true
            }
        }
    }
}
