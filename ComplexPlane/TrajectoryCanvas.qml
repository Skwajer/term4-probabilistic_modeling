import QtQuick 2.15

Item {
    id: root
    anchors.fill: parent

    property var trajectory: simulation ? simulation.trajectory : []
    property real scale: 50  // масштаб для отображения

    onTrajectoryChanged: canvas.requestPaint()

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            // Фон
            ctx.fillStyle = "#1a142b"
            ctx.fillRect(0, 0, width, height)

            // Центр координат
            var centerX = width / 2
            var centerY = height / 2

            // Оси
            ctx.strokeStyle = "#bb86fc"
            ctx.lineWidth = 1

            // Ось X
            ctx.beginPath()
            ctx.moveTo(0, centerY)
            ctx.lineTo(width, centerY)
            ctx.stroke()

            // Ось Y
            ctx.beginPath()
            ctx.moveTo(centerX, 0)
            ctx.lineTo(centerX, height)
            ctx.stroke()

            // Рисуем траекторию
            if (trajectory.length > 0) {
                ctx.strokeStyle = "#fbbf24"
                ctx.lineWidth = 2
                ctx.beginPath()

                for (var i = 0; i < trajectory.length; i++) {
                    var point = trajectory[i]
                    var x = centerX + point.x * root.scale
                    var y = centerY - point.y * root.scale  // минус чтобы Y рос вверх

                    if (i === 0) {
                        ctx.moveTo(x, y)
                    } else {
                        ctx.lineTo(x, y)
                    }

                    // Точки
                    ctx.fillStyle = i === trajectory.length - 1 ? "#fbbf24" : "#f87171"
                    ctx.beginPath()
                    ctx.arc(x, y, 3, 0, 2 * Math.PI)
                    ctx.fill()
                }
                ctx.stroke()

                // Информация о последней точке
                var last = trajectory[trajectory.length - 1]
                ctx.fillStyle = "white"
                ctx.font = "12px Courier New"
                ctx.fillText("Last: (" + last.x.toFixed(2) + ", " + last.y.toFixed(2) + ")",
                            10, 30)
                ctx.fillText("Steps: " + (trajectory.length - 1), 10, 50)
                ctx.fillText("Returned: " + (simulation.returned ? "Yes" : "No"), 10, 70)
            } else {
                ctx.fillStyle = "#888"
                ctx.font = "16px Arial"
                ctx.fillText("No trajectory", centerX - 50, centerY)
            }
        }
    }
}
