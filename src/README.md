# Driver
Our driver shows how much heavy lifting the LLVM library does. There are so many libraries to include that are extremely helpful with the user side, middle, and back-ends of the compiler. 

First, we define many command line options the user may invoke. 
We `#include "llvm/Support/CommandLine.h"` to help us easily implement these command line options.
As you can see, it is as easy as defining a `static llvm::cl::opt<type>` with the name of the cl argument, the flag to change while running, a description, and optionally a default.

Next we define a helper function to create a target machine.
Remember that a target machine consists of three pieces of metadata that defines the architecture, operating system, and distrobution.
This metadata help LLVM determine how to handle its IR after you have fully created the module.
In our case, we take a default value of whatever the user is currently running and allow a command line argument to change the target triple.

Then, we have function that emits the generated file. Again, for this we have command line arguments to change what type of file is emitted. 
By default we choose `.o` but the user can choose to emit IR or assembly.
The change from IR to any of these other file types is handled by the LLVM Pass Manager.

Our last helper function is to link the executable.
By default, our compiler will attempt to link the object file to the machine code executable using gcc.

Finally, we have our main function.
We parse our command line options using `llvm::cl::ParserCommandLineOptions`.
Then, using our `InputFiles` command line option, we iterate through each provided input file and compile each one.
We must make sure that the given file is of the proper type to be compiled by our compiler.
Then, we instantiate the source manager and all of our objects to compile.
We create our target triple and compile the file into the module.

Next, we ensure the LLVM IR has no errors and check which file type to emit.
If the user specified a file type, we simply emit that file type.
If unspecified, we create an object file and link this object file to an executable.
Finally, we delete any temporary data such as the object file.

Congratulation! We have created a working expression language compiler!

View [driver.cpp](/src/driver.cpp)

View the main README [here](/README.md)
