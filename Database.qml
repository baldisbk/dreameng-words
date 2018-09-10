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
		db.readTransaction(_loadWords)
		loadState()
	}
	function dump() {db.readTransaction(_dump)}

	function saveState() {db.transaction(_storeState)}
	function loadState() {db.readTransaction(_loadState)}
	function saveWords() {db.transaction(_storeCurrent)}

	function fromdemo() {state.populateDemo(); db.transaction(_storeWords);saveState()}
	function fromfiles(path) {state.populateFile(path); db.transaction(_storeWords);saveState()}
	function fromstolen(path) {state.populateSteal(path); db.transaction(_storeWords);saveState()}

	function _dump(tx) {
		var i
		var st = tx.executeSql('SELECT * FROM StateV1')
		var w = tx.executeSql('SELECT * FROM WordsV1')
		console.log('======== Database ========')
		console.log('-------- State --------')
		for (i = 0; i < st.rows.length; ++i) {
			var str = ""
			for (var s in st.rows.item(i)) {
				str += s+"="+st.rows.item(i)[s]+", "
			}

			console.log(str)
		}
		console.log('-------- Words --------')
		for (i = 0; i < w.rows.length; ++i)
			console.log(w.rows.item(i))
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
		var fields = ""
		var ques = ""
		var values = []
		for (var val in st) {
			if (values.length != 0) {
				fields += ", "
				ques += ", "
			}

			fields += val
			ques += "?"
			values.push(st[val])
		}
		var exp = 'INSERT INTO StateV1('+fields+') VALUES ('+ques+')'
		tx.executeSql(exp, values)
	}

	function _storeCurrent(tx) {
		//console.log('======== Store current ========')
		var words = state.changedIndexes()
		for (var i=0; i<words.length; i++) {
			var word = state.changedContents(words[i])
			var fields = ""
			var values = []
			for (var val in word) {
				if (fields.length != 0) fields += ", "
				fields += val + " = ?"
				values.push(word[val])
			}
			values.push(words[i])
			var exp = 'UPDATE WordsV1 SET '+fields+' WHERE uid = ?'
			tx.executeSql(exp, values)
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
			var fields = "uid"
			var ques = "?"
			var values = [words[i]]
			for (var val in word) {
				fields += ", " + val
				ques += ", ?"
				values.push(word[val])
			}
			var exp = 'INSERT OR IGNORE INTO WordsV1('+fields+') VALUES ('+ques+')'
			tx.executeSql(exp, values)
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
			if (ver == 0)
				tx.executeSql('INSERT INTO Version VALUES (?)', [dbVer])
			else
				tx.executeSql('UPDATE Version SET version = ?', [dbVer])
			var stateFld = state.stateFields()
			var stateFldList = ""
			for (var sfld in stateFld)
				stateFldList += ", "+stateFld[sfld]+" TEXT"
			tx.executeSql(
				'CREATE TABLE StateV1('+
				'uid INTEGER PRIMARY KEY AUTOINCREMENT'+
				stateFldList+')')
			var wordFld = state.wordFields()
			var wordFldList = ""
			for (var wfld in wordFld)
				wordFldList += ", "+wordFld[wfld]+" TEXT"
			tx.executeSql(
				'CREATE TABLE WordsV1('+
				'uid INTEGER PRIMARY KEY AUTOINCREMENT'+
				wordFldList+')')

			// actually update...

			for (var i = ver; i<dbVer; ++i) {
				switch(i) {
				case 0:
					// insert into v1 select from v0, drop v0
					break;
				default: break;
				}
			}
		}
	}
}

