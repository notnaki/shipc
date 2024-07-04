#include "lexer/Tokens.hpp"

const std::unordered_map<std::string, TokenKind> reserved_words{
    {"let", TokenKind::LET},
    {"const", TokenKind::CONST},
    {"new", TokenKind::NEW},
    {"import", TokenKind::IMPORT},
    {"impl", TokenKind::IMPL},
    {"from", TokenKind::FROM},
    {"fn", TokenKind::FN},
    {"if", TokenKind::IF},
    {"else", TokenKind::ELSE},
    {"foreach", TokenKind::FOREACH},
    {"while", TokenKind::WHILE},
    {"for", TokenKind::FOR},
    {"in", TokenKind::IN},
    {"struct", TokenKind::STRUCT},
    {"return", TokenKind::RETURN},
    {"break", TokenKind::BREAK},
    {"true", TokenKind::TRUE},
    {"false", TokenKind::FALSE},
};