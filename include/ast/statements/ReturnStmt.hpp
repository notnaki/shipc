#ifndef RETURNSTMT_HPP
#define RETURNSTMT_HPP

#include "Statement.hpp"
#include "ast/expressions/Expression.hpp"

class ReturnStmt : public Statement
{
private:
    std::unique_ptr<Expression> expression;

public:
    ReturnStmt(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}

    void codegen(CompilerContext &cc) const override
    {
        llvm::Value *returnVal = expression->codegen(cc);
        cc.getBuilder().CreateRet(returnVal);
    }
};

#endif