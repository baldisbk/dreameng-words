#ifndef WORDSELECTORS_H
#define WORDSELECTORS_H

#include <QVector>
#include <QMap>

#include "word.h"

class WordSelector
{
public:
	virtual ~WordSelector();

	virtual bool predicate(Word word) const = 0;
	virtual double priority(Word word) const = 0;
	QVector<int> candidates(QMap<int, Word> words, int size) const;
};

class LearnSelector : public WordSelector
{
	// WordSelector interface
public:
	virtual bool predicate(Word word) const override;
	virtual double priority(Word word) const override;
};

class TrainSelector : public WordSelector
{
	// WordSelector interface
public:
	virtual bool predicate(Word word) const override;
	virtual double priority(Word word) const override;
};

class RepeatSelector : public WordSelector
{
	// WordSelector interface
public:
	virtual bool predicate(Word word) const override;
	virtual double priority(Word word) const override;
};

#endif // WORDSELECTORS_H
