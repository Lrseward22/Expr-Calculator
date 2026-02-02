# Generator
Our Generator interface, similar to the Lexer, is very simple. Most of the work is done in the implementation.

The Code Generator class stores the parser to obtain the ASTs as needed. It also stores a couple very important LLVM helper classes. First is the `llvm::LLVMContext`. This class hides a lot of work from the front end compiler developer. It ensures types are consistent, constants can be shared in the same storeage if they are identical, various metadata, and other diagnostic handlers. The other LLVM class we store is `llvm::Module`. This is likely the most important abstraction LLVM provides. Getting comfortable with `llvm::Module` will make code generation much easier. The `llvm::Module` owns all global variables, function declarations and definitions, metadata, the target architecture to generate the code for, data layout, and more.

As for methods of the Generator, we have a general compile method and a way to access the module.

View the Generator Implementation README [here](/src/lib/Generator/README.md)

Go back to the main README [here](/README.md)
