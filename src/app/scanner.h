#ifndef SCANNER_H
#define SCANNER_H

#include <QtCore>
#include "token.h"

class Scanner
{
public:
    Scanner();
    Scanner(const QString &fileName);

    bool isTokenized() { return tokenized; }
    QList<Token> getTokens() const { return tokenList; }
    QList<Token>* getMutableTokens() { return &tokenList; }
    const QString originalText() const { return fileText; }
    bool setInput(const QString &fileName);
    void refreshTokens();


private:
    Token::Type determineOperator(const QString &s);
    Token::Type determineKeyword(const QString &s);
    QString myFile;

    QString fileText;
    QList<Token> tokenList;
    QStringList keywords;
    bool tokenized;
};

#endif // SCANNER_H
