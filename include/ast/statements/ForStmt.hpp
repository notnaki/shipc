#ifndef FORSTMT_HPP
#define FORSTMT_HPP

#include "Statement.hpp"
#include "ast/expressions/Expression.hpp"
#include "BlockStmt.hpp"
#include <memory>

class ForStmt : public Statement
{
private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStmt> body;
    std::unique_ptr<Expression> each;
    std::unique_ptr<Statement> var;

public:
    ForStmt(std::unique_ptr<Statement> varDecl, std::unique_ptr<Expression> cond, std::unique_ptr<Expression> after, std::unique_ptr<BlockStmt> b)
        : var(std::move(varDecl)), condition(std::move(cond)), each(std::move(after)), body(std::move(b)) {}

    void codegen(CompilerContext &cc) const override
    {
        llvm::LLVMContext &context = cc.getContext();
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::Function *function = builder.GetInsertBlock()->getParent();

        // Create basic blocks for the loop structure
        llvm::BasicBlock *initBB = builder.GetInsertBlock();
        llvm::BasicBlock *condBB = llvm::BasicBlock::Create(context, "forcond", function);
        llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(context, "forbody");
        llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context, "forafter");
        llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(context, "forcont");

        // Emit the initialization code
        if (var)
        {
            var->codegen(cc);
        }

        // Insert an explicit fall through from the current block to the condition block
        builder.CreateBr(condBB);

        // Start insertion in condBB
        builder.SetInsertPoint(condBB);
        llvm::Value *condValue = condition->codegen(cc);
        if (!condValue)
        {
            throw std::runtime_error("Condition code generation failed");
        }

        // Create the conditional branch
        builder.CreateCondBr(condValue, bodyBB, mergeBB);

        // Emit the body of the loop
        function->getBasicBlockList().push_back(bodyBB);
        builder.SetInsertPoint(bodyBB);
        body->codegen(cc);
        builder.CreateBr(afterBB);

        // Emit the after step
        function->getBasicBlockList().push_back(afterBB);
        builder.SetInsertPoint(afterBB);
        if (each)
        {
            each->codegen(cc);
        }
        builder.CreateBr(condBB);

        if (auto varDeclStmt = dynamic_cast<VarDeclStmt *>(var.get()))
        {
            cc.getTable().removeVariable(varDeclStmt->getName());
        }

        // Continue insertion in mergeBB
        function->getBasicBlockList().push_back(mergeBB);
        builder.SetInsertPoint(mergeBB);
    }
};

#endif
