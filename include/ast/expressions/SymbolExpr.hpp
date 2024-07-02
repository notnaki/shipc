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
        llvm::Function *currentFunction = cc.getBuilder().GetInsertBlock()->getParent();
        llvm::Value *var = currentFunction->getValueSymbolTable()->lookup(name);

        if (var == nullptr)
        {
            printf("Error: Variable '%s' is not found in scope.\n", name.c_str());
            throw std::runtime_error("Variable not found in scope: " + name);
        }

        if (llvm::Argument *arg = llvm::dyn_cast<llvm::Argument>(var))
        {
            return arg;
        }
        else
        {
            llvm::Type *varType = cc.getTable().getElementType(name);

            llvm::Value *varValue = cc.getBuilder().CreateLoad(varType, var, name + ".value");
            // return cc.getTable().getElementValue(name);
            return varValue;
        }
    }
};

#endif
