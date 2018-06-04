 Compiler Parsing Steps -
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

5/29/18
-------
- The AST forms complete dual-node routes with an operator. I'm going back and forth on what the next
step to take should be
- Theoretically, it needs to recurse more complicated statements than this, but I think in the spirit
of spitting out simple programs, this will do for now
- I'm going to take a look at code generation now, and see what I need to do to add in semi-colons
*End Note:
----------
- Well, the idea is there for it to spit out some assembly to do some +/- operations. The hard part
is just getting the recursive traversal of the AST down, but that will just taking playing with it a
little. I'm really happy with getting even this far though, because I figured out finally how to actually
reason through the assembly code, and that means I can actually know what to do in the compiler.

5/31/18
-------
- I didn't get to do any work on the compiler yesterday as I was in orlando to see Eric for the "Agile Training Lunch",
but man was it awesome to see him. I then had dinner with Cody, which was also great. We said we love each other for the first time. I was so happy.
- Today the goal is to get the recursive assembly generation working correctly so that I can actually output the assembly for simple arithmetic.
- If I can get that going, I will probably start extending the lexer to capably handle keywords, functions, otherwise move forward towards more complete text parsing

update: Well that wasn't too hard: It now produces assembly for basic (+/-) arithmetic operations. Feels good!
-------
- Took just over two weeks to get to this point. Not bad.
- As far as next steps, I'm considering following the Gholoum paper and perhaps really 
rounding out my handling of integers. It follows steps that are "one component at a time" and it may
bet a good way to get all elements of the compiler introduced in steps, all while having them
function completely on their own
- *Note: identifying functions vs. keywords has been a concern of mine for a while now; answer, simply treat them the same and if you hit a '(' they are a function. Duh.
- Holy shit, the recursive nature of this machine is really starting to hit me, which makes you realize the power that lies within these functions

*End Note:
----------
- I went ahead and implemented functions into the Ast, since it's actually pretty easy (and I'm tired)
- Need to make the string function more general again, since it's used for functions and identifiers
- I stopped because this will require a fundamental change to my recursion strategy, so that A)The function 
is in the top node, and B) It actually recurses to the end, making any other functions children of main
- I'm SUPER happy with how things are going so far though and actually finally feel like I can do this, for sure.
- Even the assembly generation and whatnot doesn't scare me so much-- I'm beginning to get it. And once I get functions and a few details about them down, I'll be able to compile some real (little) programs!
- Gonna need to write that assembly to a file soon, and actually get the joy of executing it! *sunglasses*


6/1/18
------
- Today's goal is to make sure that the AST is handling functions correctly, and to get started on the assembly
generation for them. 
- My previous thought about needing to chagne the recursion strategy might not be correct, it turns out, and I may indeed be able to have the function call (name) as a L_Node. We'll see today.
- I also need to make the func_or_ident() more generic, since it's going to be used for multiple things now

- I was wrong. Fundamental changes to the recursion structure required.

6/2/18
------
- So, it turns out that getting the AST to recurse properly was a bigger issue than I first expected it to be.
- Keeping the _Function_ as the head node is a challenge, largely because of the fact that my skip_white() is not doing
precisely what I need it to do. 
- After returning the token for the function name, the file pointer is still at the same point in the file. I need it to return a char* with the fp ~after~ the token. Without this, you never scan the file, you just read one word over and over.
- So today will be fixing that, which is suspect will make producing the AST significantly easier. 
- I'm trying to study the 8cc file reading strategy, but it just isn't clear to me what he's doing there. Perhaps there is some sort of intermediary buffer being used?

6/3/18
------
- Okay, the whole AST thing really had me frustrated yesterday-- largely, I couldn't tell if it was recursing over all of the statements I wanted to include in it
- On top of that I sort of went back and forth on the function-being-on-top strategy, which was bad. It needs to be on top. 
- So today I'm boing back over this and making sure that I make it happen, because it is clearly correct

- Well I almost lost my mind over this today, and it felt like I didn't make any progress, but I think a few good things came out of it:
    - I improved the skip_space() so that it doesn't need a return value. This is as it should be.
    - I realized that after every number, some sort of operator will follow. This in mind, it becomes much easier       to    handle them, because you can assume a symbol will be next.

6/4/18
------
- Well after a fucking nightmare-and-a-half, I fixed the aforementioned issue with the ast. It came down to a simple matter of how the AST node was layed out... maddening, but I wouldn't have learned how structs work with such detail otherwise.
- Now, it _actually_ builds recursive nodes, in no small part due to how closely I've examined the algorithm in my fucking combing through the code to fix this
