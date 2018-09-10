import QtQuick 2.0
import QtQuick.Controls 2.3

Rectangle {
	property string caption
	signal clicked
	radius: 10
	border {
		color: "black"
		width: 3
	}
	Label{
		anchors.fill: parent
		text: parent.caption
		font.pointSize: 30
		font.bold: true
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}
	MouseArea {
		anchors.fill: parent
		onClicked: parent.clicked()
	}
}
