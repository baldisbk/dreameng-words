import QtQuick 2.8
import QtQuick.Controls 2.1

Item {
    id: page
    property int s: width > height ? height : width
    property double coeff: 1.5

    Rectangle {
	    id: rectangle
	width: s / coeff
	height: s / coeff
	anchors.verticalCenter: parent.verticalCenter
	anchors.horizontalCenter: parent.horizontalCenter
	rotation: 45
	Rectangle {
		id: topR
	    width: parent.width / 2
	    height: parent.height / 2
	    color: "#0000ff"
	    anchors.top: parent.top
	    anchors.left: parent.left
	}
	Rectangle {
		id: leftR
	    width: parent.width / 2
	    height: parent.height / 2
	    color: "#ff0000"
	    anchors.bottom: parent.bottom
	    anchors.left: parent.left
	}
	Rectangle {
		id: bottomR
	    width: parent.width / 2
	    height: parent.height / 2
	    color: "#ffff00"
	    anchors.bottom: parent.bottom
	    anchors.right: parent.right
	}
	Rectangle {
		id: rightR
	    width: parent.width / 2
	    height: parent.height / 2
	    color: "#00ff00"
	    anchors.top: parent.top
	    anchors.right: parent.right
	}
    }
}
