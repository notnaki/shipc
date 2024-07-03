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
        llvm::IRBuilder<> &builder = cc.getBuilder();

        // Get the struct pointer
        llvm::Value *structPtr = structExpr->codegen(cc);

        // Access the member using GEP
        llvm::Type *structType = cc.getTable().getStructType("Hey");
        unsigned idx = cc.getTable().getStructMemberIdx("Hey", member);
        llvm::Value *fieldPtr = builder.CreateStructGEP(structType, structPtr, idx, "field_ptr");

        // Load the member value
        llvm::Value *extractedElement = builder.CreateLoad(fieldPtr->getType(), fieldPtr, "mem_access");

        return extractedElement;
    }
};

#endif