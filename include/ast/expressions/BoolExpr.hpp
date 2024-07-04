#ifndef BOOLEXPR_HPP
#define BOOLEXPR_HPP

#include "Expression.hpp"

class BoolExpr : public Expression
{
private:
    std::string value;

public:
    BoolExpr(std::string val) : value(val) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        return cc.getBuilder().getInt1(value == "true");
    }
};

#endif
