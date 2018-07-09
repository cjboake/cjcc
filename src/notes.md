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

- The first approach is a delayed result, where as the latter is a more immediate result, 
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

- Damn, it feels good to not have a major blocker anymore.

- Moving forward, I have a few thoughts on what to implement next:
    - Function parameters
    - Braces and semicolons for functions (but at what point to extend lexer...)
    - Strings
    - Var structs
- In fact, it's looking like I might actually have to implement a formal lexer soon

- All in all, I feel really good about where I'm at, since I don't have any "theoretical" problems, right now, just language problems like from the last two days!

6/5/18
------
- Yesterday leaves me immensely excited about moving forward on the compiler, I can't wait to make it more and more resemble a "real" compiler
- I'm not sure what I'm going to implement today, but I'm leaning towards beefing up the lexer somehow, maybe with '{'
- I feel like I want to make it more "real" and until I have braces I'll be faking it
- Actually, the damn thing is getting so big I might need to break it up soon

- *Note: the current handling of ; for function termination is crude, ultimately needs to be fixed

- So today I beefed up the lexer, and now it properly handles inputs as tokens and
as such things are more robust overall. I had kind of been putting it off but I had to bite the bullet and 
do it because if I went too much further without doing so things just would have gotten a little too fucky

6/6/18
------
- I think I'm going to start working to complete the compiler's handling of functions with both Ast's for parameters as well as semicolons for statements, then variables, and lastly returning.
- I plan on using to next 4 days to go on a marathon and pump out as much code as I possibly can, to start getting it to something more recognizably functional.

6/7/18
------
- I implemented ';' and '{' checks yesterday, which feels awesome because that means I'm really coming up on being able to read full functions, and thus entire programs. It's dope. I'm going to:
    * complete the function parsing
    * implements variables

today, but necessarily in any particular order. That will hopefully take me to the point of being able to run simple programs (e.g. arithemetic, etc.), after I do the code generation for them of course. I'm going to be beyond excited at that point, because honestly I didn't really think this was even possible at first. It's one of those things that completely re-writes where you think your limits are.
- Also, I might need to finally organize this thing into multiple files, because it's 500+ lines and a lot of the functions have significantly different application.
- TODO: refactor the _read_func_args_ method... currently doesn't work...

- It's 1am but we're doing good. I've got the parsing of the function args ~halfway done. Turns out, my instinct for doing the vars first was a good one-- args are saved as vars, so you need vars first. Somehow, I ended up on the function thing though, so this is where I'm at. Finish the args, and the function work for the day will be done.

- A note so I don't forget:
    * Making the var is failing because it's not reading the name once passed into the second method. I think this is simply due to the memory layout of the Ast node itself. So, simple fix.


6/8/18 - Day 20
---------------
- Yesterday went well, I almost finished the function args/var implementation. All I have to do is polish the Ast structure for the Var and it should be good to go. 
- After that, I think it might make sense to add basic type checking on the vars/function args. I'll just make them all ints for now, but at least the decision-making process will be there so I can expand to other types more easily.
- While I'm at that, return types for the functions would probably make sense as well.
- Once all of that is in, it would be logical to recognize the return statement of the function. At that point, it would be actually be parsing real functions!
- Then I'll update code gen to actually handle these things. This is getting to be very exciting, and look like a real compiler! 

* I'm halfway through the "40 Days" goal today, and I have to say, it seemed insane and impossible, but I think I could actually make this happen. It obviously won't exactly be on feature parity with GCC, but goddamn -- it could be pretty passable.

- Update before haircut: I just need to fix the **args struct** because after the second arg the arg[0] is coming back null. Just a simple memory allocation change.
- Update: I actually fixed it before the hair cut
- I'm about to finish vars by implemented parsing for '=' assignments
- Then I may as well do the return value and work on some code generation

- It is now time to handle code generation for the variables, which will be challenging, since I have no idea how to use variables in assembly.

Rough Idea:
===========
    * Allocate each var into registers
    * Allocate enough registers (bytes) for any var declarations
    * Save callee registers? (not sure about this one)
    * Loads args into a scratch register?
    * Perform whatever operations on the args
    * Move that into the result register
    * Restore everything and return 


- Okay that was before I really understood what the assembyl was doing. Here is more accureate, albeit abstract, view on the function assembly generation:
 
    * Do the standard base pointer/frame point load
    * Allocate any parameters in variables as necessary
    * If it is a function call, move the value into a register to be used, such as edi
    * Perform any operations on them
    * create a new var/move values onto a return register

* End of day note: I'm feeling like I finally understand local variables in assembly. I'm definitely going to be able to implement them, and function arguments, tomorrow.

=============================================================================================================
=============================================================================================================

6/9/18 - Day 21
---------------

- Wow, the old Vimrc really just put me through the ringer there...
- Went down the rabbit hole and added a bunch of sweet custom commands:
 
    * For example '@q' now automatically inserts a 'printf("\n");' for debugging purposes, since it is so commonly used.
    * Another good one is that '<Space><Return>' opens Goyo, and ':Qq' closes and saves Goyo/Vim together

* But for real ->
- I made some really good progress yesterday, I finished the function parsing (save for type checking and return values), and the spent the rest of the day studying variables in x86_64 assembly. I now feel confident enough to implement the code generation for functions, function declarations, calls, return vals, etc.
- After doing that, I'll go ahead and add some basic type checking (ints), which will actually be very interesting, and serve as the basis for more complex types. It's exciting!

- I just spend the last 1.5hrs learning about Vim and changing vim/bash settings and I've gotta say that was a hell of a 90-minutes-well-spent.
- Okay, time for some work.

- Ugh, made a very obvious mistake in parsing and didn't set it up to parse multiple vars...
- Yeah, turns out that this was a huge oversight, since having multiple expressions is a reasonably significant change to the overall shape of the data structures in use

* Ideas on the issue:
    - A program is simply a list of Ast nodes
    - Each node may/may not have children
    - But this means a *Ast could have many Ast's that exist in parallel, not necessarily being children
    - This is the same way a char* is multiple chars that are not connected
    - So, perhaps one can iterate over an Ast*, recursing on the children, and covering the whole program

- This in mind, I might have to bite the bullet on declarations, because this becomes important once you move on to code generation.

- Well, I must admit that today was not terribly productive. I was not feeling well and I became bummed out when I realized that I had misjudged in my handling of the expressions. I think I can handle it by simply treaing the *Ast as an array, but I didn't have the mental energy to finish the implementation today. Tomorrow.

6/10/18
-------
- Yesterday was not super successful, but at least I realized what an important next step is.
- To implement the ideas that I describe above, I am instead going to utilize the function->body as a body[],
so that it can hold many expressions. As such, all of the functions will still be able to connect via Ast nodes, and any looping will be much more contained.

- Well, my sanity was in question the whole time, but we can now successfully parse functions with multiples expressions. That was one of the more "magical" problems. For whatever reason, since my parsing wasn't getting characters in the proper order, I was getting random ascii values and it was really making me question... all of my life choices up until this point.

- I really made more progress than I realized today, but because I thought that this was already done, it's annoying to have to actually do it. But that's life sometimes... Now it actually works with multiple expressions though, and should hook up multiple functions too, so at least I shouldn't have to revisit this in a major way.
- So tomorrow, I'll actually move on to the code gen for the vars.
- Also, I finally properly separated everything into files, which it was very much time to do.

6/12/18
-------
- I was pretty stressed yesterday so I didn't really get to work on the compiler...
- So, as it always seems to return to, I have to add more capability to my lexer
- I've implemented variables, and they are being allocated into memory, but alas... declarations vs. expressions are becoming a problem, so I'm going to have to recognize the type keyword so that I can set the difference here.
- I might hit the return keyword while I'm at it.
- Otherwise things are good, and I made a little progress. I see more deeply how variables are implemented... it just sucks because it feels as though there is so much ground to cover.

6/13/18
-------
- Well even though I spend three hours working today, it didn't feel like I got a lot done, but I really did.
- I added var positions, which was necessary to place the the bit location of vars in the assembly code
- Unfortunately, this made me realize how far I have to go before the variables are even close to being implemented. Sigh.

6/15/18
-------
- Man, this log is not looking good lately. I didn't work on the compiler since I went to jake's house last night to try and see what the deal on this ML email app is... doesn't look like it's gonna happen. Moving on.
- So I am kind of at the bottom of another mountain in terms of implementations here... without actually having types, the code gen is getting kind of fucky. So I'm biting the bullet, and still going on the parsing, now with type checking...
- I'm doing 'int' today, and if things go well, maybe 'return'. That will required modififying the node structure for functs though which may/may not be a major pain.

- The current issue is that instead of reading the type, it's making the type it's name. I think because I deleted the read_decl when I did the git reset.

*End note: The whole issue was that there isn't an AST var/vs decl distinction, which is ultimately causing nothing to happen in rd_expr2

- Okay so I just couldn't walk away afte that. It was killing me.
- I managed to get the implemenation of vars/decls in, so that now the function parsing can read the difference
- This will give it the ability to differentiate existing vars in the fbod
- Man, that was annoying

6/20/18
-------
- Well, I haven't worked on this for a few days, since I've had to get the resume put together. That's no fun, but I learned LaTeX in the process, so it looks great.
- I'm gonna try and sort out the whole variable data structure issue today... I want to finally get to see it do something.

- Well I got the variable thing I was talking about sorted out. It checks for variables and allocates them in memory now.
- The next step, since we are now determing if they are declarations/instantiations, will be to use that to actually do things like arithemtic with.
- I will need to update the parsing to actually handle this however... it doesn't seem to be parsing arithmetic for variables. Hopefully that's a simple update.
- Unfortunately, when I refactored rd_expr2, I lost this capability so it will need to be re-implemented. Sigh.

6/29/18
-------
- I have taken the last week or so off from the compiler's development to focus on interview prep questions. I find that to be very boring in comparison though, so I wanted to jump back on the compiler this weekend and get some work done.
- It looks like I'll have to try and pick up where my last entry left off, let's see how that goes.
- So I need to essentially refactor rd_expr2 in such a way that I can once again parse arithmetic since the first implementation was very naive. Here we go.

- Well I'm losing my mind so I'm going to take a break for a bit. I implemented the arithemetic thing that I wanted to do, but frustratingly, the parsing is all messed up for "return" nodes, and isn't successfully creating them yet. I don't know why quit, but it's not reading the first expression after the "return" keyword. This is probably mostly a sign that my parsing simply isn't very robust.

*End of Day:
- I am so happy, I finished the parsing of the return statements, as well as multiple expression math!
- Basically, it was hitting the end of the file or block, returning null, and blowing up. Simply returning NULL for the EOF was really all that was needed.
- note: may need to check var positions of instances for assembly gen of their values

6/30/18
-------
- Yesterday went well, considering that I was able to clean up the parsing issue at 1:30am last night. It turned out to be a fairly simple change, but one that I just hadn't considered fully. Now, it seems obviousy, that one would need to know when they are done parsing a file.
- The goal today is implement assembly generation for the entirety of the implemented features. It can now parse declarations, instances, and variables, finally. The only thing missing from the parsing is function return types, which I will do after I finish the code generation. I really want to get this thing working.

- Things are going well, and it is printing assembly for adding x + y. I just need to make sure that I am passing the var positions to variable instances as well, which I forgot to do earlier.
- I seem to have broken the var->pos again... better fix that.
- Okay, so what's happening is that the function returns the AST_RET node as an element, but does not iterate furth upon that node in order to assign vpos to the variables. I'm taking a break, but tonight I'll go back and write a function to do that, which should solve the issue. This was a pretty easy thing to overlook, so I don't feel too bad.
- This shows that I need to make variable declariations vs. instances more robust in some way. This just feels like something that shouldn't happen.

7/2/18
------
- Well I'm feeling pretty good about things. The immediate goal is to pass the the vpos to the var instances, which would leave me in a good spot. It would mean that the compiler "officially" works, at least for compiling functions that use int variables. Still, this is exciting. I'll check back in with how that went.

Update: I succeeded, and now the positions are being correctly assigned. The compiler can finally successfully compile this program:
-------------------------------------------------------------------------------------------------
        
        main(){
            int x = 1;
            int y = 2;
            return x + y;
        }

This feels like the first major success of the project, since this is a "real" program that defines variables, performs an operation on them, and gives you the result. Of course, it is almost comically simple, but it is complete in the most basic sense. I still need to implement code generation for the function parameters, which will probably be the next step today, so that it can handle what an actual main would give it. 

After that I should probably handle return types for functions... but that would require changes to the lexer, which is the source of endless heartache. Haha. 

*Note on function params: Each paramter is passed to a new reg (edi, esi, etc.) and then those registers are passed to rbp once in the function being called. Write a looping assignment function.

- So after some frustration parsing function arguments, I realized that I am an idiot and need to differentiate between function declaration and instantiations. Duh. Guess I have to parse those return types after all.

7/4/18
------
- I'm very tired today but I'm going to go ahead and implement the code generation for assembly parameters. This will allow me to run a main() with stdin inputs and pass functions to each other. After that, adding type checking will complete my basic main!

- Okay, I see the issue. My parser is only built to accept declarations with a value, e.g. `int x = 1;`. This becomes a problem because not only can you declare `int x;`, but that's precisely how they are defined in functions. So I need to be able to parse the latter. I'm surprised this didn't dawn on me earlier.
- I'm so dumb, this isn't even something I don't have, I just needed to use the make_decl() function. Oy vey.

- Not before taking a break: I need to update the parser to handle var instances better because it's looking for a value from them even if there isn't one. More nuance.

7/5/18
------
- So first thing, I'm updating the parser to recognize var references that lack declaration/assignment. Once that is cleared up, I'm expected everything to work again and to be able to pass variables between functions. We will see.

- Well, this is embarassing -- it turns out that I was being silly and just causing more problems for myself. The parser was working all along, but I misinterpreted some warnings, and then starting trying to "fix" it. This led me to making all kinds of changes that just made things all kinds of worse. So I just stashed the changes, and one by one copied them back in. Before I knew it, everything was working perfectly.

7/7/18
------
- Well I had an entry for today, but after a goold old git reset, I lost that. Essentially, I spent two hours trying to implement lexing and parsing for pointers before realizing I could just reuse the variable parsing and check that it starts with an asterisk. Oh well.

7/8/18
------
- Yesterday was a bust, so I'm gonna try amd get pointers, as well as a few other things going today. I want to be able to start computing more meaningful programs.

- That was frustrating, but I finally got pointers implemented. It wasn't frustrating so much becaue it was a technical challenge-- I just kept making things much harder on myself by trying to re-implement lower level functions such as parsing. 
- Ultimately, I just treat them as a normal variable, check to see if they are a pointer, and ensure that they reference a valid variable. 
- In the code generation, you look at the referenced variable, and then load the memory location that said variable exists in. Not super hard stuff, but I was just wasn't being smart about it.

7/9/18
------
- The pointer implementation went well once I pulled my head out of the ground. I'm thinking I might go ahead and handle multiplication/division or add some new types for the next steps. I'm trying to think of what would bring the most utility to the compiler.
