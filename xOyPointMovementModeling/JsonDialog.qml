FileDialog {
    id: fileDialog
    title: "Select JSON configuration file"
    nameFilters: ["JSON files (*.json)", "All files (*)"]
    currentFolder: shortcuts.home

    onAccepted: {
        var path = fileDialog.selectedFile
        path = path.toString()
        path = path.replace("file://", "")

        if (Qt.platform.os === "windows") {
            path = path.replace(/^\/([a-zA-Z]:)/, "$1")
        }

        console.log("Loading JSON file:", path)

        simulation.loadFromFile(path)
    }

    onRejected: {
        console.log("File selection canceled")
    }
}
