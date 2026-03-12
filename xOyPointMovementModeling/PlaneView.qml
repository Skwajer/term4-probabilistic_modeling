import QtQuick 2.15

Item {
    id: root
    anchors.fill: parent

    property var trajectory: simulation ? simulation.trajectory : []
    property double currentY: simulation ? simulation.currentY : 0
    property int intersectionCount: simulation ? simulation.intersectionCount : 0
    property int n: simulation ? simulation.n : 100
    property double h: simulation ? simulation.h : 1.0

    // Вычисляем мин/макс для масштабирования (теперь с дробными)
    property double minY: {
        if (trajectory.length === 0) return -10
        var min = trajectory[0]
        for (var i = 1; i < trajectory.length; i++) {
            if (trajectory[i] < min) min = trajectory[i]
        }
        return min
    }
    property double maxY: {
        if (trajectory.length === 0) return 10
        var max = trajectory[0]
        for (var i = 1; i < trajectory.length; i++) {
            if (trajectory[i] > max) max = trajectory[i]
        }
        return max
    }
    property double yRange: Math.max(0.1, maxY - minY) // защита от нуля

    onTrajectoryChanged: {
        console.log("Trajectory changed, length:", trajectory.length)
        canvas.requestPaint()
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.fillStyle = "#1a142b"
            ctx.fillRect(0, 0, width, height)

            var leftMargin = 70
            var rightMargin = 70
            var topMargin = 40
            var bottomMargin = 50

            var graphWidth = width - leftMargin - rightMargin
            var graphHeight = height - topMargin - bottomMargin

            var maxX = (n - 1) * h
            if (maxX <= 0) maxX = 1

            function xPos(xValue) {
                return leftMargin + (xValue / maxX) * graphWidth
            }

            function yPos(yValue) {
                return topMargin + graphHeight - ((yValue - minY) / yRange) * graphHeight
            }

            ctx.strokeStyle = "#bb86fc"
            ctx.lineWidth = 2

            var yZero = yPos(0)
            ctx.beginPath()
            ctx.moveTo(leftMargin, yZero)
            ctx.lineTo(width - rightMargin, yZero)
            ctx.stroke()

            ctx.beginPath()
            ctx.moveTo(width - rightMargin, yZero)
            ctx.lineTo(width - rightMargin - 10, yZero + 5)
            ctx.lineTo(width - rightMargin - 10, yZero - 5)
            ctx.closePath()
            ctx.fillStyle = "#bb86fc"
            ctx.fill()

            ctx.beginPath()
            ctx.moveTo(leftMargin, topMargin)
            ctx.lineTo(leftMargin, height - bottomMargin)
            ctx.stroke()

            ctx.beginPath()
            ctx.moveTo(leftMargin, topMargin)
            ctx.lineTo(leftMargin - 5, topMargin + 10)
            ctx.lineTo(leftMargin + 5, topMargin + 10)
            ctx.closePath()
            ctx.fillStyle = "#bb86fc"
            ctx.fill()

            ctx.strokeStyle = "#bb86fc"
            ctx.lineWidth = 1
            ctx.fillStyle = "#bb86fc"
            ctx.font = "9px Arial"

            var xTickStep = Math.max(1, Math.floor(n / 10)) * h
            for (var xVal = 0; xVal <= maxX; xVal += xTickStep) {
                var xPixel = xPos(xVal)

                if (xPixel >= leftMargin && xPixel <= width - rightMargin) {
                    ctx.beginPath()
                    ctx.moveTo(xPixel, yZero - 5)
                    ctx.lineTo(xPixel, yZero + 5)
                    ctx.stroke()

                    ctx.fillText(xVal.toFixed(1), xPixel - 12, yZero + 18)
                }
            }

            ctx.strokeStyle = "#bb86fc"
            ctx.lineWidth = 1
            ctx.fillStyle = "#bb86fc"
            ctx.font = "9px Arial"

            var yStep = yRange / 8
            for (var yVal = minY; yVal <= maxY; yVal += yStep) {
                var yPixel = yPos(yVal)

                if (yPixel >= topMargin && yPixel <= height - bottomMargin) {
                    ctx.beginPath()
                    ctx.moveTo(leftMargin - 5, yPixel)
                    ctx.lineTo(leftMargin + 5, yPixel)
                    ctx.stroke()

                    ctx.fillText(yVal.toFixed(2), leftMargin - 45, yPixel + 4)
                }
            }

            ctx.fillStyle = "#ffffff"
            ctx.font = "12px Arial"
            ctx.fillText("X", width - rightMargin - 5, yZero - 10)
            ctx.fillText("Y", leftMargin - 25, topMargin + 10)

            if (trajectory.length > 0) {
                ctx.strokeStyle = "#fbbf24"
                ctx.lineWidth = 2
                ctx.beginPath()

                for (var i = 0; i < trajectory.length; i++) {
                    var xVal = i * h
                    var xPixel = xPos(xVal)
                    var yPixel = yPos(trajectory[i])

                    if (i === 0) {
                        ctx.moveTo(xPixel, yPixel)
                    } else {
                        ctx.lineTo(xPixel, yPixel)
                        ctx.stroke()
                    }

                    ctx.fillStyle = i === trajectory.length - 1 ? "#fbbf24" : "#f87171"
                    ctx.beginPath()
                    ctx.arc(xPixel, yPixel, 4, 0, 2 * Math.PI)
                    ctx.fill()
                }
                ctx.stroke()

                ctx.fillStyle = "#fbbf24"
                ctx.font = "bold 14px Courier New"
                ctx.fillText("Y = " + currentY.toFixed(3), width - 200, topMargin + 20)
                ctx.fillText("X = " + ((trajectory.length - 1) * h).toFixed(2), width - 200, topMargin + 40)
                ctx.fillText("Пересечений: " + intersectionCount, width - 200, topMargin + 60)
            } else {
                ctx.fillStyle = "#888"
                ctx.font = "16px Arial"
                ctx.fillText("NO DATA. RUN SIMULATION", width / 2 - 100, height / 2 - 100)
            }
        }
    }
}
