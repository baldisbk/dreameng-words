#include "pagestate.h"
#include "appstate.h"

QString PageState::stateToString(PageState::State state)
{
	switch (state) {
	case Main: return "Main";
	case Menu: return "Menu";
	case Header: return "Header";
	case Learn: return "Learn";
	case Check: return "Check";
	case Errors: return "Errors";
	case Train: return "Train";
	case Repeat: return "Repeat";
	case Footer: return "Again";
	default: return "Impossible";
	}
}

PageState::State PageState::stringToState(QString str)
{
	if (str == "Main") return Main;
	if (str == "Menu") return Menu;
	if (str == "Header") return Header;
	if (str == "Learn") return Learn;
	if (str == "Check") return Check;
	if (str == "Errors") return Errors;
	if (str == "Train") return Train;
	if (str == "Repeat") return Repeat;
	if (str == "Again") return Footer;
	return None;
}

PageState::PageState(PageState::State state, QObject *parent) :
	QObject(parent), m_status(state)
{}

PageState::State PageState::status() const {return m_status;}

QString PageState::dump() const
{
	return stateToString(status());
}

QString PageState::store() const
{
	return stateToString(status());
}

void PageState::load(QString ctx)
{
	setStatus(stringToState(ctx));
}

void PageState::setStatus(State status)
{
	if (m_status == status)
		return;

	m_status = status;
	emit statusChanged(m_status);
}

WordState::WordState(PageState::State state, Word word, bool wo, QObject *parent) :
	PageState(state, parent), m_word(word.word), m_translation(word.translation), m_wordOnly(wo)
{}

QString WordState::word() const {return m_word;}
QString WordState::translation() const {return m_translation;}
bool WordState::wordOnly() const {return m_wordOnly;}

void WordState::setWord(QString word)
{
	if (m_word == word)
		return;

	m_word = word;
	emit wordChanged(m_word);
}

void WordState::setTranslation(QString translation)
{
	if (m_translation == translation)
		return;

	m_translation = translation;
	emit translationChanged(m_translation);
}

void WordState::setWordOnly(bool wordOnly)
{
	if (m_wordOnly == wordOnly)
		return;

	m_wordOnly = wordOnly;
	emit wordOnlyChanged(m_wordOnly);
}

HeadState::HeadState(PageState::State state, PageState::State addState, QString desc, QObject *parent) :
	PageState(state, parent), m_otherState(addState), m_description(desc)
{}

PageState::State HeadState::otherState() const
{
	return m_otherState;
}

void HeadState::setOtherState(PageState::State otherState)
{
	if (m_otherState == otherState)
		return;

	m_otherState = otherState;
	emit otherStateChanged(m_otherState);
}

void HeadState::setDescription(QString description)
{
	if (m_description == description)
		return;

	m_description = description;
	emit descriptionChanged(m_description);
}


QString WordState::dump() const
{
	return QString("%1: %2/%3, %4").
			arg(PageState::dump()).
			arg(word()).
			arg(translation()).
			arg(wordOnly());
}

QString HeadState::dump() const
{
	return QString("%1 => %2").
		arg(PageState::dump()).
		arg(stateToString(otherState()));
}


QString HeadState::store() const
{
	return PageState::store()+";"+stateToString(otherState())+";"+description();
}

void HeadState::load(QString ctx)
{
	QStringList strs = ctx.split(";");
	if (strs.size() < 2) return;
	PageState::load(strs[0]);
	setOtherState(stringToState(strs[1]));
	if (strs.size() >= 3) setDescription(strs[2]);
}

QString HeadState::description() const
{
	return m_description;
}

QString WordState::store() const
{
	return PageState::store()+";"+
		word()+";"+
		translation()+";"+
		(wordOnly()?"0":"w");
}

void WordState::load(QString ctx)
{
	QStringList strs = ctx.split(";");
	if (strs.size() != 4) return;
	PageState::load(strs[0]);
	setWord(strs[1]);
	setTranslation(strs[2]);
	setWordOnly(strs[3]=="0");
}
