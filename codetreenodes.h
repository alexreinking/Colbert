#ifndef CODETREENODES_H
#define CODETREENODES_H

/*
  * Scoping is managed by the interpretor. The varaibles in functions are stored as tokens(wrapped by Variable) and added
  * to the current scope at run time. When compiled, they should be turned into mkvar instructions,
  * initialized to 0.0, a neutral value for doubles, or "" strings. ((use mkstrvar instead))

  * All of these classes represent a single node in the parse tree. Some can have no children, and so are "neutered"
  * of that internal knowledge. Those which do have children (While, For, If, Else(If), and Functions) do know their children.
  * Any identifier with parent 0x0 is considered part of the global scope.

  * No node needs to know anything about scoping rules. This is handled by the semantic analyzer, which looks
  * up tokens, and builds scope, and runs in parallel to the interpretor.
*/

#include <QList>
#include "token.h"

enum NodeType { None, Assignment, ReferenceAssignment, Definition, Function, Keyword,
                WhileLoop, ForLoop, If, ElseIf,
                Expression, ExpressionConstant, ExpressionFunction, ExpressionVariable };

class ExpressionNode;
class ExpressionConstantNode;
class ExpressionFunctionNode;
class ExpressionVariableNode;

class CodeTreeNode
{
public:
    CodeTreeNode() { id++; parentId = 0; mType = None; }

    void setParent(CodeTreeNode *par)
    { parentId = par; }
    CodeTreeNode* getParent()
    { return parentId; }

    virtual NodeType myType()
    { return mType; }

protected:
    static unsigned long id;
    CodeTreeNode *parentId;
    NodeType mType;
};

class AssignmentNode : public CodeTreeNode
{
public:
    AssignmentNode() { mType = Assignment; lvalue = 0; rvalue = 0; }

    //    void setLValue(QList<Token> lval)
    void setLValue(ExpressionVariableNode* lval)
    { lvalue = lval; }
    void setRValue(ExpressionNode *rval)
    { rvalue = rval; }

    //    QList<Token> getLValue() { return lvalue; }
    ExpressionVariableNode* getLValue() { return lvalue; }
    ExpressionNode* getRValue() { return rvalue; }

private:
    //    QList<Token> lvalue;
    ExpressionVariableNode* lvalue;
    ExpressionNode* rvalue;
};

class ReferenceAssignmentNode : public CodeTreeNode
{
public:
    ReferenceAssignmentNode() { mType = ReferenceAssignment; lvalue = 0; rvalue = 0; }

    void setLValue(ExpressionVariableNode* lval)
    { lvalue = lval; }
    void setRValue(ExpressionVariableNode *rval)
    { rvalue = rval; }

    ExpressionVariableNode* getLValue() { return lvalue; }
    ExpressionVariableNode* getRValue() { return rvalue; }

private:
    ExpressionVariableNode* lvalue;
    ExpressionVariableNode* rvalue;
};

class DefinitionNode : public CodeTreeNode
{
public:
    DefinitionNode() { mType = Definition; lvalue = 0; rvalue = 0; }

    //    void setLValue(QList<Token> lval)
    void setLValue(ExpressionVariableNode* lval)
    { lvalue = lval; }
    void setRValue(ExpressionNode *rval)
    { rvalue = rval; }

    //    QList<Token> getLValue() { return lvalue; }
    ExpressionVariableNode* getLValue() { return lvalue; }
    ExpressionNode* getRValue() { return rvalue; }

private:
    //    QList<Token> lvalue;
    ExpressionVariableNode* lvalue;
    ExpressionNode* rvalue;
};

class FunctionNode : public CodeTreeNode
{
public:
    FunctionNode() { mType = Function; name = Token(); }

    //    void setParameters(QList<Token> params)
    void setParameters(QList<ExpressionVariableNode*> params)
    { parameters = params; }
    void setChildren(QList<CodeTreeNode*> kids)
    { children = kids; }
    void addChild(CodeTreeNode* kid)
    { children.append(kid); }
    void setName(Token n) { name = n; }

    //    QList<Token> getParameters() { return parameters; }
    QList<ExpressionVariableNode*> getParameters() const { return parameters; }
    QList<CodeTreeNode*> getChildren() const { return children; }
    Token getName() { return name; }

private:
    QList<ExpressionVariableNode*> parameters;
    QList<CodeTreeNode*> children;
    Token name;
};

/*
  * Loops
  * -----
  * Loops are used to repeat something until a condition is met, with a counter.
  * The scope of loops save for the counter is reset with each iteration.
*/

class WhileLoopNode : public CodeTreeNode
{
public:
    WhileLoopNode() { mType = WhileLoop; }

    void setCondition(QList<Token> cond)
    { condition = cond; }
    void setChildren(QList<CodeTreeNode*> kids)
    { children = kids; }
    void addChild(CodeTreeNode* kid)
    { children.append(kid); }

    QList<Token> getCondition() { return condition; }
    QList<CodeTreeNode*> getChildren() { return children; }

private:
    QList<Token> condition;
    QList<CodeTreeNode*> children;
};

class ForLoopNode : public CodeTreeNode
{
public:
    ForLoopNode() { mType = ForLoop; identifier = Token(); }

    void setIdentifier(Token ident)
    { identifier = ident; }
    void setInitializer(QList<Token> init)
    { initializer = init; }
    void setChildren(QList<CodeTreeNode*> kids)
    { children = kids; }
    void addChild(CodeTreeNode* kid)
    { children.append(kid); }
    void setTarget(QList<Token> t)
    { target = t; }

    QList<Token> getInitializer() { return initializer; }
    QList<Token> getTarget() { return target; }
    QList<CodeTreeNode*> getChildren() { return children; }
    Token getIdentifier() { return identifier; }

private:
    QList<CodeTreeNode*> children;
    QList<Token> target;
    QList<Token> initializer;
    Token identifier;
};

class ElseIfNode;
class IfNode : public CodeTreeNode
{
public:
    IfNode() { mType = If; }

    void setCondition(ExpressionNode* cond)
    { condition = cond; }
    void setChildren(QList<CodeTreeNode*> kids)
    { children = kids; }
    void addChild(CodeTreeNode* kid)
    { children.append(kid); }
    void addNeighbor(ElseIfNode* n)
    { neighbors.append(n); }

    ExpressionNode* getCondition() { return condition; }
    QList<CodeTreeNode*> getChildren() { return children; }
    QList<ElseIfNode*> getNeighbor() { return neighbors; }

private:
    ExpressionNode* condition;
    QList<CodeTreeNode*> children;
    QList<ElseIfNode*> neighbors;
};

class ElseIfNode : public IfNode
{
public:
    ElseIfNode() { mType = ElseIf; cond = false; }

    void setCondition(ExpressionNode* cond)
    { condition = cond; }
    void setChildren(QList<CodeTreeNode*> kids)
    { children = kids; }
    void addChild(CodeTreeNode* kid)
    { children.append(kid); }
    void setConditionActive(bool t)
    { cond = t; }

    ExpressionNode* getCondition() { return condition; }
    QList<CodeTreeNode*> getChildren() { return children; }

private:
    ExpressionNode* condition;
    QList<CodeTreeNode*> children;
    bool cond;
};

class KeywordNode : public CodeTreeNode
{
public:
    KeywordNode() { mType = Keyword; word = Token(); }
    void setWord(Token name) { word = name; }
    void setOperands(QList<CodeTreeNode*> op) { operands = op; }
    Token getWord() { return word; }
    QList<CodeTreeNode*> getOperands() { return operands; }

private:
    QList<CodeTreeNode*> operands;
    Token word;
};

class ExpressionNode : public CodeTreeNode
{
public:
    ExpressionNode() { mType = Expression; negative = false; hasNot = false; op = Token(); }

    void setOperator(Token o) { op = o; }
    void setLeft(ExpressionNode* l) { left = l; }
    void setRight(ExpressionNode* r) { right = r; }

    ExpressionNode* getLeft() { return left; }
    ExpressionNode* getRight() { return right; }
    Token getOperator() { return op; }

    bool getNegative() { return negative; }
    void setNegative(bool n) { negative = n; }

    bool getNot() { return hasNot; }
    void setNot(bool n) { hasNot = n; }

private:
    ExpressionNode* left;
    ExpressionNode* right;
    Token op;
    bool negative;
    bool hasNot;
};

class ExpressionConstantNode : public ExpressionNode
{
public:
    ExpressionConstantNode() { mType = ExpressionConstant; value = Token(); }
    void setValue(Token t) { value = t; }
    Token getValue() { return value; }

protected:
    Token value;
};

class ExpressionVariableNode : public ExpressionConstantNode
{
public:
    ExpressionVariableNode() { mType = ExpressionVariable; isPointer = false; }

    void setIndex(ExpressionNode* idx) { index = idx; }
    ExpressionNode* getIndex() const { return index; }

    void setPointer(bool p) { isPointer = p; }
    bool getPointer() { return isPointer; }

private:
    ExpressionNode* index;
    bool isPointer;
};

class ExpressionFunctionNode : public ExpressionNode
{
public:
    ExpressionFunctionNode() { mType = ExpressionFunction; name = Token(); }
    void setName(Token t) { name = t; }
    Token getName() { return name; }

    void setArgs(QList<CodeTreeNode*> args) { parameters = args; }
    QList<CodeTreeNode*> getArgs() { return parameters; }

private:
    Token name;
    QList<CodeTreeNode*> parameters;
};

#endif // CODETREENODES_H
