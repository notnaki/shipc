#ifndef ASSIGNMENTEXPR_HPP
#define ASSIGNMENTEXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>
#include "Expression.hpp"

class AssignmentExpr : public Expression
{
public:
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;

    AssignmentExpr(std::unique_ptr<Expression> var, std::unique_ptr<Expression> value) : lhs(std::move(var)), rhs(std::move(value)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Value *value = rhs->codegen(cc);

        if (auto symbolExpr = dynamic_cast<SymbolExpr *>(lhs.get()))
        {
            llvm::Value *var = cc.getTable().getVariable(symbolExpr->name);
            cc.getBuilder().CreateStore(value, var);
        }
        else if (auto arrayAccessExpr = dynamic_cast<ArrayAccessExpr *>(lhs.get()))
        {
            llvm::Value *arrayPtr = arrayAccessExpr->arrayExpr->codegen(cc);
            llvm::Value *index = arrayAccessExpr->indexExpr->codegen(cc);

            llvm::Value *elementPtr = cc.getBuilder().CreateGEP(arrayPtr->getType()->getPointerElementType(),
                                                                arrayPtr,
                                                                {cc.getBuilder().getInt32(0), index},
                                                                "assignelementptr");

            // Ensure the types match before storing
            if (elementPtr->getType()->getPointerElementType() != value->getType())
            {
                value = cc.getBuilder().CreateBitCast(value, elementPtr->getType()->getPointerElementType(), "typecast");
            }

            cc.getBuilder().CreateStore(value, elementPtr);
        }
        else
        {
            throw std::runtime_error("Invalid left-hand side in assignment");
        }

        return value;
    }
};

#endif
