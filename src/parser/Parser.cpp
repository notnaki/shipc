#include "parser/Parser.hpp"

Program Parser::Parse()
{
    nud_lu = {
        {TokenKind::INT_NUMBER, &Parser::parse_int_expr},
        {TokenKind::FLOAT_NUMBER, &Parser::parse_float_expr},
        {TokenKind::DOUBLE_NUMBER, &Parser::parse_double_expr},
        {TokenKind::TRUE, &Parser::parse_bool_expr},
        {TokenKind::FALSE, &Parser::parse_bool_expr},
        {TokenKind::STRING, &Parser::parse_string_expr},
        {TokenKind::IDENTIFIER, &Parser::parse_symbol_expr},
        {TokenKind::STAR, &Parser::parse_ptr_expr},
        {TokenKind::AND, &Parser::parse_ref_expr},
        {TokenKind::NEW, &Parser::parse_struct_expr},
        {TokenKind::OPEN_CURLY, &Parser::parse_array_expr},
        {TokenKind::OPEN_PAREN, &Parser::parse_grouping_expr},

    };

    led_lu = {
        {TokenKind::OPEN_PAREN, &Parser::parse_call_expr},
        {TokenKind::DOT, &Parser::parse_member_access_expr},
        {TokenKind::OPEN_BRACKET, &Parser::parse_array_access_expr},

        {TokenKind::PLUS, &Parser::parse_binary_expr},
        {TokenKind::DASH, &Parser::parse_binary_expr},
        {TokenKind::STAR, &Parser::parse_binary_expr},
        {TokenKind::SLASH, &Parser::parse_binary_expr},
        {TokenKind::PERCENT, &Parser::parse_binary_expr},

        {TokenKind::EQUALS, &Parser::parse_binary_expr},
        {TokenKind::NOT_EQUALS, &Parser::parse_binary_expr},
        {TokenKind::LESS, &Parser::parse_binary_expr},
        {TokenKind::LESS_EQUALS, &Parser::parse_binary_expr},
        {TokenKind::GREATER, &Parser::parse_binary_expr},
        {TokenKind::GREATER_EQUALS, &Parser::parse_binary_expr},

    };

    bp_lu = {
        {TokenKind::OPEN_PAREN, BindingPower::Call},
        {TokenKind::OPEN_BRACKET, BindingPower::Call},
        {TokenKind::DOT, BindingPower::Member},

        {TokenKind::STAR, BindingPower::Multiplicative},
        {TokenKind::SLASH, BindingPower::Multiplicative},
        {TokenKind::PERCENT, BindingPower::Multiplicative},
        {TokenKind::PLUS, BindingPower::Additive},
        {TokenKind::DASH, BindingPower::Additive},

        {TokenKind::EQUALS, BindingPower::Relational},
        {TokenKind::NOT_EQUALS, BindingPower::Relational},
        {TokenKind::LESS, BindingPower::Relational},
        {TokenKind::LESS_EQUALS, BindingPower::Relational},
        {TokenKind::GREATER, BindingPower::Relational},
        {TokenKind::GREATER_EQUALS, BindingPower::Relational},
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
