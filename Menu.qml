import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

Item {
	property Database db
	FileDialog {
		property int mode: 0
		id: fileopen
		folder: shortcuts.documents
		onAccepted: {
			var ind = fileUrl.toString().indexOf("://", 0)
			var filePath
			if (ind != -1)
				filePath = fileUrl.toString().slice(ind+3)
			else
				filePath = fileUrl.toString()
			switch (mode) {
			case 1: db.fromfiles(filePath); break
			case 2: db.fromstolen(filePath); break
			default: break;
			}
		}
	}
	Dialog {
		property string dict
		property var dictList
		id: dictionaries
		width: parent.width
		height: parent.height
		contentItem: ListView {
			id: dictView
			width: parent.width
			height: parent.height
			delegate: Rectangle {
				id: itemDelegate
				height: 100
				radius: 10
				border {
					color: "black"
					width: 3
				}
				width: parent.width
				Label{
					anchors {
						bottom: parent.bottom
						top: parent.top
						right: parent.right
					}
					id: dellabel
					width: height
					text: "X"
					font.pointSize: 30
					font.bold: true
					color: "red"
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					MouseArea {
						anchors.fill: parent
						onClicked: {
							db.rmDict(dictionaries.dictList[index])
							dictionaries.dictList = db.state.dictionaries()
							dictView.model = dictionaries.dictList.length
						}
					}
				}
				Label{
					anchors {
						bottom: parent.bottom
						top: parent.top
						left: parent.left
						right: dellabel.left
					}
					text: dictionaries.dictList[index]
					font.pointSize: 30
					font.bold: true
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					MouseArea {
						anchors.fill: parent
						onClicked: {
							dictionaries.dict = dictionaries.dictList[index]
							dictionaries.accept()
						}
					}
				}
			}
		}
		onVisibleChanged: {
			if (visible) dictList = db.state.dictionaries()
			dictView.model = dictList.length
		}
		onAccepted: {
			db.state.setDictionary(dict)
			db.saveState()
		}
	}

	Dialog {
		id: settings
		width: parent.width
		height: parent.height
		standardButtons: Dialog.Ok | Dialog.Cancel
		contentItem: Column {
			Label {text: "Words in serie"}
			SpinBox {
				id: spin
				from: 1
				to: 50
				value: db.state.settings.seqLength
			}
		}
		onAccepted: db.state.settings.setSeqLength(spin.value)
	}

	Column {
		property int childHeight: (height-spacing*(children.length-1))/children.length
		spacing: 20
		anchors.fill: parent
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: "Load file"
			onClicked: {
				fileopen.mode = 1
				fileopen.open()
			}
		}
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: "Load stolen"
			onClicked: {
				fileopen.mode = 2
				fileopen.open()
			}
		}
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: "Dictionaries"
			onClicked: {dictionaries.open()}
		}
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: "Settings"
			onClicked: {settings.open()}
		}
	}
}
