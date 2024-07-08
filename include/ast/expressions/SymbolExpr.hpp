#ifndef SYMBOLEXPR_HPP
#define SYMBOLEXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>
#include "Expression.hpp"

class SymbolExpr : public Expression
{
public:
    std::string name;

    SymbolExpr(std::string n) : name(n) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        // llvm::Function *currentFunction = cc.getBuilder().GetInsertBlock()->getParent();

        // if (!currentFunction->getValueSymbolTable()->lookup(name))
        // {
        //     printf("Error: Variable '%s' is not found in scope.\n", name.c_str());
        //     throw std::runtime_error("Variable not found in scope: " + name);
        // }

        llvm::Value *var = cc.getTable().getVariable(name);

        return var;
    }
};

#endif
