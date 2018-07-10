import QtQuick 2.8
import QtQuick.Controls 2.1

Page {
    id: page
    property string word
    Label {
	    id: wordLabel
	text: word
	font.pointSize: 30
	font.bold: true
	horizontalAlignment: Text.AlignHCenter
	verticalAlignment: Text.AlignVCenter
	anchors.fill: parent
    }
}
