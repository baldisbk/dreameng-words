#ifndef BARSERIES_H
#define BARSERIES_H

#include <QObject>
#include <QVector>
#include <QColor>

class BarSerie : public QObject
{
	Q_OBJECT
public:
	typedef QPair<double, double> Value;
	typedef QVector<Value> Serie;

	explicit BarSerie(QObject *parent = nullptr) : QObject(parent), m_color(Qt::black) {}
	explicit BarSerie(Serie values, QObject *parent = nullptr) :
		QObject(parent), m_serie(values), m_color(Qt::black) {}
	virtual ~BarSerie() {}

	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
	Q_PROPERTY(int size READ size NOTIFY sizeChanged)

	Q_INVOKABLE double x(int num) const;
	Q_INVOKABLE double y(int num) const;

	int size() const;
	QColor color() const;

	void setValues(Serie values);

public slots:
	void setColor(QColor color);

signals:
	void colorChanged(QColor color);
	void sizeChanged(int size);

private:
	Serie m_serie;
	QColor m_color;
	int m_size;
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
	Q_INVOKABLE BarSerie* serie(int s);

	void clear();
	int addSerie(BarSerie *serie);
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
	QList<BarSerie*> m_series;
	GraphType m_graphType;
};

#endif // BARSERIES_H
