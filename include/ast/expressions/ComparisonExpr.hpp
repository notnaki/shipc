#ifndef COMPARISONEXPR_HPP
#define COMPARISONEXPR_HPP

#include "Expression.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

class ComparisonExpr : public Expression
{
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    std::string op;

public:
    ComparisonExpr(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, const std::string &operation)
        : left(std::move(l)), right(std::move(r)), op(operation) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Value *lvalue = left->codegen(cc);
        llvm::Value *rvalue = right->codegen(cc);

        llvm::Type *ltype = lvalue->getType();

        // Check if the type is a pointer to an i8, which we'll treat as a string
        if (ltype->isPointerTy() && ltype->getPointerElementType()->isIntegerTy(8))
        {
            llvm::Function *function = cc.getBuilder().GetInsertBlock()->getParent();

            // Assuming the strcmp function is already declared or defined somewhere
            llvm::Function *strcmpFunc = cc.getModule().getFunction("strcmp");
            if (!strcmpFunc)
            {
                llvm::errs() << "Error: strcmp function not found in module\n";
                return nullptr;
            }

            llvm::Value *args[] = {lvalue, rvalue};
            llvm::Value *cmpResult = cc.getBuilder().CreateCall(strcmpFunc, args, "cmpResult");

            // Convert result to a boolean value
            if (op == "==")
                return cc.getBuilder().CreateICmpEQ(cmpResult, cc.getBuilder().getInt32(0), "cmptmp");
            else if (op == "!=")
                return cc.getBuilder().CreateICmpNE(cmpResult, cc.getBuilder().getInt32(0), "cmptmp");
            else
            {
                llvm::errs() << "Invalid comparison operator for strings\n";
                return nullptr;
            }
        }
        else if (ltype->isFloatTy() || ltype->isDoubleTy())
        {
            // Handle floating point comparisons
            if (op == "==")
                return cc.getBuilder().CreateFCmpOEQ(lvalue, rvalue, "cmptmp");
            else if (op == "!=")
                return cc.getBuilder().CreateFCmpONE(lvalue, rvalue, "cmptmp");
            else if (op == "<")
                return cc.getBuilder().CreateFCmpOLT(lvalue, rvalue, "cmptmp");
            else if (op == ">")
                return cc.getBuilder().CreateFCmpOGT(lvalue, rvalue, "cmptmp");
            else if (op == "<=")
                return cc.getBuilder().CreateFCmpOLE(lvalue, rvalue, "cmptmp");
            else if (op == ">=")
                return cc.getBuilder().CreateFCmpOGE(lvalue, rvalue, "cmptmp");
            else
            {
                llvm::errs() << "Invalid comparison operator for floating point\n";
                return nullptr;
            }
        }
        else if (ltype->isIntegerTy())
        {
            // Handle integer comparisons
            if (op == "==")
                return cc.getBuilder().CreateICmpEQ(lvalue, rvalue, "cmptmp");
            else if (op == "!=")
                return cc.getBuilder().CreateICmpNE(lvalue, rvalue, "cmptmp");
            else if (op == "<")
                return cc.getBuilder().CreateICmpSLT(lvalue, rvalue, "cmptmp");
            else if (op == ">")
                return cc.getBuilder().CreateICmpSGT(lvalue, rvalue, "cmptmp");
            else if (op == "<=")
                return cc.getBuilder().CreateICmpSLE(lvalue, rvalue, "cmptmp");
            else if (op == ">=")
                return cc.getBuilder().CreateICmpSGE(lvalue, rvalue, "cmptmp");
            else
            {
                llvm::errs() << "Invalid comparison operator for integer\n";
                return nullptr;
            }
        }
        else
        {
            llvm::errs() << "Unsupported operand type\n";
            return nullptr;
        }
    }
};

#endif
