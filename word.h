#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QVariantMap>
#include <QDateTime>

struct Word
{
        Word(){}
        Word(QString _word, QString _translation):
		word(_word), translation(_translation),
		repeats(0), errors(0),
		age(0), speed(0), lastspeed(0)
	{}

	static QStringList keys() {
		return QStringList()
			<< "word"
			<< "translation"
			<< "repeats"
			<< "errors"
			<< "last"
			<< "age"
			<< "speed"
			<< "lastspeed"
			<< "dict";
	}
	QVariantMap store() const
	{
		QVariantMap res;
		res["word"] = word;
		res["translation"] = translation;
		res["repeats"] = repeats;
		res["errors"] = errors;
		res["last"] = last.toSecsSinceEpoch();
		res["age"] = age;
		res["speed"] = speed;
		res["lastspeed"] = lastspeed;
		res["dict"] = dict;
		return res;
	}
	QString storeStats() const {
		return QString("%1;%2;%3;%4;%5;%6").
			arg(repeats).
			arg(errors).
			arg(last.toSecsSinceEpoch()).
			arg(age).
			arg(speed).
			arg(lastspeed);
	}
	void loadStats(QString stats) {
		QStringList list = stats.split(";");
		if (list.size() < 5) return;
		repeats = list[0].toInt();
		errors = list[1].toInt();
		last.fromSecsSinceEpoch(list[2].toLongLong());
		age = list[3].toDouble();
		speed = list[4].toInt();
		lastspeed = list[5].toInt();
	}
	void load(QVariantMap m) {
		word = m.value("word").toString();
		translation = m.value("translation").toString();
		repeats = m.value("repeats", 0).toInt();
		errors = m.value("errors", 0).toInt();
		last.fromSecsSinceEpoch(m.value("last", 0).toLongLong());
		age = m.value("age", 0).toDouble();
		speed = m.value("speed", 0).toInt();
		lastspeed = m.value("lastspeed", 0).toInt();
		dict = m.value("dict", 0).toString();
	}
	double errorRate() const {
		return double(errors)/double(repeats);
	}
	QString dump() const {
		return QString("%1/%2 %3/%4 %5(%6) S%7/%8 @%9").
			arg(word).
			arg(translation).
			arg(errors).
			arg(repeats).
			arg(last.toString()).
			arg(age).
			arg(speed).
			arg(lastspeed).
			arg(dict);
	}

        QString word;
        QString translation;
        int repeats;
        int errors;
	QDateTime last;
	double age;
	int speed, lastspeed;
	QString dict;
};

#endif // WORD_H
