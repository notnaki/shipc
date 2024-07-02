#include "parser/Parser.hpp"

llvm::Type *Parser::parse_type()
{
    TokenKind tokenKind = currentTokenKind();

    switch (tokenKind)
    {
    case TokenKind::IDENTIFIER:
        /* code */
        return parse_symbol_type();
    case TokenKind::OPEN_BRACKET:
        return parse_array_type();

    default:
        throw std::runtime_error("Not set up for this type yet");
    }
}

llvm::Type *Parser::parse_symbol_type()
{
    const std::unordered_map<std::string, llvm::Type *> typesMap = {
        {"string", llvm::Type::getInt8Ty(cc.getContext())->getPointerTo()},
        {"float", llvm::Type::getFloatTy(cc.getContext())},
        {"double", llvm::Type::getDoubleTy(cc.getContext())},
        {"int", llvm::Type::getInt32Ty(cc.getContext())},
        {"void", llvm::Type::getVoidTy(cc.getContext())},
    };

    return typesMap.at(expect(TokenKind::IDENTIFIER).value);
}

llvm::Type *Parser::parse_array_type()
{
    expect(TokenKind::OPEN_BRACKET);
    int arrayLength = stoi(expect(TokenKind::INT_NUMBER).value);
    expect(TokenKind::CLOSE_BRACKET);
    llvm::Type *underlyingType = parse_type();
    return llvm::ArrayType::get(underlyingType, arrayLength);
}
