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
        auto funcval = currentFunction->getValueSymbolTable()->lookup(name);

        if (llvm::Argument *arg = llvm::dyn_cast<llvm::Argument>(funcval))
        {
            return funcval;
        }

        llvm::Value *var = cc.getTable().getVariable(name);

        return cc.getBuilder().CreateLoad(var->getType()->getPointerElementType(), var, "value");
    }
};

#endif
