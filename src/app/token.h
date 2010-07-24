#ifndef TOKEN_H
#define TOKEN_H

#include <QString>

class Token
{
public:
    enum Type { None, Let, While, For, Break, Continue, If, Else, Function, Number, Operator, Literal, Identifier,
              GreaterThan, LessThan, GreaterThanOrEqualTo, LessThanOrEqualTo, Equal, NotEqual,
              Assignment, AddAssignment, SubAssignment, MultAsssignment, DivAssignment, Addition, Subtraction,
              Multiplication, Division, Modulo, Power, Xor, And, Or, Increment, Decrement, Not, Semicolon,
              OpenParen, CloseParen, OpenBlock, CloseBlock, ArrayOpen, ArrayClose, StringDelimiter, String, Comma, To, Return,
              ReferenceAssignment, ReferenceType };

    Token() {}
    Token(Type t, const QString &text) { myType = t; constData = text; }

    Type getType() const { return myType; }
    QString getConstData() const { return constData; }

    void setType(Type t) { myType = t; }
    void setConstData(const QString& t) { constData = t; }

private:
    Type myType;
    QString constData; //Might change to accomodate classes/structures
};

#endif // TOKEN_H
