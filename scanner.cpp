#include <QtCore>
#include <iostream>
#include "scanner.h"
#include "token.h"
using namespace std;

Scanner::Scanner()
{
    fileText = "";
    keywords << "let" << "while" << "for" << "if" << "else" << "break" << "continue" << "func" << "to" << "return";
}

Scanner::Scanner(const QString &fileName)
{
    fileText = "";
    keywords << "let" << "while" << "for" << "if" << "else" << "break" << "continue" << "func" << "to" << "return";
    setInput(fileName);
}

bool Scanner::setInput(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream in(&file);
    fileText = in.readAll();
    return true;
}

inline bool isIdentifierCharacter(char c)
{
    return ( ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <='z')) || c == '_');
}

inline bool isNumerical(char c)
{
    return (c >= '0' && c <= '9');
}

inline bool isSymbol(char c)
{
    QString syms("+-*/%^;!<>(){}=,[]@:&|");
    return (syms.indexOf(QChar::fromAscii(c)) != -1);
}

Token::Type Scanner::determineKeyword(const QString &s)
{
    if(s == "let")
        return Token::Let;
    else if(s == "while")
        return Token::While;
    else if(s == "for")
        return Token::For;
    else if(s == "if")
        return Token::If;
    else if(s == "else")
        return Token::Else;
    else if(s == "break")
        return Token::Break;
    else if(s == "continue")
        return Token::Continue;
    else if(s == "func")
        return Token::Function;
    else if(s == "to")
        return Token::To;
    else if(s == "return")
        return Token::Return;
    else
        return Token::None;
}

inline bool isWhitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

Token::Type Scanner::determineOperator(const QString &s)
{
    char c1, c2, c3;
    c1 = c2 = c3 = 0;
    if(s.size() >= 1)
        c1 = s.at(0).toAscii();
    if(s.size() >= 2)
        c2 = s.at(1).toAscii();
    if(s.size() >= 3)
        c3 = s.at(2).toAscii();

    if(c1 == '>' && !c3) {
        if(c2 == '=')
            return Token::GreaterThanOrEqualTo;
        else if(!c2)
            return Token::GreaterThan;
    } else if(c1 == '<' && !c3) {
        if(c2 == '=')
            return Token::LessThanOrEqualTo;
        else if(!c2)
            return Token::LessThan;
    } else if(c1 == '=') {
        if(c2 == '=' && !c3)
            return Token::Equal;
        else if(c2 == '/' && c3 == '=')
            return Token::NotEqual;
        else if(!c2 && !c3)
            return Token::Assignment;
    } else if(c1 == '+' && !c3) {
        if(c2 == '=')
            return Token::AddAssignment;
        else if(c2 == '+')
            return Token::Increment;
        else if(!c2)
            return Token::Addition;
    } else if(c1 == '-' && !c3) {
        if(c2 == '=')
            return Token::SubAssignment;
        else if(c2 == '-')
            return Token::Decrement;
        else if(!c2)
            return Token::Subtraction;
    } else if(c1 == '*' && !c3) {
        if(c2 == '=')
            return Token::MultAsssignment;
        else if(c2 == '*')
            return Token::Power;
        else if(!c2)
            return Token::Multiplication;
    } else if(c1 == '/' && !c3) {
        if(c2 == '=')
            return Token::DivAssignment;
        else if(!c2)
            return Token::Division;
    } else if(c1 == '!') {
        if(c2 == '=' && !c3)
            return Token::NotEqual;
        else if(!c2 && !c3)
            return Token::Not;
    } else if(c1 == '&') {
        if(c2 == '&' && !c3)
            return Token::And;
    } else if(c1 == '|') {
        if(c2 == '|' && !c3)
            return Token::Or;
    } else if(c1 == '^' && !c2 && !c3) {
        return Token::Xor;
    } else if(c1 == ';' && !c2 && !c3) {
        return Token::Semicolon;
    } else if(c1 == '(' && !c2 && !c3) {
        return Token::OpenParen;
    } else if(c1 == ')' && !c2 && !c3) {
        return Token::CloseParen;
    } else if(c1 == '{' && !c2 && !c3) {
        return Token::OpenBlock;
    } else if(c1 == '}' && !c2 && !c3) {
        return Token::CloseBlock;
    } else if(c1 == '\"' && !c2 && !c3) {
        return Token::StringDelimiter;
    } else if(c1 == ',' && !c2 && !c3) {
        return Token::Comma;
    } else if(c1 == '%' && !c2 && !c3) {
        return Token::Modulo;
    } else if(c1 == '[' && !c2 && !c3) {
        return Token::ArrayOpen;
    } else if(c1 == ']' && !c2 && !c3) {
        return Token::ArrayClose;
    } else if(c1 == '@' && !c2 && !c3) {
        return Token::ReferenceType;
    } else if(c1 == ':' && c2 == '=' && !c3) {
        return Token::ReferenceAssignment;
    }
    return Token::None;
}

void Scanner::refreshTokens()
{
    enum State { Unknown, Identifier, Number, Operator, String };
    State state = Unknown;
    Token currentLexeme;
    char c;
    QString buffer = "";
    tokenized = false;

    for(int i = 0; i <= fileText.size(); i++)
    {
        if(i != fileText.size())
            c = fileText.at(i).toAscii();
        else
            c = 0;
        switch(state)
        {
        case Unknown:
            buffer += c;
            if(isIdentifierCharacter(c))
                state = Identifier;
            else if(c == '\"')
                state = String;
            else if(c == '/') {
                if(i+1 < fileText.size()) {
                    if(fileText.at(i+1).toAscii() == '/') {
                        buffer.chop(1);
                        while(i < fileText.size() && fileText.at(i).toAscii() != '\n')
                        {
                            i++;
                        }
                    }
                    else
                    {
                        state = Operator;
                    }
                }
            }
            else if(isNumerical(c))
                state = Number;
            else if(isSymbol(c))
                state = Operator;
            else {
                buffer.chop(1);
            }
            break;
        case Identifier:
            if(isIdentifierCharacter(c) || isNumerical(c)) {
                buffer += c;
            } else {
                if(keywords.contains(buffer))
                    currentLexeme.setType(determineKeyword(buffer));
                else
                    currentLexeme.setType(Token::Identifier);
                currentLexeme.setConstData(buffer);
                tokenList << currentLexeme;
                buffer = "";
                i--;
                state = Unknown;
            }
            break;
        case Number:
            if(isNumerical(c) || c == '.') {
                buffer += c;
            } else {
                currentLexeme.setType(Token::Number);
                currentLexeme.setConstData(buffer);
                tokenList << currentLexeme;
                buffer = "";
                i--;
                state = Unknown;
            }
            break;
        case Operator:
            int addedIndex;
            int j;
            bool good;
            addedIndex = 1;
            good = false;
            if(i+addedIndex < fileText.size()) {
                while(isSymbol(c))
                {
                    buffer += c;
                    addedIndex++;
                    if(addedIndex+i != fileText.size())
                        c = fileText.at(i+addedIndex).toAscii();
                    else
                        break;
                }
            }
            for(j = buffer.size()-1; j >= 0; j--) {
                QString foo = buffer.mid(0, j+1);
                if(determineOperator(buffer.mid(0, j+1)) != Token::None) {
                    good = true;
                    break;
                }
            }
            if(good) {
                currentLexeme.setType(determineOperator(buffer.mid(0, j+1)));
                currentLexeme.setConstData(buffer.mid(0, j+1));
                tokenList << currentLexeme;
                buffer = "";
                state = Unknown;
            } else {
                buffer = "";
                state = Unknown;
            }
            i += j - 1;
            break;
        case String:
            buffer.append(c);
            if(c == '\\') {
                if(i+1 < fileText.size()) {
                    i++;
                    buffer.chop(1);
                    switch(fileText.at(i).toAscii())
                    {
                    case '\\':
                        buffer.append('\\');
                        break;
                    case 'n':
                        buffer.append('\n');
                        break;
                    case '\"':
                        buffer.append('\"');
                        break;
                    default:
                        cerr << "Scanner: Unrecognised escape sequence. Omitting..." << endl;
                        break;
                    }
                }
            } else if(c == '\"') {
                buffer.chop(1);
                buffer.remove(0,1);
                currentLexeme.setType(Token::StringDelimiter);
                currentLexeme.setConstData("\"");
                tokenList << currentLexeme;
                currentLexeme.setType(Token::String);
                currentLexeme.setConstData(buffer);
                tokenList << currentLexeme;
                currentLexeme.setType(Token::StringDelimiter);
                currentLexeme.setConstData("\"");
                tokenList << currentLexeme;
                buffer = "";
                state = Unknown;
            }
            break;
        default:
            state = Unknown;
            break;
        }
    }
    tokenized = true;
}
