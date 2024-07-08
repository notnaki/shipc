#ifndef IFSTMT_HPP
#define IFSTMT_HPP

#include "Statement.hpp"
#include "ast/expressions/Expression.hpp"
#include "BlockStmt.hpp"

class IfStmt : public Statement
{
private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStmt> body;
    std::unique_ptr<BlockStmt> elseBody;
    std::vector<std::pair<std::unique_ptr<Expression>, std::unique_ptr<BlockStmt>>> elifBodies;

public:
    IfStmt(std::unique_ptr<BlockStmt> then, std::unique_ptr<BlockStmt> elseb, std::vector<std::pair<std::unique_ptr<Expression>, std::unique_ptr<BlockStmt>>> elifs, std::unique_ptr<Expression> cond) : body(std::move(then)), elseBody(std::move(elseb)), elifBodies(std::move(elifs)), condition(std::move(cond)) {}

    void codegen(CompilerContext &cc) const override
    {
        llvm::LLVMContext &context = cc.getContext();
        llvm::IRBuilder<> &builder = cc.getBuilder();
        llvm::Function *function = builder.GetInsertBlock()->getParent();

        llvm::Value *condValue = condition->codegen(cc);

        // Create blocks for the then branch, else branch, and the continuation.
        llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(context, "then", function);
        llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(context, "else");
        llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(context, "ifcont");

        // If there are elif bodies, create corresponding blocks and branch conditions.
        std::vector<llvm::BasicBlock *> elifBBs;
        std::vector<llvm::BasicBlock *> elifCondBBs;
        for (size_t i = 0; i < elifBodies.size(); ++i)
        {
            elifBBs.push_back(llvm::BasicBlock::Create(context, "elif" + std::to_string(i), function));
            elifCondBBs.push_back(llvm::BasicBlock::Create(context, "elifcond" + std::to_string(i), function));
        }

        // Branch to the first elif condition if there are elif bodies.
        if (!elifBodies.empty())
        {
            builder.CreateCondBr(condValue, thenBB, elifCondBBs[0]);

            // Generate code for each elif condition and body.
            for (size_t i = 0; i < elifBodies.size(); ++i)
            {
                llvm::Value *elifCondValue = elifBodies[i].first->codegen(cc);

                builder.SetInsertPoint(elifCondBBs[i]);
                builder.CreateCondBr(elifCondValue, elifBBs[i], (i + 1 < elifBodies.size()) ? elifCondBBs[i + 1] : elseBB);

                builder.SetInsertPoint(elifBBs[i]);
                elifBodies[i].second->codegen(cc);
                builder.CreateBr(mergeBB);
            }
        }
        else
        {
            // If no elif bodies, branch to else or merge directly.
            builder.CreateCondBr(condValue, thenBB, elseBody ? elseBB : mergeBB);
        }

        // Generate the 'then' block.
        builder.SetInsertPoint(thenBB);
        body->codegen(cc);
        builder.CreateBr(mergeBB);

        // Generate the 'else' block if it exists.
        if (elseBody)
        {
            function->getBasicBlockList().push_back(elseBB);
            builder.SetInsertPoint(elseBB);
            elseBody->codegen(cc);
            builder.CreateBr(mergeBB);
        }

        // Set insert point to the merge block.
        function->getBasicBlockList().push_back(mergeBB);
        builder.SetInsertPoint(mergeBB);
    }
};

#endif
