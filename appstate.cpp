#include "appstate.h"

#include <QRandomGenerator>

#include <QDebug>

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
}

void AppState::next(Direction dir)
{
	dump("Before next");

	// actions & stats before
	switch (page()->status()) {
	case PageState::Learn:
	case PageState::Check:
	case PageState::Errors:
	case PageState::Train:
	case PageState::Repeat:
		if (dir == Right || dir == Left)
			flipWord(dir == Right);
		break;
	case PageState::Header:
		if (dir != Nowhere)
			m_currentWord = 0;
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
		case PageState::Learn: newLearn(); break;
		case PageState::Check: newCheck(); break;
		case PageState::Errors: newErrors(); break;
		case PageState::Train: newTrain(); break;
		case PageState::Repeat: newRepeat(); break;
		case PageState::Main: finish(); break;
		default: break;
		}
		break;
	default:
		break;
	}

	// next pages
	auto status = page()->status();
	auto next = nextWord();
	//qDebug() << "Next word:" << next.word;
	switch (status) {
	case PageState::Main:
		setLower(new PageState(PageState::Menu));
		if (m_words.isEmpty()) {
			setUpper(new PageState(PageState::None));
			setLeft(new PageState(PageState::None));
			setRight(new PageState(PageState::None));
		} else {
			setUpper(new StatState(PageState::Header, PageState::Repeat));
			setLeft(new StatState(PageState::Header, PageState::Train));
			setRight(new StatState(PageState::Header, PageState::Learn));
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
		if (next.word.isEmpty()) {
			setLeft(new PageState(othState));
			setRight(new PageState(othState));
		} else {
			setLeft(new WordState(othState, next, !showWordOnState(othState)));
			setRight(new WordState(othState, next, !showWordOnState(othState)));
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
				setLeft(new StatState(PageState::Header, PageState::Check));
				setRight(new StatState(PageState::Header, PageState::Check));
			} else if (m_errorWords.isEmpty()) {
				// no errors - it's over
				if (page()->status() == PageState::Check)
					setLeft(new StatState(PageState::Header, PageState::Errors));
				else
					setLeft(new StatState(PageState::Header, PageState::Main));
				setRight(new StatState(PageState::Header, PageState::Main));
			} else {
				// had errors - fix 'em, bitch
				setLeft(new StatState(PageState::Header, PageState::Errors));
				setRight(new StatState(PageState::Header, PageState::Errors));
			}
		}
		break;
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
	foreach(int w, m_selectedWords) m_changedWords[w] = m_words[w];
	next(Nowhere);
}

void AppState::addWord(Word word, int id)
{
	if (id == -1)
		while (m_words.contains(++id)) {}
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
		QStringList list = QString(file.readLine()).split(";");
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
	Word w = m_words[index];
	++w.repeats;
	if (!ok) {
		++w.errors;
		if (m_page->status() == PageState::Check)
			m_errorWords.push_back(m_selectedWords[m_currentWord]);
	}
	m_changedWords[index] = w;
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
//	} else if (!m_errorWords.isEmpty()) {
//		return m_changedWords[m_errorWords[0]];
	} else {
		return Word();
	}
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
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select new words
	for (int i = 0; i < m_settings.seqLength(); ++i) {
		int newId = rand(m_words.size());
		for (; m_selectedWords.contains(newId); newId = rand(m_words.size())) {}
		m_selectedWords.append(newId);
		qDebug() << "\t\tadd word" << newId;
		m_changedWords[newId] = m_words[newId];
	}
	// end select new words
	m_selectedWords = m_changedWords.keys().toVector();
	m_currentWord = -1;
	shuffle();
}

void AppState::newTrain()
{
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select words to train
	for (int i = 0; i < m_settings.seqLength(); ++i) {
		int newId = rand(m_words.size());
		for (; m_selectedWords.contains(newId); newId = rand(m_words.size())) {}
		m_selectedWords.append(newId);
		qDebug() << "\t\tadd word" << newId;
		m_changedWords[newId] = m_words[newId];
	}
	// end select words to train
	m_selectedWords = m_changedWords.keys().toVector();
	m_currentWord = -1;
	shuffle();
}

void AppState::newRepeat()
{
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select old words
	for (int i = 0; i < m_settings.seqLength(); ++i) {
		int newId = rand(m_words.size());
		for (; m_selectedWords.contains(newId); newId = rand(m_words.size())) {}
		m_selectedWords.append(newId);
		qDebug() << "\t\tadd word" << newId;
		m_changedWords[newId] = m_words[newId];
	}
	// end select old words
	m_selectedWords = m_changedWords.keys().toVector();
	m_currentWord = -1;
	shuffle();
}

void AppState::newErrors()
{
	m_selectedWords = m_errorWords;
	m_errorWords.clear();
	m_currentWord = -1;
	shuffle();
}

void AppState::newCheck()
{
	m_currentWord = -1;
	m_errorWords.clear();
	shuffle();
}

void AppState::finish()
{
	for (auto wi = m_changedWords.begin(); wi != m_changedWords.end(); ++wi)
		m_words[wi.key()] = wi.value();
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	m_currentWord = -1;
}

void AppState::shuffle()
{
	// shuffle m_selected
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
