import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import AppState 1.0

QtObject {
	property AppState state

	property var db
	property int dbVer: 1

	function init() {
		db = LocalStorage.openDatabaseSync("TmpDB", "1.0", "", 0);
		db.transaction(_makedb)
		dump()
		db.readTransaction(_loadWords)
		loadState()
	}
	function dump() {db.readTransaction(_dump)}

	function saveState() {db.transaction(_storeState)}
	function loadState() {db.readTransaction(_loadState)}
	function saveWords() {db.transaction(_storeCurrent)}

	function fromdemo() {state.populateDemo(); db.transaction(_storeWords)}
	function fromfiles(path) {state.populateFile(path); db.transaction(_storeWords); dump()}
	function fromstolen(path) {state.populateFile(path); db.transaction(_storeWords); dump()}

	function _dump(tx) {
		var i
		var st = tx.executeSql('SELECT * FROM StateV1')
		var w = tx.executeSql('SELECT * FROM WordsV1')
		console.log('======== Database ========')
		console.log('-------- State --------')
		for (i = 0; i < st.rows.length; ++i)
			console.log(
				st.rows.item(i).state,
				st.rows.item(i).statectx,
				st.rows.item(i).changed,
				st.rows.item(i).selected,
				st.rows.item(i).errors,
				st.rows.item(i).current)
		console.log('-------- Words --------')
		for (i = 0; i < w.rows.length; ++i)
			console.log(
				w.rows.item(i).uid,
				w.rows.item(i).word,
				w.rows.item(i).translation,
				w.rows.item(i).repeats,
				w.rows.item(i).errors,
				w.rows.item(i).last,
				w.rows.item(i).age,
				w.rows.item(i).speed)
		console.log('======== End dump ========')
	}

	function _query(tx) {
		console.log('======== Query ========')
		var cust = tx.executeSql(query.text)
		for (i = 0; i < cust.rows.length; ++i) {
			console.log('~~~~~~~~')
			for (var prop in cust.rows.item(i))
				console.log("    ", prop, "=", cust.rows.item(i)[prop])
		}
	}

	function _storeState(tx) {
		//console.log('======== Store state ========')
		tx.executeSql('DELETE FROM StateV1')
		var st = state.stateContents()
		tx.executeSql(
			'INSERT INTO StateV1('+
				'state, statectx, changed, selected, errors, current) '+
			'VALUES (?, ?, ?, ?, ?, ?)',
			[st.state, st.statectx, st.changed, st.selected, st.errors, st.current])
	}

	function _storeCurrent(tx) {
		//console.log('======== Store current ========')
		var words = state.changedIndexes()
		for (var i=0; i<words.length; i++) {
			var word = state.changedContents(words[i])
			tx.executeSql(
				'UPDATE WordsV1 SET '+
					'word = ?, translation = ?, '+
					'repeats = ?, errors = ?, '+
					'last = ?, age = ?, speed = ? '+
				'WHERE uid = ?',
				[word.word, word.translation,
				 word.repeats, word.errors,
				 word.last, word.age, word.speed,
				 words[i]])
		}
	}

	function _loadState(tx) {
		//console.log('======== Load state ========')
		var res = tx.executeSql('SELECT * FROM StateV1')
		if (res.rows.length > 0)
			state.loadState(res.rows.item(0))
		else
			state.init()
	}

	function _storeWords(tx) {
		//console.log('======== Store words ========')
		var words = state.wordIndexes()
		for (var i=0; i<words.length; i++) {
			var word = state.wordContents(words[i])
			tx.executeSql(
				'INSERT INTO WordsV1('+
					'uid, word, translation, '+
					'repeats, errors, last, age, speed) '+
				'VALUES (?, ?, ?, ?, ?, ?, ?, ?)',
				[words[i], word.word, word.translation,
				 word.repeats, word.errors, word.last, word.age, word.speed])
		}
	}

	function _loadWords(tx) {
		//console.log('======== Load words ========')
		var res = tx.executeSql('SELECT * FROM WordsV1')
		for (var i = 0; i < res.rows.length; ++i) {
			state.addWord(res.rows.item(i));
		}
	}

	function _makedb(tx) {
		tx.executeSql(
			'CREATE TABLE IF NOT EXISTS Version(version INTEGER)')
		var res = tx.executeSql('SELECT version FROM Version')
		var ver = 0
		if (res.rows.length !== 0)
			ver = res.rows.item(0).version
		if (ver != dbVer) {
			tx.executeSql('INSERT INTO Version VALUES (?)', [dbVer])
			tx.executeSql(
				'CREATE TABLE StateV1('+
				'uid INTEGER PRIMARY KEY AUTOINCREMENT,'+
				'state INT,'+
				'statectx STRING,'+
				'changed TEXT,'+
				'selected TEXT,'+
				'errors TEXT,'+
				'current INT'+
				')')
			tx.executeSql(
				'CREATE TABLE WordsV1('+
				'uid INTEGER PRIMARY KEY AUTOINCREMENT,'+
				'word TEXT,'+
				'translation TEXT,'+
				'repeats INT,'+
				'errors INT,'+
				'last INT,'+
				'age INT,'+
				'speed INT'+
				')')

			// actually update...

			switch(ver) {
			case 0:
				break;
			default:;
			}
		}
	}
}

