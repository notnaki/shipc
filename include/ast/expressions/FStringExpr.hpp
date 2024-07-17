#ifndef FSTRING_HPP
#define FSTRING_HPP

#include "Expression.hpp"

class FStringExpr : public Expression
{
private:
    std::vector<std::unique_ptr<Expression>> parts;

public:
    FStringExpr(std::vector<std::unique_ptr<Expression>> p) : parts(std::move(p)) {}

    llvm::Value *codegen(CompilerContext &cc) const override
    {
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::LLVMContext &context = cc.getContext();
        llvm::Module &module = cc.getModule();

        // Get the snprintf function
        auto snprintfFunc = cc.getModule().getFunction("snprintf");

        std::string formatStr;
        std::vector<llvm::Value *> snprintfArgs;
        for (const auto &part : parts)
        {
            llvm::Value *partValue = part->codegen(cc);
            if (!partValue)
            {
                throw std::runtime_error("part null");
            }

            if (partValue->getType()->isPointerTy() &&
                partValue->getType()->getPointerElementType()->isIntegerTy(8))
            {
                formatStr += "%s";
            }
            else if (partValue->getType()->isIntegerTy())
            {
                if (partValue->getType()->getIntegerBitWidth() == 1)
                {

                    formatStr += "%s";

                    llvm::Constant *trueStr = llvm::ConstantDataArray::getString(context, "true");
                    llvm::Constant *falseStr = llvm::ConstantDataArray::getString(context, "false");

                    llvm::GlobalVariable *trueStrGlobal = new llvm::GlobalVariable(
                        cc.getModule(), trueStr->getType(), true,
                        llvm::GlobalValue::PrivateLinkage, trueStr, "trueStr");
                    llvm::GlobalVariable *falseStrGlobal = new llvm::GlobalVariable(
                        cc.getModule(), falseStr->getType(), true,
                        llvm::GlobalValue::PrivateLinkage, falseStr, "falseStr");

                    llvm::Value *trueStrPtr = builder.CreateBitCast(trueStrGlobal, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
                    llvm::Value *falseStrPtr = builder.CreateBitCast(falseStrGlobal, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));

                    llvm::Value *booleanToInt = builder.CreateZExt(partValue, llvm::Type::getInt32Ty(context));

                    llvm::Value *cond = builder.CreateICmpNE(booleanToInt, llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0));
                    partValue = builder.CreateSelect(cond, trueStrPtr, falseStrPtr);
                }
                else
                {

                    formatStr += "%d";
                    partValue = builder.CreateSExt(partValue, llvm::Type::getInt32Ty(context));
                }
            }
            else if (partValue->getType()->isFloatingPointTy())
            {
                formatStr += "%f";
                partValue = builder.CreateFPExt(partValue, llvm::Type::getDoubleTy(context));
            }
            else
            {
                formatStr += "%s";
            }

            snprintfArgs.push_back(partValue);
        }

        llvm::Value *formatStrVal = builder.CreateGlobalStringPtr(formatStr);

        std::vector<llvm::Value *> sizeCheckArgs = {
            llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(context)),
            builder.getInt64(0),
            formatStrVal};
        sizeCheckArgs.insert(sizeCheckArgs.end(), snprintfArgs.begin(), snprintfArgs.end());

        llvm::Value *sizeCheck = builder.CreateCall(snprintfFunc, sizeCheckArgs);

        llvm::Value *bufferSize = builder.CreateAdd(sizeCheck, builder.getInt32(1));
        llvm::Value *bufferSize64 = builder.CreateZExtOrBitCast(bufferSize, llvm::Type::getInt64Ty(context));

        llvm::Value *buffer = builder.CreateAlloca(
            llvm::Type::getInt8Ty(context),
            bufferSize,
            "fstring_buffer");

        std::vector<llvm::Value *> formatArgs = {buffer, bufferSize64, formatStrVal};
        formatArgs.insert(formatArgs.end(), snprintfArgs.begin(), snprintfArgs.end());
        builder.CreateCall(snprintfFunc, formatArgs);

        return buffer;
    }
};

#endif