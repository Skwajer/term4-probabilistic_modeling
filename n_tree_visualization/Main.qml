import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Моделирование движения в M-рном дереве"

    Connections {
        target: treeModel
        function onTreeRebuilt() {
            console.log("=== СИГНАЛ: Дерево перестроено ===")
            linkRepeater.model = 0
            nodeRepeater.model = 0

            Qt.callLater(function() {
                linkRepeater.model = treeModel.nodeCount()
                nodeRepeater.model = treeModel.nodeCount()
                canvas.update()
            })
        }

        function onMovementBlocked() {
            blockedPopup.open()
        }

        function onTargetProbabilityCalculated() {
            console.log("Вероятность достижения цели обновлена:", treeModel.targetProbability)
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Левая панель - визуализация
        Rectangle {
            id: canvasContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f8f9fa"
            border.color: "#dee2e6"
            radius: 8

            Item {
                id: canvas
                anchors.fill: parent
                anchors.margins: 20

                // Отладочная информация
                Text {
                    id: debugInfo
                    anchors.top: parent.top
                    anchors.left: parent.left
                    text: "Узлов: " + treeModel.nodeCount() +
                          " | M=" + treeModel.mValue +
                          " | H=" + treeModel.treeHeight +
                          (treeModel.isStopped ? " | ⛔ ДВИЖЕНИЕ ОСТАНОВЛЕНО" : " | ✅ МОЖНО ДВИГАТЬСЯ")
                    color: treeModel.isStopped ? "#fa5252" : "#495057"
                    font.pixelSize: 14
                    font.bold: true
                    z: 100
                }

                // Информация о цели
                Text {
                    anchors.top: debugInfo.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    text: treeModel.targetNodeId >= 0 ?
                          "🎯 Цель: узел " + treeModel.targetNodeId +
                          " | Вероятность: " + (treeModel.targetProbability * 100).toFixed(2) + "%" :
                          "🎯 Цель не выбрана"
                    color: treeModel.targetNodeId >= 0 ? "#e67700" : "#868e96"
                    font.pixelSize: 12
                    font.bold: treeModel.targetNodeId >= 0
                    z: 100
                }

                // Рисуем связи между узлами
                Repeater {
                    id: linkRepeater
                    model: treeModel.nodeCount()

                    delegate: Repeater {
                        model: treeModel.mValue

                        delegate: Shape {
                            required property int index
                            required property int modelData

                            property int parentId: index
                            property int childId: parentId * treeModel.mValue + modelData + 1

                            visible: childId < treeModel.nodeCount() &&
                                     parentId < treeModel.nodeCount() &&
                                     childId >= 0 && parentId >= 0

                            ShapePath {
                                strokeWidth: 2
                                strokeColor: "#adb5bd"
                                fillColor: "transparent"

                                startX: treeModel.getNodePosition(parentId).x
                                startY: treeModel.getNodePosition(parentId).y

                                PathLine {
                                    x: treeModel.getNodePosition(childId).x
                                    y: treeModel.getNodePosition(childId).y
                                }
                            }
                        }
                    }
                }

                // Рисуем узлы дерева
                Repeater {
                    id: nodeRepeater
                    model: treeModel.nodeCount()

                    delegate: MouseArea {
                        required property int index

                        property var pos: treeModel.getNodePosition(index)

                        width: 30
                        height: 30
                        x: pos.x - width/2
                        y: pos.y - height/2

                        onClicked: {
                            targetSelector.currentNodeId = index
                            targetSelector.open()
                        }

                        // Обводка для целевого узла
                        Rectangle {
                            anchors.fill: parent
                            radius: width/2
                            color: "transparent"
                            border.color: index === treeModel.targetNodeId ? "#e67700" : "transparent"
                            border.width: 3
                            anchors.margins: -3
                        }

                        // Сам узел
                        Rectangle {
                            anchors.centerIn: parent
                            width: 20
                            height: 20
                            radius: 10

                            color: {
                                if (index === 0) return "#40c057" // корень - зеленый
                                if (treeModel.isLeaf(index)) return "#fab005" // лист - оранжевый
                                return "#228be6" // внутренний - синий
                            }

                            border.color: "#495057"
                            border.width: 1

                            // Подсветка текущего узла
                            Rectangle {
                                anchors.fill: parent
                                radius: parent.radius
                                color: "transparent"
                                border.color: "#fa5252"
                                border.width: index === treeModel.getCurrentNodeId() ? 4 : 0
                                opacity: 0.8
                            }

                            // Номер узла
                            Text {
                                anchors.centerIn: parent
                                text: index
                                color: "white"
                                font.bold: true
                                font.pixelSize: 8
                            }
                        }

                        // Вероятность остановки
                        Text {
                            anchors.top: parent.top
                            anchors.topMargin: 22
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "p=" + treeModel.getNodeStopProbability(index).toFixed(3)
                            font.pixelSize: 8
                            color: "#fa5252"
                        }

                        // Подсказка при наведении
                        ToolTip {
                            visible: parent.containsMouse
                            text: "Узел " + index +
                                  (treeModel.isLeaf(index) ? " (лист)" : " (внутр.)") +
                                  "\nВероятность остановки: " +
                                  treeModel.getNodeStopProbability(index).toFixed(3) +
                                  "\nНажмите, чтобы выбрать как цель"
                            delay: 500
                        }
                    }
                }

                // Текущая движущаяся точка
                Rectangle {
                    id: currentPoint
                    x: treeModel.currentPosition.x - width/2
                    y: treeModel.currentPosition.y - height/2
                    width: treeModel.isStopped ? 30 : 24
                    height: treeModel.isStopped ? 30 : 24
                    radius: treeModel.isStopped ? 15 : 12
                    color: treeModel.isStopped ? "#868e96" : "#fa5252"
                    border.color: "white"
                    border.width: treeModel.isStopped ? 3 : 2

                    Behavior on x {
                        NumberAnimation {
                            duration: 300
                            easing.type: Easing.OutCubic
                        }
                    }
                    Behavior on y {
                        NumberAnimation {
                            duration: 300
                            easing.type: Easing.OutCubic
                        }
                    }
                    Behavior on width {
                        NumberAnimation { duration: 200 }
                    }
                    Behavior on height {
                        NumberAnimation { duration: 200 }
                    }
                    Behavior on radius {
                        NumberAnimation { duration: 200 }
                    }

                    Rectangle {
                        anchors.centerIn: parent
                        width: parent.width * 0.5
                        height: parent.height * 0.5
                        radius: width/2
                        color: "white"
                        opacity: 0.8
                    }

                    Text {
                        anchors.centerIn: parent
                        text: treeModel.isStopped ? "⛔" : ""
                        font.pixelSize: parent.width * 0.6
                        color: "white"
                        visible: treeModel.isStopped
                    }
                }
            }
        }

        // Правая панель - управление
        Rectangle {
            Layout.preferredWidth: 380
            Layout.fillHeight: true
            color: "#ffffff"
            border.color: "#dee2e6"
            radius: 8

            ScrollView {
                anchors.fill: parent
                anchors.margins: 15
                clip: true
                contentWidth: availableWidth

                ColumnLayout {
                    width: parent.width
                    spacing: 20

                    Label {
                        text: "🎮 Управление"
                        font.pixelSize: 20
                        font.bold: true
                        color: "#212529"
                    }

                    // Параметры дерева
                    GroupBox {
                        title: "🌳 Параметры дерева"
                        font.pixelSize: 15
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 10

                            RowLayout {
                                Label { text: "M (кол-во детей):" }
                                SpinBox {
                                    id: mSpin
                                    from: 2
                                    to: 5
                                    value: 2
                                    editable: true
                                }
                            }

                            RowLayout {
                                Label { text: "Высота дерева:" }
                                SpinBox {
                                    id: heightSpin
                                    from: 2
                                    to: 5
                                    value: 3
                                    editable: true
                                }
                            }

                            Button {
                                text: "🔄 Создать новое дерево"
                                Layout.fillWidth: true
                                font.pixelSize: 14
                                padding: 10

                                onClicked: {
                                    treeModel.rebuildTree(mSpin.value, heightSpin.value)
                                }
                            }
                        }
                    }

                    // Вероятность остановки
                    GroupBox {
                        title: "⏸️ Базовая вероятность остановки"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent

                            RowLayout {
                                Slider {
                                    id: pSlider
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 0.5
                                    value: 0.1
                                    stepSize: 0.01
                                    onValueChanged: {
                                        treeModel.setStopProbability(value)
                                        pValue.text = value.toFixed(2)
                                    }
                                }
                                Label {
                                    id: pValue
                                    text: pSlider.value.toFixed(2)
                                    font.bold: true
                                    color: "#228be6"
                                }
                            }

                            Label {
                                text: "Базовая вероятность для генерации случайных значений"
                                font.pixelSize: 11
                                color: "#868e96"
                                wrapMode: Text.WordWrap
                            }
                        }
                    }

                    // Закон распределения
                    GroupBox {
                        title: "📊 Закон распределения"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            RadioButton {
                                text: "Равномерное"
                                checked: true
                                onClicked: treeModel.setDistribution(0)
                                font.pixelSize: 14
                            }
                            RadioButton {
                                text: "Геометрическое (убывающее)"
                                onClicked: treeModel.setDistribution(1)
                                font.pixelSize: 14
                            }
                            RadioButton {
                                text: "Возрастающее"
                                onClicked: treeModel.setDistribution(2)
                                font.pixelSize: 14
                            }

                            Label {
                                text: "Текущий закон: " + treeModel.distributionType
                                font.pixelSize: 12
                                color: "#228be6"
                                font.bold: true
                            }
                        }
                    }

                    // Управление целью
                    GroupBox {
                        title: "🎯 Управление целью"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 10

                            RowLayout {
                                Label { text: "ID целевого узла:" }
                                SpinBox {
                                    id: targetSpin
                                    from: -1
                                    to: treeModel.nodeCount() - 1
                                    value: treeModel.targetNodeId
                                    editable: true
                                    onValueChanged: {
                                        treeModel.setTargetNodeId(value)
                                    }
                                }
                            }

                            Button {
                                text: "🧹 Сбросить цель"
                                Layout.fillWidth: true
                                onClicked: {
                                    targetSpin.value = -1
                                    treeModel.setTargetNodeId(-1)
                                }
                            }

                            Label {
                                text: "Совет: Нажмите на любой узел на схеме, чтобы выбрать его как цель"
                                font.pixelSize: 11
                                color: "#868e96"
                                wrapMode: Text.WordWrap
                            }
                        }
                    }

                    // Информация о цели
                    GroupBox {
                        title: "📊 Информация о цели"
                        Layout.fillWidth: true
                        visible: treeModel.targetNodeId >= 0

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            Label {
                                text: treeModel.targetInfo
                                font.pixelSize: 13
                                wrapMode: Text.WordWrap
                                color: "#212529"
                                lineHeight: 1.4
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 30
                                color: "#e7f5ff"
                                radius: 4

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 5

                                    Text {
                                        text: "🎯 Вероятность:"
                                        font.pixelSize: 12
                                        color: "#1864ab"
                                    }

                                    Text {
                                    text: "🎯 Вероятность: " + treeModel.targetProbability.toFixed(4)
                                    font.pixelSize: 14
                                        font.bold: true
                                        color: "#1864ab"
                                    }

                                    Item { Layout.fillWidth: true }

                                    Text {
                                        text: {
                                            if (treeModel.targetProbability < 0.01) return "🟢 Очень низкая"
                                            if (treeModel.targetProbability < 0.05) return "🟡 Низкая"
                                            if (treeModel.targetProbability < 0.15) return "🟠 Средняя"
                                            return "🔴 Высокая"
                                        }
                                        font.pixelSize: 11
                                    }
                                }
                            }
                        }
                    }

                    // Управление точкой
                    GroupBox {
                        title: "🎯 Управление точкой"
                        Layout.fillWidth: true

                        GridLayout {
                            anchors.fill: parent
                            columns: 2
                            rowSpacing: 10
                            columnSpacing: 10

                            Button {
                                text: treeModel.isStopped ? "⛔ Движение заблокировано" : "👣 Шаг"
                                Layout.fillWidth: true
                                font.pixelSize: 14
                                enabled: !treeModel.isStopped
                                background: Rectangle {
                                    color: parent.enabled ? "#228be6" : "#ced4da"
                                    radius: 4
                                }
                                onClicked: {
                                    treeModel.moveToChild(-1)
                                }
                            }

                            Button {
                                text: "🔄 Сброс в корень"
                                Layout.fillWidth: true
                                font.pixelSize: 14
                                background: Rectangle {
                                    color: "#40c057"
                                    radius: 4
                                }
                                onClicked: {
                                    treeModel.resetPosition()
                                }
                            }

                            Button {
                                text: "⏱️ 10 шагов"
                                Layout.fillWidth: true
                                Layout.columnSpan: 2
                                font.pixelSize: 14
                                enabled: !treeModel.isStopped
                                background: Rectangle {
                                    color: parent.enabled ? "#fab005" : "#ced4da"
                                    radius: 4
                                }
                                onClicked: {
                                    for (var i = 0; i < 10; i++) {
                                        if (!treeModel.isStopped) {
                                            treeModel.moveToChild(-1)
                                        } else {
                                            break
                                        }
                                    }
                                }
                            }

                            Button {
                                text: "🎯 К цели"
                                Layout.fillWidth: true
                                Layout.columnSpan: 2
                                font.pixelSize: 14
                                enabled: !treeModel.isStopped && treeModel.targetNodeId >= 0
                                background: Rectangle {
                                    color: parent.enabled ? "#e67700" : "#ced4da"
                                    radius: 4
                                }
                                onClicked: {
                                    // Двигаемся, пока не достигнем цели или не остановимся
                                    var maxSteps = 50 // Защита от бесконечного цикла
                                    var steps = 0
                                    while (!treeModel.isStopped &&
                                           treeModel.getCurrentNodeId() !== treeModel.targetNodeId &&
                                           steps < maxSteps) {
                                        treeModel.moveToChild(-1)
                                        steps++
                                    }
                                    if (treeModel.getCurrentNodeId() === treeModel.targetNodeId) {
                                        targetReachedPopup.open()
                                    }
                                }
                            }
                        }
                    }

                    // Информация о текущем состоянии
                    GroupBox {
                        title: "📍 Текущее состояние"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            Label {
                                text: treeModel.currentNodeInfo
                                font.pixelSize: 14
                                wrapMode: Text.WordWrap
                                color: treeModel.isStopped ? "#fa5252" : "#212529"
                            }

                            GridLayout {
                                columns: 2
                                rowSpacing: 5
                                columnSpacing: 10

                                Label { text: "Координаты:"; font.pixelSize: 12; color: "#868e96" }
                                Label {
                                    text: "(" + Math.round(treeModel.currentPosition.x) +
                                          ", " + Math.round(treeModel.currentPosition.y) + ")"
                                    font.pixelSize: 12
                                    font.bold: true
                                }

                                Label { text: "Всего узлов:"; font.pixelSize: 12; color: "#868e96" }
                                Label {
                                    text: treeModel.nodeCount()
                                    font.pixelSize: 12
                                    font.bold: true
                                }

                                Label { text: "Тип:"; font.pixelSize: 12; color: "#868e96" }
                                Label {
                                    text: treeModel.isLeaf(treeModel.getCurrentNodeId()) ? "🍃 Лист" : "📁 Внутренний"
                                    font.pixelSize: 12
                                    font.bold: true
                                }

                                Label { text: "Статус:"; font.pixelSize: 12; color: "#868e96" }
                                Label {
                                    text: treeModel.isStopped ? "⛔ Остановлено" : "✅ Активно"
                                    font.pixelSize: 12
                                    font.bold: true
                                    color: treeModel.isStopped ? "#fa5252" : "#40c057"
                                }

                                Label { text: "Цель:"; font.pixelSize: 12; color: "#868e96" }
                                Label {
                                    text: treeModel.targetNodeId >= 0 ?
                                          "узел " + treeModel.targetNodeId : "не выбрана"
                                    font.pixelSize: 12
                                    font.bold: true
                                    color: treeModel.targetNodeId >= 0 ? "#e67700" : "#868e96"
                                }
                            }
                        }
                    }

                    // Информация о приложении
                    GroupBox {
                        title: "ℹ️ О приложении"
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 5

                            Label {
                                text: "Идеальное M-рное дерево"
                                font.bold: true
                                color: "#212529"
                            }
                            Label {
                                text: "• Каждый узел имеет M детей"
                                font.pixelSize: 11
                                color: "#868e96"
                            }
                            Label {
                                text: "• У каждого узла своя случайная вероятность остановки"
                                font.pixelSize: 11
                                color: "#868e96"
                            }
                            Label {
                                text: "• После остановки движение блокируется до reset"
                                font.pixelSize: 11
                                color: "#fa5252"
                            }
                            Label {
                                text: "• Можно выбрать целевую точку и рассчитать вероятность попадания"
                                font.pixelSize: 11
                                color: "#e67700"
                            }
                        }
                    }
                }
            }
        }
    }

    // Диалог выбора цели
    Dialog {
        id: targetSelector
        title: "Выбор целевого узла"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true

        property int currentNodeId: 0

        onAccepted: {
            treeModel.setTargetNodeId(currentNodeId)
        }

        ColumnLayout {
            spacing: 15

            Label {
                text: "Вы хотите выбрать узел " + targetSelector.currentNodeId + " как цель?"
                font.pixelSize: 14
            }

            Rectangle {
                Layout.fillWidth: true
                height: 60
                color: "#f8f9fa"
                radius: 4

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        text: "Тип: " + (treeModel.isLeaf(targetSelector.currentNodeId) ? "🍃 Лист" : "📁 Внутренний")
                        font.pixelSize: 12
                    }
                    Text {
                        text: "Уровень: " + (targetSelector.currentNodeId >= 0 ?
                              treeModel.getPathToNode(targetSelector.currentNodeId).length - 1 : 0)
                        font.pixelSize: 12
                    }
                    Text {
                        text: "Вероятность остановки: " +
                              treeModel.getNodeStopProbability(targetSelector.currentNodeId).toFixed(3)
                        font.pixelSize: 12
                    }
                }
            }

            Label {
                text: "Будет рассчитана вероятность достижения этого узла из корня"
                font.pixelSize: 11
                color: "#868e96"
                wrapMode: Text.WordWrap
                Layout.maximumWidth: 300
            }
        }
    }

    // Всплывающее окно при попытке движения в заблокированном состоянии
    Popup {
        id: blockedPopup
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 300
        height: 150
        modal: true
        focus: true

        background: Rectangle {
            color: "#fff9f9"
            border.color: "#fa5252"
            border.width: 2
            radius: 10
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            Label {
                text: "⛔ Движение заблокировано!"
                font.pixelSize: 18
                font.bold: true
                color: "#fa5252"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: "Произошла остановка в узле. Нажмите 'Сброс в корень' для продолжения."
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                color: "#495057"
                Layout.fillWidth: true
            }

            Button {
                text: "OK"
                Layout.alignment: Qt.AlignHCenter
                onClicked: blockedPopup.close()
                background: Rectangle {
                    color: "#228be6"
                    radius: 4
                }
                contentItem: Text {
                    text: "OK"
                    color: "white"
                    font.bold: true
                }
            }
        }
    }

    // Всплывающее окно при достижении цели
    Popup {
        id: targetReachedPopup
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 300
        height: 150
        modal: true
        focus: true

        background: Rectangle {
            color: "#fff3bf"
            border.color: "#e67700"
            border.width: 2
            radius: 10
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            Label {
                text: "🎯 ЦЕЛЬ ДОСТИГНУТА!"
                font.pixelSize: 18
                font.bold: true
                color: "#e67700"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: "Вы успешно достигли узла " + treeModel.targetNodeId
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                color: "#495057"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                text: "Отлично!"
                Layout.alignment: Qt.AlignHCenter
                onClicked: targetReachedPopup.close()
                background: Rectangle {
                    color: "#e67700"
                    radius: 4
                }
                contentItem: Text {
                    text: "Отлично!"
                    color: "white"
                    font.bold: true
                }
            }
        }
    }

    // Статус бар
    footer: ToolBar {
        background: Rectangle {
            color: "#f1f3f5"
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            Label {
                text: "M=" + treeModel.mValue + " | Высота=" + treeModel.treeHeight +
                      " | " + treeModel.distributionType +
                      " | Базовая p=" + treeModel.stopProbability.toFixed(2)
                color: "#495057"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: (treeModel.targetNodeId >= 0 ?
                      "🎯 Цель:" + treeModel.targetNodeId + " " : "") +
                      (treeModel.isStopped ? "⛔ ОСТАНОВЛЕНО" : "✅ АКТИВНО") +
                      " | " + new Date().toLocaleTimeString()
                color: treeModel.isStopped ? "#fa5252" :
                       (treeModel.targetNodeId >= 0 ? "#e67700" : "#40c057")
                font.bold: treeModel.isStopped || treeModel.targetNodeId >= 0
            }
        }
    }
}
