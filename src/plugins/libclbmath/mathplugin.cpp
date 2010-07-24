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
        functionList << createTan();
    }
    return functionList;
}

QVariant MathPlugin::___cos(QList<VariableRow*> args)
{
    if(args.length() != 1)
        return QVariant::Invalid;
    bool ok;
    double angle = args.first()->getValue().toDouble(&ok);
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

QVariant MathPlugin::___sin(QList<VariableRow*> args)
{
    if(args.length() != 1)
        return QVariant::Invalid;
    bool ok;
    double angle = args.first()->getValue().toDouble(&ok);
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

QVariant MathPlugin::___tan(QList<VariableRow*> args)
{
    if(args.length() != 1)
        return QVariant::Invalid;
    bool ok;
    double angle = args.first()->getValue().toDouble(&ok);
    if(ok)
        return tan(angle);
    return QVariant::Invalid;
}

NativeFunctionRow* MathPlugin::createTan()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("tan");
    func->setFunction(___tan);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"tan"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(false);
    syntheticArgument->setValue(Token(Token::Identifier,"r"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

Q_EXPORT_PLUGIN2(clbmath, MathPlugin)
