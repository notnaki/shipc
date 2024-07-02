#ifndef CONTEXTTABLE_HPP
#define CONTEXTTABLE_HPP

#include <utility>
#include <unordered_map>

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

class ContextTable
{
private:
    std::unordered_map<std::string, std::pair<llvm::Value *, llvm::Type *>> contextTable;

public:
    llvm::Type *getElementType(std::string elementName);
    llvm::Value *getElementValue(std::string elementName);
    void addElement(std::string elementName, llvm::Value *elementValue, llvm::Type *elementType);
};

#endif