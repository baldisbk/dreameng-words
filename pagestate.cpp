#include "pagestate.h"
#include "appstate.h"

//const int GRAPH_CLUSTER_NUMBER = 20;

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
	case Statistic: return "Statistic";
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
	if (str == "Statistic") return Statistic;
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

QString StatState::typeToString(StatState::Types type)
{
	switch (type) {
	case None:
	case NoOfTypes:
		return QString();
	case Errors: return "Errors";
	case States: return "States";
	case Speed: return "Speed";
	case Age: return "Age";
	case LastRepeat: return "LastRepeat";
	}
	return QString();
}

StatState::Types StatState::stringToType(QString str)
{
	if (str == "Errors") return Errors;
	if (str == "States") return States;
	if (str == "Speed") return Speed;
	if (str == "Age") return Age;
	if (str == "LastRepeat") return LastRepeat;
	return None;
}

StatState::StatState(StatState::Types type, AppState *app, QObject *parent) :
	PageState(PageState::Statistic, parent), m_type(type)
{
	switch(m_type) {
	case States: {
		auto states = app->states();
		m_series.clear();
		auto n = new BarSerie(BarSerie::Serie() << BarSerie::Value(0, states[0]), this);
		n->setColor(Qt::red);
		auto t = new BarSerie(BarSerie::Serie() << BarSerie::Value(0, states[1]), this);
		t->setColor(Qt::green);
		auto r = new BarSerie(BarSerie::Serie() << BarSerie::Value(0, states[2]), this);
		r->setColor(Qt::blue);
		m_series.addSerie(n);
		m_series.addSerie(t);
		m_series.addSerie(r);
		m_series.setChecks(QVector<double>() << 0);
		m_series.adjust();
		m_series.setMinimum(0);
		m_series.setGraphType(BarSeries::Bars);
		break;
	}
	case Errors:
		fillGraph(app, "errors");
		break;
	case Speed:
		fillGraph(app, "speed");
		break;
	case Age:
		fillGraph(app, "age");
		break;
	case LastRepeat:
		fillGraph(app, "lastrepeat");
		break;
	case None:
	case NoOfTypes:
		break;
	}
}

void StatState::fillGraph(AppState *app, QString stat)
{
	m_series.clear();

	auto res = app->stats(stat);

	int index = 0;
	int sIndex = 0;
	double last = 0;
	for (auto v: res) {
		if (sIndex++ == 0) continue;
		std::sort(v.begin(), v.end());
		BarSerie::Serie graph;
		if (index != 0)
			graph << BarSerie::Value(index, last);
		for (auto vv: v)
			graph << BarSerie::Value(++index, last = vv);
		auto serie = new BarSerie(graph, this);
		switch (sIndex) {
		case 1: serie->setColor(Qt::red); break;
		case 2: serie->setColor(Qt::green); break;
		case 3: serie->setColor(Qt::blue); break;
		}
		m_series.addSerie(serie);
	}

//	double cWidth = (res.back() - res.front()) / GRAPH_CLUSTER_NUMBER;

//	int count = 0;
//	double start = res.front();
//	for (auto v: res) {
//		if (v <= start + cWidth) {
//			++count;
//		} else {
//			graph << BarSeries::Value(start, count);
//			count = 0;
//			start += cWidth;
//		}
//	}
	m_series.adjust();
	m_series.setGraphType(BarSeries::Graph);
}

QString StatState::dump() const
{
	return QString("%1 + %2").
		arg(PageState::dump()).
		arg(typeToString(type()));
}

QString StatState::store() const
{
	return PageState::store()+";"+
		typeToString(type());
}

void StatState::load(QString ctx)
{
	QStringList strs = ctx.split(";");
	if (strs.size() != 2) return;
	PageState::load(strs[0]);
	setType(stringToType(strs[1]));
}

StatState::Types StatState::type() const
{
	return m_type;
}

BarSeries *StatState::series()
{
	return &m_series;
}

void StatState::setType(StatState::Types type)
{
	if (m_type == type)
		return;

	m_type = type;
	emit typeChanged(m_type);
}
