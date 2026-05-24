import QtQuick
import QtQuick.Window
import QtMultimedia

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Webcam Stream (Qt 6)")

    // CaptureSession связывает QVideoSink из C++ с визуальным компонентом
    CaptureSession {
        id: session
        videoSink: cameraCtrl.videoSink
        // videoOutput:
        videoOutput: videoOut
    }

    VideoOutput {
        id: videoOut
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit

        // Опционально: индикатор загрузки, пока нет кадров
        Rectangle {
            anchors.centerIn: parent
            width: 120; height: 30
            color: "black"
            opacity: videoOut.frameSize === Qt.size(0,0) ? 0.8 : 0
            Text {
                anchors.centerIn: parent
                color: "white"
                text: "Загрузка камеры..."
            }
        }
    }
}
