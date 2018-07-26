import QtQuick 2.0

Item {
	property Database db
	Rectangle{
		id: first
		height: parent.height/3
		color: "#3f003f"
		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
		}
		MouseArea {
			anchors.fill: parent
			onClicked: db.dump()
		}
	}
	Rectangle{
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
	Rectangle{
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
