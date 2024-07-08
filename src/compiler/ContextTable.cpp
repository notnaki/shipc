#include "compiler/ContextTable.hpp"

void ContextTable::addVariable(std::string name, llvm::Value *value)
{
    variables[name] = value;
}

llvm::Value *ContextTable::getVariable(std::string name)
{
    ContextTable *existsTable = resolveVariable(name);

    if (existsTable == nullptr)
    {
        throw std::runtime_error("Variable '" + name + "' not found in scope.");
    }
    else
    {
        return existsTable->variables[name];
    }
}

void ContextTable::addStructType(std::string name, llvm::Type *type)
{
    structTypes[name] = type;
}

llvm::Type *ContextTable::getStructType(std::string name)
{
    return structTypes[name];
}

void ContextTable::addStructMemberMap(std::string structName, std::unordered_map<std::string, unsigned> memberMap)
{
    structMemberMap[structName] = memberMap;
}

unsigned ContextTable::getStructMemberIdx(std::string structName, std::string memberName)
{
    auto structDefIt = structMemberMap.find(structName);
    if (structDefIt == structMemberMap.end())
    {
        printf("Struct not found\n");
        return 0;
    }

    const std::unordered_map<std::string, unsigned int> &structDef = structDefIt->second;

    auto memberIt = structDef.find(memberName);
    if (memberIt == structDef.end())
    {
        throw std::runtime_error("Member '" + memberName + "' not found for struct '" + structName + "'");
    }

    return memberIt->second;
}

bool ContextTable::containsVariable(const std::string &name) const
{
    return variables.find(name) != variables.end();
}

ContextTable *ContextTable::resolveVariable(const std::string &name)
{
    if (containsVariable(name))
    {
        return this;
    }
    else if (parent)
    {
        return parent->resolveVariable(name);
    }
    return nullptr;
}

void ContextTable::setParent(ContextTable *p)
{
    parent = p;
}