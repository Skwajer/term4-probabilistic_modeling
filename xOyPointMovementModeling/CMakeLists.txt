cmake_minimum_required(VERSION 3.16)

project(xOyPointMovementModeling VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appxOyPointMovementModeling
    main.cpp
)

qt_add_qml_module(appxOyPointMovementModeling
    URI xOyPointMovementModeling
    QML_FILES
        Main.qml
        QML_FILES Style.qml
        QML_FILES CustomButton.qml
        QML_FILES StatisticsPanel.qml
        QML_FILES ParametersPanel.qml
        QML_FILES ControlButtons.qml
        QML_FILES SearchPanel.qml
        QML_FILES FileDialog.qml
        QML_FILES ResultDialog.qml
        QML_FILES ResultsDialog.qml
        SOURCES simulation.hpp
        SOURCES simulationAdapter.hpp
        SOURCES simulation.cpp
        SOURCES simulationAdapter.cpp
        SOURCES ConfigManager.hpp
        SOURCES ConfigManager.cpp
        QML_FILES PlaneView.qml
        QML_FILES JsonDialog.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appxOyPointMovementModeling PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appxOyPointMovementModeling
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appxOyPointMovementModeling
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appxOyPointMovementModeling
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

