#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Header/treemodel.h"
#include "Header/fileeditor.h"
#include <QIcon>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon("icons/app_icon.png"));
    Scanner fileScanner;
    QString rootPath = "/home/khoi";
    qDebug() << "Thư mục làm việc hiện tại của ứng dụng là: " << rootPath;
    std::unique_ptr<Directory> scannedRoot = fileScanner.scan_Directory(rootPath, -1);

    TreeModel fileTreeModel(&app);
    fileTreeModel.setRootEntry(std::move(scannedRoot));

    fileEditor fileEditor(&app);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fileTreeModel", &fileTreeModel);
    engine.rootContext()->setContextProperty("fileEditor", &fileEditor);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("file_System_Ex", "Main");
    return app.exec();
}
