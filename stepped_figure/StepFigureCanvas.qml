import QtQuick 2.15

Item {
    id: root
    anchors.fill: parent

    property var figureData: adapter ? adapter.currentFigure : []
    property double m: adapter ? adapter.M : 10
    property double h: adapter ? adapter.h : 1
    property double tau: adapter ? adapter.tau : 1

    onFigureDataChanged: {
        console.log("Figure data changed, length:", figureData.length)
        canvas.requestPaint()
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            console.log("Canvas painting...")
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            // Фон
            ctx.fillStyle = "#1a142b"
            ctx.fillRect(0, 0, width, height)

            if (!figureData || figureData.length === 0) {
                ctx.fillStyle = "#888"
                ctx.font = "16px Arial"
                ctx.fillText("No data", width/2-50, height/2)
                console.log("No figure data")
                return
            }

            console.log("Drawing", figureData.length, "steps")

            // Отступы
            var leftMargin = 60
            var rightMargin = 40
            var topMargin = 30
            var bottomMargin = 50

            var graphWidth = width - leftMargin - rightMargin
            var graphHeight = height - topMargin - bottomMargin

            // Находим максимум Y для масштабирования
            var maxY = 0
            for (var i = 0; i < figureData.length; i++) {
                if (figureData[i].y > maxY) maxY = figureData[i].y
            }
            maxY = Math.max(maxY, tau) * 1.1
            if (maxY === 0) maxY = 1

            // Оси
            ctx.strokeStyle = "#bb86fc"
            ctx.lineWidth = 2

            // Ось X
            ctx.beginPath()
            ctx.moveTo(leftMargin, height - bottomMargin)
            ctx.lineTo(width - rightMargin, height - bottomMargin)
            ctx.stroke()

            // Ось Y
            ctx.beginPath()
            ctx.moveTo(leftMargin, topMargin)
            ctx.lineTo(leftMargin, height - bottomMargin)
            ctx.stroke()

            // Подписи
            ctx.fillStyle = "#bb86fc"
            ctx.font = "10px Arial"
            ctx.fillText("0", leftMargin - 15, height - bottomMargin + 15)
            ctx.fillText(m.toFixed(1), width - rightMargin - 20, height - bottomMargin + 15)
            ctx.fillText(maxY.toFixed(1), leftMargin - 30, topMargin + 5)

            // Рисуем ступеньки
            var stepWidth = graphWidth / figureData.length

            for (var i = 0; i < figureData.length; i++) {
                var x1 = leftMargin + i * stepWidth
                var x2 = leftMargin + (i + 1) * stepWidth
                var y = height - bottomMargin - (figureData[i].y / maxY) * graphHeight

                // Ступенька
                ctx.fillStyle = "#8bc34a"
                ctx.fillRect(x1, y, stepWidth - 2, height - bottomMargin - y)

                // Контур
                ctx.strokeStyle = "#fbbf24"
                ctx.lineWidth = 1
                ctx.strokeRect(x1, y, stepWidth - 2, height - bottomMargin - y)
            }

            // Проверка на возрастание
            var isIncreasing = true
            for (var i = 1; i < figureData.length; i++) {
                if (figureData[i].y <= figureData[i-1].y) {
                    isIncreasing = false
                    break
                }
            }

            if (isIncreasing) {
                ctx.fillStyle = "#8bc34a"
                ctx.font = "bold 14px Arial"
                ctx.fillText("✓ STRICTLY INCREASING", width - 220, topMargin + 20)
            }

            console.log("Canvas paint completed")
        }
    }

    Component.onCompleted: {
        console.log("StepFigureCanvas loaded")
        canvas.requestPaint()
    }
}
