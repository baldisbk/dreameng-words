import QtQuick 2.8
import QtQuick.Controls 2.1

Page {
	id: page
    width: 600
    height: 400

    header: Label {
	    text: qsTr("Main Menu")
	    font.pixelSize: Qt.application.font.pixelSize * 2
	    padding: 10
    }

    Rectangle {
	    id: rectangle
	    x: 242
	    width: 100
	    height: 100
	    color: "#0000ff"
	    anchors.horizontalCenter: parent.horizontalCenter
	    anchors.top: parent.top
	    anchors.topMargin: 2
	    rotation: 45
    }

    Rectangle {
	    id: rectangle1
	x: 172
	y: 219
	width: 100
	height: 100
	color: "#ff0000"
	rotation: 45
	anchors.horizontalCenter: parent.horizontalCenter
	anchors.bottom: parent.bottom
 anchors.bottomMargin: 30
    }
}
