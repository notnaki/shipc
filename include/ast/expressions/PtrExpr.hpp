#ifndef PTREXPR_HPP
#define PTREXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <memory>
#include "Expression.hpp"

class PtrExpr : public Expression
{
public:
    std::unique_ptr<Expression> expr;

    PtrExpr(std::unique_ptr<Expression> e) : expr(std::move(e)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Function *currentFunction = cc.getBuilder().GetInsertBlock()->getParent();



        llvm::Value *value = expr->codegen(cc);
        //check if the value is a pointer type
        if (!value->getType()->isPointerTy())
        {
            throw std::runtime_error("Error: trying to dereference a non-pointer type");
        }
        llvm::Value *var = cc.getBuilder().CreateLoad(value->getType()->getPointerElementType(), value);

        return var;
    }
};

#endif
