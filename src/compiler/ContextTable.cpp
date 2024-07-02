#include "compiler/ContextTable.hpp"

llvm::Type *ContextTable::getElementType(std::string elementName)
{
    return contextTable[elementName].second;
}

llvm::Value *ContextTable::getElementValue(std::string elementName)
{
    return contextTable[elementName].first;
}

void ContextTable::addElement(std::string elementName, llvm::Value *elementValue, llvm::Type *elementType)
{
    auto elem = std::make_pair(elementValue, elementType);
    contextTable[elementName] = elem;
}

llvm::StructType *ContextTable::getStructType(std::string structName)
{
    return structTypes[structName];
}

void ContextTable::addStructType(std::string structName, llvm::StructType *structType)
{
    structTypes[structName] = structType;
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