#include <iostream>
#include "codegen.h"
#include "node.h"

using namespace std;

extern int yyparse();
extern NBlock* programBlock;


llvm::Function* createPrintfFunction(CodeGenContext& context)
{
    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(MyContext)); //char*

    std::cout << "printf" << std::endl;

    llvm::FunctionType* printf_type =
        llvm::FunctionType::get(
            llvm::Type::getInt32Ty(MyContext), printf_arg_types, true);

    llvm::Function *func = llvm::Function::Create(
                printf_type, llvm::Function::ExternalLinkage,
                llvm::Twine("printf"),
                context.module
           );
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}

void createCoreFunctions(CodeGenContext& context){
	llvm::Function* printfFn = createPrintfFunction(context);
}
