#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QObject>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QMediaDevices>
#include <QDebug>
#include "cameracontroller.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Создаём контроллер и передаём его в QML
    CameraController cameraCtrl;
    engine.rootContext()->setContextProperty("cameraCtrl", &cameraCtrl);

    // Безопасная загрузка QML (Qt 6.5+)
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    // engine.loadFromModule("WebcamApp", "Main");
    // engine.load(QUrl(QStringLiteral("qrc:/test_qml/Main.qml")));
    engine.loadFromModule("test_qml", "Main");


    return app.exec();
}

// int main(int argc, char *argv[])
// {
//     QGuiApplication app(argc, argv);

//     QQmlApplicationEngine engine;
//     QObject::connect(
//         &engine,
//         &QQmlApplicationEngine::objectCreationFailed,
//         &app,
//         []() { QCoreApplication::exit(-1); },
//         Qt::QueuedConnection);
//     engine.loadFromModule("test_qml", "Main");

//     return app.exec();
// }