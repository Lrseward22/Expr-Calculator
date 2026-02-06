# Lexer
The first step of the Lexer implementation is to create a way to obtain more useful information about character the buffer is currently pointing to. We do this in a namespace to prevent name collisions. We simply have checks that determine if the character is whitespace, a digit, a letter, or an alphanumeric character.

Then, at the bottom of the implementation, we create the helper methods. For peek, we create a token, call next on that token. Then reset the buffer pointer so we don't skip a token before the Parser is ready for it. Then finally return the token's type. We also include the interface for forming the token. Since the Lexer is a friend to the Token class, we may directly modify the private members of the Token class. 

Finally, we may construct the next method that actually constructs the token. First, we skip any whitespace, as whitespace is unnecessary in any good language. Then we check if we have hit the end of the file.
Now we check if the buffer currently points to an alphabetic character. If so, we know it is a key word or a variable. With only one keyword, we can directly check if it's read. Otherwise we know the token is an identifier. I leave it to the reader to implement a better way of checking if it is a keyword with more than just one.
If the buffer points to a digit initially, we know the token must be an integer literal. Again, it is left to the reader to determine how to check for float literals. 
If the buffer points to another character, we must check if it is any of our punctuators. Due to the inconsistency of punctuators being one or two characters, I determined it is easiest to manually check every punctuator and form the corresponding token.

View [Lexer.cpp](/src/lib/Lexer/Lexer.cpp)

View the main README [here](/README.md)
