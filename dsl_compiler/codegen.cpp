#include "node.h"
#include "codegen.h"
#include "parser.hpp"

#define ISTYPE(value, id) (value->getType()->getTypeID() == id)

using namespace std;

/* Compile the AST into a module */
void CodeGenContext::generateCode(NBlock& root)
{
	std::cout << "Generating code...\n";
	
	/* Create the top level interpreter function to call as entry */
	vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getInt64Ty(MyContext), makeArrayRef(argTypes), false);
	mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", mainFunction, 0);
	
	/* Push a new variable/block context */
	pushBlock(bblock);
	llvm::Value *mainRetVal = root.codeGen(*this); /* emit bytecode for the toplevel block */
	ReturnInst::Create(MyContext, mainRetVal , this -> currentBlock());
	popBlock();
	
	/* Print the bytecode in a human-readable format 
	   to see if our program compiled properly
	 */
	std::cout << "Code is generated.\n";
	// module->dump();

	legacy::PassManager pm;
	// TODO:
	pm.add(createPrintModulePass(outs()));
	pm.run(*module);
}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {
	std::cout << "Running code...\n";
	ExecutionEngine *ee = EngineBuilder( unique_ptr<Module>(module) ).create();
	ee->finalizeObject();
	vector<GenericValue> noargs;
	GenericValue v = ee->runFunction(mainFunction, noargs);
	// long long val = v.IntVal.getSExtValue();
	// std::cout << "Code was run.\n" << "res: " << val << std::endl;
	return v;
}

/* Returns an LLVM type based on the identifier */
static Type *typeOf(const NIdentifier& type) 
{
	if (type.name.compare("int") == 0) {
		return Type::getInt64Ty(MyContext);
	}
	else if (type.name.compare("double") == 0) {
		return Type::getDoubleTy(MyContext);
	}
	return Type::getVoidTy(MyContext);
}

//create loop statement
static Value* LogErrorV(const char *str) {
		fprintf(stderr, "Error: %s\n", str);
		return NULL;
}

static Value* CastToBoolean(CodeGenContext& context, Value* condValue){

    if( ISTYPE(condValue, Type::IntegerTyID) ){
        condValue = Builder.CreateIntCast(condValue, Type::getInt1Ty(MyContext), true);
        return Builder.CreateICmpNE(condValue, ConstantInt::get(Type::getInt1Ty(MyContext), 0, true));
    }else{
		std::cout<< "no castToBoolean" <<std::endl;
        return condValue;
    }
}

/* -- Code Generation -- */

Value* NInteger::codeGen(CodeGenContext& context)
{
	std::cout << "Creating integer: " << value << endl;
	return ConstantInt::get(Type::getInt64Ty(MyContext), value, true);
}

Value* NDouble::codeGen(CodeGenContext& context)
{
	std::cout << "Creating double: " << value << endl;
	return ConstantFP::get(Type::getDoubleTy(MyContext), value);
}

Value* NIdentifier::codeGen(CodeGenContext& context)
{
	std::cout << "Creating identifier reference: " << name << endl;
	if (context.locals().find(name) == context.locals().end()) {
		std::cerr << "undeclared variable " << name << endl;
		return NULL;
	}

	// return nullptr;  
	return new LoadInst(context.locals()[name]->getType(),context.locals()[name], name, false, context.currentBlock());
}

Value* NMethodCall::codeGen(CodeGenContext& context)
{
	Function *function = context.module->getFunction(id.name.c_str());
	if (function == NULL) {
		std::cerr << "no such function " << id.name << endl;
	}
	std::vector<Value*> args;
	ExpressionList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		args.push_back((**it).codeGen(context));
	}
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	std::cout << "Creating method call: " << id.name << endl;
	return call;
}

Value* NBinaryOperator::codeGen(CodeGenContext& context)
{

	std::cout << "Creating binary operation " << op << endl;
	Instruction::BinaryOps instr;
	CmpInst::Predicate com;
	switch (op) {
		case TPLUS: 	instr = Instruction::Add; goto math;
		case TMINUS: 	instr = Instruction::Sub; goto math;
		case TMUL: 		instr = Instruction::Mul; goto math;
		case TDIV: 		instr = Instruction::SDiv; goto math;

		case TCEQ: 		com = llvm::ICmpInst::ICMP_EQ; goto compare;
        case TCNE: 		com = llvm::ICmpInst::ICMP_NE; goto compare;
        case TCLT: 		com = llvm::ICmpInst::ICMP_SLT; goto compare;
        case TCLE: 		com = llvm::ICmpInst::ICMP_SLE; goto compare;
        case TCGT: 		com = llvm::ICmpInst::ICMP_SGT; goto compare;
        case TCGE: 		com = llvm::ICmpInst::ICMP_SGE; goto compare;
		
		/* TODO comparison */
	}
	return NULL;
math:
	return BinaryOperator::Create(instr, lhs.codeGen(context), 
		rhs.codeGen(context), "", context.currentBlock());
compare:
	return CmpInst::Create(llvm::Instruction::OtherOps::ICmp, com, lhs.codeGen(context), 
	rhs.codeGen(context), "", context.currentBlock());
}

Value* NAssignment::codeGen(CodeGenContext& context)
{
	std::cout << "Creating assignment for " << lhs.name << endl;
	if (context.locals().find(lhs.name) == context.locals().end()) {
		std::cerr << "undeclared variable " << lhs.name << endl;
		return NULL;
	}
	return new StoreInst(rhs.codeGen(context), context.locals()[lhs.name], false, context.currentBlock());
}

Value* NBlock::codeGen(CodeGenContext& context)
{
	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << endl;
	return last;
}

Value* NExpressionStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating code for " << typeid(expression).name() << endl;
	return expression.codeGen(context);
}

Value* NReturnStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating return code for " << typeid(expression).name() << endl;
	Value *returnValue = expression.codeGen(context);
	context.setCurrentReturnValue(returnValue);
	return returnValue;
}

Value* NVariableDeclaration::codeGen(CodeGenContext& context)
{
	std::cout << "Creating variable declaration " << type.name << " " << id.name << endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type),4, id.name.c_str(), context.currentBlock());
	context.locals()[id.name] = alloc;
	if (assignmentExpr != NULL) {
		NAssignment assn(id, *assignmentExpr);
		assn.codeGen(context);
	}
	return alloc;
}

Value* NExternDeclaration::codeGen(CodeGenContext& context)
{
    vector<Type*> argTypes;
    VariableList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++) {
        argTypes.push_back(typeOf((**it).type));
    }
    FunctionType *ftype = FunctionType::get(typeOf(type), makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, GlobalValue::ExternalLinkage, id.name.c_str(), context.module);
    return function;
}

Value* NFunctionDeclaration::codeGen(CodeGenContext& context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}
	FunctionType *ftype = FunctionType::get(typeOf(type), makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context.module);
	BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", function, 0);

	context.pushBlock(bblock);

	Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;

	for (it = arguments.begin(); it != arguments.end(); it++) {
		(**it).codeGen(context);
		
		argumentValue = &*argsValues++;
		argumentValue->setName((*it)->id.name.c_str());
		StoreInst *inst = new StoreInst(argumentValue, context.locals()[(*it)->id.name], false, bblock);
	}
	
	block.codeGen(context);
	ReturnInst::Create(MyContext, context.getCurrentReturnValue(), bblock);

	context.popBlock();
	std::cout << "Creating function: " << id.name << endl;
	return function;
}

////////////////////////////////////////////////////////////


//create if statement
llvm::Value *NIfStatement::codeGen(CodeGenContext &context) {
    llvm::Value *condV = condition.codeGen(context);
    if (!condV)
        return nullptr;
    
    llvm::Type *boolType = llvm::Type::getInt1Ty(MyContext);
    llvm::Value *zero = llvm::ConstantInt::getFalse(boolType);

    llvm::Function *currFunc = context.currentBlock()->getParent();

    llvm::BasicBlock *thenBB =
        llvm::BasicBlock::Create(MyContext, "then", currFunc);
    // llvm::BasicBlock *elseBB =
    //     llvm::BasicBlock::Create(MyContext, "else");
    llvm::BasicBlock *mergeBB =
        llvm::BasicBlock::Create(MyContext, "exitIf");

    llvm::CmpInst::Predicate predNe = llvm::CmpInst::ICMP_NE;
    llvm::Value *condInstr = llvm::ICmpInst::Create(llvm::Instruction::OtherOps::ICmp, predNe, condV,zero, "", context.currentBlock());

    llvm::BranchInst::Create(thenBB, mergeBB, condInstr, context.currentBlock());
    cout << "Created Branch Inst" << endl;

    // Set the current block to the "then" block
    context.pushBlockGlobal(thenBB);

    // Emit bytecode for "then" block
    llvm::Value *thenV = thenBlock.codeGen(context);
    if (!thenV)
        return nullptr;

    llvm::BranchInst::Create(mergeBB, context.currentBlock());

    context.popBlock();
    cout << "Created Branch Then" << endl;
    currFunc->getBasicBlockList().push_back(mergeBB);
    context.pushBlockGlobal(mergeBB);

    return condV;
}

Value* NLoopStatement::codeGen(CodeGenContext& context)
{
    llvm::Function* currFunc = context.currentBlock()->getParent();

    llvm::BasicBlock* loopConditionBB = llvm::BasicBlock::Create(MyContext, "loopcondition", currFunc);
    llvm::BasicBlock* loopBodyBB = llvm::BasicBlock::Create(MyContext, "loopbody");
    llvm::BasicBlock* loopMergeBB = llvm::BasicBlock::Create(MyContext, "loopmerge");

	llvm::BranchInst::Create(loopConditionBB, context.currentBlock());

    context.pushBlockGlobal(loopConditionBB);

    // Generate the condition value
    llvm::Value* condV = condition.codeGen(context);
    if (!condV)
        return nullptr;

    // Create the loop condition comparison instruction
    llvm::Type* boolType = llvm::Type::getInt1Ty(MyContext);
    llvm::Value* zero = llvm::ConstantInt::getFalse(boolType);
    llvm::CmpInst::Predicate predNe = llvm::CmpInst::ICMP_NE;
    llvm::Value* condInstr = llvm::ICmpInst::Create(llvm::Instruction::OtherOps::ICmp, predNe, condV, zero, "", context.currentBlock());

    // Create a branch instruction to transfer control flow based on the loop condition
    llvm::BranchInst::Create(loopBodyBB, loopMergeBB, condInstr, context.currentBlock());
	context.popBlock();

	// Set the current block to the "then" block
	currFunc->getBasicBlockList().push_back(loopBodyBB);
    context.pushBlockGlobal(loopBodyBB);

    // Emit bytecode for "then" block
    llvm::Value *blockV = block.codeGen(context);
    if (!blockV)
        return nullptr;

    llvm::BranchInst::Create(loopConditionBB, context.currentBlock());
    context.popBlock();
    std::cout << "Created Loop Block" << std::endl;
	currFunc->getBasicBlockList().push_back(loopMergeBB);
    context.pushBlockGlobal(loopMergeBB);
    std::cout << "Created Loop Merge" << std::endl;
    return condV;
}


/////////////////////////////////////////////////// DSL /////////////////////////////////////////////////

Value* NDSLCreationStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Creating person declaration " << person.name << endl;
	AllocaInst *alloc = new AllocaInst(Type::getInt64Ty(MyContext),4, person.name.c_str(), context.currentBlock());
	context.locals()[person.name] = alloc;
	NAssignment assn(person, exp);
	assn.codeGen(context);
	return alloc;
}

//create movement statement
Value* NDSLMovementStatement::codeGen(CodeGenContext& context)
{

	if (context.locals().find(person.name) == context.locals().end()) {
		std::cerr << "undeclared person " << person.name << endl;
		return NULL;
	}

	std::cout << "Creating movement statement " << endl;
	//get the value of the person
	Value *personValue = context.locals()[person.name];
	//get the value of the amount
	Value *amountValue = exp.codeGen(context);
	//get the value of the person's balance
	Value *personBalanceValue = new LoadInst(context.locals()[person.name]->getType(),context.locals()[person.name], person.name, false, context.currentBlock());
	//get the value of the person's balance + amount
	Instruction::BinaryOps instr;
	switch (op)
	{
	case TDEPOSIT:
    instr = Instruction::Add;
		break;
	case TWITHDRAW:
		instr = Instruction::Sub;
		break;
	default:
	  return NULL;
	}
	Value *newBalanceValue = BinaryOperator::Create(instr, personBalanceValue, amountValue, "", context.currentBlock());
	//store the new balance
	StoreInst *inst = new StoreInst(newBalanceValue, context.locals()[person.name], false, context.currentBlock());

	return personValue;
}

//create transfer statement
Value* NDSLTransferStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Creating transfer statement " << endl;

	if (context.locals().find(expender.name) == context.locals().end()) {
		std::cerr << "undeclared person " << expender.name << endl;
		return NULL;
	}

	if (context.locals().find(receiver.name) == context.locals().end()) {
		std::cerr << "undeclared person " << receiver.name << endl;
		return NULL;
	}
	//get the value of the expender
	Value *expenderValue = context.locals()[expender.name];
	//get the value of the amount
	Value *amountValue = exp.codeGen(context);
	//get the value of the receiver
	Value *receiverValue = context.locals()[receiver.name];
	//get the value of the receiver's balance
	Value *receiverBalanceValue = new LoadInst(context.locals()[receiver.name]->getType(),context.locals()[receiver.name], receiver.name, false, context.currentBlock());
	//get the value of the expender's balance
	Value *expenderBalanceValue = new LoadInst(context.locals()[expender.name]->getType(),context.locals()[expender.name], expender.name, false, context.currentBlock());
	//get the value of the expender's balance - amount
	Value *newExpenderBalanceValue = BinaryOperator::Create(Instruction::Sub, expenderBalanceValue, amountValue, "", context.currentBlock());
	//get the value of the receiver's balance + amount
	Value *newReceiverBalanceValue = BinaryOperator::Create(Instruction::Add, receiverBalanceValue, amountValue, "", context.currentBlock());
	//store the new expender balance
	StoreInst *inst = new StoreInst(newExpenderBalanceValue, context.locals()[expender.name], false, context.currentBlock());
	//store the new receiver balance
	StoreInst *inst2 = new StoreInst(newReceiverBalanceValue, context.locals()[receiver.name], false, context.currentBlock());

	return expenderValue;
}
