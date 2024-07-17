#include "lexer/util.hpp"

std::string tokenKindToString(TokenKind kind)
{
    switch (kind)
    {
    case TokenKind::EOF_TOKEN:
        return "EOF_TOKEN";
    case TokenKind::NULL_TOKEN:
        return "NULL_TOKEN";
    case TokenKind::TRUE:
        return "TRUE";
    case TokenKind::FALSE:
        return "FALSE";
    case TokenKind::INT_NUMBER:
        return "INT_NUMBER";
    case TokenKind::FLOAT_NUMBER:
        return "FLOAT_NUMBER";
    case TokenKind::DOUBLE_NUMBER:
        return "DOUBLE_NUMBER";
    case TokenKind::STRING:
        return "STRING";
    case TokenKind::IDENTIFIER:
        return "IDENTIFIER";
    case TokenKind::OPEN_BRACKET:
        return "OPEN_BRACKET";
    case TokenKind::CLOSE_BRACKET:
        return "CLOSE_BRACKET";
    case TokenKind::OPEN_CURLY:
        return "OPEN_CURLY";
    case TokenKind::CLOSE_CURLY:
        return "CLOSE_CURLY";
    case TokenKind::OPEN_PAREN:
        return "OPEN_PAREN";
    case TokenKind::CLOSE_PAREN:
        return "CLOSE_PAREN";
    case TokenKind::ASSIGNMENT:
        return "ASSIGNMENT";
    case TokenKind::EQUALS:
        return "EQUALS";
    case TokenKind::NOT_EQUALS:
        return "NOT_EQUALS";
    case TokenKind::NOT:
        return "NOT";
    case TokenKind::LESS:
        return "LESS";
    case TokenKind::LESS_EQUALS:
        return "LESS_EQUALS";
    case TokenKind::GREATER:
        return "GREATER";
    case TokenKind::GREATER_EQUALS:
        return "GREATER_EQUALS";
    case TokenKind::OR:
        return "OR";
    case TokenKind::AND:
        return "AND";
    case TokenKind::DOT:
        return "DOT";
    case TokenKind::SEMI_COLON:
        return "SEMI_COLON";
    case TokenKind::COLON:
        return "COLON";
    case TokenKind::COMMA:
        return "COMMA";
    case TokenKind::PLUS_PLUS:
        return "PLUS_PLUS";
    case TokenKind::MINUS_MINUS:
        return "MINUS_MINUS";
    case TokenKind::PLUS_EQUALS:
        return "PLUS_EQUALS";
    case TokenKind::MINUS_EQUALS:
        return "MINUS_EQUALS";
    case TokenKind::PLUS:
        return "PLUS";
    case TokenKind::DASH:
        return "DASH";
    case TokenKind::SLASH:
        return "SLASH";
    case TokenKind::STAR:
        return "STAR";
    case TokenKind::PERCENT:
        return "PERCENT";
    case TokenKind::LET:
        return "LET";
    case TokenKind::CONST:
        return "CONST";
    case TokenKind::NEW:
        return "NEW";
    case TokenKind::IMPORT:
        return "IMPORT";
    case TokenKind::FROM:
        return "FROM";
    case TokenKind::FN:
        return "FN";
    case TokenKind::IF:
        return "IF";
    case TokenKind::ELSE:
        return "ELSE";
    case TokenKind::FOREACH:
        return "FOREACH";
    case TokenKind::WHILE:
        return "WHILE";
    case TokenKind::FOR:
        return "FOR";
    case TokenKind::IN:
        return "IN";
    case TokenKind::STRUCT:
        return "STRUCT";
    case TokenKind::IMPL:
        return "IMPL";
    case TokenKind::RETURN:
        return "RETURN";
    case TokenKind::BREAK:
        return "BREAK";

    case TokenKind::FORMAT_INDICATOR:
        return "FORMAT_INDICATOR";
    case TokenKind::FORMAT_END:
        return "FORMAT_END";

    default:
        return "UNKNOWN_TOKEN";
    }
}

void printTokens(std::vector<Token> tokens)
{
    std::cout << "Tokens:" << std::endl;
    for (const auto &token : tokens)
    {
        std::cout << tokenKindToString(token.kind) << token.value << std::endl;
    }
}