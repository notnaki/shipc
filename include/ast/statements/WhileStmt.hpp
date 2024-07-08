#ifndef WHILESTMT_HPP
#define WHILESTMT_HPP

#include "Statement.hpp"
#include "ast/expressions/Expression.hpp"
#include "BlockStmt.hpp"

class WhileStmt : public Statement
{
private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStmt> body;

public:
    WhileStmt(std::unique_ptr<BlockStmt> b, std::unique_ptr<Expression> cond) : body(std::move(b)), condition(std::move(cond)) {}

    void codegen(CompilerContext &cc) const override
    {
        llvm::LLVMContext &context = cc.getContext();
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::Function *function = builder.GetInsertBlock()->getParent();

        llvm::BasicBlock *condBB = llvm::BasicBlock::Create(context, "whilecond", function);
        llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(context, "whilebody");
        llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(context, "whilecont");

        builder.CreateBr(condBB);

        builder.SetInsertPoint(condBB);
        llvm::Value *condValue = condition->codegen(cc);
        builder.CreateCondBr(condValue, bodyBB, mergeBB);

        function->getBasicBlockList().push_back(bodyBB);
        builder.SetInsertPoint(bodyBB);
        body->codegen(cc);
        builder.CreateBr(condBB);

        function->getBasicBlockList().push_back(mergeBB);
        builder.SetInsertPoint(mergeBB);
    }
};

#endif
