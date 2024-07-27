#include "ast/statements/Statement.hpp"
#include "lexer/TokenKinds.hpp"
#include "parser/Parser.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

std::unique_ptr<Statement> Parser::parse_stmt()
{
    switch (currentTokenKind())
    {
    case TokenKind::FN:
        return parse_fn_decl_stmt();

    case TokenKind::PRIVATE:
        return parse_fn_decl_stmt();

    case TokenKind::PUBLIC:
        return parse_fn_decl_stmt();

    case TokenKind::RETURN:
        return parse_return_stmt();

    case TokenKind::LET:
        return parse_var_decl_stmt();

    case TokenKind::STRUCT:
        return parse_struct_decl_stmt();

    case TokenKind::IF:
        return parse_if_stmt();

    case TokenKind::WHILE:
        return parse_while_stmt();

    case TokenKind::FOR:
        return parse_for_stmt();

    case TokenKind::IMPL:
        return parse_impl_stmt();

    case TokenKind::WATCH:
        return parse_watch_stmt();

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
    bool isPrivate;

    // check for public/private keywords if public set isPrivate to false else set it to true

    if (currentTokenKind() == TokenKind::PUBLIC){
        isPrivate = false;
        advance();
    }else if (currentTokenKind() == TokenKind::PRIVATE){
        isPrivate = true;
        advance();
    }else {
        isPrivate = true;
    }

    expect(TokenKind::FN);

    name = expect(TokenKind::IDENTIFIER).value;

    if (name == "main" && isPrivate == false){
        throw std::runtime_error("Main function can't be public.");
    }

    expect(TokenKind::OPEN_PAREN);

    params = parse_fn_params();

    expect(TokenKind::CLOSE_PAREN);

    returnType = parse_type();

    auto body = parse_block_stmt();

    auto funcDecl = std::make_unique<FuncDeclStmt>(name, params, returnType, std::move(body), isPrivate);

    return funcDecl;
}

std::unique_ptr<Statement> Parser::parse_watch_stmt()
{
    expect(TokenKind::WATCH);

    expect(TokenKind::LESS);

    std::string name = expect(TokenKind::IDENTIFIER).value;

    expect(TokenKind::GREATER);

    expect(TokenKind::OPEN_PAREN);

    std::string fnName = expect(TokenKind::IDENTIFIER).value;

    expect(TokenKind::CLOSE_PAREN);



    auto watchStmt = std::make_unique<WatchStmt>(name, fnName);

    expect(TokenKind::SEMI_COLON);

    return watchStmt;
}

std::unique_ptr<Statement> Parser::parse_impl_stmt(){
    expect(TokenKind::IMPL);
    std::string structName = expect(TokenKind::IDENTIFIER).value;
    std::vector<std::unique_ptr<Statement>> func_list;

    expect(TokenKind::OPEN_CURLY);

    while (currentTokenKind() != TokenKind::CLOSE_CURLY && hasTokens())
    {
        func_list.push_back(std::move(parse_stmt()));
    }

    expect(TokenKind::CLOSE_CURLY);


    return std::make_unique<ImplStmt>(structName, std::move(func_list));
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

    std::unique_ptr<Expression> value;

    expect(TokenKind::LET);
    std::string varName = expectError(TokenKind::IDENTIFIER, "Inside variable declaration expected to find variable name").value;

    expect(TokenKind::COLON);

    auto explicitType = parse_type();

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

std::unique_ptr<Statement> Parser::parse_if_stmt()
{
    expect(TokenKind::IF);
    expect(TokenKind::OPEN_PAREN);

    std::unique_ptr<Expression> condition = parse_expr(BindingPower::Default);

    expect(TokenKind::CLOSE_PAREN);

    // parse the if body | the body that will be executed if true

    std::unique_ptr<BlockStmt> body = parse_block_stmt();

    // parse the else body | the body that will be executed if false

    std::unique_ptr<BlockStmt> elseBody;
    std::vector<std::pair<std::unique_ptr<Expression>, std::unique_ptr<BlockStmt>>> elifBodies;

    while (currentTokenKind() == TokenKind::ELSE)
    {
        expect(TokenKind::ELSE);

        if (currentTokenKind() == TokenKind::IF)
        {
            expect(TokenKind::IF);
            expect(TokenKind::OPEN_PAREN);
            std::unique_ptr<Expression> elifCond = parse_expr(BindingPower::Default);
            expect(TokenKind::CLOSE_PAREN);
            std::unique_ptr<BlockStmt> elifBody = parse_block_stmt();

            elifBodies.emplace_back(std::move(elifCond), std::move(elifBody));
        }
        else
        {
            elseBody = parse_block_stmt();
            break;
        }
    }

    auto ifStmt = std::make_unique<IfStmt>(std::move(body), std::move(elseBody), std::move(elifBodies), std::move(condition));

    return ifStmt;
}

std::unique_ptr<Statement> Parser::parse_while_stmt()
{

    expect(TokenKind::WHILE);
    expect(TokenKind::OPEN_PAREN);
    std::unique_ptr<Expression> condition = parse_expr(BindingPower::Default);
    expect(TokenKind::CLOSE_PAREN);
    std::unique_ptr<BlockStmt> body = parse_block_stmt();

    auto whileStmt = std::make_unique<WhileStmt>(std::move(body), std::move(condition));

    return whileStmt;
}

std::unique_ptr<Statement> Parser::parse_for_stmt()
{
    expect(TokenKind::FOR);
    expect(TokenKind::OPEN_PAREN);
    std::unique_ptr<Statement> vardecl = parse_var_decl_stmt();

    std::unique_ptr<Expression> condition = parse_expr(BindingPower::Default);
    expect(TokenKind::SEMI_COLON);

    std::unique_ptr<Expression> after = parse_expr(BindingPower::Comma);
    expect(TokenKind::SEMI_COLON);
    expect(TokenKind::CLOSE_PAREN);

    std::unique_ptr<BlockStmt> body = parse_block_stmt();

    auto forStmt = std::make_unique<ForStmt>(std::move(vardecl), std::move(condition), std::move(after), std::move(body));

    return forStmt;
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
