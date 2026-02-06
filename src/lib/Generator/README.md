# Generator

The first thing to notice is that our generator only had one method, `CodeGen::Compile`. 
We need more machinery in our implementation [CodeGen.cpp](/src/lib/Generator/CodeGen.cpp) to actually discern and emit proper LLVM Intermediate Representation (IR). 
IR is a language more akin to a mix between assembly and C. The benefits of IR are expansive. 
The benefits that concern us are that IR is extremely easy to optimize and IR is less architecture specific. 
That is, the same IR can be used for x86 architectures or ARM architectures with some limitations. 
Compilers have a front-end, a middle, and a back-end. 
The IR is the middle stage and is typically where optimization occurs. 
LLVM handles optimization and the back-end (producing an actual assembly or machine code file). 

But how does the front-end we have been creating actually emit this IR? 
Remember the AST visitor class we defined in [AST.h](/src/include/calc/Parser/AST.h)? 
We can utilize this as the actual machine the code generator levies to generate the IR. 
As a side note, for larger compilers, Semantic Analysis is an additional step that also utilizes this visitor class. 
So, we define an extension of our `ASTVisitor` class that will employ another vital built-in from LLVM, the `llvm::IRBuilder` class.
This builder class provides many methods that require only the information associated with the instruction, and builds the instruction from there.
For example, to create an add instruction in LLVM IR, we would call `Builder.CreateAdd(left, right)`.
We do not need to know what the actual IR looks like, although I encourage you to compile some C files into IR using the llc command:

  ```llc <filename>```

With a better understanding of LLVM IR, what you must emit for each instruction will seem more intuitive, especially as you add more features into your language.

Compiling some files to LLVM IR, you may notice some oddities. 
Perhaps the most obvious is if you have a variable `x = 3` and later use the variable, say in `x++`, you will notice that the `x++` increments a temporary value named `x1` in the IR.
Then that value of `x1` is stored back into `x`.
And as you use `x` more often, you will see `x2`, `x3`, and so on. 
This is because IR follows what is known as Static Single Assignment (SSA).
This just means each variable can be used once.
SSA allows for easier optimizations at some points of the various available optimization passes LLVM provides.

Another things you may notice if you inspect more intricate coding structures like if statements of loops is the use of labels.
As you have seen what if statements and loops look like in assembly, this shouldn't be a huge surprise.
However, these are slightly different. LLVM IR adheres to the concept of Basic Blocks.
A basic block is a period of code that has exactly one entry point and exactly one exit point.
Again, this is useful for optimization.
An interesting consequence is the value of variable being dependent on the path of the code.
For example if you implement if structures as an expression:
```
  x = if y > 0 then y else -y;
```
This is solved through PHI instructions which determine a values variable based on the last Basic Block executed.
They aren't important for this language, so this is as deep as I will explain them.

Thankfully, expression languages only need one basic block, and LLVM handle SSA for us automatically.
This simplifies our IR generation immensely.

For the implementation of our IR emittor, we store some frequently used types, a symbol table to easily find variables, and the print/read functions that we call from C's standard library.
Then, we have a method we will call to start the main function of our program.
Using the visitor pattern, we can simply accept the current AST and any children expressions.
Then, we can use the values of each result and combine them with the associated operation through one of the many Builder templates provided to us.
After the current AST is fully accepted, we call our print function using `Builder.CreateCall(PrintF, {PrintStr, V})`
where PrintF is our stored declaration of the `printf` function, `PrintStr` is a stored pointer to the global string `"%d\n"`, and `V` is the value produced by the AST.
Finally, once all the ASTs are ran, we call finish main to create a return instruction.

In the actual `CodeGen::compile` method, we see the Generator actually getting the AST from the Parser and running the Visitor on the AST.
At the top of the function, though, you see some curious lines of code.
Here we create the module that stores all the IR instructions. The builder takes in this module and inserts any generated instruction into the module.
We then have to insert some metadata to the module. IR cannot be completely architecture independent.
The target triple is some information that identifies what architecture the IR will have to be turned into when emitting assembly or machine code.
Similarly, the data layout is some more metadata that determines how the instruction will be layed out.

View the implementation at [CodeGen.cpp](/src/lib/Generator/CodeGen.cpp)

View the main README [here](/README.md)
