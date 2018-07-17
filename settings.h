#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
	Q_OBJECT
public:
	explicit Settings(QObject *parent = nullptr);

	Q_PROPERTY(int seqLength READ seqLength WRITE setSeqLength NOTIFY seqLengthChanged)
	Q_PROPERTY(int seqNumber READ seqNumber WRITE setSeqNumber NOTIFY seqNumberChanged)

	int seqLength() const;
	int seqNumber() const;

signals:
	void seqLengthChanged(int seqLength);
	void seqNumberChanged(int seqNumber);

public slots:
	void setSeqLength(int seqLength);
	void setSeqNumber(int seqNumber);

private:
	int m_seqLength;
	int m_seqNumber;
};

#endif // SETTINGS_H
