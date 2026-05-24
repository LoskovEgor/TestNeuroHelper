#pragma once
#include <QObject>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QVideoFrame>
#include <QMediaDevices>
#include <QDebug>
#include <QImage>
#include <opencv2/opencv.hpp>

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
        // Конвертируем QVideoFrame в QImage для обработки
        QImage image = frame.toImage();
        if (image.isNull())
            return;

        // Конвертируем QImage в cv::Mat для OpenCV
        cv::Mat mat = qImageToCvMat(image);

        // === ЗДЕСЬ ВЫЗЫВАЕМ ФУНКЦИИ ОБРАБОТКИ ===
        
        // 1. Пример: детекция объектов через OpenCV (например, детектор лиц)
        // detectObjects(mat);
        
        // 2. Пример: вызов нейросети для распознавания
        // runNeuralNetwork(mat);
        
        // 3. Пример: простая индикация (рисуем прямоугольник)
        // cv::rectangle(mat, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(0, 255, 0), 2);
        
        // После обработки можно отправить результат обратно в QML
        // emit frameProcessed(...);
    }

signals:
    // Сигнал для передачи обработанных данных обратно в QML (если нужно)
    void frameProcessed(const QVariant &data);
    // Сигнал для отправки обработанного изображения в QML
    void newFrameReady(const QImage &image);

private:
    QCamera m_camera;
    QMediaCaptureSession m_session;
    QVideoSink m_videoSink;

    // Конвертация QImage в cv::Mat
    cv::Mat qImageToCvMat(const QImage &image)
    {
        cv::Mat mat;
        switch (image.format()) {
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, 
                         const_cast<uchar*>(image.bits()), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, 
                         const_cast<uchar*>(image.bits()), image.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR); // OpenCV использует BGR
            break;
        case QImage::Format_Grayscale8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, 
                         const_cast<uchar*>(image.bits()), image.bytesPerLine());
            break;
        default:
            // Конвертация в поддерживаемый формат
            QImage converted = image.convertToFormat(QImage::Format_ARGB32);
            mat = cv::Mat(converted.height(), converted.width(), CV_8UC4, 
                         const_cast<uchar*>(converted.bits()), converted.bytesPerLine());
            break;
        }
        return mat.clone(); // Возвращаем копию для безопасности
    }

    // Пример функции детекции объектов (заглушка)
    void detectObjects(cv::Mat &frame)
    {
        // Здесь можно использовать каскады Хаара или другие детекторы OpenCV
        // static cv::CascadeClassifier face_cascade;
        // face_cascade.load("haarcascade_frontalface_default.xml");
        // std::vector<cv::Rect> faces;
        // face_cascade.detectMultiScale(frame, faces);
        // for (const auto& face : faces) {
        //     cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        // }
    }

    // Пример функции запуска нейросети (заглушка)
    void runNeuralNetwork(cv::Mat &frame)
    {
        // Здесь можно использовать OpenCV DNN модуль или сторонние библиотеки
        // например, ONNX Runtime, TensorRT, PyTorch через libtorch
        // 
        // Пример с OpenCV DNN:
        // static cv::dnn::Net net = cv::dnn::readNetFromONNX("model.onnx");
        // cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0/255, cv::Size(416, 416));
        // net.setInput(blob);
        // cv::Mat detections = net.forward();
        // ... обработка результатов ...
    }
};
