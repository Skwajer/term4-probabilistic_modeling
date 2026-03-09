import QtQuick 2.0

Canvas {
    id: canvas
    anchors.fill: parent
    property int currentPos: simulation ? simulation.current_pos : 0
    property int bpos: simulation ? simulation.B : 0

    onPaint: {
        var ctx = getContext("2d")
        var centerY = height / 2
        var roadStart = 50
        var roadEnd = width
        var roadLength = roadEnd - roadStart
        var pit_height = 100
        var total_road_markings = width / 2
        var segment_size = 50
        var B_pos_x = bpos * segment_size
        var curr_pos_x = currentPos * segment_size

        // PIT
        // PIT - исправленная версия
        ctx.save()

        // Яма должна быть слева от roadStart
        var pitWidth = roadStart - 20  // чуть меньше roadStart
        var pitCenterX = pitWidth / 2  // центр ямы

        // Рисуем овальную яму
        ctx.beginPath()
        ctx.ellipse(pitCenterX-10, centerY-25, pitWidth + 10, pit_height/1.5)
        ctx.fillStyle = "#2C1E0E"
        ctx.fill()

        // Обводка ямы
        ctx.strokeStyle = "#8B4513"
        ctx.lineWidth = 3
        ctx.stroke()

        // Шипы на дне ямы (слева)
        ctx.fillStyle = "#A0522D"
        for (var i = 0; i < 11; i++) {
            var spikeX = 10 + i * (pitWidth - 20) / 4
            var spikeY = centerY + 25

            ctx.beginPath()
            ctx.moveTo(spikeX, spikeY)
            ctx.lineTo(spikeX+2, spikeY - 15)
            ctx.lineTo(spikeX + 5, spikeY)
            ctx.closePath()
            ctx.fill()
        }


        // Надпись "ПРОПАСТЬ"
        ctx.fillStyle = "#FF4500"
        ctx.font = "bold 14px 'Courier New'"
        ctx.shadowColor = "red"
        ctx.shadowBlur = 5
        ctx.fillText("⚠ПРОПАСТЬ⚠", 0, centerY - 30)

        ctx.shadowBlur = 0
        ctx.restore()

        // ROAD
        ctx.beginPath()
        ctx.strokeStyle = "#36454F"
        ctx.lineWidth = 40
        ctx.moveTo(roadStart, centerY)
        ctx.lineTo(roadEnd, centerY)
        ctx.stroke()

        var x = roadStart
        for (var i = 0; i < total_road_markings; i++)
        {
            if (x >= roadEnd) break
            ctx.beginPath()
            ctx.strokeStyle = "white"
            ctx.lineWidth = 2
            ctx.moveTo(x, centerY)
            ctx.lineTo(x+10, centerY)
            ctx.stroke()
            x += 20
        }

        // numerical line
        x = roadStart
        for (var j = 0; j < total_road_markings; j++)
        {
            if (x >= roadEnd) break
            ctx.beginPath()
            ctx.strokeStyle = "white"
            ctx.lineWidth = 1
            ctx.moveTo(x, centerY+4)
            ctx.lineTo(x, centerY+12)
            ctx.stroke()
            ctx.fillStyle = "white"
            ctx.font = "bold 14px Courier New"
            ctx.save()
            ctx.translate(0, centerY-8)
            ctx.fillText(j+1, x-3, 0)
            ctx.restore()
            x += 50
        }


    }

    Text {
            id: pubText
            text: "🏨"
            font.pixelSize: 60
            font.family: "Segoe UI Emoji, Noto Color Emoji, Courier New"
            font.bold: true
            color: "#FFD700"

            x: (simulation ? simulation.B * 50 - 30 : 50)
            y: parent.height/2 - 80

            style: Text.Outline
            styleColor: "#8B4513"

            Connections {
                target: simulation
                function onBChanged() {
                    pubText.x = simulation.B * 50 - 40
                }
            }


        }

    property bool isMoving: false

    NumberAnimation {
        id: walkAnimation
        target: canvas
        property: "currentPos"
        duration: 150
        easing.type: Easing.InOutQuad
        onStarted: canvas.isMoving = true
        onStopped: canvas.isMoving = false
    }

    Text {
        id: pubEmoji
        text: currentPos >= simulation.max_steps_forward ? "🥳" : "🫠"
        font.pixelSize: 30

        x: (simulation ? simulation.current_pos * 50 - 12 : 50)

        y: canvas.height/2 - 35

        SequentialAnimation on y {
            running: canvas.isMoving
            loops: Animation.Infinite
            NumberAnimation { to: root.height/2 - 40; duration: 150; easing.type: Easing.InOutSine }
            NumberAnimation { to: root.height/2 - 35; duration: 150; easing.type: Easing.InOutSine }
        }

        rotation: canvas.isMoving ? Math.sin(Date.now() / 200) * 5 : 0

        Behavior on x {
            NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
        }
    }
}
