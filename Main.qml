import QtQuick
import QtQuick.Window
import QtMultimedia

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Webcam Stream (Qt 6)")

    // CaptureSession управляет камерой и передаёт кадр в VideoOutput для отображения
    CaptureSession {
        id: session
        camera: cameraCtrl.camera
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
        
        // Пример наложения графики поверх видео (индикация обнаруженных объектов)
        // Координаты и размеры можно получать из C++ через сигналы
        Rectangle {
            // Этот прямоугольник будет виден поверх видео
            // Его параметры можно привязать к свойствам из cameraCtrl
            x: 50; y: 50
            width: 150; height: 150
            color: "transparent"
            border.color: "red"
            border.width: 3
            visible: false // Включить, когда есть обнаруженные объекты
            
            Text {
                anchors.top: parent.bottom
                color: "red"
                text: "Object detected"
            }
        }
    }
    
    // Соединение для получения обработанных данных из C++
    Connections {
        target: cameraCtrl
        function onFrameProcessed(data) {
            // Обработка данных от C++ (например, координаты объектов)
            console.log("Frame processed:", data)
        }
    }
}
