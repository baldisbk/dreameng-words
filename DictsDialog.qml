import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

Dialog {
	property string dict
	property var dictList
	property Database database
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
						database.rmDict(dictionaries.dictList[index])
						dictionaries.dictList = database.state.dictionaries()
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
		if (visible) dictList = database.state.dictionaries()
		dictView.model = dictList.length
	}
	onAccepted: {
		database.state.setDictionary(dict)
		database.saveState()
	}
}
