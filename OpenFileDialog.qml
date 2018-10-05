import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

FileDialog {
	property int mode: 0
	property Database database
	folder: shortcuts.documents
	onAccepted: {
		var ind = fileUrl.toString().indexOf("://", 0)
		var filePath
		if (ind != -1)
			filePath = fileUrl.toString().slice(ind+3)
		else
			filePath = fileUrl.toString()
		switch (mode) {
		case 1: db.fromfiles(filePath); break
		case 2: db.fromstolen(filePath); break
		default: break;
		}
	}
}
