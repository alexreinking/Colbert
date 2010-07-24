#ifndef SCOPE_H
#define SCOPE_H

#include <QtCore>
#include "symbols.h"

/*
 * Holds a symbol table.
 * Held by Interpretor as a stack.
*/
class Scope
{
public:
    QList<Symbol*> getSymbols() const { return syms; }

    void setSymbols(QList<Symbol*> s) { syms = s; }
    void addSymbol(Symbol* s) { syms.append(s); }

private:
    QList<Symbol*> syms;
};

#endif // SCOPE_H
