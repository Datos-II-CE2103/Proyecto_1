import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

// This must match the uri and version
// specified in the qml_module in the build.rs script.
import com.kdab.cxx_qt.demo 1.0

Window {
    height: 480
    title: qsTr("CMP - CLIENTE")
    visible: true
    visibility: "Maximized"
    width: 640
    color:"#363D4A"

    MyObject {
        id: myObject
        number: 1
        string: qsTr("My String with my number: %1").arg(myObject.number)
    }

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Rectangle {
            anchors.centerIn: parent
            id: page
            width: 320; height: 200
            color: "#FFFFFF"
            radius: 5

            Column{
                anchors.centerIn: parent
                anchors.margins: 10
                spacing: 20

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: helloText
                    text: "No hay conexión"
                    y: 20
                    font.pointSize: 24; font.bold: true
                    color: "#000000"
                }
                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Reintentar"
                    width: 100
                    height: 50

                    onClicked: myObject.sayHi(myObject.string, myObject.number)
                    background: Rectangle {
                        width: 100
                        height: 50
                        color: "#0C9C4C"
                        border.width: 0
                        radius: 5
                    }
                }
            }
        }

    }
}
