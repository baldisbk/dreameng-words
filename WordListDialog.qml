import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

Dialog {
	id: wordList
	property string dict
	property Database database
	property var wordList
	width: parent.width
	height: parent.height
	contentItem: ListView {
		id: wordView
		//anchors.fill: parent
		delegate: Rectangle {
			id: itemDelegate
			property var wordIndex: wordList.wordList[index]
			property var word: wordList.database.state.wordContents(wordIndex)
			property var prio: wordList.database.state.wordPriority(wordIndex)
			property int stat: wordList.database.state.wordState(wordIndex)
			property string description
			height: 100
			radius: 10
			border {
				color: "black"
				width: 3
			}
			width: parent.width
			color: (stat==1)?"green":(stat==2)?"red":(stat==4)?"blue":"gray"
			onWordChanged: {
				description = word.errors+"/"+word.repeats+" ("+prio+")"
			}
			Label{
				id: wordHead
				anchors {
					top: parent.top
					left: parent.left
					right: parent.right
				}
				height: 50
				text: itemDelegate.word.word
				font.pointSize: 30
				font.bold: true
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}
			Label{
				anchors {
					bottom: parent.bottom
					top: wordHead.bottom
					left: parent.left
					right: parent.right
				}
				text: itemDelegate.description
				font.pointSize: 15
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}
			MouseArea {
				anchors.fill: parent
				onClicked: {
					editDialog.wordId = wordIndex
					editDialog.wordContents = word
					editDialog.open()
				}
			}
		}
	}
	WordEditDialog {
		id: editDialog
		database: wordList.database
	}

	onVisibleChanged: {
		if (visible) {
			wordList.wordList = database.state.wordIndexes()
			wordView.model = wordList.wordList.length
		} else {
			wordView.model = 0
		}
	}
}
