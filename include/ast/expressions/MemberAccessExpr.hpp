#ifndef MEMBERACCESSEXPR_HPP
#define MEMBERACCESSEXPR_HPP

#include "Expression.hpp"

class MemberAccessExpr : public Expression
{
private:
    std::string member;
    std::unique_ptr<Expression> structExpr;

public:
    MemberAccessExpr(std::unique_ptr<Expression> left, std::string memberName) : structExpr(std::move(left)), member(memberName) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Value *structVal = structExpr->codegen(cc);

        std::string structName = structVal->getType()->getStructName().str();
        // unsigned idx = cc.getTable().getStructMemberIdx(structName, member);

        llvm::Value *extractedElement = cc.getBuilder().CreateExtractValue(structVal, {0}, "mem_access");

        return extractedElement;
    }
};

#endif