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
        ctx.save()

        var pitWidth = roadStart - 20
        var pitCenterX = pitWidth / 2

        ctx.beginPath()
        ctx.ellipse(0, centerY-25, pitWidth + 20, pit_height)
        ctx.fillStyle = "#2C1E0E"
        ctx.fill()

        ctx.strokeStyle = "#8B4513"
        ctx.lineWidth = 3
        ctx.stroke()

        ctx.fillStyle = "#A0522D"
        for (var i = 0; i < 11; i++) {
            var spikeX = 10 + i * (pitWidth - 20) / 4
            var spikeY = centerY + 60

            ctx.beginPath()
            ctx.moveTo(spikeX, spikeY)
            ctx.lineTo(spikeX+2, spikeY - 15)
            ctx.lineTo(spikeX + 5, spikeY)
            ctx.closePath()
            ctx.fill()
        }


        ctx.fillStyle = "#FF4500"
        ctx.font = "bold 14px 'Courier New'"
        ctx.shadowColor = "red"
        ctx.shadowBlur = 5
        ctx.fillText("⚠CLIFF⚠", 0, centerY - 30)

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
        Behavior on x {
            NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
        }
        Connections {
            target: simulation
            function onResetPressed() {
                pubEmoji.y = canvas.height/2 - 35

            }
        }
        property bool isFalling: false
        onXChanged: {
                if (simulation && simulation.current_pos === 0) {
                    isFalling = true
                    fallAnimation.start()
                }
            }

            ParallelAnimation {
                id: fallAnimation
                running: isFalling

                NumberAnimation {
                    target: pubEmoji
                    property: "x"
                    to: 12
                    duration: 500
                    easing.type: Easing.InQuad
                }

                NumberAnimation {
                       target: pubEmoji
                       property: "y"
                       to: canvas.height/2 + 40
                       duration: 800
                       easing.type: Easing.InQuad
                   }



                NumberAnimation {
                    target: pubEmoji
                    property: "rotation"
                    to: 720
                    duration: 800
                    easing.type: Easing.InQuad
                }



                onStopped: {
                    fallEffect.visible = true
                    fallEffect.opacity = 1
                }

            }

            Text {
                id: fallEffect
                text: "💀"
                font.pixelSize: 60
                anchors.centerIn: parent
                opacity: 0
                visible: false

                SequentialAnimation {
                    id: effectAnimation
                    running: fallEffect.visible
                    NumberAnimation { target: fallEffect; property: "scale"; to: 1.5; duration: 200 }
                    NumberAnimation { target: fallEffect; property: "scale"; to: 1.0; duration: 200 }
                    NumberAnimation { target: fallEffect; property: "opacity"; to: 0; duration: 500 }
                    ScriptAction { script: fallEffect.visible = false }
                }
            }

            rotation: canvas.isMoving && !isFalling ? Math.sin(Date.now() / 200) * 5 : 0

            Behavior on x {
                NumberAnimation { duration: 500; easing.type: Easing.InOutQuad }
            }
        }



}
