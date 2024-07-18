#ifndef MEMBERACCESSEXPR_HPP
#define MEMBERACCESSEXPR_HPP

#include "Expression.hpp"

class MemberAccessExpr : public Expression
{

public:
    std::string member;

    std::unique_ptr<Expression> structExpr;
    mutable llvm::Type *structType;

    MemberAccessExpr(std::unique_ptr<Expression> left, std::string memberName) : structExpr(std::move(left)), member(memberName) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {

        llvm::Value *structValue = structExpr->codegen(cc);

        unsigned idx = cc.getTable().getStructMemberIdx(structValue->getType()->getPointerElementType()->getStructName().str(), member);
        llvm::Value *memberValue = cc.getBuilder().CreateStructGEP(structValue->getType()->getPointerElementType(), structValue, idx); // Replace 0 with the actual index of the member

        llvm::Value *loadedMember = cc.getBuilder().CreateLoad(memberValue->getType()->getPointerElementType(), memberValue);

        return loadedMember;
    }
};

#endif
