import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Моделирование движения в M-рном дереве"

    // Получаем модель из контекста (она создана в main.cpp)

    // Для отладки показываем информацию
    Connections {
        target: treeModel
        function onTreeRebuilt() {
            console.log("=== СИГНАЛ: Дерево перестроено ===")
            console.log("Узлов:", treeModel.nodeCount())
            console.log("M:", treeModel.mValue, "Высота:", treeModel.treeHeight)

            // Принудительно обновляем Repeater'ы
            linkRepeater.model = 0
            nodeRepeater.model = 0

            // Небольшая задержка для гарантии обновления
            Qt.callLater(function() {
                linkRepeater.model = treeModel.nodeCount()
                nodeRepeater.model = treeModel.nodeCount()

                // Принудительно перерисовываем
                canvas.update()
            })
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
                          " | H=" + treeModel.treeHeight
                    color: "#495057"
                    font.pixelSize: 14
                    font.bold: true
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

                    delegate: Rectangle {
                        required property int index

                        property var pos: treeModel.getNodePosition(index)

                        x: pos.x - width/2
                        y: pos.y - height/2
                        width: 15
                        height: 15
                        radius: 8

                        // Цвет узла
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
                            font.pixelSize: 6
                        }

                        // Вероятность остановки (для внутренних узлов)
                        Text {
                            anchors.top: parent.bottom
                            anchors.topMargin: 2
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "p=" + treeModel.stopProbability.toFixed(2)
                            font.pixelSize: 9
                            color: "#868e96"
                            visible: !treeModel.isLeaf(index)
                        }
                    }
                }

                // Текущая движущаяся точка
                Rectangle {
                    id: currentPoint
                    x: treeModel.currentPosition.x - width/2
                    y: treeModel.currentPosition.y - height/2
                    width: 24
                    height: 24
                    radius: 12
                    color: "#fa5252"
                    border.color: "white"
                    border.width: 2

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

                    // Внутренняя точка
                    Rectangle {
                        anchors.centerIn: parent
                        width: 12
                        height: 12
                        radius: 6
                        color: "white"
                        opacity: 0.8
                    }
                }
            }
        }

        // Правая панель - управление
        Rectangle {
            Layout.preferredWidth: 320
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
                                Label { text: "M(кол-во детей)" }
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
                                    console.log("Кнопка нажата: создаем дерево M=" +
                                                mSpin.value + " высота=" + heightSpin.value)
                                    treeModel.rebuildTree(mSpin.value, heightSpin.value)
                                }
                            }
                        }
                    }

                    // Вероятность остановки
                    GroupBox {
                        title: "⏸️ Вероятность остановки p"
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
                                text: "Вероятность остановки в узле"
                                font.pixelSize: 11
                                color: "#868e96"
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
                                text: "👣 Шаг"
                                Layout.fillWidth: true
                                font.pixelSize: 14
                                onClicked: {
                                    console.log("Делаем шаг")
                                    treeModel.moveToChild(-1)
                                }
                            }

                            Button {
                                text: "🔄 Сброс в корень"
                                Layout.fillWidth: true
                                font.pixelSize: 14
                                onClicked: {
                                    console.log("Сброс в корень")
                                    treeModel.resetPosition()
                                }
                            }

                            Button {
                                text: "⏱️ 10 шагов"
                                Layout.fillWidth: true
                                Layout.columnSpan: 2
                                font.pixelSize: 14
                                onClicked: {
                                    console.log("Делаем 10 шагов")
                                    for (var i = 0; i < 10; i++) {
                                        treeModel.moveToChild(-1)
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
                                color: "#212529"
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
                                text: "• Все листья на одной глубине"
                                font.pixelSize: 11
                                color: "#868e96"
                            }
                            Label {
                                text: "• Точка начинает из корня"
                                font.pixelSize: 11
                                color: "#868e96"
                            }
                            Label {
                                text: "• p - вероятность остановки"
                                font.pixelSize: 11
                                color: "#868e96"
                            }
                        }
                    }
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
                      " | p=" + treeModel.stopProbability.toFixed(2)
                color: "#495057"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: new Date().toLocaleTimeString()
                color: "#868e96"
            }
        }
    }
}
