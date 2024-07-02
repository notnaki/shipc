#ifndef ARRAYACCESSEXPR_H
#define ARRAYACCESSEXPR_H

#include <vector>
#include <memory>

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include "Expression.hpp"

class ArrayAccessExpr : public Expression
{
private:
    std::unique_ptr<Expression> arrayExpr;
    std::vector<std::unique_ptr<Expression>> indices;

public:
    ArrayAccessExpr(std::unique_ptr<Expression> array, std::vector<std::unique_ptr<Expression>> idx)
        : arrayExpr(std::move(array)), indices(std::move(idx)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::IRBuilder<> &builder = cc.getBuilder();

        llvm::Value *array = arrayExpr->codegen(cc);

        std::vector<unsigned> llvmIndices;
        for (const auto &idx : indices)
        {
            llvm::Value *llvmIdxValue = idx->codegen(cc);
            if (llvm::ConstantInt *constInt = llvm::dyn_cast<llvm::ConstantInt>(llvmIdxValue))
            {
                llvmIndices.push_back(constInt->getLimitedValue());
            }
            else
            {
                throw std::runtime_error("Index value must be a constant integer.");
            }
        }

        llvm::Value *extractedElement = builder.CreateExtractValue(array, llvmIndices);

        return extractedElement;
    }
};

#endif
