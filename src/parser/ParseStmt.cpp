#include "parser/Parser.hpp"

std::unique_ptr<Statement> Parser::parse_stmt()
{
    switch (currentTokenKind())
    {
    case TokenKind::FN:
        return parse_fn_decl_stmt();

    case TokenKind::RETURN:
        return parse_return_stmt();

    case TokenKind::LET:
        return parse_var_decl_stmt();

    default:
        return parse_expr_stmt();
    }
}

std::unique_ptr<Statement> Parser::parse_expr_stmt()
{
    std::unique_ptr<Expression> expr = parse_expr(BindingPower::Default);
    expect(TokenKind::SEMI_COLON);

    auto exprStmt = std::make_unique<ExprStmt>(std::move(expr));
    return exprStmt;
}

std::unique_ptr<BlockStmt> Parser::parse_block_stmt()
{
    expect(TokenKind::OPEN_CURLY);

    std::vector<std::unique_ptr<Statement>> body;

    while (currentTokenKind() != TokenKind::CLOSE_CURLY && hasTokens())
    {
        body.push_back(parse_stmt());
    }

    expect(TokenKind::CLOSE_CURLY);

    auto blockStmt = std::make_unique<BlockStmt>(std::move(body));

    return blockStmt;
}

std::unique_ptr<Statement> Parser::parse_fn_decl_stmt()
{
    std::string name;
    std::vector<std::pair<std::string, llvm::Type *>> params;
    llvm::Type *returnType;

    expect(TokenKind::FN);

    name = expect(TokenKind::IDENTIFIER).value;

    expect(TokenKind::OPEN_PAREN);

    params = parse_fn_params();

    expect(TokenKind::CLOSE_PAREN);

    returnType = parse_type();

    auto body = parse_block_stmt();

    auto funcDecl = std::make_unique<FuncDeclStmt>(name, params, returnType, std::move(body));

    return funcDecl;
}

std::unique_ptr<Statement> Parser::parse_return_stmt()
{
    expect(TokenKind::RETURN);
    std::unique_ptr<Expression> value = parse_expr(BindingPower::Assignment);
    expect(TokenKind::SEMI_COLON);

    auto returnStmt = std::make_unique<ReturnStmt>(std::move(value));

    return returnStmt;
}

std::unique_ptr<Statement> Parser::parse_var_decl_stmt()
{
    llvm::Type *explicitType;
    std::unique_ptr<Expression> value;

    expect(TokenKind::LET);
    std::string varName = expectError(TokenKind::IDENTIFIER, "Inside variable declaration expected to find variable name").value;

    expect(TokenKind::COLON);

    explicitType = parse_type();

    if (currentTokenKind() != TokenKind::SEMI_COLON)
    {
        expect(TokenKind::ASSIGNMENT);
        value = parse_expr(BindingPower::Assignment);
    }

    expect(TokenKind::SEMI_COLON);

    auto varDeclStmt = std::make_unique<VarDeclStmt>(varName, explicitType, std::move(value));
    return varDeclStmt;
}

std::vector<std::pair<std::string, llvm::Type *>> Parser::parse_fn_params()
{
    std::vector<std::pair<std::string, llvm::Type *>> params;

    while (currentTokenKind() != TokenKind::CLOSE_PAREN && hasTokens())
    {
        std::string paramName = expect(TokenKind::IDENTIFIER).value;
        expect(TokenKind::COLON);
        llvm::Type *paramType = parse_type();
        params.push_back(std::make_pair(paramName, paramType));

        if (currentTokenKind() != TokenKind::CLOSE_PAREN)
        {
            expect(TokenKind::COMMA);
        }
    }

    return params;
}
