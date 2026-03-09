import QtQuick 2.15

pragma Singleton

QtObject {
    readonly property color successColor: "#8bc34a"
    readonly property color errorColor: "#e57373"
    readonly property color infoColor: "#6495ED"
    readonly property color neutralColor: "#D3D3D3"
    readonly property color buttonPurple: "#9f7aea"
    readonly property color buttonDarkPurple: "#7c4dff"
    readonly property color accent: "#bb86fc"
    readonly property color primary: "#7c4dff"
    readonly property color background: "#1a142b"
    readonly property color darkBg: "#2a1b3d"
    readonly property color darkerBg: "#1e1e2f"
    readonly property color evenDarkerBg: "#2d2d44"

    readonly property Gradient backgroundGradient: Gradient {
        GradientStop { position: 0.0; color: "#1e1a2b" }
        GradientStop { position: 0.5; color: "#2a1b3d" }
        GradientStop { position: 1.0; color: "#1f1a36" }
    }
}
