#ifndef EXPRSTMT_HPP
#define EXPRSTMT_HPP

#include "Statement.hpp"
#include "ast/expressions/Expression.hpp"

class ExprStmt : public Statement
{

private:
    std::unique_ptr<Expression> expression;

public:
    ExprStmt(std::unique_ptr<Expression> exprPtr) : expression(std::move(exprPtr)) {}

    void codegen(CompilerContext &cc) const override
    {
        expression->codegen(cc);
    }
};

#endif