#pragma once
#include <QObject>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QMediaDevices>
#include <QDebug>

class CameraController : public QObject
{
    Q_OBJECT
    // Экспортируем QVideoSink* в QML
    Q_PROPERTY( QVideoSink* videoSink READ videoSink CONSTANT)

public:
    explicit CameraController(QObject *parent = nullptr) : QObject(parent)
    {
        // Проверяем наличие камер
        if (!QMediaDevices::videoInputs().isEmpty()) {
            m_session.setCamera(&m_camera);
            m_session.setVideoSink(&m_videoSink);
            m_camera.start();
            qDebug() << "Камера запущена.";
        } else {
            qWarning() << "Видеокамеры не найдены!";
        }
    }

     QVideoSink* videoSink()  { return &m_videoSink; }

private:
    QCamera m_camera;
    QMediaCaptureSession m_session;
    QVideoSink m_videoSink;
};