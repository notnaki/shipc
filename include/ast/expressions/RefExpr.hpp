#ifndef REFEXPR_HPP
#define REFEXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>
#include "Expression.hpp"

class RefExpr : public Expression
{
public:
    std::string name;

    RefExpr(std::string n) : name(n) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Function *currentFunction = cc.getBuilder().GetInsertBlock()->getParent();

        llvm::Value *var = currentFunction->getValueSymbolTable()->lookup(name);

        if (!var)
        {
            printf("Error: Variable '%s' is not found in scope.\n", name.c_str());
            throw std::runtime_error("Variable not found in scope: " + name);
        }

        return var;
    }
};

#endif
