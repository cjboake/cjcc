- Compiler Parsing Steps -
# A general outline of the necessary operations for converting a stream of chars into tokens
# for the purpose of later parsing into your choice of syntax tree

------------------
Methodology
------------------
- Open file and return constant stream characters

- Have a function that checks for white spaces, and loops past space
- Similarly, it is important to recognize,  newlines, tabs, null bytes, and EOF

- Make decision on whether char is a letter, number, or symbol

- If a symbol, match within the symbol table, and return as token of it's own
    * check symbol to determine if it is an operator, as those are handled uniquely

- If a letter, either return as char, or concatenate into string/function name
    * Have check for functions

- If a number, return int 
    * to begin with, we will only use single char numbers

- Once you have the desire char/chars together, pass to a tokenization function
    * Once tokenized, it will be desirable to contain tokens in particular data structures
    * They could either be placed in an array at first, or directly into a syntax tree
    * Both of these approaches have pros/cons

- After creation, it is valuable to have functions that print the tokens and/or trees

