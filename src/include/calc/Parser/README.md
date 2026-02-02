# Parser

## AST definitions
We define Abstract Syntax Trees (AST) along with the parser. We define the visitor along with ASTs where we have an abstract method for each AST.

For a simple expression language, we must have an AST for each type of expression and for our three types of statements: expression statements, variable declarations and read statements.

Since ASTs are tree structures, they typically have pointers to their children. Here, we utilize unique pointers (`std::unique_ptr`) so the ASTs own their children and get automatically cleaned up when the AST goes out of scope. This precaution helps prevent memory leaks and unintentional memory errors. 

These ASTs are fairly standard with one exception. Our declare statement only holds an expression, an assign expression. This allows us to reuse parsing code but give declare statements a little more semantic meaning when it is the first time we see a variable.

Our read statement also only holds an identifier of the variable that we want to read into. 

Then, generally, our ASTs have methods of accessing children ASTs, visit methods, and a helpful print method for debugging.

## Parser interface
The Parser interface is more extensive than the Lexer's. The Parser has ownership of the Lexer so it can obtain more tokens as necessary. It also stores the current token and a `llvm::SmallVector` of declared identifiers.

Since the Lexer already has the diagnostics engine, the parser need only access the Lexer's diagnostic engine. 

Since our expressions terminate in semi colons, we provide a panic method to skip tokens until one is found in the hopes of resuming successful parsing. 

Then, it is nice to have some methods to handle the current token. We may skip over the present token with `advance`, ensure the next token is of a certain type with `expect` and error if they do not match, and a way to simply check if they match or not using `match`. We have a way to expect a certain token and skip over it called `consume`. Lastly, we also have a way to look at the type of the next token.

To actually parse, we have a parse method for each statement or expression defined in our grammar. We also create some error methods to make error reporting a little easier to read in the implementation.

View the parser implementation [here](/src/lib/Parser/Parser.cpp)
Go back to the main README [here](/README.md)
