#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <sstream>

#include "BindingPower.hpp"
#include "lexer/Tokens.hpp"
#include "lexer/util.hpp"

#include "compiler/CompilerContext.hpp"

#include "ast/Program.hpp"
#include "ast/statements/Statement.hpp"
#include "ast/statements/BlockStmt.hpp"
#include "ast/statements/FuncDeclStmt.hpp"
#include "ast/statements/ExprStmt.hpp"
#include "ast/statements/ReturnStmt.hpp"
#include "ast/statements/VarDeclStmt.hpp"
#include "ast/statements/StructDeclStmt.hpp"
#include "ast/statements/IfStmt.hpp"
#include "ast/statements/WhileStmt.hpp"
#include "ast/statements/ForStmt.hpp"
#include "ast/statements/ImplStmt.hpp"



#include "ast/expressions/ComparisonExpr.hpp"
#include "ast/expressions/BinaryExpr.hpp"
#include "ast/expressions/BoolExpr.hpp"
#include "ast/expressions/ArrayAccessExpr.hpp"
#include "ast/expressions/ArrayExpr.hpp"
#include "ast/expressions/MemberAccessExpr.hpp"
#include "ast/expressions/StructExpr.hpp"
#include "ast/expressions/PtrExpr.hpp"
#include "ast/expressions/RefExpr.hpp"
#include "ast/expressions/SymbolExpr.hpp"
#include "ast/expressions/Expression.hpp"
#include "ast/expressions/IntegerExpr.hpp"
#include "ast/expressions/FloatExpr.hpp"
#include "ast/expressions/DoubleExpr.hpp"
#include "ast/expressions/StringExpr.hpp"
#include "ast/expressions/FStringExpr.hpp"
#include "ast/expressions/CallExpr.hpp"
#include "ast/expressions/AssignmentExpr.hpp"

class Parser
{
private:
    size_t pos;
    std::vector<Token> tokens;

    CompilerContext &cc;

public:
    Parser(std::vector<Token> t, CompilerContext &context) : tokens(t), pos(0), cc(context) {}
    Program Parse();

private:
    typedef std::unique_ptr<Expression> (Parser::*NudFn)();
    typedef std::unique_ptr<Expression> (Parser::*LedFn)(std::unique_ptr<Expression> left);

    std::unordered_map<TokenKind, NudFn> nud_lu;
    std::unordered_map<TokenKind, LedFn> led_lu;
    std::unordered_map<TokenKind, BindingPower> bp_lu;

    Token currentToken();
    TokenKind currentTokenKind();
    bool hasTokens();

    Token advance();
    Token expectError(TokenKind expected, const std::string &err);
    Token expect(TokenKind expected);

    // Statement

    std::unique_ptr<Statement> parse_stmt();
    std::unique_ptr<Statement> parse_expr_stmt();
    std::unique_ptr<BlockStmt> parse_block_stmt();
    std::unique_ptr<Statement> parse_fn_decl_stmt();
    std::unique_ptr<Statement> parse_return_stmt();
    std::unique_ptr<Statement> parse_var_decl_stmt();
    std::unique_ptr<Statement> parse_struct_decl_stmt();
    std::unique_ptr<Statement> parse_if_stmt();
    std::unique_ptr<Statement> parse_while_stmt();
    std::unique_ptr<Statement> parse_for_stmt();
    std::unique_ptr<Statement> parse_impl_stmt();

    std::vector<std::pair<std::string, llvm::Type *>> parse_fn_params();

    // Expression

    std::unique_ptr<Expression> parse_expr(BindingPower bp);
    std::unique_ptr<Expression> parse_int_expr();
    std::unique_ptr<Expression> parse_float_expr();
    std::unique_ptr<Expression> parse_double_expr();
    std::unique_ptr<Expression> parse_string_expr();
    std::unique_ptr<Expression> parse_fstring_expr();
    std::unique_ptr<Expression> parse_bool_expr();
    std::unique_ptr<Expression> parse_symbol_expr();
    std::unique_ptr<Expression> parse_ptr_expr();
    std::unique_ptr<Expression> parse_ref_expr();
    std::unique_ptr<Expression> parse_grouping_expr();
    std::unique_ptr<Expression> parse_struct_expr();
    std::unique_ptr<Expression> parse_member_access_expr(std::unique_ptr<Expression> left);
    std::unique_ptr<Expression> parse_array_expr();
    std::unique_ptr<Expression> parse_assignment_expr(std::unique_ptr<Expression> left);
    std::unique_ptr<Expression> parse_array_access_expr(std::unique_ptr<Expression> arrayExpr);
    std::unique_ptr<Expression> parse_binary_expr(std::unique_ptr<Expression> left);
    std::unique_ptr<Expression> parse_call_expr(std::unique_ptr<Expression> left);

    std::vector<std::unique_ptr<Expression>> parse_call_arguments();

    // Type
    llvm::Type *parse_type();
    llvm::Type *parse_struct_type(std::string structName);
    llvm::Type *parse_symbol_type();
    llvm::Type *parse_array_type();
};

#endif
