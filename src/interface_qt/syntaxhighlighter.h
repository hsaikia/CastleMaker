#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QtGui>
#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    explicit SyntaxHighlighter(QTextDocument* document = 0);
    ~SyntaxHighlighter();
    void highlightBlock(const QString &text);

};

#endif // SYNTAXHIGHLIGHTER_H
