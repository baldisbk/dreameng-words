import QtQuick 2.0
import AppState 1.0
import QtGraphicalEffects 1.0

Item {
	property Item page
	property Item upper
	property Item lefter
	property Item righter
	property Item lower

	property bool noUpper
	property bool noLefter
	property bool noRighter
	property bool noLower

	property color leftColor: "red"
	property color rightColor: "#00ff00"
	property color upperColor: "blue"
	property color lowerColor: "yellow"

	property int dir: AppState.Nowhere

	function max(x,y) {return (x>y)?x:y}
	function abs(x) {return max(x, -x)}
	function sign(x) {return x>0?1:(x<0?-1:0)}

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

	RadialGradient {
		id: sideGrad
		property color light:
		    rotation.axis.x == 0 ? (
			sign(rotation.angle) === 1 ?
			    leftColor :
			    rightColor): (
			sign(-rotation.angle) === 1 ?
			    upperColor :
			    lowerColor)
		property double tilt: (90 - abs(abs(rotation.angle) - 90))/90
		anchors.fill: parent
		horizontalOffset: rotation.axis.y * width / 2 * sign(-rotation.angle)
		verticalOffset: rotation.axis.x * height / 2 * sign(rotation.angle)
		horizontalRadius: width / 2 * (rotation.axis.x + tilt)
		verticalRadius: height / 2 * (rotation.axis.y + tilt)
		gradient: Gradient {
			GradientStop { position: 1.0; color: "#00000000" }
			GradientStop { position: 0.0; color: sideGrad.light }
		}
		opacity: tilt
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
		propagateComposedEvents: true

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
			var maxfraq = 90
			if (axis>0) {
				if ((diffY > 0 && noLower) || (diffY < 0 && noUpper))
					maxfraq = 40
				fraq = maxfraq*diffY/height
			}else{
				if ((diffX > 0 && noRighter) || (diffX < 0 && noLefter))
					maxfraq = 40
				fraq = -maxfraq*diffX/width
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
				if (axis<0) {
					if (noLefter) {
						rotation.angle = 0
					} else {
						rotation.angle = 180
						lefter.parent = backside
						lefter.visible = true
						parent.dir = AppState.Left
					}
				} else {
					if (noLower) {
						rotation.angle = 0
					} else {
						rotation.angle = 180
						lower.parent = backside
						lower.visible = true
						parent.dir = AppState.Down
					}
				}
			} else {
				if (axis<0) {
					if (noRighter) {
						rotation.angle = 0
					} else {
						rotation.angle = -180
						righter.parent = backside
						righter.visible = true
						parent.dir = AppState.Right
					}
				} else {
					if (noUpper) {
						rotation.angle = 0
					} else {
						rotation.angle = -180
						upper.parent = backside
						upper.visible = true
						parent.dir = AppState.Up
					}
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
