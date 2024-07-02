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
