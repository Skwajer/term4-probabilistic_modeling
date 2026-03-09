cmake_minimum_required(VERSION 3.16)

project(DrunkPerson_simulation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Quick Charts)  # ← Charts здесь!


qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDrunkPerson_simulation
    main.cpp
)

qt_add_qml_module(appDrunkPerson_simulation
    URI DrunkPerson_simulation
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
        SOURCES
        SOURCES simulationAdapter.hpp
        SOURCES simulation.cpp
        SOURCES simulationAdapter.cpp
        QML_FILES Road.qml
        QML_FILES ProbabilityChart.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDrunkPerson_simulation PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDrunkPerson_simulation
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDrunkPerson_simulation
    PRIVATE Qt6::Quick
    Qt6::Charts
)

include(GNUInstallDirs)
install(TARGETS appDrunkPerson_simulation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
