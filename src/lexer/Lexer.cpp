#include "lexer/Lexer.hpp"

std::string Lexer::remainder()
{
    return source.substr(pos);
}

std::vector<Token> Lexer::tokenize()
{

    while (pos < source.size())
    {
        char current = source[pos];

        if (std::isspace(current))
        {
            ++pos;
            continue;
        }

        // handle strings
        if (current == '"')
        {
            std::size_t end_pos = source.find('"', pos + 1);
            if (end_pos != std::string::npos)
            {
                tokens.push_back({TokenKind::STRING, source.substr(pos + 1, end_pos - pos - 1)});
                pos = end_pos + 1;
                continue;
            }

            throw std::runtime_error("Lexer::Error -> Unterminated string near " + remainder());
        }

        // handle numbers
        if (std::isdigit(current))
        {
            std::size_t start_pos = pos;
            std::size_t end_pos = pos;
            TokenKind num_type = TokenKind::INT_NUMBER;

            while (std::isdigit(source[end_pos]))
            {
                ++end_pos;
            }

            // check for floating point number
            if (source[end_pos] == '.')
            {
                num_type = TokenKind::DOUBLE_NUMBER;
                ++end_pos;
                while (std::isdigit(source[end_pos]))
                {
                    ++end_pos;
                }

                // check for float
                if (source[end_pos] == 'f' || source[end_pos] == 'F')
                {
                    num_type = TokenKind::FLOAT_NUMBER;
                    ++end_pos;
                }
            }

            tokens.push_back({num_type, source.substr(start_pos, end_pos - start_pos)});
            pos = end_pos;
            continue;
        }

        // handle identifiers
        if (std::isalpha(current) || current == '_')
        {
            std::size_t end_pos = pos;
            while (std::isalnum(source[end_pos]) || source[end_pos] == '_')
            {
                ++end_pos;
            }
            std::string token_value = source.substr(pos, end_pos - pos);
            if (reserved_words.count(token_value))
            {
                tokens.push_back({reserved_words.at(token_value), token_value});
            }
            else
            {
                tokens.push_back({TokenKind::IDENTIFIER, token_value});
            }
            pos = end_pos;
            continue;
        }

        // handle single and double char tokens
        switch (current)
        {
        case '#':
            pos = source.find('\n', pos);
            if (pos == std::string::npos)
            {
                pos = source.size();
            }
            break;
        case '[':
            tokens.push_back({TokenKind::OPEN_BRACKET, "["});
            break;
        case ']':
            tokens.push_back({TokenKind::CLOSE_BRACKET, "]"});
            break;
        case '{':
            tokens.push_back({TokenKind::OPEN_CURLY, "{"});
            break;
        case '}':
            tokens.push_back({TokenKind::CLOSE_CURLY, "}"});
            break;
        case '(':
            tokens.push_back({TokenKind::OPEN_PAREN, "("});
            break;
        case ')':
            tokens.push_back({TokenKind::CLOSE_PAREN, ")"});
            break;
        case '=':
            if (pos + 1 < source.size() && source[pos + 1] == '=')
            {
                tokens.push_back({TokenKind::EQUALS, "=="});
                ++pos;
            }
            else
            {
                tokens.push_back({TokenKind::ASSIGNMENT, "="});
            }
            break;
        case '!':
            if (pos + 1 < source.size() && source[pos + 1] == '=')
            {
                tokens.push_back({TokenKind::NOT_EQUALS, "!="});
                ++pos;
            }
            else
            {
                tokens.push_back({TokenKind::NOT, "!"});
            }
            break;
        case '<':
            if (pos + 1 < source.size() && source[pos + 1] == '=')
            {
                tokens.push_back({TokenKind::LESS_EQUALS, "<="});
                ++pos;
            }
            else
            {
                tokens.push_back({TokenKind::LESS, "<"});
            }
            break;
        case '>':
            if (pos + 1 < source.size() && source[pos + 1] == '=')
            {
                tokens.push_back({TokenKind::GREATER_EQUALS, ">="});
                ++pos;
            }
            else
            {
                tokens.push_back({TokenKind::GREATER, ">"});
            }
            break;
        case '|':
            if (pos + 1 < source.size() && source[pos + 1] == '|')
            {
                tokens.push_back({TokenKind::OR, "||"});
                ++pos;
            }
            else
            {
                throw std::runtime_error("Lexer::Error -> unrecognized token near " + remainder());
            }
            break;
        case '&':
            if (pos + 1 < source.size() && source[pos + 1] == '&')
            {
                tokens.push_back({TokenKind::LAND, "&&"});
                ++pos;
            }
            else
            {
                tokens.push_back({TokenKind::AND, "&"});
            }
            break;
        case '.':
            tokens.push_back({TokenKind::DOT, "."});
            break;
        case ';':
            tokens.push_back({TokenKind::SEMI_COLON, ";"});
            break;
        case ':':
            tokens.push_back({TokenKind::COLON, ":"});
            break;
        case ',':
            tokens.push_back({TokenKind::COMMA, ","});
            break;
        case '+':
            if (pos + 1 < source.size() && source[pos + 1] == '+')
            {
                tokens.push_back({TokenKind::PLUS_PLUS, "++"});
                ++pos;
            }
            else if (pos + 1 < source.size() && source[pos + 1] == '=')
            {
                tokens.push_back({TokenKind::PLUS_EQUALS, "+="});
                ++pos;
            }
            else
            {
                tokens.push_back({TokenKind::PLUS, "+"});
            }
            break;
        case '-':
            if (pos + 1 < source.size() && source[pos + 1] == '-')
            {
                tokens.push_back({TokenKind::MINUS_MINUS, "--"});
                ++pos;
            }
            else if (pos + 1 < source.size() && source[pos + 1] == '=')
            {
                tokens.push_back({TokenKind::MINUS_EQUALS, "-="});
                ++pos;
            }
            else
            {
                tokens.push_back({TokenKind::DASH, "-"});
            }
            break;
        case '*':
            tokens.push_back({TokenKind::STAR, "*"});
            break;
        case '/':
            tokens.push_back({TokenKind::SLASH, "/"});
            break;
        case '%':
            tokens.push_back({TokenKind::PERCENT, "%"});
            break;
        default:

            throw std::runtime_error("Lexer::Error -> unrecognized token near " + remainder());
        }

        ++pos;
    }

    // push the eof token
    tokens.push_back({TokenKind::EOF_TOKEN, "EOF"});
    return tokens;
}