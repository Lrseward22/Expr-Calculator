#ifndef CALC_PARSER_PARSER_H
#define CALC_PARSER_PARSER_H

#include "AST.h"
#include <calc/Parser/AST.h>
#include <calc/Lexer/Lexer.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/SmallVector.h"
#include <iostream>

class Parser {
    Lexer &Lex;
    Token Tok;
    bool HasError;
    llvm::SmallVector<llvm::StringRef, 256> declaredIdentifiers;

    calc::DiagnosticsEngine &getDiagnostics() const {
        return Lex.getDiagnostics();
    }

    void panic() {
        expect(tok::SEMI);
        while (!match(tok::SEMI) && !atEnd())
            advance();
    }

    void advance() { Lex.next(Tok); }

    bool expect(tok::TokenKind Kind) {
        if (Tok.getKind() != Kind) {
            std::string received;
            std::string expected;
            received = tok::formatTokenKind(Tok.getKind());
            expected = tok::formatTokenKind(Kind);
            getDiagnostics().report(Tok.getLocation(), diag::err_unexpected_token, 
                    received, expected);
            return true;
        }
        return false;
    }

    bool match(tok::TokenKind Kind) {
        return (Kind == Tok.getKind());
    }

    bool atEnd() {
        return (Tok.getKind() == tok::TokenKind::EOI);
    }

    bool consume(tok::TokenKind Kind) {
        if (expect(Kind))
            return true;
        advance();
        return false;
    }

    tok::TokenKind peek() {
        return Lex.peek();
    }

    std::unique_ptr<AST> parsecalc();

    // EXPRs
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseAssign();
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseTermExpr();
    std::unique_ptr<Expr> parseUnary();
    std::unique_ptr<Expr> parseGrouping();
    std::unique_ptr<Expr> parseBaseExpr();
    
    // STMTs
    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Stmt> parseExprStmt();
    std::unique_ptr<Stmt> parseDeclare();
    std::unique_ptr<Stmt> parseRead();

    void unmatchedCharError(Token tok) {
        std::string ch = tok::formatTokenKind(tok.getKind());
        getDiagnostics().report(tok.getLocation(), diag::err_unmatched_char, ch);
    }

    void InvalidExprError() {
        getDiagnostics().report(Tok.getLocation(), diag::err_invalid_expr);
    }

    void UndeclaredVariableError(Token tok) {
        getDiagnostics().report(tok.getLocation(), diag::err_undeclared_var, tok.getIdentifier());
    }
    
    public:
    Parser(Lexer &Lex) : Lex(Lex), HasError(false) {
        Tok = Token();
        advance();
    }

    bool hasError() { return HasError; }

    std::unique_ptr<AST> parse();
};

#endif
