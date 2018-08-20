#include "appstate.h"

#include <QRandomGenerator>

#include <QDebug>

const int TrainThreshold = 7;
const double OldRepeatCoeff = 0.8;
const double TrainErrorRatio = 0.3;
const double TrainErrorCost = 1.0;
const double SecondToAgeCoeff = 1.0/10.0; //1.0/86400.0;
const double BadSpeedRatio = 2.0;
const double BadSpeedCost = 1.0;

QString listToString(QVector<int> list)
{
	QStringList res;
	foreach (auto i, list) res.append(QString::number(i));
	return res.isEmpty()?" ":res.join(" ");
}

QVector<int> stringToList(QString str)
{
	QStringList strs = str.split(" ");
	QVector<int> res;
	foreach (auto i, strs) if (!i.isEmpty()) res.append(i.toInt());
	return res;
}

AppState::AppState(QObject *parent) : QObject(parent),
	m_upper(nullptr), m_lower(nullptr), m_left(nullptr), m_right(nullptr), m_page(nullptr)
{
	m_timer.start();
}

void AppState::next(Direction dir)
{
	dump("Before next");

	int prevElapsed = m_lastElapsed;
	m_lastElapsed = m_timer.restart();

	// actions & stats before
	switch (page()->status()) {
	case PageState::Learn:
	case PageState::Check:
	case PageState::Errors:
	case PageState::Train:
	case PageState::Repeat:
		switch (dir) {
		case Right:
		case Left:
			flipWord(dir == Right);
			m_totalElapsed += m_lastElapsed;
			break;
		case Down:
			m_lastElapsed += prevElapsed;
			break;
		case Up:
			cancel();
			break;
		default:
			break;
		}
		break;
	case PageState::Header:
		if (dir != Nowhere) {
			m_currentWord = 0;
			m_totalElapsed = 0;
		}
		if (dir == Up)
			cancel();
		break;
	case PageState::Footer:
		if (dir == Left)
			finish();
		break;
	default:
		break;
	}

	// flip current page
	switch (dir) {
	case Up: m_page = m_upper; m_upper = nullptr; break;
	case Down: m_page = m_lower; m_lower = nullptr; break;
	case Left: m_page = m_left; m_left = nullptr; break;
	case Right: m_page = m_right; m_right = nullptr; break;
	default: break;
	}

	// actions & stats after
	switch (page()->status()) {
	case PageState::Header:
		if (dir == Nowhere) break;
		switch(static_cast<StatState*>(page())->otherState()) {
		case PageState::Learn: newLearn(); m_prevTotalElapsed = 0; break;
		case PageState::Check: newCheck(); break;
		case PageState::Errors: newErrors(); break;
		case PageState::Train: newTrain(); m_prevTotalElapsed = 0; break;
		case PageState::Repeat: newRepeat(); m_prevTotalElapsed = 0; break;
		default: break;
		}
		break;
	case PageState::Footer:
		// TODO someday: do this after flip start and before it ends
		if (m_prevTotalElapsed == 0) {
			static_cast<StatState*>(page())->setDescription(
				QString(tr("Run took %1 seconds.\nTry to improve?")).
					arg(double(m_totalElapsed)/1000));
		} else {
			static_cast<StatState*>(page())->setDescription(
				QString(tr("Run took %2 seconds (previous %1).\nTry to improve?")).
					arg(double(m_prevTotalElapsed)/1000).
					arg(double(m_totalElapsed)/1000));
		}
		m_prevTotalElapsed = m_totalElapsed;
		again();
		break;
	default:
		break;
	}

	// next pages
	auto status = page()->status();
	auto next = nextWord();
	switch (status) {
	case PageState::Main:
		setLower(new PageState(PageState::Menu));
		if (m_words.isEmpty()) {
			setUpper(new PageState(PageState::None));
			setLeft(new PageState(PageState::None));
			setRight(new PageState(PageState::None));
		} else {
			setUpper(new StatState(
				PageState::Header, PageState::Repeat,
				QString(tr("You gonna remind %1 words")).arg(m_settings.seqLength())));
			setLeft(new StatState(
				PageState::Header, PageState::Train,
				QString(tr("You gonna repeat %1 words")).arg(m_settings.seqLength())));
			setRight(new StatState(
				PageState::Header, PageState::Learn,
				QString(tr("You gonna learn %1 new words")).arg(m_settings.seqLength())));
		}
		break;
	case PageState::Menu:
		setUpper(new PageState(PageState::Main));
		setLower(new PageState(PageState::None));
		setLeft(new PageState(PageState::None));
		setRight(new PageState(PageState::None));
		break;
	case PageState::Header: {
		auto othState = static_cast<StatState*>(page())->otherState();
		setUpper(new PageState(PageState::Main));
		setLower(new PageState(PageState::None));
		if (isWordState(othState)) {
			if (m_selectedWords.isEmpty()) {
				setLeft(new PageState(PageState::None));
				setRight(new PageState(PageState::None));
			} else {
				setLeft(new WordState(othState, next, !showWordOnState(othState)));
				setRight(new WordState(othState, next, !showWordOnState(othState)));
			}
		} else {
			setLeft(new PageState(othState));
			setRight(new PageState(othState));
		}
		break;
	}
	case PageState::Errors:
	case PageState::Learn:
	case PageState::Check:
	case PageState::Train:
	case PageState::Repeat:
		setUpper(new PageState(PageState::Main));
		if (canShowWordOnState(status))
			setLower(new WordState(status, curWord(), false));
		else
			setLower(new PageState(PageState::None));
		if (!next.word.isEmpty()) {
			setLeft(new WordState(status, next, !showWordOnState(status)));
			setRight(new WordState(status, next, !showWordOnState(status)));
		} else {
			if (showWordOnState(status)) {
				// there were translations? check 'em
				setLeft(new StatState(
					PageState::Header, PageState::Check,
					QString(tr("Let's check these words"))));
				setRight(new StatState(
					PageState::Header, PageState::Check,
					QString(tr("Let's check these words"))));
			} else if (m_errorWords.isEmpty()) {
				// no errors - it's over
				if (!showWordOnState(status))
					// this last will be with error
					setLeft(new StatState(
						PageState::Header, PageState::Errors,
						QString(tr("You should fix that error"))));
				else
					setLeft(new StatState(
						PageState::Footer, PageState::Check,
						QString(tr("Some statistics here"))));
				setRight(new StatState(
					PageState::Footer, PageState::Check,
					QString(tr("Some statistics here"))));
			} else {
				// had errors - fix 'em, bitch
				setLeft(new StatState(
					PageState::Header, PageState::Errors,
					QString(tr("You should fix %1 errors")).arg(m_errorWords.size()+1)));
				setRight(new StatState(
					PageState::Header, PageState::Errors,
					QString(tr("You should fix %1 errors")).arg(m_errorWords.size())));
			}
		}
		break;
	case PageState::Footer: {
		auto othState = static_cast<StatState*>(page())->otherState();
		setUpper(new PageState(PageState::None));
		setLower(new PageState(PageState::None));
		setLeft(new PageState(PageState::Main));
		setRight(new StatState(
			PageState::Header, othState,
			QString(tr("Let's try to improve"))));
		break;
	}
//	case PageState::Ask:
		// n/a
		break;
	default:
		break;
	}
	dump("After next");
}

void AppState::init()
{
	m_settings.setSeqLength(3);
	m_settings.setSeqNumber(1);

	m_page = new PageState(PageState::Main);
	finish();
	next(Nowhere);
}

void AppState::addWord(QVariantMap word)
{
	int id = word.value("uid").toInt();
	Word newWord;
	newWord.load(word);
	addWord(newWord, id);
}

void AppState::loadState(QVariantMap state)
{
	m_settings.setSeqLength(3);
	m_settings.setSeqNumber(1);

	if (m_page!=nullptr) delete m_page;
	switch (PageState::stringToState(state["state"].toString())) {
	case PageState::Learn:
	case PageState::Check:
	case PageState::Errors:
	case PageState::Train:
	case PageState::Repeat:
		m_page = new WordState();
		break;
	case PageState::Header:
	case PageState::Footer:
		m_page = new StatState();
		break;
	default:
		m_page = new PageState();
		break;
	}
	m_page->load(state["statectx"].toString());
	m_selectedWords = stringToList(state["selected"].toString());
	m_errorWords = stringToList(state["errors"].toString());
	m_currentWord = state["current"].toInt();
	m_totalElapsed = state["runtime"].toInt();
	m_prevTotalElapsed = state["prevruntime"].toInt();
	QStringList list = state["changed"].toString().split(" ");
	int i = 0;
	for(int w: m_selectedWords) {
		m_changedWords[w] = m_words[w];
		m_changedWords[w].loadStats(list[i++]);
	}
	next(Nowhere);
}

void AppState::addWord(Word word, int id)
{
	if (id == -1)
		while (m_words.contains(++id)) {}
	//qDebug() << "Add word" << id;
	m_words[id] = word;

}

void AppState::populateDemo()
{
	addWord(Word("Zero", "Null"));
	addWord(Word("One", "Jeden"));
	addWord(Word("Two", "Dva"));
	addWord(Word("Three", "Tri"));
	addWord(Word("Four", "Ctyri"));
	addWord(Word("Five", "Pet"));
	addWord(Word("Six", "Sest"));
	addWord(Word("Seven", "Sedm"));
	addWord(Word("Eight", "Osm"));
	addWord(Word("Nine", "Devet"));
	addWord(Word("Ten", "Deset"));
	addWord(Word("Eleven", "Jedenact"));
	addWord(Word("Twelve", "Dvanact"));
	addWord(Word("Thirteen", "Trinact"));
	addWord(Word("Fourteen", "Ctrnact"));
	addWord(Word("Fifteen", "Patnact"));
	addWord(Word("Sixteen", "Sesnact"));
	addWord(Word("Seventeen", "Sedmnact"));
	addWord(Word("Eighteen", "Osmnact"));
	addWord(Word("Nineteen", "Devetnact"));
	addWord(Word("Twenty", "Dvacet"));
}

void AppState::populateFile(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Error file" << file.errorString();
		return;
	}
	while (!file.atEnd()) {
		QStringList list = QString(file.readLine()).trimmed().split(";");
		if (list.size() < 2) continue;
		Word word(list[0], list[1]);
		if (list.size() > 2) word.repeats = list[2].toInt();
		addWord(word);
	}
}

QList<int> AppState::wordIndexes() const
{
	return m_words.keys();
}

QVariantMap AppState::wordContents(int index) const
{
	if (!m_words.contains(index))
		return QVariantMap();
	const Word w = m_words[index];
	QVariantMap res = w.store();
	res["uid"] = index;
	return res;
}

QList<int> AppState::changedIndexes() const
{
	return m_changedWords.keys();
}

QVariantMap AppState::changedContents(int index) const
{
	if (!m_changedWords.contains(index))
		return QVariantMap();
	const Word w = m_changedWords[index];
	QVariantMap res = w.store();
	res["uid"] = index;
	return res;
}

QVariantMap AppState::stateContents() const
{
	QVariantMap res;
	res["state"] = PageState::stateToString(page()->status());
	res["statectx"] = page()->store();
	res["selected"] = listToString(m_selectedWords);
	res["errors"] = listToString(m_errorWords);
	res["current"] = m_currentWord;
	QStringList ch;
	for(auto index: m_selectedWords)
		ch.append(m_changedWords[index].storeStats());
	res["changed"] = ch.join(" ");
	res["runtime"] = m_totalElapsed;
	res["prevruntime"] = m_prevTotalElapsed;
	return res;
}

void AppState::clearWords()
{
	m_words.clear();
	m_selectedWords.clear();
	m_errorWords.clear();
}

Settings *AppState::settings()
{
	return &m_settings;
}

PageState *AppState::upper() const
{
	return m_upper;
}

PageState *AppState::lower() const
{
	return m_lower;
}

PageState *AppState::left() const
{
	return m_left;
}

PageState *AppState::right() const
{
	return m_right;
}

PageState *AppState::page() const
{
	return m_page;
}

void AppState::setUpper(PageState *upper)
{
	if (m_upper) delete m_upper;
	m_upper = upper;
}

void AppState::setLower(PageState *lower)
{
	if (m_lower) delete m_lower;
	m_lower = lower;
}

void AppState::setLeft(PageState *left)
{
	if (m_left) delete m_left;
	m_left = left;
}

void AppState::setRight(PageState *right)
{
	if (m_right) delete m_right;
	m_right = right;
}

void AppState::flipWord(bool ok)
{
	int index = m_selectedWords[m_currentWord];
	Word w = m_changedWords[index];
	if (w.age > TrainThreshold) {
		w.repeats *= OldRepeatCoeff;
		w.errors *= OldRepeatCoeff;
	}
	int newspeed = w.speed + (m_lastElapsed-w.speed)/(w.repeats/2+1);
	++w.repeats;
	double since = double(w.last.secsTo(QDateTime().currentDateTime()))*SecondToAgeCoeff;
	w.last = QDateTime().currentDateTime();
	if (!ok) {
		++w.errors;
		if (!showWordOnState(m_page->status()))
			m_errorWords.push_back(m_selectedWords[m_currentWord]);
		since -= TrainErrorCost * w.errorRate();
	}
	if (w.speed != 0 && newspeed > w.speed*BadSpeedRatio) {
		since -= (double(newspeed)/double(w.speed) - BadSpeedRatio)*BadSpeedCost;
	}
	if (w.age > TrainThreshold && (w.errorRate() > TrainErrorRatio)) {
		w.age = 0;
	} else if (since > 0) {
		w.age += since;
	}
	w.speed = newspeed;
	m_changedWords[index] = w;
	qDebug() << "-- flip" << ok;
	++m_currentWord;
}

void AppState::showWord()
{
	// alter word's statictics
}

Word AppState::curWord() const
{
	if (m_currentWord == -1 || m_currentWord >= m_selectedWords.size())
		return Word();
	else
		return m_changedWords[m_selectedWords[m_currentWord]];
}

Word AppState::nextWord() const
{
	if (m_changedWords.isEmpty()) {
		return Word();
	} else if (m_currentWord < m_selectedWords.size() - 1) {
		return m_changedWords[m_selectedWords[m_currentWord+1]];
	} else {
		return Word();
	}
}

bool AppState::isWordState(PageState::State state)
{
	return
		(state == PageState::Learn) ||
		(state == PageState::Check) ||
		(state == PageState::Errors) ||
		(state == PageState::Repeat) ||
		(state == PageState::Train);
}

bool AppState::showWordOnState(PageState::State state)
{
	return (state == PageState::Learn) || (state == PageState::Errors);
}

bool AppState::canShowWordOnState(PageState::State state)
{
	return !showWordOnState(state) &&
	((state == PageState::Repeat) || (state == PageState::Train));
}

int rand(int size)
{
	return QRandomGenerator::global()->bounded(size);
}

void AppState::newLearn()
{
	qDebug() << "-- newLearn";
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select new words
	QVector<int> candidates;
	candidates.reserve(m_words.size());
	for (int i = 0; i < m_words.size(); ++i) {
		if (m_words[i].repeats == 0)
			candidates.append(i);
	}
	shuffle(candidates);
	for (int i = 0; i < m_settings.seqLength() && i < candidates.size(); ++i) {
		int newId = candidates[i];
		m_selectedWords.append(newId);
		m_changedWords[newId] = m_words[newId];
	}
	// end select new words
	m_currentWord = -1;
	shuffle(m_selectedWords);
}

void AppState::newTrain()
{
	qDebug() << "-- newTrain";
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select words to train
	typedef QPair<int, double> T;
	QVector<T> candidates;
	candidates.reserve(m_words.size());
	for (int i = 0; i < m_words.size(); ++i) {
		if (m_words[i].repeats > 0 && m_words[i].age <= TrainThreshold)
			candidates.append(T(i, m_words[i].age));
	}
	std::sort(candidates.begin(), candidates.end(),
		  [](const T& a, const T& b){return a.second>b.second;});
	for (int i = 0; i < m_settings.seqLength() && i < candidates.size(); ++i) {
		int newId = candidates[i].first;
		m_selectedWords.append(newId);
		m_changedWords[newId] = m_words[newId];
	}
	// end select words to train
	m_currentWord = -1;
	shuffle(m_selectedWords);
}

void AppState::newRepeat()
{
	qDebug() << "-- newRepeat";
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select old words
	typedef QPair<int, double> T;
	QVector<T> candidates;
	candidates.reserve(m_words.size());
	for (int i = 0; i < m_words.size(); ++i) {
		if (m_words[i].repeats > 0 && m_words[i].age > TrainThreshold)
			candidates.append(T(i, m_words[i].repeats - m_words[i].errors));
	}
	std::sort(candidates.begin(), candidates.end(),
		  [](const T& a, const T& b){return a.second<b.second;});
	for (int i = 0; i < m_settings.seqLength() && i < candidates.size(); ++i) {
		int newId = candidates[i].first;
		m_selectedWords.append(newId);
		m_changedWords[newId] = m_words[newId];
	}
	// end select old words
	m_currentWord = -1;
	shuffle(m_selectedWords);
}

void AppState::newErrors()
{
	qDebug() << "-- newErrors";
	m_selectedWords = m_errorWords;
	m_errorWords.clear();
	m_currentWord = -1;
	shuffle(m_selectedWords);
}

void AppState::newCheck()
{
	qDebug() << "-- newCheck";
	m_currentWord = -1;
	m_errorWords.clear();
	shuffle(m_selectedWords);
}

void AppState::finish()
{
	emit saveWords();
	qDebug() << "-- finish";
	for (auto wi = m_changedWords.begin(); wi != m_changedWords.end(); ++wi)
		m_words[wi.key()] = wi.value();
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	m_currentWord = -1;
}

void AppState::again()
{
	emit saveWords();
	qDebug() << "-- again";
	for (auto wi = m_changedWords.begin(); wi != m_changedWords.end(); ++wi)
		m_words[wi.key()] = wi.value();
	m_currentWord = -1;
	m_errorWords.clear();
	m_selectedWords = m_changedWords.keys().toVector();
	shuffle(m_selectedWords);
}

void AppState::cancel()
{
	qDebug() << "-- cancel";
	m_changedWords.clear();
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	m_currentWord = -1;
}

void AppState::shuffle(QVector<int> &list)
{
	for (int i = list.size()-1; i>0; --i) {
		int other = rand(i);
		int tmp = list[i];
		list[i] = list[other];
		list[other] = tmp;
	}
}

void AppState::dump(QString prefix)
{
	return;
	qDebug() << prefix;
	qDebug() << "\tpage:" << page()->dump();
	qDebug() << "\tleft:" << (left()?left()->dump():"null");
	qDebug() << "\tright:" << (right()?right()->dump():"null");
	qDebug() << "\tupper:" << (upper()?upper()->dump():"null");
	qDebug() << "\tlower:" << (lower()?lower()->dump():"null");
	qDebug() <<
		QString("\tcurrent %1, all %2, selected %3, errors %4").
		    arg(m_currentWord).arg(m_changedWords.size()).
		    arg(m_selectedWords.size()).arg(m_errorWords.size());
}
