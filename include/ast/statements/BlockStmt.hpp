#ifndef BLOCKSTMT_HPP
#define BLOCKSTMT_HPP

#include "Statement.hpp"

class BlockStmt : public Statement
{
private:
    std::vector<std::unique_ptr<Statement>> body;

public:
    BlockStmt(std::vector<std::unique_ptr<Statement>> statements) : body(std::move(statements)) {}

    void codegen(CompilerContext &cc) const override
    {
        ContextTable oldTable = cc.getTable();
        ContextTable newTable;

        newTable.setParent(&oldTable);

        cc.setTable(newTable);

        for (const auto &stmt : body)
        {
            stmt->codegen(cc);
        }

        cc.setTable(oldTable);
    }
};

#endif
