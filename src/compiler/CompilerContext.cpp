#include "compiler/CompilerContext.hpp"
#include "compiler/ContextTable.hpp"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"

llvm::LLVMContext &CompilerContext::getContext() { return context; }
llvm::Module &CompilerContext::getModule() { return module; }
llvm::IRBuilder<> &CompilerContext::getBuilder() { return builder; }
ContextTable &CompilerContext::getTable() { return contextTable; }

void CompilerContext::emitIR(std::string filename)
{
    std::error_code errorCode;
    llvm::raw_fd_ostream outLL(filename, errorCode);
    module.print(outLL, nullptr);
}

void CompilerContext::createObjectFile(std::string filename){
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string targetTriple = llvm::sys::getDefaultTargetTriple();
    module.setTargetTriple(targetTriple);

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

    if (!target) {
        llvm::errs() << error;
        return;
    }

    auto cpu = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);

    module.setDataLayout(targetMachine->createDataLayout());

    llvm::legacy::PassManager pm;

    pm.add(llvm::createInstructionCombiningPass());
    pm.add(llvm::createReassociatePass());
    pm.add(llvm::createGVNPass());
    pm.add(llvm::createCFGSimplificationPass());

    pm.run(module);

    std::error_code errorCode;
    llvm::raw_fd_ostream dest(filename, errorCode, llvm::sys::fs::OF_None);

    if (errorCode){
        llvm::errs() << "Could not open file: " << errorCode.message();
        return;
    }


    llvm::legacy::PassManager pass;
    auto fileType = llvm::CGFT_ObjectFile;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)){
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return;
    }

    pass.run(module);
    dest.flush();

    llvm::outs() << "Wrote " << filename << "\n";


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

llvm::Constant *CompilerContext::getTrueStrConst()
{
    if (!trueStrConst)
    {
        trueStrConst = getBuilder().CreateGlobalStringPtr("true", "true.str");
    }
    return trueStrConst;
}

llvm::Constant *CompilerContext::getFalseStrConst()
{
    if (!falseStrConst)
    {
        falseStrConst = getBuilder().CreateGlobalStringPtr("false", "false.str");
    }
    return falseStrConst;
}
