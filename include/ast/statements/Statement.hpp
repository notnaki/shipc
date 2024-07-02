#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <llvm/IR/IRBuilder.h>

#include "compiler/CompilerContext.hpp"

class Statement
{
public:
    virtual ~Statement() {}
    virtual void codegen(CompilerContext &cc) const = 0;
};

#endif
