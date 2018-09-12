#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{

}

int Settings::seqLength() const
{
	return m_seqLength;
}

int Settings::seqNumber() const
{
	return m_seqNumber;
}

QString Settings::store() const
{
	return QString::number(m_seqLength);
}

void Settings::setSeqLength(int seqLength)
{
	if (m_seqLength == seqLength)
		return;

	m_seqLength = seqLength;
	emit seqLengthChanged(m_seqLength);
}

void Settings::setSeqNumber(int seqNumber)
{
	if (m_seqNumber == seqNumber)
		return;

	m_seqNumber = seqNumber;
	emit seqNumberChanged(m_seqNumber);
}

void Settings::load(QString str)
{
	m_seqLength = str.toInt();
}
