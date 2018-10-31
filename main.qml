import QtQuick 2.8
import QtQuick.Controls 2.1
import AppState 1.0
import PageState 1.0
import WordState 1.0
import HeadState 1.0
import StatState 1.0
import Settings 1.0

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Language")

	AppState {
		id: appState
		onSaveWords: db.saveWords()
	}
	Database {
		id: db
		state: appState
	}

	property Component mainFact: Qt.createComponent("MainForm.ui.qml")
	property Component wordFact: Qt.createComponent("WordForm.ui.qml")
	property Component headFact: Qt.createComponent("HeaderForm.ui.qml")
	property Component menuFact: Qt.createComponent("Menu.qml")
	property Component statFact: Qt.createComponent("StatPage.qml")

	function makePage(state){
		var obj
		switch (state.status) {
		case PageState.Learn:
		case PageState.Check:
		case PageState.Errors:
		case PageState.Train:
		case PageState.Repeat:
			obj = wordFact.createObject(flip, {"anchors.fill": flip, "visible": false})
			break
		case PageState.Header:
		case PageState.Footer:
			obj = headFact.createObject(flip, {"anchors.fill": flip, "visible": false})
			break
		case PageState.Main:
			obj = mainFact.createObject(flip, {"anchors.fill": flip, "visible": false})
			break
		case PageState.Menu:
			obj = menuFact.createObject(flip, {"anchors.fill": flip, "visible": false, "db": db})
			break
		case PageState.Statistic:
			obj = statFact.createObject(flip, {"anchors.fill": flip, "visible": false, "bars": state.series})
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
			break
		case PageState.Footer:
			page.headCap = qsTr("AGAIN?")
			page.smallCap = state.description
			break
		case PageState.Header:
			switch (state.otherState) {
			case PageState.Learn:
				page.headCap = qsTr("LEARN")
				page.smallCap = state.description
				break
			case PageState.Check:
				page.headCap = qsTr("CHECK")
				page.smallCap = state.description
				break
			case PageState.Errors:
				page.headCap = qsTr("ERRORS")
				page.smallCap = state.description
				break
			case PageState.Train:
				page.headCap = qsTr("TRAIN")
				page.smallCap = state.description
				break
			case PageState.Repeat:
				page.headCap = qsTr("REPEAT")
				page.smallCap = state.description
				break
			case PageState.Main:
				page.headCap = qsTr("COMPLETE")
				page.smallCap = state.description
				break
			case PageState.Footer:
				page.headCap = qsTr("AGAIN?")
				page.smallCap = state.description
				break
			default:
				page.headCap = qsTr("UNEXPECTED! ")+state.otherState
				page.smallCap = state.description
				break
			}
			break
		case PageState.Statistic:
			console.log(state.type)
			switch (state.type) {
			case StatState.Errors:
				page.headCap = qsTr("Errors")
				break
			case StatState.States:
				page.headCap = qsTr("State")
				break
			case StatState.Speed:
				page.headCap = qsTr("Speed")
				break
			default:
				page.headCap = qsTr("UNEXPECTED! ")+state.type
				break
			}
			break
		default:
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
		case PageState.Footer:
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
		case PageState.Statistic:
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
			db.saveState()
		}
	}

	Component.onCompleted: {
		db.init()
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
		flip.noLefter = appState.left.status == PageState.None
		flip.noRighter = appState.right.status == PageState.None
		flip.noUpper = appState.upper.status == PageState.None
		flip.noLower = appState.lower.status == PageState.None
	}
}
