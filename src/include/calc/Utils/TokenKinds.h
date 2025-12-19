#ifndef CALC_UTILS_TOKENKINDS_H
#define CALC_UTILS_TOKENKINDS_H

#include <string>

namespace calc {

    namespace tok {
        enum TokenKind : unsigned short {
            #define TOK(ID) ID,
            #include "TokenKinds.def"
            NUM_TOKENS
        };

        const char *getTokenName(TokenKind Kind);
        const char *getPunctuatorSpelling(TokenKind Kind);

        enum TokenCategory : unsigned char {
            TC_Identifier,
            TC_Literal,
            TC_Punctuator,
            TC_Other
        };

        extern const TokenCategory TokenCategories[];

        bool isIdentifier(TokenKind Kind);
        bool isLiteral(TokenKind Kind);
        bool isPunctuator(TokenKind Kind);

        std::string formatTokenKind(TokenKind Kind);
    }
}

#endif
