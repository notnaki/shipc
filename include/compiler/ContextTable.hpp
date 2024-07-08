#ifndef CONTEXTTABLE_HPP
#define CONTEXTTABLE_HPP

#include <utility>
#include <unordered_map>

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class ContextTable
{
private:
    ContextTable *parent;

    std::unordered_map<std::string, llvm::Value *> variables;

    std::unordered_map<std::string, llvm::Type *> structTypes;
    std::unordered_map<std::string, std::unordered_map<std::string, unsigned>> structMemberMap;

    bool containsVariable(const std::string &name) const;
    ContextTable *resolveVariable(const std::string &name);
    ContextTable *resolveStructType(const std::string &name);

public:
    ContextTable() : parent(nullptr) {}

    void setParent(ContextTable *p);

    void addVariable(std::string name, llvm::Value *value);
    llvm::Value *getVariable(std::string name);

    void addStructType(std::string name, llvm::Type *structType);
    llvm::Type *getStructType(std::string name);

    void addStructMemberMap(std::string structName, std::unordered_map<std::string, unsigned> memberMap);
    unsigned getStructMemberIdx(std::string structName, std::string memberName);
};

#endif