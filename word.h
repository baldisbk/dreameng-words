#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QVariantMap>

struct Word
{
        Word(){}
        Word(QString _word, QString _translation):
	word(_word), translation(_translation), repeats(0), errors(0) {}

	QVariantMap store() const
	{
		QVariantMap res;
		res["word"] = word;
		res["translation"] = translation;
		res["errors"] = errors;
		res["repeats"] = repeats;
		return res;
	}
	void load(QVariantMap m) {
		errors = m.value("errors", 0).toInt();
		repeats = m.value("repeats", 0).toInt();
		translation = m.value("translation").toString();
		word = m.value("word").toString();
	}

        QString word;
        QString translation;
        int repeats;
        int errors;
};

#endif // WORD_H
