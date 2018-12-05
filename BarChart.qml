import QtQuick 2.0
import BarSeries 1.0

Canvas {
	property int screenSize: (height>width)?width:height
	property int frameWidth: screenSize/10
	property double gapPercent: 0.2
	property int fontSize: screenSize/40
	property double fontAdd: screenSize/80
	property double lineWidth: screenSize/100
	property BarSeries bars
	function format(num) {return num}
	onPaint: {
		var ctx = getContext("2d");
		ctx.strokeStyle = "black"
		ctx.lineWidth = lineWidth
		ctx.font = fontSize+"px sans-serif"

		ctx.beginPath()
		ctx.moveTo(frameWidth, frameWidth)
		ctx.lineTo(frameWidth, height - frameWidth)
		ctx.lineTo(width - frameWidth, height - frameWidth)
		ctx.stroke()
		ctx.closePath()

		ctx.lineWidth = 1
		ctx.beginPath()
		ctx.text(format(bars.maximum),
			 frameWidth - ctx.measureText(bars.maximum).width - fontAdd,
			 frameWidth + fontSize)
		ctx.text(format(bars.minimum),
			 frameWidth - ctx.measureText(bars.minimum).width - fontAdd,
			 height - frameWidth)

		ctx.text(format(bars.start),
			 frameWidth,
			 height - frameWidth + fontSize + fontAdd)
		ctx.text(format(bars.finish),
			 width - frameWidth - ctx.measureText(bars.finish).width,
			 height - frameWidth + fontSize + fontAdd)
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
				ctx.fillStyle = bars.serie(j).color
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

		ctx.lineWidth = lineWidth

		for (var j=0; j<serieNum; ++j) {
			var first = true
			ctx.beginPath()
			var serie = bars.serie(j)
			ctx.strokeStyle = serie.color
			for (var i=0; i<serie.size; ++i) {
				if (first)
					ctx.moveTo(
						frameWidth + (serie.x(i)-bars.start)*vCoeff,
						bottomLine - (serie.y(i)-bars.minimum)*hCoeff)
				else
					ctx.lineTo(
						frameWidth + (serie.x(i)-bars.start)*vCoeff,
						bottomLine - (serie.y(i)-bars.minimum)*hCoeff)
				first = false
			}
			ctx.stroke()
			ctx.closePath()
		}
	}
}
