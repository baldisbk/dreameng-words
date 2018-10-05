import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

Dialog {
	property Database database
	width: parent.width
	height: parent.height
	standardButtons: Dialog.Ok | Dialog.Cancel
	contentItem: Column {
		Label {text: qsTr("Words in serie")}
		SpinBox {
			id: spin
			from: 1
			to: 50
			value: database.state.settings.seqLength
		}
	}
	onAccepted: database.state.settings.setSeqLength(spin.value)
}
