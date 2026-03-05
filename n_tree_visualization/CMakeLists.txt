cmake_minimum_required(VERSION 3.16)

project(n_tree_visualization VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appn_tree_visualization
    main.cpp
)

qt_add_qml_module(appn_tree_visualization
    URI n_tree_visualization
    QML_FILES
        Main.qml
        SOURCES tree_realization.cpp
        SOURCES tree_realization.hpp
        SOURCES tree_moovement.pro
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appn_tree_visualization PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appn_tree_visualization
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appn_tree_visualization
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appn_tree_visualization
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
