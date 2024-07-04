#ifndef PTREXPR_HPP
#define PTREXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>
#include "Expression.hpp"

class PtrExpr : public Expression
{
public:
    std::string name;

    PtrExpr(std::string n) : name(n) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Function *currentFunction = cc.getBuilder().GetInsertBlock()->getParent();

        if (!currentFunction->getValueSymbolTable()->lookup(name))
        {
            printf("Error: Variable '%s' is not found in scope.\n", name.c_str());
            throw std::runtime_error("Variable not found in scope: " + name);
        }

        llvm::Value *value = cc.getTable().getVariable(name);
        llvm::Value *var = cc.getBuilder().CreateLoad(value->getType()->getPointerElementType(), value);

        return var;
    }
};

#endif
