#ifndef BARSERIES_H
#define BARSERIES_H

#include <QObject>
#include <QVector>

class BarSeries : public QObject
{
	Q_OBJECT
public:
	explicit BarSeries(QObject *parent = nullptr);
	virtual ~BarSeries() {}

	Q_PROPERTY(int seriesNum READ seriesNum NOTIFY seriesNumChanged)
	Q_PROPERTY(double minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
	Q_PROPERTY(double maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
	Q_PROPERTY(int checksNum READ checksNum NOTIFY checksNumChanged)
	Q_PROPERTY(double start READ start WRITE setStart NOTIFY startChanged)
	Q_PROPERTY(double finish READ finish WRITE setFinish NOTIFY finishChanged)

	int seriesNum() const;
	double minimum() const;
	double maximum() const;
	int checksNum() const;
	double start() const;
	double finish() const;

	Q_INVOKABLE double checkAt(int num) const;
	Q_INVOKABLE double yCheckAt(int serie, int num) const;
	Q_INVOKABLE int serieSize(int serie) const;
	Q_INVOKABLE double x(int serie, int num) const;
	Q_INVOKABLE double y(int serie, int num) const;

	typedef QPair<double, double> Value;
	typedef QVector<Value> Serie;

	void clear();
	int addSerie(Serie serie);
	void delSerie(int num);
	void setChecks(QVector<double> checks);
	void adjust();

signals:
	void seriesNumChanged(int seriesNum);
	void minimumChanged(double minimum);
	void maximumChanged(double maximum);
	void checksNumChanged(int checksNum);
	void startChanged(double start);
	void finishChanged(double finish);

public slots:
	void setMinimum(double minimum);
	void setMaximum(double maximum);
	void setStart(double start);
	void setFinish(double finish);

private:
	double m_minimum;
	double m_maximum;
	double m_start;
	double m_finish;

	QVector<double> m_checks;
	QList<Serie> m_series;
};

#endif // BARSERIES_H
