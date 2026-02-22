cmake_minimum_required(VERSION 3.16)

project(Unemployment_Simulation2026 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)


qt_add_executable(appUnemployment_Simulation2026
    main.cpp
)

qt_add_qml_module(appUnemployment_Simulation2026
    URI Unemployment_Simulation2026
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES simulation.cpp
        SOURCES simulation.hpp
        SOURCES simulationAdapter.cpp
        SOURCES simulationAdapter.hpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appUnemployment_Simulation2026 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appUnemployment_Simulation2026
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appUnemployment_Simulation2026
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appUnemployment_Simulation2026
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
