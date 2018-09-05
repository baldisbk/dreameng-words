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
					anchors.fill: parent
					text: dictionaries.dictList[index]
					font.pointSize: 30
					font.bold: true
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
				}
				MouseArea {
					anchors.fill: parent
					onClicked: {
						dictionaries.dict = dictionaries.dictList[index]
						dictionaries.accept()
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
		}
	}

	Column {
		property int childHeight: (height-spacing*(children.length-1))/children.length
		spacing: 20
		anchors.fill: parent
	Rectangle {
		id: first
		height: parent.childHeight
		width: parent.width
		radius: 10
		border {
			color: "black"
			width: 3
		}
		Label{
			anchors.fill: parent
			text: "Load file"
			font.pointSize: 30
			font.bold: true
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
		MouseArea {
			anchors.fill: parent
			onClicked: {
				fileopen.mode = 1
				fileopen.open()
			}
		}
	}
	Rectangle {
		id: second
		height: parent.childHeight
		width: parent.width
		radius: 10
		border {
			color: "black"
			width: 3
		}
		Label{
			anchors.fill: parent
			text: "Load stolen"
			font.pointSize: 30
			font.bold: true
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
		MouseArea {
			anchors.fill: parent
			onClicked: {
				fileopen.mode = 2
				fileopen.open()
			}
		}
	}
	Rectangle {
		id: third
		height: parent.childHeight
		width: parent.width
		radius: 10
		border {
			color: "black"
			width: 3
		}
		Label{
			anchors.fill: parent
			text: "Dictionaries"
			font.pointSize: 30
			font.bold: true
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
		MouseArea {
			anchors.fill: parent
			onClicked: {dictionaries.open()}
		}
	}
	}
}
