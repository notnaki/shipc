#ifndef ARRAYEXPR_HPP
#define ARRAYEXPR_HPP

#include "Expression.hpp"

#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include <cstddef>

class ArrayExpr : public Expression
{
private:
    std::vector<std::unique_ptr<Expression>> elements;
    llvm::Type* arrayType;

public:
    ArrayExpr(std::vector<std::unique_ptr<Expression>> arrayElements,llvm::Type* type)
        : elements(std::move(arrayElements)) , arrayType(type) {}

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








        llvm::Type *elementType = arrayType->getArrayElementType();
        llvm::AllocaInst *arrayPtr = builder.CreateAlloca(arrayType, nullptr, "array");

        for (size_t i = 0; i < elementValues.size(); ++i)
        {
            llvm::Value *index = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), i);
            llvm::Value *elementPtr = builder.CreateGEP(arrayPtr->getAllocatedType(), arrayPtr, {builder.getInt32(0), index}, "elementptr");
            builder.CreateStore(elementValues[i], elementPtr);
        }

        for (size_t i = elementValues.size(); i < arrayType->getArrayNumElements(); ++i){
            elementValues.push_back(llvm::Constant::getNullValue(elementType));
        }

        // Load the array pointer before returning
        // llvm::Value *loadedArrayPtr = builder.CreateLoad(arrayType, arrayPtr, "loadedarray");

        return arrayPtr;
    }
};

#endif
