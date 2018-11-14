import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

Dialog {
	id: dialog
	property Database database
	property int wordId
	property var wordContents
	property var wordFields: database.state.wordFields()
	width: parent.width
	height: parent.height
	standardButtons: Dialog.Ok | Dialog.Cancel
	contentItem: ListView {
		id: wordView
		delegate: Rectangle {
			id: itemDelegate
			property string pName: dialog.wordFields[index]
			property string pValue: wordContents[pName]
			height: 100
			radius: 10
			border {
				color: "black"
				width: 3
			}
			width: parent.width
			Label{
				id: propName
				anchors {
					bottom: parent.bottom
					top: parent.top
					left: parent.left
				}
				width: parent.width / 3
				text: parent.pName
				font.pointSize: 15
				font.bold: true
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}
			Rectangle {
				id: middleLine
				border {
					color: "black"
					width: 3
				}
				width: border.width*2
				anchors {
					top: parent.top
					bottom: parent.bottom
					left: propName.right
				}
			}
			Label {
				id: propValue
				anchors {
					bottom: parent.bottom
					top: parent.top
					right: parent.right
					left: middleLine.right
				}
				width: parent.width / 2 - parent.border.width
				text: parent.pValue
				font.pointSize: 15
				font.bold: true
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}
		}
	}
	onVisibleChanged: {
		if (visible) wordView.model = wordFields.length
		else wordView.model = 0

	}
	onAccepted: {
		database.state.setWord(wordId, wordContents)
	}
}
