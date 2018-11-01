#include "barseries.h"

#include <QDebug>

BarSeries::BarSeries(QObject *parent) : QObject(parent)
{
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
	while (s.at(index).first < pos) {
		++index;
		if (index >= s.size())
			return 0;
	}
	return s.at(index).second;
}

int BarSeries::serieSize(int serie) const
{
	if (serie < 0 || serie >= m_series.size())
		return 0;
	return m_series.at(serie).size();
}

double BarSeries::x(int serie, int num) const
{
	if (serie < 0 || serie >= m_series.size())
		return 0;
	auto s = m_series.at(serie);
	if (num < 0 || num >= s.size())
		return 0;
	return s.at(num).first;
}

double BarSeries::y(int serie, int num) const
{
	if (serie < 0 || serie >= m_series.size())
		return 0;
	auto s = m_series.at(serie);
	if (num < 0 || num >= s.size())
		return 0;
	return s.at(num).second;
}

void BarSeries::clear()
{
	m_series.clear();
	m_checks.clear();
	emit checksNumChanged(0);
	emit seriesNumChanged(0);
}

int BarSeries::addSerie(BarSeries::Serie serie)
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
		for(auto val: serie) {
			if (first || val.first < start) start = val.first;
			if (first || val.first > finish) finish = val.first;
			if (first || val.second < min) min = val.second;
			if (first || val.second > max) max = val.second;
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
	qWarning("Floating point comparison needs context sanity check");
	if (qFuzzyCompare(m_minimum, minimum))
		return;

	m_minimum = minimum;
	emit minimumChanged(m_minimum);
}

void BarSeries::setMaximum(double maximum)
{
	qWarning("Floating point comparison needs context sanity check");
	if (qFuzzyCompare(m_maximum, maximum))
		return;

	m_maximum = maximum;
	emit maximumChanged(m_maximum);
}

void BarSeries::setStart(double start)
{
	qWarning("Floating point comparison needs context sanity check");
	if (qFuzzyCompare(m_start, start))
		return;

	m_start = start;
	emit startChanged(m_start);
}

void BarSeries::setFinish(double finish)
{
	qWarning("Floating point comparison needs context sanity check");
	if (qFuzzyCompare(m_finish, finish))
		return;

	m_finish = finish;
	emit finishChanged(m_finish);
}

int Serie::size() const
{
	return m_serie.size();
}

double Serie::x(int num) const
{
	if (num < 0 || num >= m_serie.size())
		return 0;
	return m_serie.at(num).first;
}

double Serie::y(int num) const
{
	if (num < 0 || num >= m_serie.size())
		return 0;
	return m_serie.at(num).second;
}

QColor Serie::color() const
{
	return m_color;
}
