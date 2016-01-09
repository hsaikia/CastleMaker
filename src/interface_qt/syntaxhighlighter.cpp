#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* document):
    QSyntaxHighlighter(document)
{}
SyntaxHighlighter::~SyntaxHighlighter()
{}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    enum { NewLiteral, //encountered at the beginning and after ';' is found
           NewFunc,    //encountered after '-->' is found
           Args,       //encountered after '(' is found
           Succs,      //encountered after '{' is found
           Prob,       //encountered after '[' is found
           Comment,    //encountered after '*/' is found
           Error       //when ';' or ',' or '[' is expected
         };
    int state = NewLiteral;
    int start = 0;

    for (int i = 0; i < text.length(); ++i)
    {
        if(state == Error){
            setFormat(i, 1, Qt::red);
        } else if(text.mid(i, 2) == "/*") {
            state = Comment;
            start = i;
            setFormat(i, 1, Qt::gray);
        } else if (state == Comment && text.mid(i, 2) != "*/"){
            setFormat(i, 1, Qt::gray);
        } else if (state == Comment && text.mid(i, 2) == "*/"){
            setFormat(i, i - start + 2, Qt::gray);
            i++;
            start = i + 1;
            state = NewLiteral;
        } else if(text.at(i) == ';' && state == Error && state != Comment){
            state = NewLiteral;
            setFormat(i, 1, Qt::gray);
            start = i + 1;
        }
        else if(text.mid(i, 3) == "-->" && state != Error && state != Comment){
            state = NewFunc;
            setFormat(start, i - start + 1, Qt::darkRed);
            setFormat(i, 3, Qt::blue);
            start = i + 3;
        }
        else if(text.at(i) == '=' && state != Error && state != Comment){
            state = NewFunc;
            setFormat(start, i - start + 1, Qt::darkRed);
            setFormat(i, 1, Qt::blue);
            start = i + 1;
        }
        else if(text.at(i) == '(' && state != Args && state != Error && state != Comment){
            state = Args;
            setFormat(start, i - start, Qt::darkGreen);
            start = i;
        }
        else if(text.at(i) == ')' && state != Error && state != Comment){
            state = Succs;
            setFormat(start, i - start + 1, Qt::darkBlue);
            start = i + 1;
        }
        else if(text.at(i) == '}' && state != Error && state != Comment){
            //state = Error;
            setFormat(start, i - start + 1, Qt::darkCyan);
            start = i + 1;
        }
    }
}
