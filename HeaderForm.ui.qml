import QtQuick 2.8
import QtQuick.Controls 2.1

Page {
    id: page
    property string headCap: "header"
    property string smallCap: "caption"
    Label {
	    id: head
	text: page.headCap
	width: page.width
	height: page.height / 2
	anchors.verticalCenterOffset: -height / 2
	anchors.verticalCenter: parent.verticalCenter
	anchors.horizontalCenter: parent.horizontalCenter
	font.pointSize: 30
	font.bold: true
	horizontalAlignment: Text.AlignHCenter
	verticalAlignment: Text.AlignVCenter
    }
    Label {
	    id: cap
	text: page.smallCap
	width: page.width
	height: page.height / 2
	anchors.verticalCenterOffset: height / 2
	anchors.verticalCenter: parent.verticalCenter
	anchors.horizontalCenter: parent.horizontalCenter
	font.pointSize: 12
	horizontalAlignment: Text.AlignHCenter
	verticalAlignment: Text.AlignVCenter
    }
}
