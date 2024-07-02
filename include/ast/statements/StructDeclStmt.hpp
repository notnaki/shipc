#ifndef STRUCTDECL_HPP
#define STRUCTDECL_HPP

#include "Statement.hpp"

class StructDeclStmt : public Statement
{
public:
    StructDeclStmt() {}
    void codegen(CompilerContext &cc) const override {}
};

#endif
