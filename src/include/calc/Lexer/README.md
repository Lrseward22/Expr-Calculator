# Lexer Interface
Luckily, the lexer interface is very simple. The work comes in with the implementation.

Our Lexer need only contain a source manager (`llvm::SourceMgr`) to access the source files buffer, a buffer to the current position in the source buffer, and our diagnostics engine.

On top of that, we only need methods to form the next token and a way peek ahead to the next token.

It also becomes very helpful to provide methods to obtain the `llvm::SMLoc` of the current pointer and simple way to form tokens.

View the implementation of the Utils modules [here](/src/lib/Utils/README.md)

Go back to the main README [here](/README.md)
