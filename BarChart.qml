import QtQuick 2.0
import BarSeries 1.0

Canvas {
	property int frameWidth: (height>width)?(width/10):(height/10)
	property double gapPercent: 0.2
	property BarSeries bars
	onPaint: {
		var ctx = getContext("2d");
		ctx.strokeStyle = "black"
		ctx.lineWidth = 1
		ctx.beginPath()
		ctx.moveTo(frameWidth, frameWidth)
		ctx.lineTo(frameWidth, height - frameWidth)
		ctx.lineTo(width - frameWidth, height - frameWidth)
		ctx.stroke()
		ctx.closePath()

		switch (bars.graphType) {
		case BarSeries.Bars:
			drawBars(ctx)
			break
		case BarSeries.Graph:
			drawGraph(ctx)
			break
		}

		ctx.restore()
	}
	function drawBars(ctx) {
		var serieNum = bars.seriesNum
		var checkNum = bars.checksNum
		var bottomLine = height - frameWidth
		var visH = height - frameWidth*2
		var totalH = bars.maximum
		if (totalH == 0) totalH = visH
		var hCoeff = visH/totalH
		if (serieNum*checkNum == 0) return
		var barWidth = (width - 2*frameWidth) / (gapPercent*checkNum + gapPercent + serieNum*checkNum);
		var pos = barWidth*gapPercent + frameWidth;

		for (var i=0; i<checkNum; ++i) {
			for (var j=0; j<serieNum; ++j) {
				ctx.fillStyle = "green"
				var h = bars.yCheckAt(j, i)*hCoeff
				ctx.fillRect(pos, bottomLine - h, barWidth, h)
				pos += barWidth
			}
			pos += barWidth*gapPercent
		}
	}
	function drawGraph(ctx) {
		var serieNum = bars.seriesNum
		var bottomLine = height - frameWidth

		var visH = height - frameWidth*2
		var totalH = bars.maximum - bars.minimum
		if (totalH == 0) totalH = visH
		var hCoeff = visH/totalH

		var visW = width - frameWidth*2
		var totalW = bars.finish - bars.start
		if (totalW == 0) totalW = visW
		var vCoeff = visW/totalW

		if (serieNum == 0) return

		for (var j=0; j<serieNum; ++j) {
			var first = true
			ctx.beginPath()
			if (j==0)
				ctx.strokeStyle = "green"
			else
				ctx.strokeStyle = "red"
			for (var i=0; i<bars.serieSize(j); ++i) {
				if (first)
					ctx.moveTo(
						frameWidth + (bars.x(j,i)-bars.start)*vCoeff,
						bottomLine - (bars.y(j,i)-bars.minimum)*hCoeff)
				else
					ctx.lineTo(
						frameWidth + (bars.x(j,i)-bars.start)*vCoeff,
						bottomLine - (bars.y(j,i)-bars.minimum)*hCoeff)
				first = false
			}
			ctx.stroke()
			ctx.closePath()
		}
	}
}
