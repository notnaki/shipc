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

    case TokenKind::STRUCT:
        return parse_struct_decl_stmt();

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

std::unique_ptr<Statement> Parser::parse_struct_decl_stmt()
{
    // TODO add to parse_type symboltype
    expect(TokenKind::STRUCT);

    std::vector<llvm::Type *> memberTypes;
    std::unordered_map<std::string, llvm::Type *> props;
    std::unordered_map<std::string, unsigned> memberMap;

    std::string structName = expect(TokenKind::IDENTIFIER).value;

    expect(TokenKind::OPEN_CURLY);

    llvm::StructType *structType = llvm::StructType::create(cc.getContext(), structName);
    cc.getTable().addStructType(structName, structType);

    unsigned idx = 0;
    while (currentTokenKind() != TokenKind::CLOSE_CURLY && hasTokens())
    {
        std::string propName = expect(TokenKind::IDENTIFIER).value;
        expectError(TokenKind::COLON, "Expected to find colon following property name inside struct declaration");

        llvm::Type *propType = parse_type();

        if (propType->isStructTy())
        {
            if (propType->getStructName() == structName)
            {
                propType = propType->getPointerTo();
            }
        }

        expect(TokenKind::SEMI_COLON);

        if (props[propName] != nullptr)
        {
            throw std::runtime_error("Property " + propName + " has already been defined in struct declaration");
        }

        memberTypes.push_back(propType);
        props[propName] = propType;
        memberMap[propName] = idx;
        ++idx;
    }

    expect(TokenKind::CLOSE_CURLY);
    structType->setBody(memberTypes);

    cc.getTable().addStructType(structName, structType);
    cc.getTable().addStructMemberMap(structName, memberMap);

    auto structDeclStmt = std::make_unique<StructDeclStmt>();

    return structDeclStmt;
}

// |----- Helpers -----|

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