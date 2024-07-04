#ifndef ARRAYACCESSEXPR_HPP
#define ARRAYACCESSEXPR_HPP

#include "Expression.hpp"

#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

class ArrayAccessExpr : public Expression
{
private:
    std::unique_ptr<Expression> arrayExpr;
    std::unique_ptr<Expression> indexExpr;

public:
    ArrayAccessExpr(std::unique_ptr<Expression> array, std::unique_ptr<Expression> index)
        : arrayExpr(std::move(array)), indexExpr(std::move(index)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::LLVMContext &context = cc.getContext();

        llvm::Value *arrayPtr = arrayExpr->codegen(cc);
        llvm::Value *index = indexExpr->codegen(cc);

        if (!arrayPtr || !index)
        {
            throw std::runtime_error("Array pointer or index can't be null");
        }

        llvm::Value *elementPtr = builder.CreateGEP(arrayPtr->getType()->getPointerElementType(), arrayPtr, {builder.getInt32(0), index}, "elementptr");
        llvm::Value *loadedElement = builder.CreateLoad(elementPtr->getType()->getPointerElementType(), elementPtr, "loadedelement");

        return loadedElement;
    }
};

#endif
