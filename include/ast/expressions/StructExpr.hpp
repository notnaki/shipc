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

        unsigned int numStructElements = structType->getNumElements();

        while (elementValues.size() < numStructElements)
        {
            elementValues.push_back(llvm::Constant::getNullValue(structType->getElementType(elementValues.size())));
        }

        return llvm::ConstantStruct::get(structType, elementValues);
    }
};

#endif
