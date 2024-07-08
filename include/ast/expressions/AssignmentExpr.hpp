#ifndef ASSIGNMENTEXPR_HPP
#define ASSIGNMENTEXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>
#include "Expression.hpp"

class AssignmentExpr : public Expression
{
public:
    std::string name;
    std::unique_ptr<Expression> newVal;

    AssignmentExpr(std::string n, std::unique_ptr<Expression> value) : name(n), newVal(std::move(value)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {

        llvm::Value *variable = cc.getTable().getVariable(name);
        llvm::Value *value = newVal->codegen(cc);
        cc.getBuilder().CreateStore(value, variable);

        return value;
    }
};

#endif
