#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "statements/Statement.hpp"

class Program
{
public:
    Program(std::vector<std::unique_ptr<Statement>> statements) : body(std::move(statements)) {}

    void codegen(CompilerContext &cc)
    {
        for (const auto &stmt : body)
        {
            stmt->codegen(cc);
        }
    }

private:
    std::vector<std::unique_ptr<Statement>> body;
};

#endif
