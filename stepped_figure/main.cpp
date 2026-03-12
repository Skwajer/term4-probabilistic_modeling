#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include "stepfigureAdapter.hpp"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    StepFigureAdapter adapter;
    engine.rootContext()->setContextProperty("adapter", &adapter);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("stepped_figure", "Main");

    return app.exec();
}
