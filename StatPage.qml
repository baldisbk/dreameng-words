import QtQuick 2.8
import QtQuick.Controls 2.1
import BarSeries 1.0

Page {
    id: page
    property string headCap: "header"
    property BarSeries bars
    Label {
	id: head
	text: page.headCap
	width: page.width
	height: page.height / 6
	anchors.top: parent.top
	font.pointSize: 30
	font.bold: true
	horizontalAlignment: Text.AlignHCenter
	verticalAlignment: Text.AlignVCenter
    }

    BarChart {
	id: chart
	width: page.width
	anchors.top: head.bottom
	anchors.bottom: parent.bottom
	bars: page.bars
    }
}
