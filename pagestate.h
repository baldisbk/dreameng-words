#ifndef PAGESTATE_H
#define PAGESTATE_H

#include <QObject>

#include "word.h"
#include "barseries.h"

class PageState : public QObject
{
	Q_OBJECT
public:	
	enum State {
		None = 0,
		Ask = None,	// for now they're equal
		Main,
		Menu,

		Header,
		Footer,

		Learn,
		Check,
		Errors,
		Train,
		Repeat,

		Statistic
	};
	Q_ENUM(State)

	static QString stateToString(State state);
	static State stringToState(QString str);

	explicit PageState(QObject *parent = nullptr) : QObject(parent) {}
	PageState(State state, QObject *parent = nullptr);

	Q_PROPERTY(State status READ status WRITE setStatus NOTIFY statusChanged)
	State status() const;

	virtual QString dump() const;
	virtual QString store() const;
	virtual void load(QString ctx);

signals:
	void statusChanged(State status);

public slots:
	void setStatus(State status);

private:
	State m_status;
};

class WordState : public PageState
{
	Q_OBJECT
public:
	explicit WordState(QObject *parent = nullptr) : PageState(parent) {}
	WordState(State state, Word word = Word(), bool wo = true, QObject *parent = nullptr);

	Q_PROPERTY(QString word READ word WRITE setWord NOTIFY wordChanged)
	Q_PROPERTY(QString translation READ translation WRITE setTranslation NOTIFY translationChanged)
	Q_PROPERTY(bool wordOnly READ wordOnly WRITE setWordOnly NOTIFY wordOnlyChanged)

	QString word() const;
	QString translation() const;
	bool wordOnly() const;

	virtual QString dump() const override;
	virtual QString store() const override;
	virtual void load(QString ctx) override;

signals:
	void wordChanged(QString word);
	void translationChanged(QString translation);
	void wordOnlyChanged(bool wordOnly);

public slots:
	void setWord(QString word);
	void setTranslation(QString translation);
	void setWordOnly(bool wordOnly);

private:
	QString m_word;
	QString m_translation;
	bool m_wordOnly;
};

class HeadState : public PageState
{
	Q_OBJECT
public:
	explicit HeadState(QObject *parent = nullptr) : PageState(parent) {}
	HeadState(State state, State addState, QString desc, QObject *parent = nullptr);

	Q_PROPERTY(State otherState READ otherState WRITE setOtherState NOTIFY otherStateChanged)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

	State otherState() const;
	QString description() const;

	virtual QString dump() const override;
	virtual QString store() const override;
	virtual void load(QString ctx) override;

signals:
	void otherStateChanged(State otherState);
	void descriptionChanged(QString description);

public slots:
	void setOtherState(State otherState);
	void setDescription(QString description);

private:
	State m_otherState;
	QString m_description;
};

class StatState : public PageState
{
	Q_OBJECT

public:
	enum Types {
		None = 0,
		Errors,
		States,
		Speed
	};
	Q_ENUM(Types)

	static QString typeToString(Types type);
	static Types stringToType(QString str);

	explicit StatState(QObject *parent = nullptr) : PageState(parent) {}
	StatState(State state, Types type, QObject *parent = nullptr);

	Q_PROPERTY(Types type READ type WRITE setType NOTIFY typeChanged)
	Q_PROPERTY(BarSeries* series READ series)

	virtual QString dump() const override;
	virtual QString store() const override;
	virtual void load(QString ctx) override;

	Types type() const;

	BarSeries *series();

public slots:
	void setType(Types type);

signals:
	void typeChanged(Types type);

private:
	Types m_type;
	BarSeries m_series;
};

#endif // PAGESTATE_H
