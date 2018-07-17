#include "appstate.h"

AppState::AppState(QObject *parent) : QObject(parent)
{
	m_page = new PageState(PageState::Main);
	next(Nowhere);
}

void AppState::next(Direction dir)
{
	// actions & stats
	switch (page()->status()) {
	case PageState::Header:
		switch(((StatState*)page())->otherState()) {
		case PageState::Learn: newLearn(); break;
		case PageState::Check: newCheck(); break;
		case PageState::Errors: newErrors(); break;
		case PageState::Train: newTrain(); break;
		case PageState::Repeat: newRepeat(); break;
		default: break;
		}
		break;
	case PageState::Learn:
	case PageState::Check:
	case PageState::Errors:
	case PageState::Train:
	case PageState::Repeat:
		if (dir == Right || dir == Left)
			flipWord(dir == Right);
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

	// next pages
	auto status = page()->status();
	auto next = nextWord();
	switch (status) {
	case PageState::Main:
		setUpper(new StatState(PageState::Header, PageState::Repeat));
		setLower(new PageState(PageState::Menu));
		setLeft(new StatState(PageState::Header, PageState::Train));
		setRight(new StatState(PageState::Header, PageState::Learn));
		break;
	case PageState::Menu:
		setUpper(new PageState(PageState::Main));
		setLower(new PageState(PageState::None));
		setLeft(new PageState(PageState::None));
		setRight(new PageState(PageState::None));
		break;
	case PageState::Header:
		setUpper(new PageState(PageState::Main));
		setLower(new PageState(PageState::None));
		setLeft(new WordState(((StatState*)page())->otherState(), next, true));
		setRight(new WordState(((StatState*)page())->otherState(), next, true));
		break;
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
}

void AppState::addWord(QVariantMap word)
{
	int id = word.value("_id").toInt();
	if (id == -1)
		while (m_words.contains(++id)) {}
	Word newWord;
	newWord.errors = word.value("errors", 0).toInt();
	newWord.repeats = word.value("repeats", 0).toInt();
	newWord.translation = word.value("translation").toString();
	newWord.word = word.value("word").toString();
}

void AppState::populateDemo()
{
	if (!m_words.contains(0)) m_words[0] = Word("Zero", "Null");
	if (!m_words.contains(1)) m_words[1] = Word("One", "Jeden");
	if (!m_words.contains(2)) m_words[2] = Word("Two", "Dva");
	if (!m_words.contains(3)) m_words[3] = Word("Three", "Tri");
	if (!m_words.contains(4)) m_words[4] = Word("Four", "Ctyri");
	if (!m_words.contains(5)) m_words[5] = Word("Five", "Pet");
	if (!m_words.contains(6)) m_words[6] = Word("Six", "Sest");
	if (!m_words.contains(7)) m_words[7] = Word("Seven", "Sedm");
	if (!m_words.contains(8)) m_words[8] = Word("Eight", "Osm");
	if (!m_words.contains(9)) m_words[8] = Word("Nine", "Devet");
	if (!m_words.contains(10)) m_words[8] = Word("Ten", "Deset");
	if (!m_words.contains(11)) m_words[8] = Word("Eleven", "Jedenact");
	if (!m_words.contains(12)) m_words[8] = Word("Twelve", "Dvanact");
	if (!m_words.contains(13)) m_words[8] = Word("Thirteen", "Trinact");
	if (!m_words.contains(14)) m_words[8] = Word("Fourteen", "Ctrnact");
	if (!m_words.contains(15)) m_words[8] = Word("Fifteen", "Patnact");
	if (!m_words.contains(16)) m_words[8] = Word("Sixteen", "Sesnact");
	if (!m_words.contains(17)) m_words[8] = Word("Seventeen", "Sedmnact");
	if (!m_words.contains(18)) m_words[8] = Word("Eighteen", "Osmnact");
	if (!m_words.contains(19)) m_words[8] = Word("Nineteen", "Devetnact");
	if (!m_words.contains(20)) m_words[8] = Word("Twenty", "Dvacet");

	m_settings.setSeqLength(10);
	m_settings.setSeqNumber(1);

	m_page = new PageState(PageState::Main);
	next(Nowhere);
}

void AppState::populateFile(QString /*filename*/)
{

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
	QVariantMap res;
	res["word"] = w.word;
	res["translation"] = w.translation;
	res["errors"] = w.errors;
	res["repeats"] = w.repeats;
	res["_id"] = index;
	return res;
}

Settings *AppState::settings()
{
	return &m_settings;
}

const PageState *AppState::upper() const
{
	return m_upper;
}

const PageState *AppState::lower() const
{
	return m_lower;
}

const PageState *AppState::left() const
{
	return m_left;
}

const PageState *AppState::right() const
{
	return m_right;
}

const PageState *AppState::page() const
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
			m_errorWords.push_back(m_currentWord);
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
	if (m_currentWord == -1)
		return m_changedWords[m_selectedWords[0]];
	else if (m_currentWord < m_selectedWords.size() - 1)
		return m_changedWords[m_selectedWords[m_currentWord+1]];
	else if (!m_errorWords.isEmpty())
		return m_changedWords[m_errorWords[0]];
	else
		return Word();
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

void AppState::newLearn()
{
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select new words
	for (int i = 0; i < m_settings.seqLength(); ++i) {
		int newId;
		while (m_selectedWords.contains(newId=qrand()%m_words.size())) {}
		m_selectedWords.append(newId);
		m_changedWords[newId] = m_words[newId];
	}
	// end select new words
	m_selectedWords = m_changedWords.keys().toVector();
	m_currentWord = 0;
	shuffle();
}

void AppState::newTrain()
{
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select words to train
	for (int i = 0; i < m_settings.seqLength(); ++i) {
		int newId;
		while (m_selectedWords.contains(newId=qrand()%m_words.size())) {}
		m_selectedWords.append(newId);
		m_changedWords[newId] = m_words[newId];
	}
	// end select words to train
	m_selectedWords = m_changedWords.keys().toVector();
	m_currentWord = 0;
	shuffle();
}

void AppState::newRepeat()
{
	m_selectedWords.clear();
	m_changedWords.clear();
	m_errorWords.clear();
	// select old words
	for (int i = 0; i < m_settings.seqLength(); ++i) {
		int newId;
		while (m_selectedWords.contains(newId=qrand()%m_words.size())) {}
		m_selectedWords.append(newId);
		m_changedWords[newId] = m_words[newId];
	}
	// end select old words
	m_selectedWords = m_changedWords.keys().toVector();
	m_currentWord = 0;
	shuffle();
}

void AppState::newErrors()
{
	m_selectedWords = m_errorWords;
	m_currentWord = 0;
	shuffle();
}

void AppState::newCheck()
{
	m_selectedWords = m_errorWords;
	m_currentWord = 0;
	m_errorWords.clear();
	shuffle();
}

void AppState::shuffle()
{
	// shuffle
}
