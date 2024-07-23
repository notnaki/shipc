#ifndef MEMBERACCESSEXPR_HPP
#define MEMBERACCESSEXPR_HPP

#include "Expression.hpp"
#include <stdexcept>

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

        //check if struct type exists and if its a pointer

        if (!structValue)
        {
            throw std::runtime_error("Error: Struct value is null");
        }

        if (!structValue->getType()->isPointerTy())
        {
            throw std::runtime_error("Error: Trying to access member of non-pointer type");
        }

        // check if the struct is a struct type


        structType = structValue->getType()->getPointerElementType();

        if (!structType->isStructTy())
        {
            throw std::runtime_error("Error: Member access can only be performed on references to structs");
        }


        unsigned idx = cc.getTable().getStructMemberIdx(structType->getStructName().str(), member);
        llvm::Value *memberValue = cc.getBuilder().CreateStructGEP(structType, structValue, idx); // Replace 0 with the actual index of the member

        llvm::Value *loadedMember = cc.getBuilder().CreateLoad(memberValue->getType()->getPointerElementType(), memberValue);

        return loadedMember;
    }
};

#endif
