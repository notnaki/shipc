#ifndef FLOATEXPR_HPP
#define FLOATEXPR_HPP

#include "Expression.hpp"

class FloatExpr : public Expression
{
private:
    float value;

public:
    FloatExpr(float val) : value(val) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        return llvm::ConstantFP::get(cc.getContext(), llvm::APFloat(value));
    }
};

#endif
