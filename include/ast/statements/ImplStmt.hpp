#ifndef IMPLSTMT_HPP
#define IMPLSTMT_HPP

#include "Statement.hpp"
#include "FuncDeclStmt.hpp"


class ImplStmt : public Statement
{
private:
    std::string name;
    std::vector<std::unique_ptr<Statement>> functions;

public:
    ImplStmt(std::string structName, std::vector<std::unique_ptr<Statement>> func_list) : name(structName), functions(std::move(func_list)) {}


    void codegen(CompilerContext &cc) const override
    {
        cc.setScopeName(name);
        for (const auto &func : functions)
        {
            if (auto *f = dynamic_cast<FuncDeclStmt *>(func.get())){
                f->setStructName(name);
            }
            else
            {
                throw std::runtime_error("ImplStmt::codegen: Statement is not a Function");
            }
            func->codegen(cc);
        }
        cc.clearScopeName();
    }
};

#endif
