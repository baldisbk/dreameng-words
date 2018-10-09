import QtQuick 2.8
import QtQuick.Controls 2.1

Page {
    id: page
    property string headCap: "header"
    property Database database
    Label {
	id: head
	text: page.headCap
	width: page.width
	height: page.height / 4
	anchors.top: parent.top
	font.pointSize: 30
	font.bold: true
	horizontalAlignment: Text.AlignHCenter
	verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
	id: contents
	width: page.width
	height: page.height * 3 / 4
	anchors.bottom: parent.bottom
	color: "green"
    }
}
