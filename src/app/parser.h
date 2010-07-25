#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "codetreenodes.h"

class Parser
{
public:
    Parser(Scanner *input);
    void buildTree();
    void printTree(QList<CodeTreeNode*> nodes);
    QList<CodeTreeNode*> getTree() { return codeTree; }

private:
    QList<CodeTreeNode*> parseBlock(CodeTreeNode* parent, int& index);
    QList<CodeTreeNode*> parseIf(CodeTreeNode* parent, int& index); // ((else)) if ( cond ) { ... }
    QList<CodeTreeNode*> parseWhile(CodeTreeNode* parent, int& index); // while ( cond ) { ... }
    QList<CodeTreeNode*> parseFor(CodeTreeNode* parent, int& index); // for ID=EXP to EXP { ... }
    QList<CodeTreeNode*> parseFunc(CodeTreeNode* parent, int& index); // func ID( [ID,]+ ) { ... }

    CodeTreeNode* parseLet(CodeTreeNode* parent, int& index); //let LVALUE (= EXP);
    CodeTreeNode* parseAssignment(CodeTreeNode* parent, int& index); // LVALUE (= EXP|:= LVALUE);

    ExpressionVariableNode* parseLValue(CodeTreeNode* parent, int& index, bool allowRef = false); //((@))ID(([EXP]))

    CodeTreeNode* parseLogic(CodeTreeNode* parent, int& index); //Only builds a tree, does NOT evaluate.
    CodeTreeNode* parseIntermediateLogic(CodeTreeNode* parent, int& index);
    CodeTreeNode* parseExpression(CodeTreeNode* parent, int& index);
    CodeTreeNode* parseAddition(CodeTreeNode* parent, int& index);
    CodeTreeNode* parseMultiplication(CodeTreeNode* parent, int& index);
    CodeTreeNode* parsePower(CodeTreeNode* parent, int& index);
    CodeTreeNode* parseFactor(CodeTreeNode* parent, int& index);

    QString printExp(QList<ExpressionNode*> nodes);

    QList<CodeTreeNode*> codeTree;
    Scanner *myScanner;
};

#endif // PARSER_H
