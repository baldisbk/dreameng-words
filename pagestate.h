#ifndef PAGESTATE_H
#define PAGESTATE_H

#include <QObject>

#include "word.h"

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

		Learn,
		Check,
		Errors,
		Train,
		Repeat
	};
	Q_ENUM(State)

	static QString stateToString(State state);

	explicit PageState(QObject *parent = nullptr) : QObject(parent) {}
	PageState(State state, QObject *parent = nullptr);

	Q_PROPERTY(State status READ status WRITE setStatus NOTIFY statusChanged)
	State status() const;

	virtual QString dump() const;

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

class StatState : public PageState
{
	Q_OBJECT
public:
	explicit StatState(QObject *parent = nullptr) : PageState(parent) {}
	StatState(State state, State addState, QObject *parent = nullptr);

	Q_PROPERTY(State otherState READ otherState WRITE setOtherState NOTIFY otherStateChanged)
	State otherState() const;

	virtual QString dump() const override;

signals:
	void otherStateChanged(State otherState);

public slots:
	void setOtherState(State otherState);

private:
	State m_otherState;
};

#endif // PAGESTATE_H