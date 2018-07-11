*CJCC* - CJ's C Compiler
=======================

This compiler is a running project under development, and still quite far from complete. Still, there's a lot to learn
even at this stage.

I'm writing it to learn about how compilers work, which when I started, was something I knew nothing about. While in
the process, I'm keeping fairly detailed notes in my notes.md file. If you are interested in my thought process as 
I wrote each element, that would be a good place to look. 

Ultimately, I might write a series of blog posts, or even a PDF document that describes my experience,
and serves as a guide for others who would like to write their own compilers. After much research,
I found surprisingly few such projects that I could use as reference.

It seems that much of the documentation on the subject is either highly theoretical (which is very useful, 
but hard to implement sometimes), incomplete in scope, or simply not was I was looking for (a C compiler in C).
This is compounded by the fact that the source code for production compilers is often overwhelmingly complex,
even for smaller examples. 

If any of this interestests you or you have any questions, please don't hesitate to reach out via email,
on here. I am planning on doing a write-up on this process regardless, but if I had anyone whom I knew was interested
I would do it sooner.

Compiling and Running 
---------------------
Right now everything is very simple:
1. `git clone https://github.com/cjboake/cjcc.git` 
2. `cd src`
2. `make cjcc`
3. `./cjcc num` This will run the test file `num` in the src
directory. I will add more programs to `test_programs` soon.
*6/30/18 - This command will print assembly to your console,
without execution.*
4. `make clean`  Delete the object files and executables.
5. Currently there is no `make install`, but I will add this 
once *cjcc* is ready for more rigorous testing. The same follows
for `make tests`.

It will compile a simple function such as this one now:
```
int main(int a, int b)
{
    int x = 2;
    int y = 3;
    int a = 1;
    int *p = &a;
    return x + y;
}
```
Right now it only supports variables of type `int` and `*pointer`. The function is obviously not a proper `main`,
but all that is left to do for that is add the char[] type.

This will currently print the assembly output for the test program, *num*. As this process changes,
I'll update the instructions to follow. 

The compiler may or may not completely output assembly/execute output (or do so correctly)
after a given commit, depending on the stage of development, but once it can compile a complete *main* I will begin
tagging commits with working versions.
