#include "compiler/CompilerContext.hpp"

llvm::LLVMContext &CompilerContext::getContext() { return context; }
llvm::Module &CompilerContext::getModule() { return module; }
llvm::IRBuilder<> &CompilerContext::getBuilder() { return builder; }
ContextTable &CompilerContext::getTable() { return contextTable; }
void CompilerContext::save(std::string filename)
{
    std::error_code errorCode;
    llvm::raw_fd_ostream outLL(filename, errorCode);
    module.print(outLL, nullptr);
}

void CompilerContext::setupExternalFunctions()
{
    auto bytePtrTy = builder.getInt8Ty()->getPointerTo();
    module.getOrInsertFunction("printf",
                               llvm::FunctionType::get(
                                   /*return type*/ builder.getInt32Ty(),
                                   /*format arg*/ bytePtrTy,
                                   /*vararg*/ true));

    module.getOrInsertFunction("strcmp",
                               llvm::FunctionType::get(
                                   builder.getInt32Ty(),                             // return type
                                   {builder.getInt8PtrTy(), builder.getInt8PtrTy()}, // parameter types
                                   false));
}
