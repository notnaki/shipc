#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>
#include <unordered_map>

#include "TokenKinds.hpp"

struct Token
{
    TokenKind kind;
    std::string value;
};

extern const std::unordered_map<std::string, TokenKind> reserved_words;

#endif