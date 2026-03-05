#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tree_realization.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MTreeModel>("TreeModel", 1, 0, "MTreeModel");

    QQmlApplicationEngine engine;

    MTreeModel *treeModel = new MTreeModel(&engine);
    engine.rootContext()->setContextProperty("treeModel", treeModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("n_tree_visualization", "Main");

    return app.exec();
}
