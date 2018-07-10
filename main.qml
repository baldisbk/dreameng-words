import QtQuick 2.8
import QtQuick.Controls 2.1

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Tabs")

	FlipPage {
		anchors.fill: parent
		id: flip

		property int ln: 0
		property int rn: 0
		property int tn: 0
		property int bn: 0

		property Component menuFact: Qt.createComponent("MainForm.ui.qml")
		property Component wordFact: Qt.createComponent("WordForm.ui.qml")

		function fill(state, dir){
			switch (dir) {
			case 0: page = menuFact.createObject(flip, {"anchors.fill": flip}); break
			case 1: ln++; break
			case 2: tn++; break
			case 3: rn++; break
			case 4: bn++; break
			}
			lefter = wordFact.createObject(flip, {
				"visible": false,
				"anchors.fill": flip,
				"word": "left"+ln
			})
			upper = wordFact.createObject(flip, {
				"visible": false,
				"anchors.fill": flip,
				"word": "up"+tn
			})
			righter = wordFact.createObject(flip, {
				"visible": false,
				"anchors.fill": flip,
				"word": "right"+rn
			})
			lower = wordFact.createObject(flip, {
				"visible": false,
				"anchors.fill": flip,
				"word": "bottom"+bn
			})
		}
		onDone: {
			if (dir!=0) {
				if (dir!=1) lefter.destroy()
				if (dir!=2) upper.destroy()
				if (dir!=3) righter.destroy()
				if (dir!=4) lower.destroy()
			}
			fill(0, dir)
		}
	}
}
