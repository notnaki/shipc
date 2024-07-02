#ifndef ARRAYEXPR_HPP
#define ARRAYEXPR_HPP

#include <vector>
#include <stdio.h>
#include <iostream>

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Constants.h>

#include "Expression.hpp"
#include "SymbolExpr.hpp"

class ArrayExpr : public Expression
{
private:
    std::vector<std::unique_ptr<Expression>> elements;

public:
    ArrayExpr(std::vector<std::unique_ptr<Expression>> arrayElements)
        : elements(std::move(arrayElements)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::LLVMContext &context = cc.getContext();

        std::vector<llvm::Constant *> elementValues;
        for (const auto &element : elements)
        {

            llvm::Value *elementValue = element->codegen(cc);
            if (!elementValue)
            {
                throw std::runtime_error("Element value can't be null");
            }

            llvm::Constant *constantValue = llvm::dyn_cast<llvm::Constant>(elementValue);
            if (!constantValue)
            {
                throw std::runtime_error("Non-constant initialization not supported in array initialization.");
                return nullptr;
            }

            elementValues.push_back(constantValue);
            continue;
        }

        llvm::Type *elementType = nullptr;

        if (auto symbolExpr = dynamic_cast<SymbolExpr *>(elements[0].get()))
        {
            elementType = cc.getTable().getElementType(symbolExpr->name);
        }
        else
        {
            llvm::Value *zero = elements[0]->codegen(cc);
            elementType = zero->getType();
        }

        size_t arraySize = elements.size();
        llvm::ArrayType *arrayType = llvm::ArrayType::get(elementType, arraySize);

        return llvm::ConstantArray::get(arrayType, elementValues);
    }
};

#endif
