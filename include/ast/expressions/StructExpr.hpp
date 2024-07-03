#ifndef STRUCTEXPR_HPP
#define STRUCTEXPR_HPP

#include "Expression.hpp"

class StructExpr : public Expression
{
private:
    llvm::StructType *structType;
    std::vector<std::unique_ptr<Expression>> elements;

public:
    StructExpr(llvm::StructType *type, std::vector<std::unique_ptr<Expression>> structElements) : structType(type), elements(std::move(structElements)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::LLVMContext &context = cc.getContext();

        std::vector<llvm::Value *> elementValues;
        for (const auto &element : elements)
        {
            llvm::Value *elementValue = element->codegen(cc);
            if (!elementValue)
            {
                throw std::runtime_error("Element value can't be null");
            }

            elementValues.push_back(elementValue);
        }

        for (size_t i = elements.size(); i < structType->getStructNumElements(); ++i)
        {
            elementValues.push_back(llvm::Constant::getNullValue(structType->getElementType(i)));
        }

        // Create a struct pointer (GEP instruction)
        llvm::Value *structPtr = builder.CreateAlloca(structType, nullptr, "struct_ptr");
        for (size_t i = 0; i < elementValues.size(); ++i)
        {
            llvm::Value *fieldPtr = builder.CreateStructGEP(structType, structPtr, i, "f");
            builder.CreateStore(elementValues[i], fieldPtr);
        }

        return structPtr;
    }
};

#endif
