#include <calc/Generator/CodeGen.h>
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Host.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

namespace {
class IRVisitor : public ASTVisitor {
    Module* M;
    IRBuilder<> Builder;
    Type* VoidTy;
    Type* Int32Ty;
    PointerType *PtrTy;
    Constant* Int32Zero;
    Value* PrintStr;
    Value* ReadStr;
    Value* V;
    StringMap<AllocaInst*> nameMap;

    FunctionType* PrintFTy;
    FunctionCallee PrintF;
    FunctionType* ScanFTy;
    FunctionCallee ScanF;

public:
    IRVisitor(Module* M) : M(M), Builder(M->getContext()) {
        VoidTy = Type::getVoidTy(M->getContext());
        Int32Ty = Type::getInt32Ty(M->getContext());
        PtrTy = PointerType::getUnqual(M->getContext());
        Int32Zero = ConstantInt::get(Int32Ty, 0, true);

        PrintFTy = FunctionType::get(Builder.getInt32Ty(), Builder.getInt8PtrTy(), true);
        PrintF = M->getOrInsertFunction("printf", PrintFTy);
        ScanFTy = FunctionType::get(Builder.getInt32Ty(), Builder.getInt8PtrTy(), true);
        ScanF = M->getOrInsertFunction("scanf", ScanFTy);
    }

    void createMain() {
        FunctionType* MainFty = FunctionType::get(
                Int32Ty, {Int32Ty, PtrTy}, false);
        Function* MainFn = Function::Create(
                MainFty, GlobalValue::ExternalLinkage, "main", M);
        BasicBlock* BB = BasicBlock::Create(
                M->getContext(), "entry", MainFn);
        Builder.SetInsertPoint(BB);
        PrintStr = Builder.CreateGlobalStringPtr("%d\n", "pfmt", 0, M);
        ReadStr = Builder.CreateGlobalStringPtr("%d", "rfmt", 0, M);
    }

    void finishMain() {
        Builder.CreateRet(Int32Zero);
    }

    void run(std::unique_ptr<AST> Tree) {
        Tree->accept(*this);
        Builder.CreateCall(PrintF, {PrintStr, V});
    }

    // We need an accept method for each one in the abstract class
    // Expression ASTs
    virtual void visit(Expr &expr) override {
        //expr.print();
    };
    virtual void visit(BinaryOp &expr) override {
        //expr.print();
        expr.getLeft()->accept(*this);
        Value* left = V;
        expr.getRight()->accept(*this);
        Value* right = V;
        if (expr.getOp().is(tok::TokenKind::PLUS))
            V = Builder.CreateAdd(left, right);
        else if (expr.getOp().is(tok::TokenKind::MINUS))
            V = Builder.CreateSub(left, right);
        else if (expr.getOp().is(tok::TokenKind::STAR))
            V = Builder.CreateMul(left, right);
    };
    virtual void visit(UnaryOp &expr) override {
        //expr.print();
        expr.getExpr()->accept(*this);
        Value* e = V;
        V = Builder.CreateNSWNeg(e);
    };
    virtual void visit(Grouping &expr) override {
        //expr.print();
        expr.getExpr()->accept(*this);
    };
    virtual void visit(Literal &expr) override {
        //expr.print();
        int intval;
        expr.getData().getAsInteger(10, intval);
        V = ConstantInt::get(Int32Ty, intval, true);
    };
    virtual void visit(Variable &expr) override {
        //expr.print();
        AllocaInst* id = nameMap[expr.getData()];
        V = Builder.CreateLoad(Int32Ty, id, expr.getData());
    };
    virtual void visit(Assign &expr) override {
        //expr.print();
        auto id = expr.getIdentifier().getIdentifier();
        AllocaInst* alloca = nameMap[id];
        if (!alloca) {
            alloca = Builder.CreateAlloca(Int32Ty, nullptr, id);
            nameMap[id] = alloca;
        }
        expr.getExpr()->accept(*this);
        if (expr.getOp().is(tok::TokenKind::PLUSEQUAL)) {
            Value* cur = Builder.CreateLoad(Int32Ty, alloca);
            V = Builder.CreateNSWAdd(cur, V);
        } else if (expr.getOp().is(tok::TokenKind::MINUSEQUAL)) {
            Value* cur = Builder.CreateLoad(Int32Ty, alloca);
            V = Builder.CreateNSWSub(cur, V);
        }
        Builder.CreateStore(V, alloca);
    };

    // Statement ASTs
    virtual void visit(Stmt &stmt) override {
        //stmt.print();
    };
    virtual void visit(Declare &stmt) override {
        //stmt.print();
        stmt.getExpr()->accept(*this);
    };
    virtual void visit(ExprStmt &stmt) override {
        //stmt.print();
        stmt.getExpr()->accept(*this);
    };
    virtual void visit(Read &stmt) override {
        //stmt.print();
        auto id = stmt.getIdentifier().getIdentifier();
        AllocaInst* alloca = nameMap[id];
        if (!alloca) {
            alloca = Builder.CreateAlloca(Int32Ty, nullptr, id);
            nameMap[id] = alloca;
        }
        Builder.CreateCall(ScanF, {ReadStr, alloca});
        V = Builder.CreateLoad(Int32Ty, alloca, stmt.getIdentifier().getIdentifier());
    };
};
}

void CodeGen::compile(const char* Argv0, const char* F, llvm::TargetMachine* TM) {
    M = std::make_unique<Module>(F, Ctx);
    M->setTargetTriple(TM->getTargetTriple().str());
    M->setDataLayout(TM->createDataLayout());
    /* A linux executable generally follows PIE
     * I cant get it to work */
    //M->setPICLevel(llvm::PICLevel::Level::BigPIC);
    //M->setPIELevel(llvm::PIELevel::Level::Large);

    IRVisitor IRV(M.get());
    IRV.createMain();
    while (1) {
        std::unique_ptr<AST> Tree = std::move(parser.parse());
        if (!Tree) break;
        IRV.run(std::move(Tree));
    }
    IRV.finishMain();

    if (!TM) {
        llvm::errs() << "Could not create target machine\n";
        return;
    }

    M->print(outs(), nullptr);
}
