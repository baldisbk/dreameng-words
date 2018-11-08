#include "barseries.h"

#include <QDebug>

BarSeries::BarSeries(QObject *parent) : QObject(parent)
{
	setGraphType(Bars);
}

int BarSeries::seriesNum() const
{
	return m_series.size();
}

double BarSeries::minimum() const
{
	return m_minimum;
}

double BarSeries::maximum() const
{
	return m_maximum;
}

int BarSeries::checksNum() const
{
	return m_checks.size();
}

double BarSeries::checkAt(int num) const
{
	if (num >= 0 && num < m_checks.size())
		return m_checks.at(num);
	return 0;
}

double BarSeries::yCheckAt(int serie, int num) const
{
	if (num < 0 || num >= m_checks.size())
		return 0;
	if (serie < 0 || serie >= m_series.size())
		return 0;
	double pos = m_checks.at(num);
	int index = 0;
	auto s = m_series.at(serie);
	while (s->x(index) < pos) {
		++index;
		if (index >= s->size())
			return 0;
	}
	return s->y(index);
}

BarSerie *BarSeries::serie(int s)
{
	if (s < 0 || s >= m_series.size())
		return nullptr;
	return m_series.value(s);
}

void BarSeries::clear()
{
	m_series.clear();
	m_checks.clear();
	emit checksNumChanged(0);
	emit seriesNumChanged(0);
}

int BarSeries::addSerie(BarSerie *serie)
{
	m_series.append(serie);
	emit seriesNumChanged(m_series.size()-1);
	return m_series.size();
}

void BarSeries::delSerie(int num)
{
	m_series.removeAt(num);
	emit seriesNumChanged(m_series.size());
}

void BarSeries::setChecks(QVector<double> checks)
{
	m_checks = checks;
	emit checksNumChanged(checks.size());
}

void BarSeries::adjust()
{
	double start = 0;
	double finish = 0;
	double min = 0;
	double max = 0;
	bool first = true;
	for(auto serie: m_series)
		for(int i = 0; i < serie->size(); ++i) {
			if (first || serie->x(i) < start) start = serie->x(i);
			if (first || serie->x(i) > finish) finish = serie->x(i);
			if (first || serie->y(i) < min) min = serie->y(i);
			if (first || serie->y(i) > max) max = serie->y(i);
			first = false;
		}
	if (!m_checks.isEmpty()) {
		start = m_checks.front();
		finish = m_checks.last();
	}
	setMaximum(max);
	setMinimum(min);
	setStart(start);
	setFinish(finish);
}

BarSeries::GraphType BarSeries::graphType() const
{
	return m_graphType;
}

double BarSeries::start() const
{
	return m_start;
}

double BarSeries::finish() const
{
	return m_finish;
}

void BarSeries::setMinimum(double minimum)
{
	if (qFuzzyCompare(m_minimum, minimum))
		return;

	m_minimum = minimum;
	emit minimumChanged(m_minimum);
}

void BarSeries::setMaximum(double maximum)
{
	if (qFuzzyCompare(m_maximum, maximum))
		return;

	m_maximum = maximum;
	emit maximumChanged(m_maximum);
}

void BarSeries::setStart(double start)
{
	if (qFuzzyCompare(m_start, start))
		return;

	m_start = start;
	emit startChanged(m_start);
}

void BarSeries::setFinish(double finish)
{
	if (qFuzzyCompare(m_finish, finish))
		return;

	m_finish = finish;
	emit finishChanged(m_finish);
}

void BarSeries::setGraphType(BarSeries::GraphType graphType)
{
	if (m_graphType == graphType)
		return;

	m_graphType = graphType;
	emit graphTypeChanged(m_graphType);
}

int BarSerie::size() const
{
	return m_serie.size();
}

double BarSerie::x(int num) const
{
	if (num < 0 || num >= m_serie.size())
		return 0;
	return m_serie.at(num).first;
}

double BarSerie::y(int num) const
{
	if (num < 0 || num >= m_serie.size())
		return 0;
	return m_serie.at(num).second;
}

QColor BarSerie::color() const
{
	return m_color;
}

void BarSerie::setValues(BarSerie::Serie values)
{
	m_serie = values;
	emit sizeChanged(size());
}

void BarSerie::setColor(QColor color)
{
	if (m_color == color)
		return;

	m_color = color;
	emit colorChanged(m_color);
}
