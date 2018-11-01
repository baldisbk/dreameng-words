#ifndef BARSERIES_H
#define BARSERIES_H

#include <QObject>
#include <QVector>
#include <QColor>

class Serie : public QObject
{
	Q_OBJECT
public:
	explicit Serie(QObject *parent = nullptr) : QObject(parent) {}
	virtual ~Serie() {}

	Q_PROPERTY(QColor color READ color)

	Q_INVOKABLE int size() const;
	Q_INVOKABLE double x(int num) const;
	Q_INVOKABLE double y(int num) const;

	typedef QPair<double, double> Value;

	QColor color() const;

private:
	QVector<Value> m_serie;
	QColor m_color;
};

class BarSeries : public QObject
{
	Q_OBJECT
public:
	enum GraphType {
		Bars = 0,
		Graph
	};
	Q_ENUM(GraphType)

	explicit BarSeries(QObject *parent = nullptr);
	virtual ~BarSeries() {}

	Q_PROPERTY(int seriesNum READ seriesNum NOTIFY seriesNumChanged)
	Q_PROPERTY(double minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
	Q_PROPERTY(double maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
	Q_PROPERTY(int checksNum READ checksNum NOTIFY checksNumChanged)
	Q_PROPERTY(double start READ start WRITE setStart NOTIFY startChanged)
	Q_PROPERTY(double finish READ finish WRITE setFinish NOTIFY finishChanged)
	Q_PROPERTY(GraphType graphType READ graphType WRITE setGraphType NOTIFY graphTypeChanged)

	int seriesNum() const;
	double minimum() const;
	double maximum() const;
	int checksNum() const;
	double start() const;
	double finish() const;
	GraphType graphType() const;

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
	void graphTypeChanged(GraphType graphType);

public slots:
	void setMinimum(double minimum);
	void setMaximum(double maximum);
	void setStart(double start);
	void setFinish(double finish);
	void setGraphType(GraphType graphType);

private:
	double m_minimum;
	double m_maximum;
	double m_start;
	double m_finish;

	QVector<double> m_checks;
	QList<Serie> m_series;
	GraphType m_graphType;
};

#endif // BARSERIES_H
