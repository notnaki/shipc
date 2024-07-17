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
                                   builder.getInt32Ty(),
                                   false));

    module.getOrInsertFunction("snprintf",
                               llvm::FunctionType::get(
                                   llvm::Type::getInt32Ty(context),
                                   {llvm::Type::getInt8PtrTy(context), llvm::Type::getInt64Ty(context), llvm::Type::getInt8PtrTy(context)},
                                   true));
}

void CompilerContext::setTable(ContextTable t)
{
    contextTable = t;
}