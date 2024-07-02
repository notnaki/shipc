#ifndef MEMBERACCESSEXPR_HPP
#define MEMBERACCESSEXPR_HPP

#include "Expression.hpp"

class MemberAccessExpr : public Expression
{
private:
    std::string member;
    std::unique_ptr<Expression> structExpr;

public:
    mutable std::string sName;
    MemberAccessExpr(std::unique_ptr<Expression> left, std::string memberName) : structExpr(std::move(left)), member(memberName), sName("") {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::Value *structVal = structExpr->codegen(cc);

        unsigned idx;

        sName = structVal->getType()->getStructName().str();
        idx = cc.getTable().getStructMemberIdx(sName, member);

        llvm::Value *extractedElement = cc.getBuilder().CreateExtractValue(structVal, {idx}, "mem_access");

        return extractedElement;
    }
};

#endif