#include <iostream>
#include <string>
#include "strplugin.h"
using namespace std;

unsigned long CodeTreeNode::id = 0;

QList<NativeFunctionRow*> StringPlugin::getFunctions()
{
    if(functionList.isEmpty()) {
        functionList << createSplit();
        functionList << createStrlen();
        functionList << createJoin();
    }
    return functionList;
}

QVariant StringPlugin::__split(QList<VariableRow*> args)
{
    if(args.length() != 1)
        return -1;
    VariableRow* var = args.first()->getPointer();
    QString str = var->getValue().toString();
    for(int i = 0; i < str.length(); i++)
        var->setValue(str.at(i),i);
    return 0;
}

NativeFunctionRow* StringPlugin::createSplit()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("split");
    func->setFunction(__split);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"split"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(true);
    syntheticArgument->setValue(Token(Token::Identifier,"str"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

QVariant StringPlugin::__join(QList<VariableRow*> args)
{
    if(args.length() != 1)
        return -1;
    VariableRow* var = args.first()->getPointer();
    QString full = "";
    foreach(QVariant item, var->getValueArray()) {
        full += item.toString();
    }
    var->setValueArray(QVector<QVariant>() << full);
    return full;
}

NativeFunctionRow* StringPlugin::createJoin()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("join");
    func->setFunction(__join);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"join"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(true);
    syntheticArgument->setValue(Token(Token::Identifier,"str"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

QVariant StringPlugin::__strlen(QList<VariableRow*> args)
{
    if(args.length() != 1)
        return QVariant::Invalid;
    VariableRow* var = args.first();
    QString str = var->getValue().toString();
    return str.length();
}

NativeFunctionRow* StringPlugin::createStrlen()
{
    NativeFunctionRow* func = new NativeFunctionRow();
    func->setName("strlen");
    func->setFunction(__strlen);

    FunctionNode* syntheticNode = new FunctionNode();
    syntheticNode->setName(Token(Token::Identifier,"strlen"));

    ExpressionVariableNode* syntheticArgument = new ExpressionVariableNode();
    syntheticArgument->setPointer(false);
    syntheticArgument->setValue(Token(Token::Identifier,"str"));

    syntheticNode->setParameters(QList<ExpressionVariableNode*>() << syntheticArgument);

    func->setNode(syntheticNode);
    return func;
}

Q_EXPORT_PLUGIN2(clbstr, StringPlugin)
