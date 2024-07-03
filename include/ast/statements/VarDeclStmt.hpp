#ifndef VARDECLSTMT_H
#define VARDECLSTMT_H

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include "Statement.hpp"
#include "ast/expressions/Expression.hpp"

class VarDeclStmt : public Statement
{
private:
    std::string varName;
    llvm::Type *expectedType;
    std::unique_ptr<Expression> initialValue;

public:
    VarDeclStmt(const std::string &name, llvm::Type *type, std::unique_ptr<Expression> initValue)
        : varName(name), expectedType(type), initialValue(std::move(initValue)) {}

    void codegen(CompilerContext &cc) const override
    {
        llvm::Function *currentFunction = cc.getBuilder().GetInsertBlock()->getParent();
        llvm::AllocaInst *allocaInst = cc.getBuilder().CreateAlloca(expectedType, nullptr, varName);

        if (initialValue)
        {

            llvm::Value *value = initialValue->codegen(cc);
            cc.getTable().addElement(varName, value, expectedType);
            if (expectedType->isStructTy())
            {
            }

            else if (expectedType->isArrayTy())
            {

                std::string actualTypeStr;
                llvm::raw_string_ostream actualTypeStream(actualTypeStr);
                value->getType()->print(actualTypeStream);

                std::string expectedTypeStr;
                llvm::raw_string_ostream expectedTypeStream(expectedTypeStr);
                expectedType->print(expectedTypeStream);

                if (expectedTypeStr != actualTypeStr)
                {

                    llvm::errs() << "Error: Initial value type mismatch for variable '" << varName << "'.\n";
                    llvm::errs() << "Expected type: " << expectedTypeStr << "\n";
                    llvm::errs() << "Actual type: " << actualTypeStr << "\n";

                    throw std::runtime_error("Initial value type mismatch for variable '" + varName + "'");
                }

                cc.getBuilder().CreateStore(value, allocaInst);
                return;
            }
            else
            {

                if (value->getType() != expectedType)
                {

                    std::string actualTypeStr;
                    llvm::raw_string_ostream actualTypeStream(actualTypeStr);
                    value->getType()->print(actualTypeStream);

                    std::string expectedTypeStr;
                    llvm::raw_string_ostream expectedTypeStream(expectedTypeStr);
                    expectedType->print(expectedTypeStream);

                    llvm::errs() << "Error: Initial value type mismatch for variable '" << varName << "'.\n";
                    llvm::errs() << "Expected type: " << expectedTypeStr << "\n";
                    llvm::errs() << "Actual type: " << actualTypeStr << "\n";

                    throw std::runtime_error("Initial value type mismatch for variable '" + varName + "'");
                }
            }

            cc.getBuilder().CreateStore(value, allocaInst);
        }
    }
};

#endif
