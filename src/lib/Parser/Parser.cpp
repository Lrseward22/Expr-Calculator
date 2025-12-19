#include <calc/Parser/Parser.h>
#include <iostream>
#include <calc/Utils/TokenKinds.h>
#include "llvm/ADT/STLExtras.h"

using namespace calc;

std::unique_ptr<AST> Parser::parse() {
    if (!atEnd())
        return std::move(parseStmt());
    return nullptr;
}

// EXPRESSIONS

std::unique_ptr<Expr> Parser::parseExpression() {
    tok::TokenKind lookAhead = peek();
    std::unique_ptr<Expr> expr;
    if (lookAhead == tok::TokenKind::EQUAL)
        expr = parseAssign();
    else
        expr = parseExpr();
    return expr;
}

std::unique_ptr<Expr> Parser::parseAssign() {
    Token identifier = Tok;
    advance();
    Token op = Tok;
    advance();
    std::unique_ptr<Expr> expr = parseExpr();
    return std::make_unique<Assign>(identifier, op, std::move(expr));
}


std::unique_ptr<Expr> Parser::parseExpr() {
    std::unique_ptr<Expr> left = parseTermExpr();

    while (Tok.isOneOf(tok::TokenKind::PLUS, tok::TokenKind::MINUS)) {
        Token tok = Tok;
        advance();
        std::unique_ptr<Expr> right = parseExpr();
        left = std::make_unique<BinaryOp>(std::move(left), tok, std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseTermExpr() {
    std::unique_ptr<Expr> left = parseUnary();

    while (Tok.is(tok::TokenKind::STAR)) {
        Token tok = Tok;
        advance();
        std::unique_ptr<Expr> right = parseTermExpr();
        left = std::make_unique<BinaryOp>(std::move(left), tok, std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseUnary() {
    if (Tok.is(tok::TokenKind::MINUS)) {
        Token op = Tok;
        advance();
        std::unique_ptr<Expr> expr = parseGrouping();
        return std::make_unique<UnaryOp>(op, std::move(expr));
    } else {
        std::unique_ptr<Expr> expr = parseGrouping();
        return expr;
    }
}

std::unique_ptr<Expr> Parser::parseGrouping() {
    std::unique_ptr<Expr> expr;

    if (match(tok::TokenKind::L_PAREN)) {
        advance();
        if (match(tok::TokenKind::R_PAREN))
            InvalidExprError();
        else
            expr = parseExpr();
        consume(tok::TokenKind::R_PAREN);
    } else {
        expr = parseBaseExpr();
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parseBaseExpr() {
    Token tok = Tok;
    advance();
    if (tok.getKind() == tok::TokenKind::IDENTIFIER) {
        if (llvm::find(declaredIdentifiers, tok.getIdentifier()) == declaredIdentifiers.end()) {
            UndeclaredVariableError(tok);
            return nullptr;
        }
        return std::make_unique<Variable>(tok);
    } else if (tok::isLiteral(tok.getKind()))
        return std::make_unique<Literal>(tok);
    InvalidExprError();
    return nullptr;
}

// STATEMENTS

std::unique_ptr<Stmt> Parser::parseStmt() {
    if (Tok.is(tok::IDENTIFIER) && (peek() == tok::TokenKind::EQUAL
                                ||  peek() == tok::TokenKind::PLUSEQUAL
                                ||  peek() == tok::TokenKind::MINUSEQUAL))
        return parseDeclare();
    if (match(tok::TokenKind::kw_read))
        return parseRead();
    return parseExprStmt();
}

std::unique_ptr<Stmt> Parser::parseDeclare() {
    if (llvm::find(declaredIdentifiers, Tok.getIdentifier()) == declaredIdentifiers.end())
        declaredIdentifiers.push_back(Tok.getIdentifier());
    std::unique_ptr<Expr> expr = parseAssign();
    panic();
    consume(tok::TokenKind::SEMI);
    return std::make_unique<Declare>(std::move(expr));
}

std::unique_ptr<Stmt> Parser::parseExprStmt() {
    std::unique_ptr<Stmt> stmt = std::make_unique<ExprStmt>(std::move(parseExpression()));
    panic();
    consume(tok::TokenKind::SEMI);
    return stmt;
}

std::unique_ptr<Stmt> Parser::parseRead() { 
    consume(tok::TokenKind::kw_read);
    expect(tok::TokenKind::IDENTIFIER);
    Token identifier = Tok;
    advance();
    panic();
    consume(tok::TokenKind::SEMI);
    return std::make_unique<Read>(identifier);
}
