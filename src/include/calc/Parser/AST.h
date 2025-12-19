#ifndef CALC_AST_AST_H
#define CALC_AST_AST_H

#include <calc/Utils/Token.h>
#include "llvm/ADT/StringRef.h"
#include <memory>
#include <iostream>
#include <string>

class AST;

class Expr;
class BinaryOp;
class UnaryOp;
class Grouping;
class Literal;
class Variable;
class Assign;

class Stmt;
class Declare;
class ExprStmt;
class Read;

class ASTVisitor {
    public:
        virtual void visit(AST &) {};
        // Expression ASTs
        virtual void visit(Expr &) = 0;
        virtual void visit(BinaryOp &) = 0;
        virtual void visit(UnaryOp &) = 0;
        virtual void visit(Grouping &) = 0;
        virtual void visit(Literal &) = 0;
        virtual void visit(Variable &) = 0;
        virtual void visit(Assign &) = 0;

        // Statement ASTs
        virtual void visit(Stmt &) = 0;
        virtual void visit(Declare &) = 0;
        virtual void visit(ExprStmt &) = 0;
        virtual void visit(Read &) = 0;
};

class AST {
    public:
        virtual ~AST() {}
        virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST {
    public:
        Expr() {}
        virtual void print(int indent = 0) = 0;
};

// TermExpr
class BinaryOp : public Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    public:
        BinaryOp(std::unique_ptr<Expr> left, const Token& op, std::unique_ptr<Expr> right) 
            : left(std::move(left)), op(op), right(std::move(right)) {}
        Expr* getLeft() { return left.get(); }
        Expr* getRight() { return right.get(); }
        Token getOp() { return op; }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Binary Op: (" << op.getLexeme().str() << ")" << std::endl;
            if (left) left->print(indent + 2);
            if (right) right->print(indent + 2);
        }
};

class UnaryOp : public Expr {
    Token op;
    std::unique_ptr<Expr> expr;

    public:
        UnaryOp(const Token& op, std::unique_ptr<Expr> expr)
            : op(op), expr(std::move(expr)) {}
        Token getOp() { return op; }
        Expr* getExpr() { return expr.get(); }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Unary Op: (" << op.getLexeme().str() << ")" << std::endl;
            if (expr) expr->print(indent + 2);
        }
};

class Grouping : public Expr {
    std::unique_ptr<Expr> expr;

    public:
        Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
        Expr* getExpr() { return expr.get(); }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Grouping:" << std::endl;
            if (expr) expr->print(indent + 2);
        }
};

class Literal : public Expr {
    Token literal;

    public:
        Literal(const Token& tok) : literal(tok) {}
        llvm::StringRef getData() { return literal.getLiteralData(); }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Literal: (" << literal.getLiteralData().str() << ')' << std::endl;
        }
};

class Variable : public Expr {
    Token identifier;

    public:
        Variable(const Token& tok) : identifier(tok) {}
        llvm::StringRef getData() { return identifier.getIdentifier(); }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Identifier: (" << identifier.getIdentifier().str() << ')' << std::endl;
        }
};

class Assign : public Expr {
    Token identifier;
    Token op;
    std::unique_ptr<Expr> expr;

    public:
        Assign(Token identifier, const Token& op, std::unique_ptr<Expr> expr)
            : identifier(identifier), op(op), expr(std::move(expr)) {}
        Token getIdentifier() { return identifier; }
        Token getOp() { return op; }
        Expr* getExpr() { return expr.get(); }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Assign: (" << identifier.getIdentifier().str() << ")" << std::endl;
            std::cout << std::string(indent + 2, ' ') << "Op: (" << op.getLexeme().str() << ")" << std::endl;
            if (expr) expr->print(indent + 2);
        }
};

class Stmt : public AST {
    public:
        Stmt() {}
        virtual void print(int indent = 0) = 0;
};

class Declare : public Stmt {
    std::unique_ptr<Expr> expr;

    public:
        Declare(std::unique_ptr<Expr> expr)
            : expr(std::move(expr)) {}
        Expr* getExpr() { return expr.get(); }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Declare: " << "\n";
            if (expr) expr->print(indent + 2);
        }
};

class ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;

    public:
        ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
        Expr* getExpr() { return expr.get(); }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Expr: \n";
            if (expr) expr->print(indent + 2);
        }
};

class Read : public Stmt {
    Token identifier;

    public:
        Read(const Token& identifier)
            : identifier(identifier) {}
        Token getIdentifier() { return identifier; }
        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
        virtual void print(int indent = 0) override {
            std::cout << std::string(indent, ' ') << "Read: " << identifier.getLexeme().str() << std::endl;
        }
};

#endif
