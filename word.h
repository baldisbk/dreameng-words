#ifndef WORD_H
#define WORD_H

struct Word
{
        Word(){}
        Word(QString _word, QString _translation):
        word(_word), translation(_translation) {}

        QString word;
        QString translation;
        int repeats;
        int errors;
};

#endif // WORD_H
