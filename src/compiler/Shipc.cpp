#include "compiler/Shipc.hpp"

void Shipc::exec()
{
    std::string source = readFile(name);
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // printTokens(tokens);

    CompilerContext cc("m_mod");

    Parser parser(tokens, cc);
    auto ast = parser.Parse();

    ast.codegen(cc);

    cc.save("out.ll");
}

std::string Shipc::replace_escape_sequences(const std::string &input)
{
    std::string result;
    result.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i] == '\\' && i + 1 < input.size())
        {
            switch (input[i + 1])
            {
            case 'n':
                result += '\n';
                ++i;
                break;
            case '\\':
                result += '\\';
                ++i;
                break;
            default:
                result += input[i];
                break;
            }
        }
        else
        {
            result += input[i];
        }
    }

    return result;
}

std::string Shipc::readFile(std::string filename)
{

    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_contents = buffer.str();

    file.close();

    return replace_escape_sequences(file_contents);
}
