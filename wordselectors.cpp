#include "wordselectors.h"

#include "constants.h"

WordSelector::~WordSelector() {}

QVector<int> WordSelector::candidates(QMap<int, Word> words, int size) const
{
	typedef QPair<int, double> T;
	QVector<T> vec;
	vec.reserve(words.size());
	for (int i = 0; i < words.size(); ++i) {
		if (predicate(words[i]))
			vec.append(T(i, priority(words[i])));
	}
	std::sort(vec.begin(), vec.end(), [](const T& a, const T& b){return a.second<b.second;});
	QVector<int> res;
	for (int i = 0; i < size && i < vec.size(); ++i) {
		res.append(vec[i].first);
	}
	return res;
}

bool LearnSelector::predicate(Word word) const
{
	return word.repeats == 0;
}

double LearnSelector::priority(Word /*word*/) const
{
	return qrand();
}

bool TrainSelector::predicate(Word word) const
{
	return word.repeats > 0 && word.age <= TrainThreshold;
}

double TrainSelector::priority(Word word) const
{
	int elapsed = int(word.last.secsTo(QDateTime::currentDateTime())) / TrainElapsedRound;
	return word.age - elapsed * TrainElapsedCoeff;

}

bool RepeatSelector::predicate(Word word) const
{
	return word.repeats > 0 && word.age > TrainThreshold;
}

double RepeatSelector::priority(Word word) const
{
	return -word.errorRate();
}
