# Parser

As you have had some parsing experience, the implementation of the parser should be straight forward. You simply use the helper functions defined in the header file to obtain new tokens and create the ASTs based on the rules of the grammar.

The main difference is to ensure that you are moving ownership of unique pointers at every stage

In just an expression language, semantic analysis is limited. Thus, it makes sense to perform it during the parsing stage. We simply hold a `llvm::StringMap` as our symbols table. We insert during variable declaration statements and ensure variables are declared in the Map when parsing a variable.

View the main README [here](/README.md)
