#ifndef STRINGEXPR_HPP
#define STRINGEXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include "Expression.hpp"

class StringExpr : public Expression
{
private:
    std::string value;

public:
    StringExpr(std::string val) : value(val) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::LLVMContext &context = cc.getContext();

        llvm::Constant *strConstant = llvm::ConstantDataArray::getString(context, value, true);
        llvm::GlobalVariable *strGlobal = new llvm::GlobalVariable(
            cc.getModule(), strConstant->getType(), true,
            llvm::GlobalValue::PrivateLinkage, strConstant, ".str");

        llvm::Value *strPtr = builder.CreateBitCast(strGlobal, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));

        return strPtr;
    }
};

#endif // STRINGEXPR_H
