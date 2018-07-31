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
		age(0), speed(0)
	{}

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
		return res;
	}
	void load(QVariantMap m) {
		word = m.value("word").toString();
		translation = m.value("translation").toString();
		repeats = m.value("repeats", 0).toInt();
		errors = m.value("errors", 0).toInt();
		last.fromSecsSinceEpoch(m.value("last", 0).toLongLong());
		age = m.value("age", 0).toDouble();
		speed = m.value("speed", 0).toInt();
	}
	double errorRate() const {
		return double(errors)/double(repeats);
	}

        QString word;
        QString translation;
        int repeats;
        int errors;
	QDateTime last;
	double age;
	int speed;
};

#endif // WORD_H
