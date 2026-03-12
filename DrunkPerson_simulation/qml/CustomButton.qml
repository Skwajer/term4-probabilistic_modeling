import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Button {
    id: root

    property color baseColor: Style.primary
    property color hoverColor: Style.buttonPurple
    property color textColor: "white"
    property bool hasShadow: true
    property int animationDuration: Style.animationDuration

    implicitHeight: 40

    background: Rectangle {
        color: root.hovered ? root.hoverColor : root.baseColor
        radius: Style.radiusMedium

        Behavior on color {
            ColorAnimation { duration: root.animationDuration }
        }

        layer.enabled: root.hasShadow
        layer.effect: DropShadow {
            transparentBorder: true
            radius: 8
            samples: 16
            color: "#40" + root.baseColor.toString().slice(1)
            horizontalOffset: 0
            verticalOffset: 2
        }

        scale: root.pressed ? 0.95 : 1.0

        Behavior on scale {
            NumberAnimation { duration: root.animationDuration }
        }
    }

    contentItem: Text {
        text: root.text
        color: root.textColor
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
