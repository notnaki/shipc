#ifndef LEXERUTIL_HPP
#define LEXERUTIL_HPP

#include <string>
#include <vector>
#include <iostream>

#include "Tokens.hpp"
#include "TokenKinds.hpp"

std::string tokenKindToString(TokenKind kind);

void printTokens(std::vector<Token> tokens);

#endif