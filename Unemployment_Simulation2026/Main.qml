import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import QtQuick.Controls.Material 2.15
import Qt5Compat.GraphicalEffects

ApplicationWindow {
    visible: true
    width: 650
    height: 500
    title: "Disease Simulation"

    // ========== ГЛОБАЛЬНЫЙ СТИЛЬ ==========
    Material.theme: Material.Dark
    Material.accent: "#bb86fc"        // Мягкий фиолетовый для акцентов
    Material.primary: "#7c4dff"       // Средний фиолетовый
    Material.foreground: "#e0e0e0"    // Светло-серый текст
    Material.background: "#1a142b"     // Темно-фиолетовый фон

    // Градиентный фон всего окна
    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1e1a2b" }
            GradientStop { position: 0.5; color: "#2a1b3d" }
            GradientStop { position: 1.0; color: "#1f1a36" }
        }
    }

    // Кастомные цвета для разных состояний
    readonly property color successColor: "#8bc34a"
    readonly property color errorColor: "#e57373"
    readonly property color infoColor: "#6495ED"
    readonly property color neutralColor: "#D3D3D3"
    readonly property color buttonPurple: "#9f7aea"
    readonly property color buttonDarkPurple: "#7c4dff"

    Dialog {
        id: resultDialog
        title: "Recovered People"
        standardButtons: Dialog.Ok
        width: 400
        height: 300
        modal: true

        // Стилизация диалога
        background: Rectangle {
            color: "#2a1b3d"
            radius: 12
            border.color: "#bb86fc"
            border.width: 1
        }

        header: Label {
            text: "Recovered People"
            font.bold: true
            font.pixelSize: 16
            color: "#bb86fc"
            padding: 12
        }

        property var resultIds: []

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10

            Text {
                text: "Found " + resultDialog.resultIds.length + " recovered people:"
                font.bold: true
                color: Material.foreground
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: resultDialog.resultIds
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

    FileDialog {
        id: fileDialog
        title: "Select data file"
        nameFilters: ["Text files (*.txt)", "All files (*)"]
        currentFolder: shortcuts.home

        onAccepted: {
            var path = fileDialog.selectedFile
            path = path.toString()
            path = path.replace("file://", "")

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
        spacing: 1

        // Statistics
        GroupBox {
            title: "Statistics"
            Layout.fillWidth: true

            label: Text {
                text: "Statistics"
                color: "#bb86fc"
                font.bold: true
                font.pixelSize: 20
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

                Text { text: "Healthy:"; color: successColor; font.bold: true }
                Text {
                    text: simulation ? simulation.healthyCount : "0"
                    color: successColor
                    font.bold: true
                    font.family: "Courier New"
                    font.pixelSize: 20
                }

                Text { text: "Infected:"; color: errorColor; font.bold: true }
                Text {
                    text: simulation ? simulation.infectiousCount : "0"
                    color: errorColor
                    font.bold: true
                    font.family: "Courier New"
                    font.pixelSize: 20
                }

                Text { text: "Recovered:"; color: infoColor; font.bold: true }
                Text {
                    text: simulation ? simulation.recoveredCount : "0"
                    color: infoColor
                    font.bold: true
                    font.family: "Courier New"
                    font.pixelSize: 20
                }

                Text { text: "Total people:"; color: neutralColor; font.bold: true }
                Text {
                    text: simulation ? simulation.totalPeople : "0"
                    color: neutralColor
                    font.bold: true
                    font.family: "Courier New"
                    font.pixelSize: 20
                }

                Text {
                    text: "Time step:"
                    font.family: "Courier New"
                    font.pixelSize: 16
                    font.bold: true
                    color: successColor
                }
                Text {
                    text: simulation ? simulation.TimeStep : "0"
                    font.family: "Courier New"
                    font.pixelSize: 16
                    font.bold: true
                    color: successColor
                }
            }
        }
        // Parameters
        GroupBox {
            title: "Parameters"
            Layout.fillWidth: true

            label: Text {
                text: "Parameters"
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
                columns: 3
                anchors.fill: parent
                anchors.margins: 10

                Text {
                    text: "p1 (infection):"
                    color: errorColor
                    font.italic: true
                    font.bold: true
                }

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

                    // Стилизация слайдера
                    background: Rectangle {
                        x: p1Slider.leftPadding
                        y: p1Slider.topPadding + p1Slider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 6
                        width: p1Slider.availableWidth
                        height: implicitHeight
                        radius: 3
                        color: "#3d3d5c"

                        Rectangle {
                            width: p1Slider.visualPosition * parent.width
                            height: parent.height
                            color: errorColor
                            radius: 3
                        }
                    }

                    handle: Rectangle {
                        x: p1Slider.leftPadding + p1Slider.visualPosition * (p1Slider.availableWidth - width)
                        y: p1Slider.topPadding + p1Slider.availableHeight / 2 - height / 2
                        implicitWidth: 20
                        implicitHeight: 20
                        radius: 10
                        color: errorColor
                        border.color: "white"
                        border.width: 1

                        // Анимация при наведении
                        scale: p1Slider.pressed ? 1.2 : 1.0

                        Behavior on scale {
                            NumberAnimation { duration: 100 }
                        }
                    }
                }

                Text {
                    text: p1Slider.value.toFixed(2)
                    color: errorColor
                    font.bold: true
                    font.family: "Courier New"
                }

                Text {
                    text: "p2 (recovery):"
                    color: infoColor
                    font.italic: true
                    font.bold: true
                }

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
                        implicitHeight: 6
                        width: p2Slider.availableWidth
                        height: implicitHeight
                        radius: 3
                        color: "#3d3d5c"

                        Rectangle {
                            width: p2Slider.visualPosition * parent.width
                            height: parent.height
                            color: infoColor
                            radius: 3
                        }
                    }

                    handle: Rectangle {
                        x: p2Slider.leftPadding + p2Slider.visualPosition * (p2Slider.availableWidth - width)
                        y: p2Slider.topPadding + p2Slider.availableHeight / 2 - height / 2
                        implicitWidth: 20
                        implicitHeight: 20
                        radius: 10
                        color: infoColor
                        border.color: "white"
                        border.width: 1

                        scale: p2Slider.pressed ? 1.2 : 1.0

                        Behavior on scale {
                            NumberAnimation { duration: 100 }
                        }
                    }
                }

                Text {
                    text: p2Slider.value.toFixed(2)
                    color: infoColor
                    font.bold: true
                    font.family: "Courier New"
                }
            }
        }

        // Control buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            // Load file button
            Button {
                text: "Load file"
                Layout.fillWidth: true
                onClicked: fileDialog.open()

                // Стиль кнопки
                background: Rectangle {
                    implicitHeight: 40
                    color: parent.hovered ? "#9f7aea" : "#7c4dff"
                    radius: 8

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }

                    layer.enabled: true
                    layer.effect:  DropShadow{
                        transparentBorder: true
                        radius: 8
                        samples: 16
                        color: "#407c4dff"
                        horizontalOffset: 0
                        verticalOffset: 2
                    }
                }

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Infect button (primary action)
            Button {
                id: infect
                text: "INFECT"
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

            // Play/Pause button
            Button {
                text: simulation.running ? "PAUSE" : "PLAY"
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

        // Search section
        GroupBox {
            title: "Search"
            Layout.fillWidth: true

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

                // Search buttons with unified style
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
                                case 0: result = simulation.findNeverInfected(); break
                                case 1:
                                    var res = simulation.findRecovered()
                                    resultDialog.resultIds = res
                                    resultDialog.open()
                                    return
                                case 2: result = simulation.findRecoveredWithUnrecoveredNeighbors(); break
                                case 3: result = simulation.findNeverInfectedWithAllNeighborsInfected(); break
                            }

                            if (result) {
                                console.log(text + ":", result)
                                // Можно показать в том же диалоге
                                resultDialog.resultIds = result
                                resultDialog.title = text
                                resultDialog.open()
                            }
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
        width: 400
        height: 300

        background: Rectangle {
            color: "#2a1b3d"
            radius: 12
            border.color: "#bb86fc"
            border.width: 1
        }

        property var results: []

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
}
