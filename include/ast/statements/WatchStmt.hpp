#ifndef WATCHSTMT_HPP
#define WATCHSTMT_HPP

#include "Statement.hpp"
#include <string>

class WatchStmt : public Statement
{
private:
std::string watchName;
std::string watchFunctionName;

public:
    WatchStmt(std::string name, std::string funcName) : watchName(name), watchFunctionName(funcName)  {}

    void codegen(CompilerContext &cc) const override
    {
        cc.getTable().addWatch(watchName, watchFunctionName);
    }
};

#endif
