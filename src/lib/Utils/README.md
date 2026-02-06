# Utils Implementations
## Diagnostics Engine
The only parts of the implementation for [Diagnostics.cpp](/src/lib/Utils/Diagnostics.cpp) that remains is the way to access the text and kind associated with an diagnostic name.

To acheive this functionality, we define a macro for each that create an array with the Diagnostics enum ID as the index. Then, we only need to index the array at the enum ID for the diagnostic.

## Token

Similar to the Diagnostics Engine implemntation, there is little more work to do for [TokenKinds.cpp](/src/lib/Utils/TokenKinds.cpp). We need to define macros to get the name and associated lexeme for the token kinds with fixed lexemes. We then provide the simple methods to determine the category of a token's type. 

Lastly, we provide a method to format the token for debug info and diagnostic messaging based on the category of token.

View the main README [here](/README.md)
