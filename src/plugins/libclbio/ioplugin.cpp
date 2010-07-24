#include <iostream>
#include <string>
#include "ioplugin.h"
using namespace std;

unsigned long CodeTreeNode::id = 0;

QList<NativeFunctionRow*> IOPlugin::getFunctions()
{
    if(functionList.isEmpty()) {
        functionList << createPrint();
        functionList << createPrompt();
    }
    return functionList;
}

QVariant IOPlugin::___print(QList<QVariant> args)
{
    cout << qPrintable(args.first().toString());
    return 0;
}

NativeFunctionRow* IOPlugin::createPrint()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("print");
    func->setFunction(___print);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"print"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(false);
    syntheticArgument->setValue(Token(Token::Identifier,"str"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

QVariant IOPlugin::___prompt(QList<QVariant> args)
{
    cout << qPrintable(args.first().toString());
    string in;
    cin >> in;
    return QVariant(QString(in.c_str()));
}

NativeFunctionRow* IOPlugin::createPrompt()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("prompt");
    func->setFunction(___prompt);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"prompt"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(false);
    syntheticArgument->setValue(Token(Token::Identifier,"message"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

Q_EXPORT_PLUGIN2(clbio, IOPlugin)
