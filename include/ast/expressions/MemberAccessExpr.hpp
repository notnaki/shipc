#ifndef MEMBERACCESSEXPR_HPP
#define MEMBERACCESSEXPR_HPP

#include "Expression.hpp"

class MemberAccessExpr : public Expression
{
private:
    std::string member;
    std::unique_ptr<Expression> structExpr;

public:
    mutable std::string mName;
    mutable llvm::Type *structType;

    MemberAccessExpr(std::unique_ptr<Expression> left, std::string memberName) : structExpr(std::move(left)), member(memberName), mName(memberName) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::IRBuilder<> &builder = cc.getBuilder();

        llvm::Value *structVal = structExpr->codegen(cc);

        auto al = llvm::dyn_cast<llvm::AllocaInst>(structVal);
        if (al)
        {
            structType = al->getAllocatedType();
            if (!al->getName().empty())
            {
                structVal = builder.CreateLoad(structVal->getType(), structVal, "l");
            }
        }
        else
        {
            auto mi = dynamic_cast<MemberAccessExpr *>(structExpr.get());
            if (mi)
            {
                unsigned index = cc.getTable().getStructMemberIdx(mi->structType->getStructName().str(), mi->mName);
                if (mi->structType->getStructElementType(index)->isStructTy())
                {
                    structType = mi->structType->getStructElementType(index);
                }
                else
                {
                    structType = mi->structType;
                }
            }
            else
            {

                throw std::runtime_error("Nested member types can only be called when nested");
            }
        }

        unsigned idx = cc.getTable().getStructMemberIdx(structType->getStructName().str(), member);

        llvm::Value *fieldPtr = builder.CreateStructGEP(structType, structVal, idx, "field_ptr");

        llvm::Value *extractedElement = builder.CreateLoad(fieldPtr->getType(), fieldPtr, "mem_access");

        return extractedElement;
    }
};

#endif