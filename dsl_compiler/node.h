#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node {
public:
	virtual ~Node() {}
	virtual llvm::Value* codeGen(CodeGenContext& context) { return NULL; }
};

class NExpression : public Node {
};

class NStatement : public Node {
};

class NInteger : public NExpression {
public:
	long long value;
	NInteger(long long value) : value(value) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NIdentifier : public NExpression {
public:
	std::string name;
	NIdentifier(const std::string& name) : name(name) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NMethodCall : public NExpression {
public:
	const NIdentifier& id;
	ExpressionList arguments;
	NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
		id(id), arguments(arguments) { }
	NMethodCall(const NIdentifier& id) : id(id) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression {
public:
	int op;
	NExpression& lhs;
	NExpression& rhs;
	NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
		lhs(lhs), rhs(rhs), op(op) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NAssignment : public NExpression {
public:
	NIdentifier& lhs;
	NExpression& rhs;
	NAssignment(NIdentifier& lhs, NExpression& rhs) : 
		lhs(lhs), rhs(rhs) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBlock : public NExpression {
public:
	StatementList statements;
	NBlock() { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement {
public:
	NExpression& expression;
	NExpressionStatement(NExpression& expression) : 
		expression(expression) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NReturnStatement : public NStatement {
public:
	NExpression& expression;
	NReturnStatement(NExpression& expression) : 
		expression(expression) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NVariableDeclaration : public NStatement {
public:
	const NIdentifier& type;
	NIdentifier& id;
	NExpression *assignmentExpr;
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
		type(type), id(id) { assignmentExpr = NULL; }
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
		type(type), id(id), assignmentExpr(assignmentExpr) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExternDeclaration : public NStatement {
public:
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList arguments;
    NExternDeclaration(const NIdentifier& type, const NIdentifier& id,
            const VariableList& arguments) :
        type(type), id(id), arguments(arguments) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionDeclaration : public NStatement {
public:
	const NIdentifier& type;
	const NIdentifier& id;
	VariableList arguments;
	NBlock& block;
	NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, 
			const VariableList& arguments, NBlock& block) :
		type(type), id(id), arguments(arguments), block(block) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

//////////////////////////////////////////////////

//create if statement
class NIfStatement : public NStatement {
public:
	NExpression& condition;
	NBlock& thenBlock;
	NIfStatement(NExpression& condition, NBlock& block) :
		condition(condition), thenBlock(block) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

//create Loop statement
class NLoopStatement : public NStatement {
public:
	NExpression& condition;
	NBlock& block;
	NLoopStatement(NExpression& condition, NBlock& block) : 
		condition(condition), block(block) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

//create Loop statement
// class NLoopStatement : public NStatement {
// public:
// 	NExpression& condition;
// 	NBlock& block;
// 	NLoopStatement(NExpression& condition, NBlock& block) : 
// 		condition(condition), block(block) { }
// 	virtual llvm::Value* codeGen(CodeGenContext& context);
// };


/////////////////////////////////////////////////// DSL /////////////////////////////////////////////////

class NDSLMovementStatement : public NStatement {
public:
	int op;
	NIdentifier& person;
	NExpression& exp;
	NDSLMovementStatement(int op, NIdentifier& person, NExpression& exp) :
		op(op), person(person), exp(exp) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDSLCreationStatement : public NStatement {
public:
	NIdentifier& person;
	NExpression& exp;
	NDSLCreationStatement(NIdentifier& person, NExpression& exp) : 
	person(person), exp(exp) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};
class NDSLTransferStatement : public NStatement {
public:
	NIdentifier& expender;
	NIdentifier& receiver;
	NExpression& exp;
	NDSLTransferStatement(NIdentifier& expender, NIdentifier& receiver, NExpression& exp) : 
	expender(expender), receiver(receiver), exp(exp) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};