QT += quick qml
CONFIG += c++11

SOURCES += \
    main.cpp \
    tree_realization.cpp

HEADERS += \
    tree_realization.hpp

RESOURCES += \
    qml.qrc

# Добавляем путь к QML модулю
QML_IMPORT_PATH = $$PWD

target.path = $$[QT_INSTALL_EXAMPLES]/tree_movement
INSTALLS += target
