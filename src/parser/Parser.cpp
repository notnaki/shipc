#include "parser/Parser.hpp"

Program Parser::Parse()
{
    nud_lu = {
        {TokenKind::INT_NUMBER, &Parser::parse_int_expr},
        {TokenKind::FLOAT_NUMBER, &Parser::parse_float_expr},
        {TokenKind::DOUBLE_NUMBER, &Parser::parse_double_expr},
        {TokenKind::STRING, &Parser::parse_string_expr},
        {TokenKind::IDENTIFIER, &Parser::parse_symbol_expr},
        {TokenKind::OPEN_CURLY, &Parser::parse_array_expr}

    };

    led_lu = {
        {TokenKind::OPEN_PAREN, &Parser::parse_call_expr},
        {TokenKind::OPEN_BRACKET, &Parser::parse_array_access_expr},

        {TokenKind::PLUS, &Parser::parse_binary_expr},
        {TokenKind::DASH, &Parser::parse_binary_expr},
        {TokenKind::STAR, &Parser::parse_binary_expr},
        {TokenKind::SLASH, &Parser::parse_binary_expr},
        {TokenKind::PERCENT, &Parser::parse_binary_expr},

    };

    bp_lu = {
        // {TokenKind::PLUS, BindingPower::Additive},
        // {TokenKind::DASH, BindingPower::Additive},
        {TokenKind::OPEN_PAREN, BindingPower::Call},
        {TokenKind::OPEN_BRACKET, BindingPower::Member},

        {TokenKind::STAR, BindingPower::Multiplicative},
        {TokenKind::SLASH, BindingPower::Multiplicative},
        {TokenKind::PERCENT, BindingPower::Multiplicative},
        {TokenKind::PLUS, BindingPower::Additive},
        {TokenKind::DASH, BindingPower::Additive},
    };

    std::vector<std::unique_ptr<Statement>> body;

    while (hasTokens())
    {
        body.push_back(parse_stmt());
    }

    Program program(std::move(body));
    return program;
}

Token Parser::currentToken() { return tokens[pos]; };
TokenKind Parser::currentTokenKind() { return currentToken().kind; };
bool Parser::hasTokens() { return pos < tokens.size() && currentTokenKind() != TokenKind::EOF_TOKEN; };

Token Parser::advance()
{
    Token t = currentToken();
    ++pos;
    return t;
}

Token Parser::expectError(TokenKind expected, const std::string &err)
{
    TokenKind kind = currentTokenKind();
    if (kind != expected)
    {
        std::string errorMsg = err;
        if (errorMsg.empty())
        {
            errorMsg = "Expected " + tokenKindToString(expected) + " but received " + tokenKindToString(kind) + " instead";
        }
        throw std::runtime_error(errorMsg);
    }

    return advance();
}

Token Parser::expect(TokenKind expected)
{
    return expectError(expected, "");
}
