import QtQuick 2.8
import QtQuick.Controls 2.1

Page {
    id: page
    property string word: "Word"
    property string trans: "Translation"
    property bool transVisible: true

    Label {
	    id: wordLabel
	text: page.word
	width: page.width
	height: page.height/2
	anchors.verticalCenterOffset: page.transVisible?-height/2:0
	anchors.verticalCenter: parent.verticalCenter
	anchors.horizontalCenter: parent.horizontalCenter
	font.pointSize: 30
	font.bold: true
	horizontalAlignment: Text.AlignHCenter
	verticalAlignment: Text.AlignVCenter
    }

    Label {
	    id: translLabel
	text: page.trans
	visible: page.transVisible
	width: page.width
	height: page.height/2
	anchors.verticalCenterOffset: height/2
	anchors.verticalCenter: parent.verticalCenter
	font.pointSize: 30
	font.italic: true
	verticalAlignment: Text.AlignVCenter
	horizontalAlignment: Text.AlignHCenter
	anchors.horizontalCenter: parent.horizontalCenter
    }
}
