#ifndef CALC_LEXER_LEXER_H
#define CALC_LEXER_LEXER_H

#include <calc/Utils/Token.h>
#include <calc/Utils/Diagnostics.h>
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/MemoryBuffer.h"
//#include <iostream>

class Lexer {
    const char *BufferPtr;
    const llvm::SourceMgr &SrcMgr;
    calc::DiagnosticsEngine &Diag;

public:
    Lexer(const llvm::SourceMgr &SrcMgr, calc::DiagnosticsEngine &Diag) :
    SrcMgr(SrcMgr), Diag(Diag) {
        const llvm::MemoryBuffer *Buffer = SrcMgr.getMemoryBuffer(SrcMgr.getMainFileID());
        const char *BufferStart = Buffer->getBufferStart();
        BufferPtr = BufferStart;
    }

    calc::DiagnosticsEngine &getDiagnostics() const {
        return Diag;
    }

    void next (Token &token);

    tok::TokenKind peek();

private:
    void formToken(Token &Result, const char *TokEnd, tok::TokenKind Kind);
    llvm::SMLoc getLoc() { return llvm::SMLoc::getFromPointer(BufferPtr); }
};

#endif

