#ifndef BINARYEXPR_HPP
#define BINARYEXPR_HPP

#include "Expression.hpp"

class BinaryExpr : public Expression
{
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    char op;

public:
    BinaryExpr(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, char operation)
        : left(std::move(l)), right(std::move(r)), op(operation) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Value *lvalue = left->codegen(cc);
        llvm::Value *rvalue = right->codegen(cc);

        if (!lvalue || !rvalue)
            return nullptr;

        llvm::Type *ltype = lvalue->getType();

        if (ltype->isFloatTy())
        {
            switch (op)
            {
            case '+':
                return cc.getBuilder().CreateFAdd(lvalue, rvalue, "faddtmp");
            case '-':
                return cc.getBuilder().CreateFSub(lvalue, rvalue, "fsubtmp");
            case '*':
                return cc.getBuilder().CreateFMul(lvalue, rvalue, "fmultmp");
            case '/':
                return cc.getBuilder().CreateFDiv(lvalue, rvalue, "fdivtmp");
            default:
                llvm::errs() << "Invalid binary operator for float\n";
                return nullptr;
            }
        }
        else if (ltype->isDoubleTy())
        {
            switch (op)
            {
            case '+':
                return cc.getBuilder().CreateFAdd(lvalue, rvalue, "daddtmp");
            case '-':
                return cc.getBuilder().CreateFSub(lvalue, rvalue, "dsubtmp");
            case '*':
                return cc.getBuilder().CreateFMul(lvalue, rvalue, "dmultmp");
            case '/':
                return cc.getBuilder().CreateFDiv(lvalue, rvalue, "ddivtmp");
            default:
                llvm::errs() << "Invalid binary operator for double\n";
                return nullptr;
            }
        }
        else if (ltype->isIntegerTy())
        {
            switch (op)
            {
            case '+':
                return cc.getBuilder().CreateAdd(lvalue, rvalue, "addtmp");
            case '-':
                return cc.getBuilder().CreateSub(lvalue, rvalue, "subtmp");
            case '*':
                return cc.getBuilder().CreateMul(lvalue, rvalue, "multmp");
            case '/':
                return cc.getBuilder().CreateSDiv(lvalue, rvalue, "divtmp");
            case '%':
                return cc.getBuilder().CreateSRem(lvalue, rvalue, "modtmp");
            default:
                llvm::errs() << "Invalid binary operator for integer\n";
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