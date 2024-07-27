#ifndef TOKENKINDS_HPP
#define TOKENKINDS_HPP

enum class TokenKind
{
    EOF_TOKEN,
    NULL_TOKEN,
    TRUE,
    FALSE,
    INT_NUMBER,
    FLOAT_NUMBER,
    DOUBLE_NUMBER,
    STRING,
    IDENTIFIER,

    // Paranthese variations
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_CURLY,
    CLOSE_CURLY,
    OPEN_PAREN,
    CLOSE_PAREN,

    // equals stuff
    ASSIGNMENT,
    EQUALS,
    NOT_EQUALS,
    NOT,

    // conditions
    LESS,
    LESS_EQUALS,
    GREATER,
    GREATER_EQUALS,

    // logic
    OR,
    LAND,

    // Symbols
    DOT,
    SEMI_COLON,
    COLON,
    COMMA,

    // Shorthand
    PLUS_PLUS,
    MINUS_MINUS,
    PLUS_EQUALS,
    MINUS_EQUALS,

    // Maths
    PLUS,
    DASH,
    SLASH,
    STAR,
    PERCENT,

    // Pointers
    AND,

    // Reserved Keywords
    LET,
    CONST,
    NEW,
    IMPORT,
    FROM,
    FN,
    IF,
    ELSE,
    FOREACH,
    WHILE,
    FOR,
    IN,
    STRUCT,
    IMPL,
    RETURN,
    BREAK,

    FORMAT_INDICATOR,
    FORMAT_END,
    WATCH,
    PRIVATE,
    PUBLIC,
};

#endif
