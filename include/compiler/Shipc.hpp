#ifndef SHIPC_HPP
#define SHIPC_HPP

#include <string>
#include <fstream>
#include <iostream>

#include "ast/Program.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "CompilerContext.hpp"

class Shipc
{
public:
    Shipc(const std::string &filename) : cc("m_mod"), name(filename) {}

    void exec();

private:
    std::string name;
    CompilerContext cc;

    std::string replace_escape_sequences(const std::string &input);
    std::string readFile(std::string filename);
};

#endif