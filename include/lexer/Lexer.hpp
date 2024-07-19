#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>
#include <unordered_map>

#include "Tokens.hpp"

class Lexer
{
private:
    size_t pos;
    std::string source;
    std::vector<Token> tokens;

    std::string remainder();

public:
    Lexer(std::string src) : source(src), pos(0) {}

    std::vector<Token> tokenize();
};

#endif
