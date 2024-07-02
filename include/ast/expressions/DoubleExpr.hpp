#ifndef DOUBLEEXPR_HPP
#define DOUBLEEXPR_HPP

#include "Expression.hpp"

class DoubleExpr : public Expression
{
private:
    double value;

public:
    DoubleExpr(double val) : value(val) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        return llvm::ConstantFP::get(cc.getContext(), llvm::APFloat(value));
    }
};

#endif
