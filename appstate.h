#ifndef APPSTATE_H
#define APPSTATE_H

#include <QObject>
#include <QQuickItem>

#include "settings.h"
#include "pagestate.h"
#include "word.h"

class AppState : public QObject
{
	Q_OBJECT
public:
	enum Direction {
		Nowhere = 0,
		Up,
		Down,
		Left,
		Right
	};
	Q_ENUM(Direction)

	explicit AppState(QObject *parent = nullptr);

	Q_PROPERTY(PageState* upper READ upper)
	Q_PROPERTY(PageState* lower READ lower)
	Q_PROPERTY(PageState* left READ left)
	Q_PROPERTY(PageState* right READ right)
	Q_PROPERTY(PageState* page READ page)

	Q_PROPERTY(Settings* settings READ settings CONSTANT)

	// navigate
	Q_INVOKABLE void next(Direction dir);

	// initial list fill
	Q_INVOKABLE void populateDemo();
	Q_INVOKABLE void populateFile(QString filename);
	// for DB storage
	Q_INVOKABLE QList<int> wordIndexes() const;
	Q_INVOKABLE QVariantMap wordContents(int index) const;
	// for DB load
	Q_INVOKABLE void addWord(QVariantMap word);

	Settings* settings();

	PageState* upper() const;
	PageState* lower() const;
	PageState* left() const;
	PageState* right() const;
	PageState *page() const;

public slots:
	void setUpper(PageState *upper);
	void setLower(PageState* lower);
	void setLeft(PageState *left);
	void setRight(PageState* right);

private:
	void flipWord(bool ok);
	void showWord();

	Word curWord() const;
	Word nextWord() const;

	static bool showWordOnState(PageState::State state);
	static bool canShowWordOnState(PageState::State state);

	// init modes
	void newLearn();
	void newTrain();
	void newRepeat();
	void newErrors();
	void newCheck();

	void shuffle();

	void dump(QString prefix);

	QMap<int, Word> m_words;

	QMap<int, Word> m_changedWords;	// selected words
	QVector<int> m_selectedWords;	// indexes of them, shuffled
	QVector<int> m_errorWords;	// indexes of errors
	int m_currentWord;		// position in m_selectedWords

	Settings m_settings;

	PageState *m_upper;
	PageState *m_lower;
	PageState *m_left;
	PageState *m_right;
	PageState *m_page;
};

#endif // APPSTATE_H
