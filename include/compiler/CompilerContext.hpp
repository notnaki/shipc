#ifndef COMPILERCONTEXT_HPP
#define COMPILERCONTEXT_HPP

#include <string>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>

#include "ContextTable.hpp"

class CompilerContext
{
private:
    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

    ContextTable contextTable;

    llvm::Constant *trueStrConst = nullptr;
    llvm::Constant *falseStrConst = nullptr;

public:
    CompilerContext(const std::string &moduleName) : module(moduleName, context), builder(context)
    {
        context.setOpaquePointers(false);
        setupExternalFunctions();
    }

    llvm::LLVMContext &getContext();
    llvm::Module &getModule();
    llvm::IRBuilder<> &getBuilder();
    ContextTable &getTable();

    void setTable(ContextTable t);

    void setupExternalFunctions();
    void save(std::string filename);

    llvm::Constant *getTrueStrConst();
    llvm::Constant *getFalseStrConst();
};

#endif
