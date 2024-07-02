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
    std::unordered_map<std::string, std::pair<llvm::Value *, llvm::Type *>> contextTable;
    std::unordered_map<std::string, llvm::StructType *> structTypes;

    std::unordered_map<std::string, std::unordered_map<std::string, unsigned>> structMemberMap;

public:
    llvm::Type *getElementType(std::string elementName);
    llvm::Value *getElementValue(std::string elementName);
    void addElement(std::string elementName, llvm::Value *elementValue, llvm::Type *elementType);

    llvm::StructType *getStructType(std::string structName);
    void addStructType(std::string structName, llvm::StructType *structType);
    void addStructMemberMap(std::string structName, std::unordered_map<std::string, unsigned> memberMap);
    unsigned getStructMemberIdx(std::string structName, std::string memberName);
};

#endif