import QtQuick 2.15
import QtQuick.Controls 2.15
import "."

Item {
    id: chart
    anchors.fill: parent

    property var fallProbabilities: simulation ? simulation.fallProbabilities : []
    property var pubProbabilities: simulation ? simulation.pubProbabilities : []
    property var bValues: simulation ? simulation.bValues : []
    property double currentP: simulation ? simulation.p : 0.4
    property int currentB: simulation ? simulation.B : 10

    // ОТЛАДКА
    onFallProbabilitiesChanged: console.log("fallProbabilities changed, length=", fallProbabilities.length)
    onPubProbabilitiesChanged: console.log("pubProbabilities changed, length=", pubProbabilities.length)
    onBValuesChanged: console.log("bValues changed, length=", bValues.length)

    Connections {
        target: simulation
        function onStatsChanged() {
            console.log("STATS CHANGED! Данные обновлены")
            canvas.requestPaint()
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        anchors.bottomMargin: 50

        onPaint: {
            console.log("Canvas paint, bValues length =", chart.bValues.length)
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            // Отступы
            var padL = 44, padR = 24, padT = 24, padB = 44
            var W = width  - padL - padR
            var H = height - padT - padB
            if (W <= 0 || H <= 0) return

            var bVals = chart.bValues
            var fallVals = chart.fallProbabilities
            var pubVals = chart.pubProbabilities
            var bMax = (bVals.length > 0) ? bVals[bVals.length - 1] : 20

            // ── Grid ─────────────────────────────────────────────────────────
            ctx.strokeStyle = "#2a2048"
            ctx.lineWidth = 1

            // Горизонтальные линии (вероятности 0.0 до 1.0)
            for (var i = 0; i <= 5; i++) {
                var val = i / 5.0
                var yg = padT + H - val * H
                ctx.beginPath()
                ctx.moveTo(padL, yg)
                ctx.lineTo(padL + W, yg)
                ctx.stroke()

                ctx.fillStyle = "#a5a1b8"
                ctx.font = "10px monospace"
                ctx.textAlign = "right"
                ctx.fillText(val.toFixed(1), padL - 6, yg + 4)
            }

            // Вертикальные линии (значения B)
            var bStep = Math.max(1, Math.floor(bMax / 8))
            for (var b = 1; b <= bMax; b += bStep) {
                var xg = padL + ((b - 1) / (bMax - 1)) * W
                ctx.beginPath()
                ctx.moveTo(xg, padT)
                ctx.lineTo(xg, padT + H)
                ctx.stroke()

                ctx.fillStyle = "#a5a1b8"
                ctx.font = "10px monospace"
                ctx.textAlign = "center"
                ctx.fillText(b, xg, padT + H + 16)
            }

            // Подписи осей
            ctx.fillStyle = "#a5a1b8"
            ctx.font = "11px monospace"
            ctx.textAlign = "center"
            ctx.fillText("B  (стартовая позиция)", padL + W / 2, height - 4)

            ctx.save()
            ctx.translate(11, padT + H / 2)
            ctx.rotate(-Math.PI / 2)
            ctx.fillText("вероятность", 0, 0)
            ctx.restore()

            // p label сверху
            ctx.fillStyle = "#fbbf24"
            ctx.font = "bold 12px monospace"
            ctx.textAlign = "right"
            ctx.fillText("p = " + chart.currentP.toFixed(2) + "   q = " + (1 - chart.currentP).toFixed(2),
                         padL + W, padT - 6)

            if (bVals.length < 2) {
                ctx.fillStyle = "#a5a1b8"
                ctx.font = "13px monospace"
                ctx.textAlign = "center"
                ctx.fillText("Нажми 'Запустить статистику'", padL + W / 2, padT + H / 2)
                return
            }

            function bToX(bVal) {
                return padL + ((bVal - 1) / (bMax - 1)) * W
            }

            // ── Заливка для падений ────────────────────────────────────────
            ctx.beginPath()
            for (var k = 0; k < bVals.length; k++) {
                var px = bToX(bVals[k])
                var py = padT + H - fallVals[k] * H
                if (k === 0) ctx.moveTo(px, py)
                else ctx.lineTo(px, py)
            }
            ctx.lineTo(bToX(bVals[bVals.length-1]), padT + H)
            ctx.lineTo(bToX(bVals[0]), padT + H)
            ctx.closePath()
            ctx.fillStyle = "rgba(248,113,113,0.07)"
            ctx.fill()

            // ── Линия падений ──────────────────────────────────────────────
            ctx.beginPath()
            ctx.strokeStyle = "#f87171"
            ctx.lineWidth = 2.5
            ctx.lineJoin = "round"
            for (var k2 = 0; k2 < bVals.length; k2++) {
                var px2 = bToX(bVals[k2])
                var py2 = padT + H - fallVals[k2] * H
                if (k2 === 0) ctx.moveTo(px2, py2)
                else ctx.lineTo(px2, py2)
            }
            ctx.stroke()

            // ── Линия побед (достижение паба/конца) ────────────────────────
            ctx.beginPath()
            ctx.strokeStyle = "#34d399"
            ctx.lineWidth = 2.5
            ctx.lineJoin = "round"
            for (var m = 0; m < bVals.length; m++) {
                var pxm = bToX(bVals[m])
                var pym = padT + H - pubVals[m] * H
                if (m === 0) ctx.moveTo(pxm, pym)
                else ctx.lineTo(pxm, pym)
            }
            ctx.stroke()

            // ── Маркер текущего B ──────────────────────────────────────────
            var curB = chart.currentB
            if (curB >= 1 && curB <= bMax) {
                var curX = bToX(curB)
                ctx.beginPath()
                ctx.strokeStyle = "#fbbf24"
                ctx.lineWidth = 1.5
                ctx.setLineDash([5, 4])
                ctx.moveTo(curX, padT)
                ctx.lineTo(curX, padT + H)
                ctx.stroke()
                ctx.setLineDash([])

                // Точка на линии падений для текущего B
                for (var t = 0; t < bVals.length; t++) {
                    if (Math.abs(bVals[t] - curB) < 0.1) {
                        var fallY = padT + H - fallVals[t] * H
                        ctx.beginPath()
                        ctx.arc(curX, fallY, 5, 0, Math.PI * 2)
                        ctx.fillStyle = "#f87171"
                        ctx.fill()
                        ctx.strokeStyle = "#0f0c1f"
                        ctx.lineWidth = 1.5
                        ctx.stroke()

                        var pubY = padT + H - pubVals[t] * H
                        ctx.beginPath()
                        ctx.arc(curX, pubY, 5, 0, Math.PI * 2)
                        ctx.fillStyle = "#34d399"
                        ctx.fill()
                        ctx.stroke()
                        break
                    }
                }

                ctx.fillStyle = "#fbbf24"
                ctx.font = "bold 11px monospace"
                ctx.textAlign = "center"
                ctx.fillText("B=" + curB, curX, padT - 6)
            }

            // ── Легенда ────────────────────────────────────────────────────
            var lx = padL + 10, ly = padT + 10
            ctx.fillStyle = "#f87171"
            ctx.fillRect(lx, ly, 18, 3)
            ctx.fillStyle = "#e0d9ff"
            ctx.font = "11px monospace"
            ctx.textAlign = "left"
            ctx.fillText("P(упасть)", lx + 22, ly + 4)

            ctx.fillStyle = "#34d399"
            ctx.fillRect(lx, ly + 16, 18, 3)
            ctx.fillStyle = "#e0d9ff"
            ctx.fillText("P(победа)", lx + 22, ly + 20)
        }
    }

    Button {
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 10
        }
        width: 220
        height: 36
        text: "🚀 Запустить статистику (1000)"

        background: Rectangle {
            color: parent.hovered ? "#9f7aea" : "#7c4dff"
            radius: 8
        }

        contentItem: Text {
            text: parent.text
            color: "white"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        onClicked: {
            console.log("Кнопка нажата!")
            if (simulation) {
                simulation.runStatistics(1000)
            }
        }
    }
}
