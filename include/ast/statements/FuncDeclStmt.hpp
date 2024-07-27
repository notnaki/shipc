#ifndef FUNCDECLSTMT_HPP
#define FUNCDECLSTMT_HPP

#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>

#include "Statement.hpp"
#include "BlockStmt.hpp"

class FuncDeclStmt : public Statement
{
private:
    std::string structName;
    std::string functionName;
    std::vector<std::pair<std::string, llvm::Type *>> parameters;
    llvm::Type *returnType;
    std::unique_ptr<BlockStmt> body;

    bool isPrivate;

public:
    FuncDeclStmt(const std::string &name, const std::vector<std::pair<std::string, llvm::Type *>> &params, llvm::Type *returnTy, std::unique_ptr<BlockStmt> bodyPtr, bool isPriv)
        : functionName(name), structName(""), parameters(params), returnType(returnTy), body(std::move(bodyPtr)), isPrivate(isPriv)
    {
    }

    std::string getName()
    {
        return functionName;
    }

    void setStructName(std::string name)
    {
        structName = name + ".";
    }

    void codegen(CompilerContext &cc) const override
    {
        std::vector<llvm::Type *> paramTypes;
        for (const auto &param : parameters)
        {
            paramTypes.push_back(param.second);
        }

        llvm::FunctionType *funcType = llvm::FunctionType::get(returnType, paramTypes, false);
        llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, structName + functionName, cc.getModule());

        auto argIt = func->arg_begin();
        for (size_t i = 0; i < parameters.size(); ++i, ++argIt)
        {
            argIt->setName(parameters[i].first);
        }

        if (!body && !returnType->isVoidTy())
        {
            llvm::errs() << "Error: Function '" << structName + functionName << "' is missing a body.\n";
            throw std::runtime_error("Function '" + structName + functionName + "' is missing a body.");
        }

        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(cc.getContext(), "entry", func);
        cc.getBuilder().SetInsertPoint(entryBB);

        if (body)
        {
            body->codegen(cc);

            if (returnType->isVoidTy())
            {
                cc.getBuilder().CreateRetVoid();
                // llvm::errs() << "Error: Function '" << functionName << "' must end with a return statement.\n";
                // throw std::runtime_error("Function '" + functionName + "' must end with a return statement.");
            }
        }

        llvm::verifyFunction(*func);
        cc.addFunction(structName + functionName, func, isPrivate);
    }
};

#endif
