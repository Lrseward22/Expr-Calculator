#include <calc/Utils/TokenKinds.h>
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

using namespace calc;

static const char * const TokNames[] = {
#define TOK(ID) #ID,
#include <calc/Utils/TokenKinds.def>
#undef TOK
    nullptr
};

const char *tok::getTokenName(TokenKind Kind) {
    return TokNames[Kind];
}

const char *tok::getPunctuatorSpelling(TokenKind Kind) {
    switch (Kind) {
#define PUNCTUATOR(ID, SP) case ID: return SP;
#include <calc/Utils/TokenKinds.def>
#undef PUNCTUATOR
        default: break;
    }
    return nullptr;
}

#undef TOK
#undef IDENTIFIER
#undef LITERAL
#undef PUNCTUATOR


#define TOK(ID)             TC_Other,
#define IDENTIFIER(ID)      TC_Identifier,
#define LITERAL(ID)         TC_Literal,
#define PUNCTUATOR(ID, SP)  TC_Punctuator,

const tok::TokenCategory tok::TokenCategories[] = {
    #include <calc/Utils/TokenKinds.def>
    #undef TOK
    #undef IDENTIFIER
    #undef LITERAL
    #undef PUNCTUATOR
    tok::TC_Other
};

bool tok::isIdentifier(TokenKind Kind) {
    return tok::TokenCategories[Kind] == TC_Identifier;
}

bool tok::isLiteral(TokenKind Kind) {
    return tok::TokenCategories[Kind] == TC_Literal;
}

bool tok::isPunctuator(TokenKind Kind) {
    return tok::TokenCategories[Kind] == TC_Punctuator;
}

std::string tok::formatTokenKind(TokenKind Kind) {
    std::string buffer;
    if (tok::isIdentifier(Kind))
        buffer = "identifier";
    else if (tok::isLiteral(Kind)) {
        buffer = std::string(tok::getTokenName(Kind));
        for (char& c : buffer) {
            if (c == '_') c = ' ';
            else c = std::tolower(c);
        }
    } else if (tok::isPunctuator(Kind))
        buffer = "\'" + std::string(tok::getPunctuatorSpelling(Kind)) + "\'";
    else
        buffer = "end of input or unformattable token";
    return buffer;
}
