#include <iostream>
#include "interpretor.h"
#include "symbols.h"
#include "token.h"
#include "nativeinterface.h"
using std::cout;
using std::cerr;
using std::endl;

//#define DEBUG

Interpretor::Interpretor(Parser* p)
{
    myParser = p;
}

void Interpretor::beginInterpretation()
{
    loadGlobalSymbols();
    loadPlugins();
    Symbol* func = findSymbol("main");
    if(!func) {
        cerr << "No main function!" << endl;
        return;
    }
    if(func->getType() == FunctionSymbol)
        interpretFunction(static_cast<FunctionRow*>(func)); //Invoke main()
}

void Interpretor::loadGlobalSymbols()
{
    Scope *sc = new Scope();
    scopes.push(sc);
    for(int i = 0; i < myParser->getTree().size(); i++)
    {
        CodeTreeNode* node = myParser->getTree().at(i);
        if(node) {
            switch(node->myType())
            {
            case Definition:
                interpretDefinition(static_cast<DefinitionNode*>(node));
                break;
            case Function:
                addFunctionToScope(static_cast<FunctionNode*>(node));
                break;
            default:
                delete node;
                node = 0;
                myParser->getTree().removeAt(i--);
                break;
            }
        }
    }
}

void Interpretor::loadPlugins()
{
    QDir pluginsDir(QDir::currentPath());
    pluginsDir.cd("plugins");
    foreach(QString filename, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(filename));
        QObject *plugin = loader.instance();
        if(plugin)
        {
            NativeInterface* nativePlugin = qobject_cast<NativeInterface*>(plugin);
            for(int i = 0; i < nativePlugin->getFunctions().length(); i++) {
                scopes.top()->addSymbol(nativePlugin->getFunctions().at(i));
#ifdef DEBUG
                cout << "Adding native function: " << qPrintable(static_cast<NativeFunctionRow*>(nativePlugin->getFunctions().at(i))->getName()) << endl;
#endif
            }
        } else {
            cerr << "Bad library: " << qPrintable(pluginsDir.absoluteFilePath(filename)) << endl;
        }
    }
}

void Interpretor::interpretDefinition(DefinitionNode *def)
{
    Scope* currentScope = scopes.top();
    if(currentScope)
    {
        VariableRow *var = new VariableRow();
        var = var->getPointer();
        var->setName(def->getLValue()->getValue().getConstData());
        var->setStatic(false);
        QVariant type = interpretMath(def->getRValue());
        var->setValue(type);
        currentScope->addSymbol(var);
        if(type.canConvert(QVariant::Double)) { //Add error-checking
            QVariant up = interpretMath(def->getLValue()->getIndex());
            int q = up.toInt();
            if(q > 0) {
                var->setValue(0.0,q-1); //Do only one resize.
                for(int i = 1; i < q-1; i++)
                    var->setValue(0.0,i);
            }
        } else {
            QVariant up = interpretMath(def->getLValue()->getIndex());
            int q = up.toInt();
            if(q > 0) {
                var->setValue(0.0,q-1); //Do only one resize.
                for(int i = 1; i < q-1; i++)
                    var->setValue("",i);
            }
        }
        QString val = var->getValue().toString();
        if(val.isEmpty())
            val = "undefined";
#ifdef DEBUG
        cout << "Adding \"" << qPrintable(var->getName()) << "\" to scope with value: " << qPrintable(val) << endl;
#endif
    }
}

void Interpretor::addFunctionToScope(FunctionNode *func)
{
    Scope* currentScope = scopes.top();
    if(currentScope)
    {
        FunctionRow* f = new FunctionRow();
        f->setBody(func->getChildren());
        f->setName(func->getName().getConstData());
        f->setNode(func);
        currentScope->addSymbol(f);
    }
}

QVariant Interpretor::interpretFunction(FunctionRow *func, QList<VariableRow*> args)
{
    QVariant result = Invalid;
    if(func->getType() == NativeFunctionSymbol) {
//        QList<QVariant> nativeArgs;
//        foreach(VariableRow* var, args)
//        {
//            VariableRow* fullyResolved = var;
//            while(fullyResolved->isPointer())
//                fullyResolved = fullyResolved->getPointer();
//            nativeArgs << fullyResolved->getValue();
//        }
        return static_cast<NativeFunctionRow*>(func)->callFunction(args);
    }
    Scope *myScope = new Scope();
    scopes.push(myScope);
    foreach(VariableRow* var, args)
    {
        scopes.top()->addSymbol(var);
    }
    for(int i = 0; i < func->getBody().size(); i++)
    {
        CodeTreeNode* currentInstruction = func->getBody().at(i);
        switch(currentInstruction->myType())
        {
        case Definition:
            interpretDefinition(static_cast<DefinitionNode*>(currentInstruction));
            break;
        case Function:
            addFunctionToScope(static_cast<FunctionNode*>(currentInstruction));
            break;
        case Assignment:
            interpretAssignment(static_cast<AssignmentNode*>(currentInstruction));
            break;
        case Keyword:
            if(static_cast<KeywordNode*>(currentInstruction)->getWord().getConstData() == "return")
            {
                result = interpretMath(static_cast<ExpressionNode*>(static_cast<KeywordNode*>(currentInstruction)->getOperands().first()));
                return result;
            }
            break;
        case If:
            interpretIf(static_cast<IfNode*>(currentInstruction));
            break;
        case WhileLoop:
            interpretWhileLoop(static_cast<WhileLoopNode*>(currentInstruction));
            break;
        default:
            break;
        }
    }
    return result;
}

void Interpretor::interpretAssignment(AssignmentNode *assign)
{
    Symbol* l = findSymbol(assign->getLValue()->getValue().getConstData());
    if(l && static_cast<VariableRow*>(l))
    {
        int idx = 0;
        VariableRow* lval = static_cast<VariableRow*>(l)->getPointer();
        QVariant up = interpretMath(assign->getLValue()->getIndex());
        if(up.canConvert(QVariant::Double)) {
            double tmp = up.toDouble();
            idx = (int)tmp;
        }
        lval->setValue(interpretMath(assign->getRValue()),idx);
#ifdef DEBUG
        cout << "Assigning \"" << qPrintable(lval->getName()) << "\" in scope with value: " << qPrintable(lval->getValue(idx).toString()) << endl;
#endif
    }
    else
    {
        cerr << "WARN: Creating symbol \'" << qPrintable(assign->getLValue()->getValue().getConstData()) << "\'." << endl;
        DefinitionNode *tmp = new DefinitionNode();
        tmp->setLValue(assign->getLValue());
        tmp->setRValue(assign->getRValue());
        tmp->setParent(assign->getParent());
        interpretDefinition(tmp);
        delete tmp;
    }
}


void Interpretor::interpretWhileLoop(WhileLoopNode *whileNode)
{
    Scope *myScope = new Scope();
    scopes.push(myScope);

    QVariant conditionResult;
    conditionResult = interpretMath(whileNode->getCondition());
    while(int(conditionResult.toDouble()))
    {
        interpretBlock(whileNode->getChildren());
        conditionResult = interpretMath(whileNode->getCondition());
    }
    scopes.pop();
}


void Interpretor::interpretIf(IfNode *ifNode)
{
    Scope *myScope = new Scope();
    scopes.push(myScope);

    QVariant conditionResult;
    conditionResult = interpretMath(ifNode->getCondition());
    if(int(conditionResult.toDouble())) {
        interpretBlock(ifNode->getChildren());
    } else {
        foreach(ElseIfNode* elseIf, ifNode->getNeighbor())
        {
            if(elseIf->getCondition()) {
                conditionResult = interpretMath(elseIf->getCondition());
                if(int(conditionResult.toDouble()))
                {
                    interpretBlock(elseIf->getChildren());
                    break;
                }
            } else {
                interpretBlock(elseIf->getChildren());
                break;
            }
        }
    }
    scopes.pop();
}

void Interpretor::interpretBlock(QList<CodeTreeNode *>block)
{
    for(int i = 0; i < block.size(); i++)
    {
        CodeTreeNode* currentInstruction = block.at(i);
        switch(currentInstruction->myType())
        {
        case Definition:
            interpretDefinition(static_cast<DefinitionNode*>(currentInstruction));
            break;
        case Function:
            addFunctionToScope(static_cast<FunctionNode*>(currentInstruction));
            break;
        case Assignment:
            interpretAssignment(static_cast<AssignmentNode*>(currentInstruction));
            break;
        case If:
            interpretIf(static_cast<IfNode*>(currentInstruction));
            break;
        case WhileLoop:
            interpretWhileLoop(static_cast<WhileLoopNode*>(currentInstruction));
            break;
        default:
            break;
        }
    }
}

QVariant Interpretor::interpretMath(ExpressionNode *exp)
{
    QVariant result = QVariant::Invalid;
    if(!exp)
        return result;
    QVariant right, left;
    QString tmp = "";
    double l, r;
    bool okD1, okD2;
    l = r = 0;
    okD1 = okD2 = false;
    switch(exp->myType())
    {
    case Expression:
        left = interpretMath(exp->getLeft());
        right = interpretMath(exp->getRight());
        switch(exp->getOperator().getType()) {
        case Token::Addition:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = l + r;
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                result = left.toString() + right.toString();
            }
            break;
        case Token::Subtraction:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = l - r;
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else if(okD2) // STRING - NUM == STRING.CHOP([NUM])
            {
                QString tmp;
                tmp = left.toString();
                tmp.chop(floor(right.toDouble()));
                result = tmp;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::Multiplication:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = l * r;
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else if(okD2)
            {
                QString tmp;
                for(int i = 0; i < floor(r); i++) {
                    tmp += left.toString() + " ";
                }
                tmp.chop(1);
                result = tmp;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::Division:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                if(r == 0.0) {
                    cerr << "FATAL: Divide by 0!" << endl;
                    exit(1);
                }
                result = l / r;
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::Power:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = pow(l, r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::Equal:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l==r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::NotEqual:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l!=r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::GreaterThan:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l>r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::GreaterThanOrEqualTo:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l>=r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::LessThan:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l<r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::LessThanOrEqualTo:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l<=r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::And:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l&&r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::Or:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (l||r);
                if(exp->getNegative())
                    result = result.toDouble() * -1.0;
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        case Token::Xor:
            l = left.toDouble(&okD1);
            r = right.toDouble(&okD2);
            if(okD1 && okD2)
            {
                result = (int(l)^int(r));
                if(exp->getNegative()) {
                    result = result.toDouble() * -1.0;
                }
            }
            else
            {
                return QVariant::Invalid;
            }
            break;
        default:
            break;
        }
        if(static_cast<ExpressionFunctionNode*>(exp)->getNot()) {
            double r = !int(result.toDouble());
            result = QString("%1").arg(r);
        }
        break;
    case ExpressionConstant:
        tmp = static_cast<ExpressionConstantNode*>(exp)->getValue().getConstData();
        if(Symbol* s = findSymbol(tmp))
        {
            if(s->getType() == VariableSymbol)
                tmp = static_cast<VariableRow*>(s)->getPointer()->getValue().toString();
        }
        if(static_cast<ExpressionConstantNode*>(exp)->getNegative()) {
            double r = -1 * tmp.toDouble();
            tmp = QString("%1").arg(r);
        }
        if(static_cast<ExpressionConstantNode*>(exp)->getNot()) {
            double r = !int(tmp.toDouble());
            tmp = QString("%1").arg(r);
        }
        result = tmp;
        break;
    case ExpressionVariable:
        tmp = static_cast<ExpressionVariableNode*>(exp)->getValue().getConstData();
        if(Symbol* s = findSymbol(tmp))
        {
            if(s->getType() == VariableSymbol) {
                QVariant val = interpretMath(static_cast<ExpressionVariableNode*>(exp)->getIndex());
                tmp = static_cast<VariableRow*>(s)->getPointer()->getValue(int(val.toDouble())).toString();
            }
        }
        if(static_cast<ExpressionVariableNode*>(exp)->getNot()) {
            double r = !int(tmp.toDouble());
            tmp = QString("%1").arg(r);
        }
        result = tmp;
        break;
    case ExpressionFunction:
        Symbol* func;
        func = findSymbol(static_cast<ExpressionFunctionNode*>(exp)->getName().getConstData());
        if(!func) {
            cerr << "FATAL: Function \"" << qPrintable(static_cast<ExpressionFunctionNode*>(exp)->getName().getConstData())
                    << "\" not found!" << endl;
            exit(1);
        }
        if(func->getType() == FunctionSymbol || func->getType() == NativeFunctionSymbol) {
            QList<VariableRow*> vars;
            int i = 0;
            foreach(CodeTreeNode* arg, static_cast<ExpressionFunctionNode*>(exp)->getArgs())
            {
                VariableRow* r = new VariableRow();
                r->setName(static_cast<FunctionRow*>(func)->getNode()->getParameters().at(i)->getValue().getConstData());
                FunctionNode* originalFunction = static_cast<FunctionRow*>(func)->getNode();
                if(originalFunction->getParameters().at(i)->getPointer()) {
                    ExpressionVariableNode* argAsPtr = static_cast<ExpressionVariableNode*>(arg);
                    if(argAsPtr) {
                        r->setPointer(static_cast<VariableRow*>(findSymbol(argAsPtr->getValue().getConstData())));
                    }
                } else {
                    r->setValue(interpretMath(static_cast<ExpressionNode*>(arg)));
                }
                vars.append(r);
                i++;
            }
            result = interpretFunction(static_cast<FunctionRow*>(func), vars);
            if(static_cast<ExpressionFunctionNode*>(exp)->getNot()) {
                double r = !int(result.toDouble());
                result = QString("%1").arg(r);
            }
        }
        break;
    default:
        return QVariant::Invalid;
        break;
    }
    return result;
}

Symbol* Interpretor::findSymbol(const QString &name)
{
    for(int i = scopes.size() - 1; i >= 0; i--)
    {
        Scope* sc = scopes.at(i);
        for(int j = 0; j < sc->getSymbols().size(); j++)
        {
            Symbol* sym = sc->getSymbols().at(j);
            if(sym->getName() == name)
            {
                return sym;
            }
        }
    }
    return 0;
}
