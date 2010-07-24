#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <QString>
#include <QVector>
#include <QVariant>
#include "token.h"
#include "codetreenodes.h"

enum SymbolType { Invalid, VariableSymbol, FunctionSymbol, NativeFunctionSymbol };

class Symbol
{
public:
    Symbol() { name = ""; mType = Invalid; }

    SymbolType getType() const { return mType; }
    QString getName() const { return name; }
    void setName(const QString& n) { name = n; }

protected:
    QString name;
    SymbolType mType;
};

class VariableRow : public Symbol
{
public:
    VariableRow() { value << QString(""); mType = VariableSymbol; me = this; }

    QVariant getValue(int row = 0) const { return value.at(row); }
    QVector<QVariant> getValueArray() const { return value; }
    bool getStatic() const { return isStatic; }
    bool getArray() const { return isArray; }


    void setValue(const QVariant& v, int row = 0) {
        if(row >= value.size()) {
            value.resize(row+1);
        }
        value[row] = v;
    }
    void setValueArray(const QVector<QVariant>& v) { value = v; }
    void setStatic(bool s) { isStatic = s; }
    void setArray(bool a) { isArray = a; }

    void setPointer(VariableRow* newMe) { me = newMe; }
    VariableRow* getPointer() { return me; }

    void resetPointer() { me = this; }

    bool isPointer() { return me != this; }

private:
    VariableRow *me;
    QVector<QVariant> value;
    bool isStatic;
    bool isArray;
};

class FunctionRow : public Symbol
{
public:
    FunctionRow() { mType = FunctionSymbol; }

    QList<CodeTreeNode*> getBody() { return body; }
    void setBody(QList<CodeTreeNode*> b) { body = b; }

    FunctionNode* getNode() { return node; }
    void setNode(FunctionNode* n) { node = n; }

protected:
    FunctionNode* node;

private:
    QList<CodeTreeNode*> body;
};

class NativeFunctionRow : public FunctionRow
{
public:
    NativeFunctionRow() { mType = NativeFunctionSymbol; f = 0; }
    QVariant callFunction(QList<VariableRow*> args)
    {
        if(f) {
            return f(args);
        } else {
            return QVariant::Invalid;
        }
    }
    void setFunction(QVariant (*func)(QList<VariableRow*>)) { f = func; }

private:
    QVariant (*f)(QList<VariableRow*>);
};


#endif // SYMBOLS_H
