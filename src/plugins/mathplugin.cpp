#include <iostream>
#include <string>
#include "mathplugin.h"
using namespace std;

unsigned long CodeTreeNode::id = 0;

QList<NativeFunctionRow*> MathPlugin::getFunctions()
{
    if(functionList.isEmpty()) {

        functionList << createCos();
        functionList << createSin();
        functionList << createPrint();
        functionList << createPrompt();
    }
    return functionList;
}

QVariant MathPlugin::___cos(QList<QVariant> args)
{
    if(args.length() != 1)
        return QVariant::Invalid;
    bool ok;
    double angle = args.first().toDouble(&ok);
    if(ok)
        return cos(angle);
    return QVariant::Invalid;
}

NativeFunctionRow* MathPlugin::createCos()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("cos");
    func->setFunction(___cos);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"cos"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(false);
    syntheticArgument->setValue(Token(Token::Identifier,"r"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

QVariant MathPlugin::___sin(QList<QVariant> args)
{
    if(args.length() != 1)
        return QVariant::Invalid;
    bool ok;
    double angle = args.first().toDouble(&ok);
    if(ok)
        return sin(angle);
    return QVariant::Invalid;
}

NativeFunctionRow* MathPlugin::createSin()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("sin");
    func->setFunction(___sin);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"sin"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(false);
    syntheticArgument->setValue(Token(Token::Identifier,"r"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

QVariant MathPlugin::___print(QList<QVariant> args)
{
    cout << qPrintable(args.first().toString());
    return 0;
}

NativeFunctionRow* MathPlugin::createPrint()
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

QVariant MathPlugin::___prompt(QList<QVariant> args)
{
    cout << qPrintable(args.first().toString());
    string in;
    cin >> in;
    return QVariant(QString(in.c_str()));
}

NativeFunctionRow* MathPlugin::createPrompt()
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

Q_EXPORT_PLUGIN2(clb_math, MathPlugin)
