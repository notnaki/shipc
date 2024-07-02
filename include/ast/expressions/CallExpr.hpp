#ifndef CALLEXPR_HPP
#define CALLEXPR_HPP

#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Argument.h>
#include <llvm/ADT/iterator_range.h>

#include "Expression.hpp"

class CallExpr : public Expression
{
private:
    std::vector<std::unique_ptr<Expression>> args;
    std::string fnName;

public:
    CallExpr(std::string name, std::vector<std::unique_ptr<Expression>> arguments)
        : fnName(std::move(name)), args(std::move(arguments)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        auto fn = cc.getModule().getFunction(fnName);

        if (fn == nullptr)
        {
            printf("Error: Function '%s' is not defined.\n", fnName.c_str());
            throw std::runtime_error("Function not defined: " + fnName);
        }

        if (fn->arg_size() != args.size())
        {
            if (!fn->isVarArg() || fn->arg_size() > args.size())
            {
                printf("Error: Function '%s' called with incorrect number of arguments.\n", fnName.c_str());
                throw std::runtime_error("Incorrect number of arguments for function: " + fnName);
            }
        }

        std::vector<llvm::Value *> arguments;
        for (const auto &arg : args)
        {
            arguments.push_back(arg->codegen(cc));
        }

        return cc.getBuilder().CreateCall(fn, arguments);
    }
};

#endif
