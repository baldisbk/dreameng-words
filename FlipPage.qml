import QtQuick 2.0
import AppState 1.0

Item {
	property Item page
	property Item upper
	property Item lefter
	property Item righter
	property Item lower
	property int dir: AppState.Nowhere

	Flipable {
		id: flipable
		anchors.fill: parent
		front: Item {
			id: frontside
			anchors.fill: parent
		}
		back: Item {
			id: backside
			anchors.fill: parent
		}
		transform: Rotation {
			id: rotation
			origin.x: flipable.width/2
			origin.y: flipable.height/2
			axis.z: 0
			angle: 0
			Behavior on angle {
				id: rotbeh
				NumberAnimation {
					id: na
					duration: 500
				}
			}
		}
	}

	Connections {
		target: na
		onRunningChanged: {
			if (rotation.angle != 180 && rotation.angle != -180)
				return

			rotbeh.enabled = false
			page.visible = false
			switch (dir) {
			case AppState.Left:
				lefter.parent = frontside
				page = lefter
				break
			case AppState.Up:
				upper.parent = frontside
				page = upper
				break
			case AppState.Right:
				righter.parent = frontside
				page = righter
				break
			case AppState.Down:
				lower.parent = frontside
				page = lower
				break
			}
			rotation.angle = 0
			rotbeh.enabled = true
			done(dir)
			dir = 0
		}
	}

	MouseArea{
		property int pressX
		property int pressY
		property int angle
		property int axis: 0

		anchors.fill: parent

		function max(x,y) {return (x>y)?x:y}
		function abs(x) {return max(x, -x)}

		onPressed: {
			pressX = mouse.x
			pressY = mouse.y
			angle = rotation.angle
		}

		onPositionChanged: {
			var diffX = pressX - mouse.x
			var diffY = pressY - mouse.y
			var fraq = 0
			if (axis == 0) {
				if (max(abs(diffX), abs(diffY)) < 20)
					return
				if (abs(diffX)>abs(diffY)) {
					axis = -1
				}else{
					axis = 1
				}
			}
			rotbeh.enabled = false
			if (axis>0) {
				fraq = 90*diffY/height
			}else{
				fraq = -90*diffX/width
			}
			rotation.axis.x = (axis>0)?1:0
			rotation.axis.y = (axis<0)?1:0
			rotation.angle = angle + fraq
			rotbeh.enabled = true
		}
		onReleased: {
			if (axis == 0) {
				return
			}
			if (abs(rotation.angle)<30)
				rotation.angle = 0
			else if (rotation.angle>0) {
				rotation.angle = 180
				if (axis<0) {
					lefter.parent = backside
					lefter.visible = true
					parent.dir = AppState.Left
				} else {
					lower.parent = backside
					lower.visible = true
					parent.dir = AppState.Down
				}
			} else {
				rotation.angle = -180
				if (axis<0) {
					righter.parent = backside
					righter.visible = true
					parent.dir = AppState.Right
				} else {
					upper.parent = backside
					upper.visible = true
					parent.dir = AppState.Up
				}
			}
			axis = 0
		}
	}
	signal done(int dir)

	function init(c, l, r, t, b) {
		page = c
		lefter = l
		righter = r
		upper = t
		lower = b

		page.parent = frontside
		page.visible = true
	}
}
