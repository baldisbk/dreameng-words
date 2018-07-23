import QtQuick 2.8
import QtQuick.Controls 2.1
import AppState 1.0
import PageState 1.0
import WordState 1.0
import StatState 1.0
import Settings 1.0

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Language")

	AppState {
		id: appState
	}

	property Component mainFact: Qt.createComponent("MainForm.ui.qml")
	property Component wordFact: Qt.createComponent("WordForm.ui.qml")
	property Component headFact: Qt.createComponent("HeaderForm.ui.qml")
	property Component menuFact: Qt.createComponent("Menu.qml")

	function makePage(state){
		var obj
		switch (state.status) {
		case PageState.Learn:
		case PageState.Check:
		case PageState.Errors:
		case PageState.Train:
		case PageState.Repeat:
			//console.log("create word")
			obj = wordFact.createObject(flip, {"anchors.fill": flip, "visible": false})
			break
		case PageState.Header:
			//console.log("create head")
			obj = headFact.createObject(flip, {"anchors.fill": flip, "visible": false})
			break
		case PageState.Main:
			//console.log("create main")
			obj = mainFact.createObject(flip, {"anchors.fill": flip, "visible": false})
			break
		case PageState.Menu:
			//console.log("create menu")
			obj = menuFact.createObject(flip, {"anchors.fill": flip, "visible": false})
			break
		case PageState.None:
			// fallthrough
		default:
			// this should never occur
			obj = Qt.createQmlObject(
				'import QtQuick 2.0; Item { anchors.fill: flip }', flip, "");
		}
		fillPage(state, obj)
		return obj
	}
	function fillPage(state, page) {
		switch (state.status) {
		case PageState.Learn:
		case PageState.Check:
		case PageState.Errors:
		case PageState.Train:
		case PageState.Repeat:
			page.word = state.word
			page.trans = state.translation
			page.transVisible = !state.wordOnly
			//console.log("fill word", state.status, page.word, page.trans, page.transVisible)
			break
		case PageState.Header:
			switch (state.otherState) {
			case PageState.Learn:
				page.headCap = "LEARN"
				page.smallCap = "smallCap"
				break
			case PageState.Check:
				page.headCap = "CHECK"
				page.smallCap = "smallCap"
				break
			case PageState.Errors:
				page.headCap = "ERRORS"
				page.smallCap = "smallCap"
				break
			case PageState.Train:
				page.headCap = "TRAIN"
				page.smallCap = "smallCap"
				break
			case PageState.Repeat:
				page.headCap = "REPEAT"
				page.smallCap = "smallCap"
				break
			case PageState.Main:
				page.headCap = "COMPLETE"
				page.smallCap = "smallCap"
				break
			default:
				page.headCap = "UNEXPECTED! "+state.otherState
				page.smallCap = "smallCap"
				break
			}
			//console.log("fill head", state.status, page.headCap, page.smallCap)
			break
		default:
			//console.log("fill main", state.status)
			break
		}
	}

	function flipColor(stateFrom, stateTo, dir) {
		if (stateTo.status === PageState.None)
			return "#00000000";
		switch (stateFrom.status) {
		case PageState.Main:
		case PageState.Check:
		case PageState.Repeat:
		case PageState.Train:
			switch(dir) {
			case AppState.Up: return "blue"
			case AppState.Down: return "yellow"
			case AppState.Left: return "red"
			case AppState.Right: return "#00ff00"
			default: break
			}
			break;
		case PageState.Menu:
			switch(dir) {
			case AppState.Up: return "blue"
			default: break
			}
			break;
		case PageState.Header:
		case PageState.Learn:
		case PageState.Errors:
			switch(dir) {
			case AppState.Up: return "blue"
			case AppState.Down: return "yellow"
			case AppState.Left: return "#00ff00"
			case AppState.Right: return "#00ff00"
			default: break
			}
			break;
		default: break
		}
		return "#00000000"
	}

	FlipPage {
		anchors.fill: parent
		id: flip

		onDone: {
			if (dir != AppState.Left)
				lefter.destroy()
			if (dir != AppState.Up)
				upper.destroy()
			if (dir != AppState.Right)
				righter.destroy()
			if (dir != AppState.Down)
				lower.destroy()

			appState.next(dir)
			fillPage(appState.page, page)
			lefter = makePage(appState.left)
			noLefter = appState.left.status == PageState.None
			leftColor = flipColor(appState.page, appState.left, AppState.Left)
			righter = makePage(appState.right)
			noRighter = appState.right.status == PageState.None
			rightColor = flipColor(appState.page, appState.right, AppState.Right)
			upper = makePage(appState.upper)
			noUpper = appState.upper.status == PageState.None
			upperColor = flipColor(appState.page, appState.upper, AppState.Up)
			lower = makePage(appState.lower)
			noLower = appState.lower.status == PageState.None
			lowerColor = flipColor(appState.page, appState.lower, AppState.Down)
		}
	}

	Component.onCompleted: {
		appState.populateDemo()
		flip.init(
			makePage(appState.page),
			makePage(appState.left),
			makePage(appState.right),
			makePage(appState.upper),
			makePage(appState.lower)
		)
		flip.leftColor = flipColor(appState.page, appState.left, AppState.Left)
		flip.rightColor = flipColor(appState.page, appState.right, AppState.Right)
		flip.upperColor = flipColor(appState.page, appState.upper, AppState.Up)
		flip.lowerColor = flipColor(appState.page, appState.lower, AppState.Down)
	}
}
