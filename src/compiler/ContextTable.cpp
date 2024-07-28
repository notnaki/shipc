#include "compiler/ContextTable.hpp"

void ContextTable::addVariable(std::string name, llvm::Value *value)
{
    if (resolveVariable(name))
    {
        throw std::runtime_error("Variable '" + name + "' is already defined");
    }
    variables[name] = value;
}

void ContextTable::removeVariable(std::string name)
{

    variables.erase(name);
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
    ContextTable *existsTable = resolveStructType(name);

    if (existsTable == nullptr)
    {
        throw std::runtime_error("Struct type '" + name + "' not found in scope.");
    }
    else
    {
        return existsTable->structTypes[name];
    }
}

void ContextTable::addStructMemberMap(std::string structName, std::unordered_map<std::string, unsigned> memberMap)
{
    structMemberMap[structName] = memberMap;
}

unsigned ContextTable::getStructMemberIdx(std::string structName, std::string memberName)
{
    ContextTable *existsTable = resolveStructType(structName);

    if (existsTable == nullptr)
    {
        throw std::runtime_error("Struct '" + structName + "' not found in scope.");
    }

    const std::unordered_map<std::string, unsigned int> &structDef = existsTable->structMemberMap[structName];

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

ContextTable *ContextTable::resolveStructType(const std::string &name)
{
    if (structTypes.find(name) != structTypes.end())
    {
        return this;
    }
    else if (parent)
    {
        return parent->resolveStructType(name);
    }
    return nullptr;
}

void ContextTable::setParent(ContextTable *p)
{
    parent = p;
}


void ContextTable::addWatch(std::string name, std::string watchFunctionName) {
    watchedVariables[name] = watchFunctionName;
}

std::string ContextTable::getWatch(std::string name)  {
    ContextTable *existsTable = resolveWatch(name);

    if (existsTable == nullptr)
    {
        return "";
    }
    else
    {
        return existsTable->watchedVariables[name];
    }
}

bool ContextTable::containsWatch(const std::string &name) const
{
    return watchedVariables.find(name) != watchedVariables.end();
}

ContextTable *ContextTable::resolveWatch(const std::string &name)
{
    if (containsWatch(name))
    {
        return this;
    }
    else if (parent)
    {
        return parent->resolveWatch(name);
    }
    return nullptr;
}

void ContextTable::addStructMemberPrivacy(std::string structName, std::unordered_map<std::string, bool> memberPrivacy){
    structMemberPrivacy[structName] = memberPrivacy;
}

bool ContextTable::isStructMemberPrivate(std::string structName, std::string memberName){
    ContextTable *existsTable = resolveStructType(structName);

    if (existsTable == nullptr)
    {
        throw std::runtime_error("Struct '" + structName + "' not found in scope.");
    }

    const std::unordered_map<std::string, bool> &structDef = existsTable->structMemberPrivacy[structName];

    auto memberIt = structDef.find(memberName);
    if (memberIt == structDef.end())
    {
        throw std::runtime_error("Member '" + memberName + "' not found for struct '" + structName + "'");
    }

    return memberIt->second;
}
