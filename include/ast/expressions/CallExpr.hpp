#ifndef CALLEXPR_HPP
#define CALLEXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Argument.h>
#include <llvm/ADT/iterator_range.h>
#include <stdexcept>

#include "Expression.hpp"
#include "SymbolExpr.hpp"
#include "ast/expressions/MemberAccessExpr.hpp"

class CallExpr : public Expression
{
private:
    std::vector<std::unique_ptr<Expression>> args;
    std::unique_ptr<Expression> function;

public:
    CallExpr(std::unique_ptr<Expression> fn, std::vector<std::unique_ptr<Expression>> arguments)
        : function(std::move(fn)), args(std::move(arguments)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        std::string fnName;
        std::vector<llvm::Value *> arguments;

        if (auto symbolExpr = dynamic_cast<SymbolExpr *>(function.get()))
        {
            fnName = symbolExpr->name;
        }

        if (auto memberExpr = dynamic_cast<MemberAccessExpr*>(function.get())){
            llvm::Value* structExpr = memberExpr->structExpr->codegen(cc);
            llvm::Type *structType = structExpr->getType();

            if (!structType->isPointerTy()){
                throw std::runtime_error("Struct method call on non-pointer type");
            }

            if (!structType->getPointerElementType()->isStructTy()){
                throw std::runtime_error("Struct method call on non-struct pointer type");
            }

            fnName = structType->getPointerElementType()->getStructName().str() + "."+ memberExpr->member;
            arguments.push_back(structExpr);
        }

        if (!cc.isFunctionCallable(fnName)){

            throw std::runtime_error("Function " + fnName + " is private and can only be called in the scope of its struct.");
        }

        auto fn = cc.getModule().getFunction(fnName);

        if (fn == nullptr)
        {
            printf("Error: Function '%s' is not defined.\n", fnName.c_str());
            throw std::runtime_error("Function not defined: " + fnName);
        }

        for (const auto &arg : args)
        {
            arguments.push_back(arg->codegen(cc));
        }

        if (fn->arg_size() != arguments.size())
        {
            if (!fn->isVarArg() || fn->arg_size() > args.size())
            {
                printf("Error: Function '%s' called with incorrect number of arguments.\n", fnName.c_str());
                throw std::runtime_error("Incorrect number of arguments for function: " + fnName);
            }
        }

        return cc.getBuilder().CreateCall(fn, arguments);
    }
};

#endif
