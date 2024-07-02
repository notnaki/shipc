#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>

#include "compiler/CompilerContext.hpp"

class Expression
{
public:
    virtual ~Expression() {}
    virtual llvm::Value *codegen(CompilerContext &cc) const = 0;
};

#endif