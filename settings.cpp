#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{

}

int Settings::seqLength() const
{
	return m_seqLength;
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

void Settings::load(QString str)
{
	m_seqLength = str.toInt();
}
