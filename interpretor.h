#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include <QStack>
#include <QList>
#include "scope.h"
#include "parser.h"
#include "codetreenodes.h"

class Interpretor
{
public:
    Interpretor(Parser* p);
    void beginInterpretation();

private:
    void loadGlobalSymbols();

    void interpretDefinition(DefinitionNode* def);
    void interpretAssignment(AssignmentNode* assign);

    QVariant interpretFunction(FunctionRow* func, QList<VariableRow*> args = QList<VariableRow*>());
    QVariant interpretMath(ExpressionNode* exp);

    void addFunctionToScope(FunctionNode* func);
    void addFunctionToScope(FunctionNode* func, Scope* scope);

    Symbol* findSymbol(const QString &name);
    QVariant getVariableValue(const QString &name);

    Parser* myParser;
    QStack<Scope*> scopes;
};

#endif // INTERPRETOR_H
