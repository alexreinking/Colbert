#include <QtCore>
#include <iostream>
#include "scanner.h"
#include "parser.h"
#include "codetreenodes.h"
using namespace std;

Parser::Parser(Scanner *input)
{
    myScanner = input;
}

void Parser::buildTree()
{
    int index = 0;
    codeTree = parseBlock(0, index);
}

QList<CodeTreeNode*> Parser::parseBlock(CodeTreeNode *parent, int& index)
{
    Token currentToken = myScanner->getTokens().at(index);
    QList<CodeTreeNode*> currentTree;
    CodeTreeNode* tmp;
    QList<CodeTreeNode*> tmpList;
    while(index < myScanner->getTokens().size()) {
        currentToken = myScanner->getTokens().at(index);
        switch(currentToken.getType())
        {
        case Token::Let:
            tmp = parseLet(parent, index);
            if(tmp) {
                currentTree << tmp;
            } else {
                while(myScanner->getTokens().at(index++).getType() != Token::Semicolon) ;
            }
            break;
        case Token::Identifier:
            tmp = parseAssignment(parent, index);
            if(tmp)
                currentTree << tmp;
            else {
                while(myScanner->getTokens().at(index++).getType() != Token::Semicolon) ;
            }
            break;
        case Token::If:
            tmpList = parseIf(parent, index);
            if(tmpList.size())
                currentTree << tmpList;
            else {
                while(myScanner->getTokens().at(index++).getType() != Token::Semicolon) ;
            }
            break;
        case Token::While:
            tmpList = parseWhile(parent, index);
            if(tmpList.size())
                currentTree << tmpList;
            else {
                while(myScanner->getTokens().at(index++).getType() != Token::Semicolon) ;
            }
            break;
        case Token::For:
            tmpList = parseFor(parent, index);
            if(tmpList.size())
                currentTree << tmpList;
            else {
                while(myScanner->getTokens().at(index++).getType() != Token::Semicolon) ;
            }
            break;
        case Token::Function:
            tmpList = parseFunc(parent, index);
            if(tmpList.size())
                currentTree << tmpList;
            else {
                while(myScanner->getTokens().at(index++).getType() != Token::Semicolon) ;
            }
            break;
        case Token::Break:
        case Token::Continue:
        case Token::Return:
            CodeTreeNode* kn;
            kn = new KeywordNode();
            static_cast<KeywordNode*>(kn)->setWord(currentToken);
            static_cast<KeywordNode*>(kn)->setParent(parent);
            index+=2;
            if(currentToken.getType() == Token::Return)
            {
                index--;
                static_cast<KeywordNode*>(kn)->setOperands(QList<CodeTreeNode*>() << parseLogic(parent,index));
                index++;
            }
            currentTree << kn;
            break;
        case Token::None:
        case Token::CloseBlock:
            index++;
            return currentTree;
        default:
            break;
        }
    }
    return currentTree;
}

CodeTreeNode* Parser::parseLet(CodeTreeNode *parent, int &index)
{
    CodeTreeNode *n = new DefinitionNode();
    ExpressionVariableNode *lval;

    if(myScanner->getTokens().at(index).getType() != Token::Let)
    {
        cerr << "Parser: Malformed let expression!" << endl;
        return 0;
    }
    index++;

    lval = parseLValue(n,index);

    if(myScanner->getTokens().at(index).getType() == Token::Assignment)
    {
        index++; //Skip =
        static_cast<DefinitionNode*>(n)->setRValue(static_cast<ExpressionNode*>(parseLogic(parent, index))); //Ends at semicolon
    }
    else if(myScanner->getTokens().at(index).getType() != Token::Semicolon)
    {
        cerr << "Parser: Malformed let expression!" << endl;
        return 0;
    }
    n->setParent(parent);
    static_cast<DefinitionNode*>(n)->setLValue(lval);
    index++;
    return n;
}

CodeTreeNode* Parser::parseAssignment(CodeTreeNode *parent, int& index) //Will convert ++ statements to addition
{
    CodeTreeNode *n = new AssignmentNode();
    ExpressionVariableNode *lval;

    if(myScanner->getTokens().at(index).getType() != Token::Identifier)
    {
        cerr << "Parser: Malformed assignment!" << endl;
        return 0;
    }
    lval = parseLValue(n,index);

    //Branches end at semicolon
    if(myScanner->getTokens().at(index).getType() == Token::Assignment)
    {
        n = new AssignmentNode();
        index++; //Skip =
        static_cast<AssignmentNode*>(n)->setRValue(static_cast<ExpressionNode*>(parseLogic(parent, index)));
        static_cast<AssignmentNode*>(n)->setLValue(lval);
    }
    else if(myScanner->getTokens().at(index).getType() == Token::ReferenceAssignment)
    {
        n = new ReferenceAssignmentNode();
        index++;
        static_cast<ReferenceAssignmentNode*>(n)->setLValue(lval);
        static_cast<ReferenceAssignmentNode*>(n)->setRValue(parseLValue(n,index));
    }
    else
    {
        index++;
        cerr << "Parser: Malformed assignment!" << endl;
        return 0;
    }
    n->setParent(parent);
    index++;
    return n;
}

ExpressionVariableNode* Parser::parseLValue(CodeTreeNode *parent, int &index, bool allowRef)
{
    ExpressionVariableNode* ret = new ExpressionVariableNode();

    while(index < myScanner->getTokens().size())
    {
        Token currentToken = myScanner->getTokens().at(index);
        if(myScanner->getTokens().at(index).getType() == Token::ReferenceType) {
            ret->setPointer(allowRef);
            index++;
            currentToken = myScanner->getTokens().at(index);
        }
        if(myScanner->getTokens().at(index).getType() == Token::Identifier) {
            ret->setParent(parent);
            ret->setValue(currentToken);

            if(myScanner->getTokens().at(index+1).getType() == Token::ArrayOpen) {
                index+=2;
                CodeTreeNode* idx = parseLogic(ret,index);
                ret->setIndex(static_cast<ExpressionNode*>(idx));
                if(myScanner->getTokens().at(index).getType() == Token::ArrayClose) {
                    index++;
                    break;
                } else {
                    delete ret;
                    return 0;
                }
            } else {
                index++;
                ExpressionConstantNode *idx = new ExpressionConstantNode();
                idx->setValue(Token(Token::Number,"0"));
                ret->setIndex(idx);
            }
            break;
        } else {
            delete ret;
            return 0;
        }
    }
    return ret;
}

QList<CodeTreeNode*> Parser::parseIf(CodeTreeNode *parent, int &index)
{
    QList<CodeTreeNode*> branch;

    while(index < myScanner->getTokens().size())
    {
        if(myScanner->getTokens().at(index).getType() == Token::If)
        {
            if(branch.size() == 0) {
                branch.append(new IfNode());
                static_cast<IfNode*>(branch.first())->setParent(parent);
            }
            index++;
            if(myScanner->getTokens().at(index).getType() == Token::OpenParen)
            {
                index++;
                ExpressionNode* cond = static_cast<ExpressionNode*>(parseLogic(branch.last(),index));
                if(branch.size() > 1)
                    static_cast<ElseIfNode*>(branch.last())->setCondition(cond);
                else
                    static_cast<IfNode*>(branch.first())->setCondition(cond);
                index++;

                cout << "HERE! Token Info(" << index << ") const-data: " << qPrintable(myScanner->getTokens().at(index).getConstData()) << endl;

                if(myScanner->getTokens().at(index).getType() == Token::OpenBlock)
                {
                    index++;
                    if(branch.size() > 1)
                        static_cast<ElseIfNode*>(branch.last())->setChildren(parseBlock(branch.last(), index));
                    else
                        static_cast<IfNode*>(branch.first())->setChildren(parseBlock(branch.first(), index));
                }
            }
            else
            {
                cerr << "Parser: Malformed if statement!" << endl;
                return QList<CodeTreeNode*>();
            }
        }
        else if(myScanner->getTokens().at(index).getType() == Token::Else && branch.size())
        {
            branch.append(new ElseIfNode());
            static_cast<IfNode*>(branch.first())->addNeighbor(static_cast<ElseIfNode*>(branch.last()));
            static_cast<ElseIfNode*>(branch.last())->setParent(parent);
            index++; //move on to the IF/{
            if(myScanner->getTokens().at(index).getType() == Token::OpenBlock)
            {
                static_cast<ElseIfNode*>(branch.last())->setConditionActive(false);
                static_cast<ElseIfNode*>(branch.last())->setCondition(0);
                static_cast<ElseIfNode*>(branch.last())->setChildren(parseBlock(branch.first(), ++index));
            }
        }
        else {
            break;
        }
    }
    return branch;
}

QList<CodeTreeNode*> Parser::parseWhile(CodeTreeNode *parent, int &index)
{
    QList<CodeTreeNode*> branch;
    while(index < myScanner->getTokens().size())
    {
        Token currentToken = myScanner->getTokens().at(index);
        if(myScanner->getTokens().at(index).getType() == Token::While)
        {
            branch.append(new WhileLoopNode());
            static_cast<WhileLoopNode*>(branch.last())->setParent(parent);
            index++;
            if(myScanner->getTokens().at(index).getType() == Token::OpenParen)
            {
                index++;
                int depth = 1;
                QList<Token> cond;
                while(index < myScanner->getTokens().size() && depth)
                {
                    if(myScanner->getTokens().at(index).getType() == Token::CloseParen)
                        depth--;
                    else if(myScanner->getTokens().at(index).getType() == Token::OpenParen)
                        depth++;
                    cond << myScanner->getTokens().at(index++);
                }
                cond.removeLast();
                static_cast<WhileLoopNode*>(branch.last())->setCondition(cond);
                if(myScanner->getTokens().at(index).getType() == Token::OpenBlock)
                {
                    index++;
                    static_cast<WhileLoopNode*>(branch.last())->setChildren(parseBlock(branch.last(), index));
                }
            }
            else
            {
                cerr << "Parser: Malformed while loop!" << endl;
                return QList<CodeTreeNode*>();
            }
        }
        else {
            break;
        }
    }
    return branch;
}

QList<CodeTreeNode*> Parser::parseFor(CodeTreeNode *parent, int &index)
{
    QList<CodeTreeNode*> branch;

    while(index < myScanner->getTokens().size())
    {
        Token currentToken = myScanner->getTokens().at(index);
        if(myScanner->getTokens().at(index).getType() == Token::For)
        {
            branch.append(new ForLoopNode());
            static_cast<ForLoopNode*>(branch.last())->setParent(parent);
            index++;
            if(myScanner->getTokens().at(index).getType() == Token::Identifier)
            {
                static_cast<ForLoopNode*>(branch.last())->setIdentifier(myScanner->getTokens().at(index++));
                if(myScanner->getTokens().at(index).getType() == Token::Assignment)
                {
                    index++;
                    int depth = 1;
                    QList<Token> init;
                    while(index < myScanner->getTokens().size() && depth)
                    {
                        if(myScanner->getTokens().at(index).getType() == Token::To)
                            depth--;
                        init << myScanner->getTokens().at(index++);
                    }
                    init.removeLast();
                    static_cast<ForLoopNode*>(branch.last())->setInitializer(init);

                    init.clear();
                    depth = 1;
                    while(index < myScanner->getTokens().size() && depth)
                    {
                        if(myScanner->getTokens().at(index).getType() == Token::OpenBlock)
                            depth--;
                        init << myScanner->getTokens().at(index++);
                    }
                    init.removeLast();
                    index--;
                    static_cast<ForLoopNode*>(branch.last())->setTarget(init);

                    if(myScanner->getTokens().at(index).getType() == Token::OpenBlock)
                    {
                        index++;
                        static_cast<ForLoopNode*>(branch.last())->setChildren(parseBlock(branch.last(), index));
                    }
                }
            }
            else
            {
                cerr << "Parser: Malformed for loop!" << endl;
                return QList<CodeTreeNode*>();
            }
        }
        else {
            break;
        }
    }

    return branch;
}

QList<CodeTreeNode*> Parser::parseFunc(CodeTreeNode *parent, int &index)
{
    QList<CodeTreeNode*> branch;
    while(index < myScanner->getTokens().size())
    {
        Token currentToken = myScanner->getTokens().at(index);
        if(myScanner->getTokens().at(index).getType() == Token::Function)
        {
            branch.append(new FunctionNode());
            static_cast<FunctionNode*>(branch.last())->setParent(parent);
            index++;
            if(myScanner->getTokens().at(index).getType() == Token::Identifier)
            {
                static_cast<FunctionNode*>(branch.last())->setName(myScanner->getTokens().at(index++));
                if(myScanner->getTokens().at(index).getType() == Token::OpenParen)
                {
                    QList<ExpressionVariableNode*> args;
                    while(index < myScanner->getTokens().size())
                    {
                        index++;
                        Token::Type t = myScanner->getTokens().at(index).getType();
                        if(t == Token::CloseParen)
                            break;
                        if(t == Token::Identifier || t == Token::ReferenceType) {
                            args << parseLValue(branch.last(),index, true);
                            index--;
                        } else if(t != Token::Comma) {
                            cerr << "Malformed function!" << endl;
                            return QList<CodeTreeNode*>();
                        }
                    }
                    static_cast<FunctionNode*>(branch.last())->setParameters(args);
                    if(myScanner->getTokens().at(++index).getType() == Token::OpenBlock)
                    {
                        index++;
                        static_cast<FunctionNode*>(branch.last())->setChildren(parseBlock(branch.last(), index));
                    }
                }
                else
                {
                    cerr << "Parser: Malformed function!" << endl;
                    return QList<CodeTreeNode*>();
                }
            }
            else
            {
                cerr << "Parser: Malformed function!" << endl;
                return QList<CodeTreeNode*>();
            }
        }
        else {
            break;
        }
    }
    return branch;
}

/* logic:
   expr && expr
   expr || expr
   expr == expr
   expr != expr
*/

CodeTreeNode* Parser::parseLogic(CodeTreeNode* parent, int& index)
{
    CodeTreeNode *top = parseExpression(parent, index);
    while(index < myScanner->getTokens().size())
    {
        Token op = myScanner->getTokens().at(index);
        switch(op.getType())
        {
        case Token::And:
        case Token::Or:
        case Token::Equal:
        case Token::NotEqual:
        case Token::LessThan:
        case Token::LessThanOrEqualTo:
        case Token::GreaterThan:
        case Token::GreaterThanOrEqualTo:
            index++;
            break;
        default:
            return top;
            break;
        }

        ExpressionNode* tmp = new ExpressionNode();
        CodeTreeNode *right = parseExpression(parent,index);
        tmp->setLeft(static_cast<ExpressionNode*>(top));
        tmp->setRight(static_cast<ExpressionNode*>(right));
        tmp->setOperator(op);
        tmp->setParent(parent);
        top = tmp;
    }
    return top;
}

CodeTreeNode* Parser::parseExpression(CodeTreeNode* parent, int& index)
{
    CodeTreeNode *top = parseAddition(parent, index);
    while(index < myScanner->getTokens().size())
    {
        Token op = myScanner->getTokens().at(index);
        if(op.getType() != Token::Xor)
            return top;
        index++;

        ExpressionNode *tmp = new ExpressionNode();
        CodeTreeNode *right = parseAddition(parent, index);
        static_cast<ExpressionNode*>(tmp)->setLeft(static_cast<ExpressionNode*>(top));
        static_cast<ExpressionNode*>(tmp)->setRight(static_cast<ExpressionNode*>(right));
        static_cast<ExpressionNode*>(tmp)->setOperator(op);
        static_cast<ExpressionNode*>(tmp)->setParent(parent);
        top = tmp;
    }
    return top;
}

CodeTreeNode* Parser::parseAddition(CodeTreeNode* parent, int& index)
{
    CodeTreeNode *top = parseMultiplication(parent, index);
    while(index < myScanner->getTokens().size())
    {
        Token op = myScanner->getTokens().at(index);
        if(op.getType() != Token::Addition && op.getType() != Token::Subtraction)
            return top;
        index++;

        CodeTreeNode *tmp = new ExpressionNode();
        CodeTreeNode *right = parseMultiplication(parent, index);
        static_cast<ExpressionNode*>(tmp)->setLeft(static_cast<ExpressionNode*>(top));
        static_cast<ExpressionNode*>(tmp)->setRight(static_cast<ExpressionNode*>(right));
        static_cast<ExpressionNode*>(tmp)->setOperator(op);
        static_cast<ExpressionNode*>(tmp)->setParent(parent);
        top = tmp;
    }
    return top;
}

CodeTreeNode* Parser::parseMultiplication(CodeTreeNode* parent, int& index)
{
    CodeTreeNode *top = parsePower(parent, index);
    while(index < myScanner->getTokens().size())
    {
        Token op = myScanner->getTokens().at(index);
        if(op.getType() != Token::Multiplication && op.getType() != Token::Division && op.getType() != Token::Modulo)
            return top;
        index++;

        CodeTreeNode *tmp = new ExpressionNode();
        CodeTreeNode *right = parsePower(parent, index);
        static_cast<ExpressionNode*>(tmp)->setLeft(static_cast<ExpressionNode*>(top));
        static_cast<ExpressionNode*>(tmp)->setRight(static_cast<ExpressionNode*>(right));
        static_cast<ExpressionNode*>(tmp)->setOperator(op);
        static_cast<ExpressionNode*>(tmp)->setParent(parent);
        top = tmp;
    }
    return top;
}

CodeTreeNode* Parser::parsePower(CodeTreeNode* parent, int& index)
{
    CodeTreeNode *top = parseFactor(parent, index);
    while(index < myScanner->getTokens().size())
    {
        Token op = myScanner->getTokens().at(index);
        if(op.getType() != Token::Power)
            return top;
        index++;

        CodeTreeNode *tmp = new ExpressionNode();
        CodeTreeNode *right = parseFactor(parent, index);
        static_cast<ExpressionNode*>(tmp)->setLeft(static_cast<ExpressionNode*>(top));
        static_cast<ExpressionNode*>(tmp)->setRight(static_cast<ExpressionNode*>(right));
        static_cast<ExpressionNode*>(tmp)->setOperator(op);
        static_cast<ExpressionNode*>(tmp)->setParent(parent);
        top = tmp;
    }
    return top;
}

CodeTreeNode* Parser::parseFactor(CodeTreeNode* parent, int& index)
{
    CodeTreeNode *top = 0;
    bool neg = false;
    bool hasNot = false;
    if(myScanner->getTokens().at(index).getType() == Token::Subtraction) {
        neg = true;
        index++;
    } else if(myScanner->getTokens().at(index).getType() == Token::Not) {
        hasNot = true;
        index++;
    }
    if(myScanner->getTokens().at(index).getType() == Token::Number)
    {
        top = new ExpressionConstantNode();
        static_cast<ExpressionConstantNode*>(top)->setValue(myScanner->getTokens().at(index));
        index++;
    } else if(myScanner->getTokens().at(index).getType() == Token::Identifier) {
        if(myScanner->getTokens().at(index+1).getType() == Token::OpenParen)
        {
            top = new ExpressionFunctionNode();
            static_cast<ExpressionFunctionNode*>(top)->setName(myScanner->getTokens().at(index));
            index+=2;
            QList<CodeTreeNode*> args;
            do
            {
                if(myScanner->getTokens().at(index).getType() == Token::Comma)
                    index++;
                else if(myScanner->getTokens().at(index).getType() == Token::CloseParen) {
                    index++;
                    break;
                }
                args.append(parseLogic(top, index));
            } while(true) ;
            static_cast<ExpressionFunctionNode*>(top)->setArgs(args);
        }
        else
        {
            top = parseLValue(0,index);
        }
    } else if(myScanner->getTokens().at(index).getType() == Token::StringDelimiter) {
        index++;
        if(myScanner->getTokens().at(index).getType() == Token::String) {
            top = new ExpressionConstantNode();
            static_cast<ExpressionConstantNode*>(top)->setValue(myScanner->getTokens().at(index));
            index++;
            if(myScanner->getTokens().at(index).getType() != Token::StringDelimiter) {
                index++;
                delete top;
                return 0;
            }
            index++;
        }
    } else if(myScanner->getTokens().at(index).getType() == Token::OpenParen) {
        index++;
        top = parseLogic(parent, index);
        if(myScanner->getTokens().at(index).getType() != Token::CloseParen)
            return 0;
        index++;
        if(neg && static_cast<ExpressionNode*>(top)->getNegative())
        {
            static_cast<ExpressionNode*>(top)->setNegative(false);
        } else if(neg || static_cast<ExpressionNode*>(top)->getNegative()) {
            static_cast<ExpressionNode*>(top)->setNegative(true);
        } else {
            static_cast<ExpressionNode*>(top)->setNegative(false);
        }
        static_cast<ExpressionNode*>(top)->setParent(parent);
        static_cast<ExpressionNode*>(top)->setNot(hasNot);
        return top;
    } else {
        return 0;
    }
    static_cast<ExpressionNode*>(top)->setNegative(neg);
    static_cast<ExpressionNode*>(top)->setNot(hasNot);
    static_cast<ExpressionNode*>(top)->setParent(parent);
    return top;
}

void Parser::printTree(QList<CodeTreeNode*> nodes)
{
    static int d = 0;
    QString tmp = "";
    QString indent = "";
    for(int i = 0; i < d; i++)
    {
        indent += "    ";
    }
    foreach(CodeTreeNode* n, nodes)
    {
        tmp = "";
        if(!n) {
            qDebug("%s", qPrintable(QString("%1").arg("FATAL: MEMORY LEAK")));
            exit(1);
        }
        switch(n->myType())
        {
        case Definition:
            cout << qPrintable(indent) << "Definition:" << n->getParent() << endl;
            cout << qPrintable(indent) << "LVal:\t" << qPrintable(static_cast<DefinitionNode*>(n)->getLValue()->getValue().getConstData()) << endl;
            if(static_cast<DefinitionNode*>(n)->getRValue() == 0)
                tmp = "<No RValue>";
            else {
                tmp = "(";
                tmp += printExp(QList<ExpressionNode*>() << static_cast<DefinitionNode*>(n)->getRValue());
                tmp += ")";
            }
            cout << qPrintable(indent) << "RVal:\t" << qPrintable(tmp) << endl << endl;
            break;
        case Assignment:
            cout << qPrintable(indent) << "Assignment:" << n->getParent() << endl;
            cout << qPrintable(indent) << "LVal:\t" << qPrintable(static_cast<AssignmentNode*>(n)->getLValue()->getValue().getConstData()) << endl;
            if(static_cast<AssignmentNode*>(n)->getRValue() == 0)
                tmp = "<No RValue>";
            else {
                tmp = "(";
                tmp += printExp(QList<ExpressionNode*>() << static_cast<AssignmentNode*>(n)->getRValue());
                tmp += ")";
            }
            cout << qPrintable(indent) << "RVal:\t" << qPrintable(tmp) << endl << endl;
            break;
        case ReferenceAssignment:
            cout << qPrintable(indent) << "Reference Assignment:" << n->getParent() << endl;
            cout << qPrintable(indent) << "LVal:\t" << qPrintable(static_cast<AssignmentNode*>(n)->getLValue()->getValue().getConstData()) << endl;
            if(static_cast<ReferenceAssignmentNode*>(n)->getRValue() == 0)
                tmp = "<No RValue>";
            else {
                tmp = "(";
                tmp += printExp(QList<ExpressionNode*>() << static_cast<ReferenceAssignmentNode*>(n)->getRValue());
                tmp += ")";
            }
            cout << qPrintable(indent) << "RVal:\t" << qPrintable(tmp) << endl << endl;
            break;
        case If:
            tmp = "";
            cout << qPrintable(indent) << "If( " << qPrintable(printExp(QList<ExpressionNode*>() << static_cast<IfNode*>(n)->getCondition() )) << " ): " << qPrintable(tmp) << " : " << n->getParent() << endl;
            cout << qPrintable(indent) << "|--- Children ---|" << endl;
            d++;
            printTree(static_cast<IfNode*>(n)->getChildren());
            cout << qPrintable(indent) << "|---    End   ---|" << endl;
            d--;
            break;
        case ElseIf:
            tmp = "";
            cout << qPrintable(indent) << "Else If( " << qPrintable(printExp(QList<ExpressionNode*>() << static_cast<ElseIfNode*>(n)->getCondition() )) << " ): " << qPrintable(tmp) << " : " << n->getParent() << endl;
            cout << qPrintable(indent) << "|--- Children ---|" << endl;
            d++;
            printTree(static_cast<ElseIfNode*>(n)->getChildren());
            cout << qPrintable(indent) << "|---    End   ---|" << endl;
            d--;
            break;
        case WhileLoop:
            tmp = "";
            foreach(Token t, static_cast<WhileLoopNode*>(n)->getCondition()) {
                tmp += t.getConstData();
            }
            cout << qPrintable(indent) << "While: " << qPrintable(tmp) << " : " << n->getParent() << endl;
            cout << qPrintable(indent) << "|--- Children ---|" << endl;
            d++;
            printTree(static_cast<WhileLoopNode*>(n)->getChildren());
            cout << qPrintable(indent) << "|---    End   ---|" << endl;
            d--;
            break;
        case ForLoop:
            tmp = static_cast<ForLoopNode*>(n)->getIdentifier().getConstData();
            tmp += "=";
            foreach(Token t, static_cast<ForLoopNode*>(n)->getInitializer()) {
                tmp += t.getConstData();
            }
            cout << qPrintable(indent) << "For: " << qPrintable(tmp);
            tmp = "";
            foreach(Token t, static_cast<ForLoopNode*>(n)->getTarget()) {
                tmp += t.getConstData();
            }
            cout << " to " << qPrintable(tmp) << " : " << n->getParent() << endl;
            cout << qPrintable(indent) << "|--- Children ---|" << endl;
            d++;
            printTree(static_cast<ForLoopNode*>(n)->getChildren());
            cout << qPrintable(indent) << "|---    End   ---|" << endl;
            d--;
            break;
        case Function:
            cout << "Function " << qPrintable(static_cast<FunctionNode*>(n)->getName().getConstData()) << " : " << static_cast<FunctionNode*>(n)->getParent() << endl;
            tmp = "Arguments: ";
            foreach(ExpressionVariableNode* t, static_cast<FunctionNode*>(n)->getParameters()) {
                tmp += t->getValue().getConstData() + ",";
            }
            tmp.chop(1);
            cout << qPrintable(tmp) << endl;

            cout << qPrintable(indent) << "|--- Children ---|" << endl;
            d++;
            printTree(static_cast<FunctionNode*>(n)->getChildren());
            cout << qPrintable(indent) << "|---    End   ---|" << endl;
            d--;
            break;
        case Keyword:
            cout << qPrintable(indent) << "Keyword(" << static_cast<KeywordNode*>(n)->getParent() << "): " << qPrintable(static_cast<KeywordNode*>(n)->getWord().getConstData()) << endl;
            cout << qPrintable(indent) << "  Value: (";
            cout << qPrintable(printExp(QList<ExpressionNode*>() << static_cast<ExpressionNode*>(static_cast<KeywordNode*>(n)->getOperands().first())));
            cout << ")" << endl;
            break;
        default:
            break;
        }
    }
}

QString Parser::printExp(QList<ExpressionNode*> nodes)
{
    QString tmp;
    foreach(ExpressionNode* node, nodes)
    {
        if(!node)
            continue;
        switch(node->myType())
        {
        case Expression:
            tmp += node->getOperator().getConstData();
            if(node->getNegative())
                tmp += "-";
            if(node->getNot())
                tmp += "!";
            tmp += " (";
            tmp += printExp(QList<ExpressionNode*>() << node->getLeft());
            tmp += ")  (";
            tmp += printExp(QList<ExpressionNode*>() << node->getRight());
            tmp += ")";
            break;
        case ExpressionVariable:
        case ExpressionConstant:
            if(node->getNegative())
                tmp += "-";
            if(node->getNot())
                tmp += "!";
            tmp += static_cast<ExpressionConstantNode*>(node)->getValue().getConstData();
            break;
        case ExpressionFunction:
            if(node->getNot())
                tmp += "!";
            tmp += "call " + static_cast<ExpressionFunctionNode*>(node)->getName().getConstData();
        default:
            break;
        }
    }
    return tmp;
}
