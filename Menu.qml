import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

Item {
	property Database db
	FileDialog {
		signal ready
		id: fileopen
		folder: shortcuts.documents
		onAccepted: {
			var ind = fileUrl.toString().indexOf("://", 0)
			var filePath
			if (ind != -1)
				filePath = fileUrl.toString().slice(ind+3)
			else
				filePath = fileUrl.toString()
			db.fromfiles(filePath)
		}
	}
	Rectangle {
		id: first
		height: parent.height/3
		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
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
				fileopen.open()
			}
		}
	}
	Rectangle {
		id: second
		height: parent.height/3
		color: "#3f3f00"
		anchors {
			top: first.bottom
			left: parent.left
			right: parent.right
		}
		MouseArea {
			anchors.fill: parent
			onClicked: db.saveState()
		}
	}
	Rectangle {
		id: third
		height: parent.height/3
		color: "#003f3f"
		anchors {
			top: second.bottom
			left: parent.left
			right: parent.right
		}
		MouseArea {
			anchors.fill: parent
			onClicked: db.fromdemo()
		}
	}
}
