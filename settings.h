#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
	Q_OBJECT
public:
	explicit Settings(QObject *parent = nullptr);

	Q_PROPERTY(int seqLength READ seqLength WRITE setSeqLength NOTIFY seqLengthChanged)

	int seqLength() const;
	QString store() const;

signals:
	void seqLengthChanged(int seqLength);

public slots:
	void setSeqLength(int seqLength);
	void load(QString str);

private:
	int m_seqLength;
};

#endif // SETTINGS_H
