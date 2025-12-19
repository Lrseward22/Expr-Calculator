#include <calc/Lexer/Lexer.h>
#include <iostream>

namespace charinfo {
    LLVM_READNONE inline bool isHorizontalWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\f' || c == '\v';
    }

    LLVM_READNONE inline bool isVerticalWhitespace(char c) {
        return c == '\r' || c == '\n';
    }

    LLVM_READNONE inline bool isWhitespace(char c) {
        return isHorizontalWhitespace(c) || isVerticalWhitespace(c);
    }

    LLVM_READNONE inline bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    LLVM_READNONE inline bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    LLVM_READNONE inline bool isAlphaNumeric(char c) {
        return isDigit(c) || isLetter(c);
    }
}

void Lexer::next(Token &token) {
    while (*BufferPtr && charinfo::isWhitespace(*BufferPtr)) {
        BufferPtr++;
    }
    if (!*BufferPtr) {
        token.Kind = tok::EOI;
        return;
    }
    if (charinfo::isLetter(*BufferPtr)) {
        const char *end = BufferPtr + 1;
        while (charinfo::isAlphaNumeric(*end) || *end == '_')
            end++;
        llvm::StringRef Name(BufferPtr, end - BufferPtr);
        if (Name == "read")
            formToken(token, end, tok::kw_read);
        else
            formToken(token, end, tok::IDENTIFIER);
        return;
    } else if (charinfo::isDigit(*BufferPtr)) {
        const char *end = BufferPtr + 1;
        bool hasRadix = false;
        while (charinfo::isDigit(*end)) {
            end++;
        }
        formToken(token, end, tok::INTEGER_LITERAL);
        return;
    } else {
        switch (*BufferPtr) {
            case '+': 
                if (*(BufferPtr + 1) == '=')
                    formToken(token, BufferPtr + 2, tok::PLUSEQUAL);
                else
                    formToken(token, BufferPtr + 1, tok::PLUS);
                break;
            case '-': 
                if (*(BufferPtr + 1) == '=')
                    formToken(token, BufferPtr + 2, tok::MINUSEQUAL);
                else
                    formToken(token, BufferPtr + 1, tok::MINUS);
                break;
            case '*': 
                formToken(token, BufferPtr + 1, tok::STAR);
                break;
            case ';': 
                formToken(token, BufferPtr + 1, tok::SEMI);
                break;
            case '=': 
                formToken(token, BufferPtr + 1, tok::EQUAL);
                break;
            case '(':
                formToken(token, BufferPtr + 1, tok::L_PAREN);
                break;
            case ')':
                formToken(token, BufferPtr + 1, tok::R_PAREN);
                break;
            default:
                Diag.report(getLoc(), diag::err_illegal_char);
                formToken(token, BufferPtr + 1, tok::UNKNOWN);
        }
        return;
    }
}

tok::TokenKind Lexer::peek() {
    const char *buffPtr = BufferPtr;
    Token tok{};
    next(tok);
    BufferPtr = buffPtr;
    return tok.getKind();
}

void Lexer::formToken(Token &Tok, const char *TokEnd, tok::TokenKind Kind) {
    Tok.Kind = Kind;
    Tok.Ptr = BufferPtr;
    Tok.Length = TokEnd - BufferPtr;
    BufferPtr = TokEnd;
}
