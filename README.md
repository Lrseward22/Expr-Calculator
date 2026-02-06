# Expr-Calculator
A basic expression-based compiler built in LLVM

## Overview
This expression language is built as an introduction to LLVM. It includes integers as the only valid data type. 
Consequently, only addition, subtraction, and multiplication are included operations. 
This language features variables and the ability to read integers into variables. 
The result of every expression prints the calculated value.

## Installing the requirements
### Installing the dependencies of LLVM
Most Linux distrobution come with a version of LLVM pre-installed. 
However, the installed version is often insufficient for developing with LLVM.
We need git to install LLVM, a C/C++ compiler, cmake and ninja to help with compilation.
Lastly, we need zlib, a library that LLVM depends on.

Here are the installations for some popular Operating Systems. 
Note that I use Ubuntu, so note their might be differences in OS and coding performance.
#### Ubuntu
```
$ sudo apt -y install gcc g++ git cmake ninja-build zlib1g-dec
```
#### Windows
Windows does not come with a C/C++ package manager.
I recommend installing Visual Studio Community 2022 [here](https://visualstudio.microsoft.com/vs/community/).
Although 2026 recently came out so you could use that as well.
Make sure you install the Desktop Development with C++ option.

Next, install the Scoop package manager [here](https://scoop.sh/) so we can install the remaining dependencies.
With this installed, go to the x64 Native Tools Command Prompt for VS 2022 or substitute.

Run these commands. Note that you continue reading this section before running these commands.
```
$ scoop install git cmake ninja python gzip bzip2 coreutils
$ scoop bucket add extras
$ scoop install zlib
```

Make sure to watch the scoop output carefully.
It may ask to add registry keys. This is important.
Copy the registry paths. They look like:
```
$ %HOMEPATH%\scoop\apps\python\current\install-pep-514.reg
$ %HOMEPATH%\scoop\apps\zlib\current\register.reg
```

After each command, a pop-up will ask if you really want to import those keys.
You need to click YES to finish the imports.

For Windows, you will need to use the x64 Native Tools Command Prompt for VS 2022 or substitute.
This way, the compilers are automatically added to seach path.
#### Fedora and RedHat
```
$ sudo dnf -y install gcc gcc-c++ git cmake ninja-build zlib-devel
```
#### FreeBSD
```
$ sudo pkg install -y git cmake ninja zlib-ng
```
#### OS X 
```
$ brew install git cmake ninja zlib
```

### Configuring Git (if you haven't)
Check if you have configured your email and username by:
```
$ git config user.email
$ git config user.name
```

If you need to configure either of these:
```
$ git config --global user.email "johndoe@gmail.com"
$ git config --global user.name "johndoe"
```

### Cloning LLVM
This will take a while.
#### For all non-Windows platforms
```
$ git clone https://github.com/llvm/llvm-project.git
```
#### For Windows
```
$ git clone --config core.autocrlf=false https://github.com/llvm/llvm-project.git
```

Now that you have the repository, enter its directory and switch to the version I use:
```
$ cd llvm-project
$ git checkout -b llvm-17 llvmorg-17.0.1
```

### Generating the Build System
Make a directory in the llvm-project directory.

Before continuing, LLVM defines many CMake variables for specific installation requirement.
I used the following default, but if your requirement are different, you may want to check the complete list of LLVM CMake documentation [here](https://releases.llvm.org/17.0.1/docs/CMake.html#llvm-specific-variables).

To change CMake native variables, look at the complete list by running:
```
$ cmake --help-variable-list
```
Note that this is very long and you may want to pipe the output into another program so it is easier to view.

Now we run:
```
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release =DLLVM_ENABLE_PROJECTS=clang -B build -S llvm
```

To compile LLVM and clange:
```
$ cmake --build build
```

Recommended, you can test your installation with a full test:
```
$ cmake --build build --target check-all
```
Or a quick manual test:
```
$ build/bin/llc --version
```

Finally, install the binaries;
```
$ cmake --install build
```



## Usage
### Installing this repository
With LLVM installed, you now need to install this repository. Navigate to the directory you would like to have this repository under. Then:
#### For all non-Windows platforms
```
$ git clone https://github.com/Lrseward22/Expr-Calculator.git
```
#### For Windows
```
$ git clone --config core.autocrlf=false https://github.com/Lrseward22/Expr-Calculator.git
```

### Building the compiler
Now that we have all dependencies and my code, building this project is easy.
I define a [CMakeLists.txt](CMakeLists.txt) file for all CMake instructions to automate the build process.

This file adds my driver and source files to be compiled.
Then we add the list of LLVM libraries we need to link to.

So, create a build directory and navigate to it:
```
$ mkdir build
$ cd build
```

Then, initialize ninja by running:
```
$ cmake -G Ninja ../
```

Now, to compile or recompile after changes, it is simply:
```
$ ninja
```

Similar to make, CMake and Ninja make compiling much faster by only updating compiled code if it has been changed.

### Running the Compiler
After running `ninja`, we should have an executable for our compiler.
Now we need something to run it on.
Create a file `test.calc`:
```
3+4;
2*6;
```

Now, to compile our file:
```
./calc test.calc
```

And to run the compiled file:
```
./test
```

## Environment
Do I need this section? I think it is handled in the previous two sections.

## Language Examples
The result of every statement in our expression language is printing out the result.
With only integers, it is natural only to include addition, subtraction, and multiplication.

### Addition
```
3 + 5;
```
Prints the result of 8 whenever the executable is run.

```
3 + 5;
4 + 1 + 2;
```
Prints:
```
8
7
```

### Subtraction
As you might expect:
```
18 - 7;
```
Prints:
```
11
```

### Multiplication
```
3 * 12;
```
Prints:
```
36
```

### More complex expression
We also preserve order of operations:
```
3 + 4 * 5;
```
Prints:
```
23
```
And
```
3 + 4 * 5;
(3+4) * 5;
```
Prints:
```
23
35
```

### Variable
We can also declare and use variables
```
x = 5;
y = x + 1;
x + y;
```
Output:
```
5
6
11
```

### User input
Finally, we can also read values from the user to have more dynamic programs.
```
x = 17;
read y;
x + y;
```
Say you type the value 5 when prompted (it just stops, not actually prompts you). Output:
```
17
5
5
22
```

Notice 5 is duplicated. This is because you type 5 and hit enter to flush the input to the program.
Then, the compiler finishes the statement which then prints the statement's value which is 5.

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
