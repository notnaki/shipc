#ifndef INTEGEREXPR_HPP
#define INTEGEREXPR_HPP

#include "Expression.hpp"

class IntegerExpr : public Expression
{
private:
    int value;

public:
    IntegerExpr(int val) : value(val) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        return cc.getBuilder().getInt32(value);
    }
};

#endif
