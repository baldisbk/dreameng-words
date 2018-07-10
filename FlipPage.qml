import QtQuick 2.0

Item {
	property Item page
	property Item upper
	property Item lefter
	property Item righter
	property Item lower
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

	property int dir: 0
	// 0 - init
	// 1 - left
	// 2 - top
	// 3 - right
	// 4 - bottom
	Connections {
		target: na
		onRunningChanged: {
			if (rotation.angle != 180 && rotation.angle != -180)
				return

			rotbeh.enabled = false
			page.visible = false
			switch (dir) {
			case 1:
				lefter.parent = frontside
				page = lefter
				break
			case 2:
				upper.parent = frontside
				page = upper
				break
			case 3:
				righter.parent = frontside
				page = righter
				break
			case 4:
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
		onPressed: {
			pressX = mouse.x
			pressY = mouse.y
			angle = rotation.angle
		}
		function max(x,y) {return (x>y)?x:y}
		function abs(x) {return max(x, -x)}

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
			if (abs(rotation.angle)<45)
				rotation.angle = 0
			else if (rotation.angle>0) {
				rotation.angle = 180
				if (axis<0) {
					lefter.parent = backside
					lefter.visible = true
					parent.dir = 1
				} else {
					lower.parent = backside
					lower.visible = true
					parent.dir = 4
				}
			} else {
				rotation.angle = -180
				if (axis<0) {
					righter.parent = backside
					righter.visible = true
					parent.dir = 3
				} else {
					upper.parent = backside
					upper.visible = true
					parent.dir = 2
				}
			}
			axis = 0
		}
	}
	Component.onCompleted: {
		done(0)
		page.parent = frontside
	}
	signal done(int dir)
}
