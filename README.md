# Expr-Calculator
A basic expression-based compiler built in LLVM

## Overview
This expression language is built as an introduction to LLVM. It includes integers as the only valid data type. Consequently, only addition, subtraction, and multiplication are included operations. This language features variables and the ability to read integers into variables. The result of every expression prints the calculated value.

## Installing the requirements


## Usage
	Usage -> building calculator file and running what it outputs

## Environment
	About Environment

## Language Examples
	Examples -> some statements and what they would output

## Project Layout
### Utils
To create a programmer and user friendly compiler multiple modules are created for useful abstractions.

We have a diagnostics engine to efficiently report errors and other messages to the user. Additionally, we define the types of Tokens available in our language as well as the Token class.

For more information:

[click here for the Utils interface](src/include/calc/Utils/README.md)

[click here for the Utils implementation](src/lib/Utils/README.md)

### Lexer
The lexer takes in a source manager and creates tokens from the provided source. 

For more information:

[click here for the Lexer interface](src/include/calc/Lexer/README.md)

[click here for the Lexer implementation](src/lib/Lexer/README.md)

### Parser
The parser takes in the tokens generated from the lexer and organizes them into abstract syntax trees. Then it conducts basic semantic analysis.

For more information:

[click here for the Parser interface](src/include/calc/Parser/README.md)

[click here for the Parser implementation](src/lib/Parser/README.md)

### Generator
In this expression language, the generator emits LLVM Intermediate Representation (IR).

For more information:

[click here for the Generator interface](src/include/calc/Generator/README.md)

[click here for the Generator implementation](src/lib/Generator/README.md)

### Driver
Finally, the driver stitches everything together. The driver handles command line arguments of our compiler, generates the IR, and culminates in creating the desired compiled output.

For more information:

[click here for the Driver](src/README.md)
