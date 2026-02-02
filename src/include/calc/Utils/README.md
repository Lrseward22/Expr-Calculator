# Utils Modules

## Diagnostics

### Diagnostics.def
In This file, we simply define a macro data table so we can easily add new diagnostics messages and their associated level. With this, it becomes as simple as adding a new entry in this file with a name of the message, the level as error, warning, or note to get LLVM, and the message you want to report.

### Diagnostics.h
Here we can utilize the diagnostics data table to create an enum of all our messages to report. We use namespacing, in this case `calc`, to associate the diagnostics engine with all our modules in this project.

LLVM provides a diagnostics system to report messages, so we create a custom interface to manipulate the provided system to our needs. Our diagnostics engine simply provides a way to format our error messages, count the number of errors in a program, and actually report the errors and their corresponding location in the source file.

To acheive this result, LLVM also provides a couple more built ins. First is a Source Manager class that allows interfacing with the source code to become more streamlined and translate locations in the form of SMLoc to the actual pointer within the source file. 

This is the first file we also see the `llvm::SmallVector`. LLVM provides lighter weight re-writes of many standard library objects that are often far too bloated for the uses in compilers. So, when working with LLVM make sure you are using the `llvm` alternative to standard library classes and data structures.

## Tokens

### TokenKinds.def
Similar to Diagnostics.def, this file is a data table for macros. In this case, it defines all the tokens available in the language. We will later see how adding new keywords to the lexing process is simply just adding a new entry in the key words section.

This macro definition file creates the types of tokens and organizes them into general tokens, identifier tokens, literal tokens, punctuator tokens, and key word tokens. This separation can later become useful when determining what properties the token should have.

### TokenKinds.h
Here we create the enum to interface with our token definitions. We also provide functions to determine string of characters associated with a token, the name of the token, the token category, and, as mentioned earlier, ways to identify the category of the token.

### Token.h
Here we define what our tokens look like. To hold the lexeme, we only store a pointer to the source buffer and the length of the lexeme. We then store the token kind.

Likely unseen, we define Token as a friend class to the Lexer class. This simply allows the lexer to access private methods and member variables of the Token class. This is helpful due to how intrinsically linked the two classes are.

As mentioned earlier, LLVM provides a way to obtain the line and column number in a source file based on a pointer. We use this functionality from `llvm::SMLoc::getFromPointer` to get the exact location of a token in the source file.

We also use LLVMs replacement of `std::string` known as `llvm::StringRef`. These are simply just non-owning references to strings. The benefit of using StringRefs are that they are extremely cheap to pass around as they are basically just a pointer. We utilize StringRefs in the Token class to obtain the lexeme of tokens. 

Lastly, Tokens contain methods to determine if a given token is of a certain type. This becomes useful when organizing the tokens into abstract syntax trees.
