- Compiler Parsing Steps -
# A general outline of the necessary operations for converting a stream of chars into tokens
# for the purpose of later parsing into your choice of syntax tree

Methodology
============

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


Parsing
=======
- The compiler can now read inputs and return values based on a delimiter of my choice

- This represents a fork in the road regarding possible approaches:
    1. Produce a (relatively) full-blown tokenizer that will allow me to handle
    entire function. This can then be parsed in to an AST, code gen, etc.
    2. Continue in the style of 8cc, immediately introduce AST+Gen, and render statements
    without function bodies.

- The first approac is a delayed result, where as the latter is a more immediate result, 
but without being what you might call a "real" compiler (i.e. not having Lexing/Parsing/Gen phases). 
Obviously, it will get there, but is at first more elementary.

- As I write this, I get the feeling that the second approach (8cc), would perhaps be preferable
because it is always easier to build upon a working MVC, than to try to pull a monolith together
at the very end.

- Another possible advantage posed with this approach is a more general front-to-back understanding
of what the compiler is doing. 

- I don't feel like writing a tokenizer today.

- Second approach it is.


What Does This Mean
===================
- My next step will consist of recognizing simple arithmetic expressions (e.g. 1+2, 4-2+2, etc.).
- This will require being able to recognize 1) Numbers 2) Operators 3) Variable names/assignment
- These will be assigned to the AST, and recursed over to generate machine code

- First, we will simply print the assembly to stdout, ensure it is reasonably correct, and then
write to a file and execute it from there.

- The tricky part will be recursing the AST and generating assembly. 

*End Note 5/27/18: 
------------------
- It is now reading multi-char numbers and returning ints, in an AST (woo!)
- The next step will be adding AST for operators
- Then fix the control flow to organize the whole thing into a proper tree

- Optionally, I could then jump on some code gen, to actually start to see something happen.


5/28/18 - Refactoring and adding operators
------------------------------------------
- Today I am going to go through and refactor the original parsing scheme into something that
simply returns AST nodes
- The current scheme is closer to something that would be used in an actual tokenizing lexer, but 
currently I feel that it will be easier to get to the syntax tree, and later tokenize as necessary.
- My reasoning for this is that visualizing the AST is the hard part here, and if I can do that,
intermediate steps should be be easier to to accomplish
- The current strategy is more like the typical switch statement parser, and although I think that 
will ultimately come into play, it's not the best way to think about the design, from a recursive
standpoint

*End Note:
---------
- Just need to get the white space skipping down. If I can do that, it will be able to process simple
equations into an AST, as well as strings.
