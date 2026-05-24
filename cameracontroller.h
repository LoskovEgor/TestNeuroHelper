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
    // Экспортируем QCamera* в QML для управления камерой
    Q_PROPERTY(QCamera* camera READ camera CONSTANT)
    // Экспортируем QVideoSink* для обработки кадров в C++ (OpenCV, нейросеть)
    Q_PROPERTY(QVideoSink* videoSink READ videoSink CONSTANT)

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
        
        // Подключаемся к сигналу нового кадра для обработки
        connect(&m_videoSink, &QVideoSink::videoFrameChanged,
                this, &CameraController::onVideoFrameChanged);
    }

    QCamera* camera() { return &m_camera; }
    QVideoSink* videoSink() { return &m_videoSink; }

public slots:
    // Слот для обработки нового кадра
    void onVideoFrameChanged(const QVideoFrame &frame)
    {
        // Здесь будет вызов функции обработки OpenCV / нейросети
        // processFrame(frame);
    }

signals:
    // Сигнал для передачи обработанных данных обратно в QML (если нужно)
    void frameProcessed(const QVariant &data);

private:
    QCamera m_camera;
    QMediaCaptureSession m_session;
    QVideoSink m_videoSink;
};