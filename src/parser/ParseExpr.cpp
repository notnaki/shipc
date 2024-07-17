#include "parser/Parser.hpp"

std::unique_ptr<Expression> Parser::parse_expr(BindingPower bp)
{
    TokenKind tkind = currentTokenKind();

    auto nud_iter = nud_lu.find(tkind);
    if (nud_iter == nud_lu.end())
    {
        throw std::runtime_error("Not implemented NUD handler for token " + tokenKindToString(tkind));
    }

    std::unique_ptr<Expression> left = (this->*(nud_iter->second))();

    while (static_cast<int>(bp_lu[currentTokenKind()]) > static_cast<int>(bp))
    {
        tkind = currentTokenKind();
        auto led_iter = led_lu.find(tkind);
        if (led_iter == led_lu.end())
        {
            throw std::runtime_error("Not implemented LED handler for token " + tokenKindToString(tkind));
        }

        left = (this->*(led_iter->second))(std::move(left));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parse_int_expr()
{
    int intValue = std::stoi(advance().value);
    return std::make_unique<IntegerExpr>(intValue);
}

std::unique_ptr<Expression> Parser::parse_float_expr()
{
    float floatValue = std::stof(advance().value);
    return std::make_unique<FloatExpr>(floatValue);
}

std::unique_ptr<Expression> Parser::parse_double_expr()
{
    double doubleValue = std::stod(advance().value);
    return std::make_unique<DoubleExpr>(doubleValue);
}

std::unique_ptr<Expression> Parser::parse_string_expr()
{
    return std::make_unique<StringExpr>(advance().value);
}

std::unique_ptr<Expression> Parser::parse_fstring_expr()
{
    std::vector<std::unique_ptr<Expression>> parts;
    expect(TokenKind::FORMAT_INDICATOR);

    while (currentTokenKind() != TokenKind::FORMAT_END)
    {
        parts.push_back(parse_expr(BindingPower::Logical));
    }

    expect(TokenKind::FORMAT_END);
    return std::make_unique<FStringExpr>(std::move(parts));
}

std::unique_ptr<Expression> Parser::parse_symbol_expr()
{
    return std::make_unique<SymbolExpr>(advance().value);
}

std::unique_ptr<Expression> Parser::parse_bool_expr()
{
    return std::make_unique<BoolExpr>(advance().value);
}

std::unique_ptr<Expression> Parser::parse_ref_expr()
{
    expect(TokenKind::AND);
    return std::make_unique<RefExpr>(advance().value);
}

std::unique_ptr<Expression> Parser::parse_ptr_expr()
{
    expect(TokenKind::STAR);
    return std::make_unique<PtrExpr>(advance().value);
}

std::unique_ptr<Expression> Parser::parse_grouping_expr()
{
    expect(TokenKind::OPEN_PAREN);
    std::unique_ptr<Expression> expr = parse_expr(BindingPower::Default);
    expect(TokenKind::CLOSE_PAREN);
    return expr;
}

std::unique_ptr<Expression> Parser::parse_call_expr(std::unique_ptr<Expression> left)
{

    std::vector<std::unique_ptr<Expression>> params;

    expect(TokenKind::OPEN_PAREN);
    params = parse_call_arguments();
    expect(TokenKind::CLOSE_PAREN);

    auto callExpr = std::make_unique<CallExpr>(std::move(left), std::move(params));

    return callExpr;
}

std::unique_ptr<Expression> Parser::parse_assignment_expr(std::unique_ptr<Expression> left)
{

    expect(TokenKind::ASSIGNMENT);
    std::unique_ptr<Expression> newVal = parse_expr(BindingPower::Assignment);

    auto assignmentExpr = std::make_unique<AssignmentExpr>(std::move(left), std::move(newVal));
    return assignmentExpr;
}

std::unique_ptr<Expression> Parser::parse_struct_expr()
{
    expect(TokenKind::NEW);

    llvm::Type *t = parse_type();

    llvm::StructType *structType = llvm::dyn_cast<llvm::StructType>(t);
    if (!structType)
    {
        throw std::runtime_error("Type is not a struct type");
    }

    expect(TokenKind::OPEN_CURLY);

    std::vector<std::unique_ptr<Expression>> elements;

    while (currentTokenKind() != TokenKind::CLOSE_CURLY)
    {
        auto e = parse_expr(BindingPower::Logical);
        if (!e)
        {
            throw std::runtime_error("Failed to parse expression in struct initializer.");
        }

        elements.push_back(std::move(e));

        if (currentTokenKind() == TokenKind::COMMA)
        {
            expect(TokenKind::COMMA);
        }
        else if (currentTokenKind() != TokenKind::CLOSE_CURLY)
        {
            throw std::runtime_error("Expected ',' or '}' in struct expression.");
        }
    }

    expect(TokenKind::CLOSE_CURLY);

    if (elements.empty())
    {
        throw std::runtime_error("Struct cannot be empty.");
    }

    auto structExpr = std::make_unique<StructExpr>(structType, std::move(elements));

    return structExpr;
}

std::unique_ptr<Expression> Parser::parse_member_access_expr(std::unique_ptr<Expression> left)
{
    expect(TokenKind::DOT);

    std::string memberName = expect(TokenKind::IDENTIFIER).value;
    auto memberAccessExpr = std::make_unique<MemberAccessExpr>(std::move(left), memberName);

    return memberAccessExpr;
}

std::unique_ptr<Expression> Parser::parse_array_expr()
{
    expect(TokenKind::OPEN_CURLY); // eat {

    std::vector<std::unique_ptr<Expression>> elements;

    while (currentTokenKind() != TokenKind::CLOSE_CURLY)
    {
        auto e = parse_expr(BindingPower::Logical);
        if (!e)
        {
            throw std::runtime_error("Failed to parse expression in array initializer.");
        }

        // if (auto symbolExpr = dynamic_cast<SymbolExpr *>(e.get()))
        // {

        // }
        elements.push_back(std::move(e));

        if (currentTokenKind() == TokenKind::COMMA)
        {
            expect(TokenKind::COMMA); // eat ,
        }
        else if (currentTokenKind() != TokenKind::CLOSE_CURLY)
        {
            throw std::runtime_error("Expected ',' or '}' in array expression.");
        }
    }

    expect(TokenKind::CLOSE_CURLY); // eat }

    if (elements.empty())
    {
        throw std::runtime_error("Array cannot be empty.");
    }

    return std::make_unique<ArrayExpr>(std::move(elements));
}

std::unique_ptr<Expression> Parser::parse_array_access_expr(std::unique_ptr<Expression> arrayExpr)
{
    std::unique_ptr<Expression> index;

    expect(TokenKind::OPEN_BRACKET);           // eat [
    index = parse_expr(BindingPower::Primary); // Parse index expression
    expect(TokenKind::CLOSE_BRACKET);          // eat ]

    return std::make_unique<ArrayAccessExpr>(std::move(arrayExpr), std::move(index));
}

std::unique_ptr<Expression> Parser::parse_binary_expr(std::unique_ptr<Expression> left)
{
    Token op_token = currentToken();
    std::string op = op_token.value; // Use string to handle multi-character operators

    if (op != "+" && op != "-" && op != "*" && op != "/" && op != "%" &&
        op != "==" && op != "!=" && op != "<" && op != ">" && op != "<=" && op != ">=")
    {
        std::string errorMessage = "Operator '";
        errorMessage += op;
        errorMessage += "' is not a valid binary operator";
        throw std::runtime_error(errorMessage);
    }

    int current_bp = static_cast<int>(bp_lu[op_token.kind]);

    advance();

    std::unique_ptr<Expression> right = parse_expr(static_cast<BindingPower>(current_bp));

    while (current_bp < static_cast<int>(bp_lu[currentTokenKind()]))
    {
        op_token = currentToken();
        op = op_token.value; // Use string to handle multi-character operators
        current_bp = static_cast<int>(bp_lu[op_token.kind]);
        advance();
        right = parse_expr(static_cast<BindingPower>(current_bp));
    }

    // Check for comparison operators and create ComparisonExpr instead of BinaryExpr
    if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=")
    {
        auto compExpr = std::make_unique<ComparisonExpr>(std::move(left), std::move(right), op);
        return compExpr;
    }
    else
    {
        auto binaryExpr = std::make_unique<BinaryExpr>(std::move(left), std::move(right), op[0]); // Use op[0] for single character operators
        return binaryExpr;
    }
}

std::vector<std::unique_ptr<Expression>> Parser::parse_call_arguments()
{
    std::vector<std::unique_ptr<Expression>> exprs;

    while (currentTokenKind() != TokenKind::CLOSE_PAREN && hasTokens())
    {
        std::unique_ptr<Expression> expr = parse_expr(BindingPower::Default);
        exprs.push_back(std::move(expr));

        if (currentTokenKind() != TokenKind::CLOSE_PAREN)
        {
            expect(TokenKind::COMMA);
        }
    }

    return exprs;
}