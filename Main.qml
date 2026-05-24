import QtQuick
import QtQuick.Window
import QtMultimedia

Window {
    width: 640
    height: 480
    visible: true

    Camera {
        id: camera
        active: true
    }

    CaptureSession {
        camera: camera
        videoOutput: videoOutput // Вот здесь назначается видео выход
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }
}
