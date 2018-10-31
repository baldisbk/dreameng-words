import QtQuick 2.0
import BarSeries 1.0

Canvas {
	property int frameWidth: (height>width)?(width/10):(height/10)
	property double gapPercent: 0.2
	property BarSeries bars
	onPaint: {
		var ctx = getContext("2d");
		ctx.strokeStyle = Qt.rgba(0, 0, 0, 1)
		ctx.lineWidth = 1
		ctx.beginPath()
		ctx.moveTo(frameWidth, frameWidth)
		ctx.lineTo(frameWidth, height - frameWidth)
		ctx.lineTo(width - frameWidth, height - frameWidth)
		ctx.stroke()

		var serieNum = bars.seriesNum
		var checkNum = bars.checksNum
		var bottomLine = height - frameWidth
		if (serieNum*checkNum == 0) return
		var barWidth = (width - 2*frameWidth) / (gapPercent*checkNum + gapPercent + serieNum*checkNum);
		var pos = barWidth*gapPercent + frameWidth;

		for (var i=0; i<checkNum; ++i) {
			for (var j=0; j<serieNum; ++j) {
				ctx.fillStyle = Qt.rgba(1, 0, 0, 1)
				var h = bars.yCheckAt(j, i)
				ctx.fillRect(pos, bottomLine - h, barWidth, h)
				pos += barWidth
			}
			pos += barWidth*gapPercent
		}
	}
}
