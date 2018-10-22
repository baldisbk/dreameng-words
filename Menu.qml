import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

Item {
	property Database db
	OpenFileDialog {
		id: fileopen
		database: db
	}

	DictsDialog {
		id: dictionaries
		database: db
	}

	SettingDialog{
		id: settings
		database: db
	}

	Column {
		property int childHeight: (height-spacing*(children.length-1))/children.length
		spacing: 20
		anchors.fill: parent
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: qsTr("Load file")
			onClicked: {
				fileopen.mode = 1
				fileopen.open()
			}
		}
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: qsTr("Load stolen")
			onClicked: {
				fileopen.mode = 2
				fileopen.open()
			}
		}
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: qsTr("Dictionaries")
			onClicked: {dictionaries.open()}
		}
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: qsTr("Settings")
			onClicked: {settings.open()}
		}
		MenuButton {
			width: parent.width
			height: parent.childHeight
			caption: qsTr("Debug dump")
			onClicked: {
				fileopen.mode = 3
				fileopen.open()
			}
		}
	}
}
