import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

// ========== ФАЙЛОВЫЙ ДИАЛОГ ==========
FileDialog {
    id: fileDialog
    title: "Select data file"
    nameFilters: ["Text files (*.txt)", "All files (*)"]
    currentFolder: shortcuts.home

    onAccepted: {
        var path = fileDialog.selectedFile
        path = path.toString()
        path = path.replace("file://", "")

        if (Qt.platform.os === "windows") {
            path = path.replace(/^\/([a-zA-Z]:)/, "$1")
        }

        console.log("Loading file:", path)
        simulation.loadFromFile(path)
    }

    onRejected: {
        console.log("File selection canceled")
    }
}

