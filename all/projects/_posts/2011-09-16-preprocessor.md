---
layout: post
title:  Fixing tricky C/C++ compilation errors
---

{{ page.title }}
----------------

<p class="publish_date">
17 July 2011

</p>
Both C and C** share the same preprocessor; a preprocessor transforms the source code before it is compiled; Now many compilation errors are due to the programmer having misunderstood how the preprocessor changed the text of the source file.

Luckily with GCC and other compilers (aCC for instance), it is possible to see the preprocessor output as is, before it is passed on to the compiler.

-   On both gcc and aCC (the HP C compiler) you can just add the -E option to the compiler ; now the compiler will not compile the program, only preprocessor does something, and the output of the preprocessor step is written to standard output.
-   With gcc you can use compiler option -save-temps; with this option all intermediate results (preprocessor result, assembly listing) will be stored into separate files; For example

<!-- -->

    gcc -save-temps example.cpp -o example.o 

After running this you will have example.ii - with output of the preprocessor right in the same directory as example.cpp (also example.s has the assembly listing).
The output of the preprocessor is [described here](http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html) Now it is difficult for a human to parse this text;

So it's time for a tool that would annotate the preprocessor output.

-   So for each line of the preprocessor output I would like to add the source file + line number; so given a compilation error I can easily find the line number that has caused it.
-   For each line of the preprocessor output I would like to have the text of the source file that caused it, If these two data points are different, I would like to see them both at the same spot.

That's exactly what we are doing with a nice Perl script, welcome to the [cppbeautifier script](https://github.com/MoserMichael/cstuff/blob/master/scripts/cppbeautifier)

    Usage:
        cppbeautifier -i <cpp_file> [-d <directory>] -o <report_file>

    Description
        Takes an output file produced by the c processor and anotatets it, 
        so that it will be readable by humans.

        For each line of preprocessor output we find the corresponding file from 
        the source file; if these are different then both source and preprocessor 
        line are displayed, this way it it is easier for the programmer to determine 
        the result of the macro expansion.

        Lines from the source file have prefix <filename>:<line number>:+ and lines 
        from preprocessor output have prefix <filename>:<line number>:-

        The preprocessor output format is described here:    
        http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html 

        Please note that the preprocessor often includes reference to source 
        files with their relative path, so you should either use this program from 
        the same current directory where you have run the preprocessor / compiler.
        Alternatively set the -d option to this directory. 

Lets show off with some example output: here a generator macro is creating a function, and the script output shows us how the preprocessor output correlates to the source macro invocation. Lines with + are from the source file, lines with - are the expanded macro - preprocessor output.

<pre>
# 167 "bf.h"
bf.h:167:+:BF_DECLARE_PUT_MACRO( uint8_t )
bf.h:167:-:static inline int BF_put_uint8_t ( BF *bf, uint8_t val ) { if ((bf->end - bf->put_pos) < (ptrdiff_t) sizeof( uint8_t ) ) { return -1 ; } do { uint8_t nval = (val); *( ( uint8_t *) bf->put_pos) = nval; bf->put_pos += sizeof ( uint8_t ); } while(0); return 0; }
bf.h:168:+:BF_DECLARE_PUT_MACRO( int8_t )
bf.h:168:-:static inline int BF_put_int8_t ( BF *bf, int8_t val ) { if ((bf->end - bf->put_pos) < (ptrdiff_t) sizeof( int8_t ) ) { return -1 ; } do { int8_t nval = (val); *( ( int8_t *) bf->put_pos) = nval; bf->put_pos += sizeof ( int8_t ); } while(0); return 0; }
bf.h:169:+:BF_DECLARE_PUT_MACRO( uint16_t )
bf.h:169:-:static inline int BF_put_uint16_t ( BF *bf, uint16_t val ) { if ((bf->end - bf->put_pos) < (ptrdiff_t) sizeof( uint16_t ) ) { return -1 ; } do { uint16_t nval = htons( val ); *( ( uint16_t *) bf->put_pos) = nval; bf->put_pos += sizeof ( uint16_t ); } while(0); return 0; }
bf.h:170:+:BF_DECLARE_PUT_MACRO( int16_t )
bf.h:170:-:static inline int BF_put_int16_t ( BF *bf, int16_t val ) { if ((bf->end - bf->put_pos) < (ptrdiff_t) sizeof( int16_t ) ) { return -1 ; } do { int16_t nval = htons( val ); *( ( int16_t *) bf->put_pos) = nval; bf->put_pos += sizeof ( int16_t ); } while(0); return 0; }
bf.h:171:+:BF_DECLARE_PUT_MACRO( uint32_t )
bf.h:171:-:static inline int BF_put_uint32_t ( BF *bf, uint32_t val ) { if ((bf->end - bf->put_pos) < (ptrdiff_t) sizeof( uint32_t ) ) { return -1 ; } do { uint32_t nval = htonl( val ); *( ( uint32_t *) bf->put_pos) = nval; bf->put_pos += sizeof ( uint32_t ); } while(0); return 0; }
bf.h:172:+:BF_DECLARE_PUT_MACRO( int32_t )
bf.h:172:-:static inline int BF_put_int32_t ( BF *bf, int32_t val ) { if ((bf->end - bf->put_pos) < (ptrdiff_t) sizeof( int32_t ) ) { return -1 ; } do { int32_t nval = htonl( val ); *( ( int32_t *) bf->put_pos) = nval; bf->put_pos += sizeof ( int32_t ); } while(0); return 0; }
</pre>

### A short (and probably incomplete) history of macro facilities

Macro facilities have a common problem - they just transform text / rewrite text from one form into another; so the macro facility does not know anything about the syntax of the text that it is being transformed. For simple usage - conditional compilation of certain blocks of text, this ok; for macros that rewrite the source into something else we will have to look separately at the macro text, and at the result of the macro expansion.

So Macro facilities were the preferred solution with languages that have no or little syntax;

-   The assembly language of S/390 ; Back in the days of the mainframe, people were writing very complicated application programs all in assembly. Now the cost of leasing processor cycles was very high; so organizations preferred to keep a lot of assembly programmers rather than compromising for an application that would be slower at runtime. Now the IBM assembler has a very strong macro facility; in fact all system calls were macro invocations (there is no such thing as a runtime library on the mainframe); Applications were regularly doing huge macros that were expanding into hundreds and thousands of lines of code; Now macros where a central feature of the language; in order to debug them the assembler would produce a fairly huge assembly listing; all levels of macro invocations where listed along with how they expanded the text / one could exactly see what text was being modified by which macro invocation. It is interesting that this feature of exactly listing the macro invocation input versus its output seems to have been lost thereafter.

\* LISP (or Lisp) macros; Lisp doesn't have a syntax; in fact a Lisp program is sort of a textual representation of a Abstract Syntax tree, in this minimal language the macro facility was very strong, a macro invocation can do computations of its own; Macros are the preferred way of extending the language; for example Common Lisp does not have a loop construct, instead there is a built in loop macro. Lisp Macros are the preferred way of doing 'internal DSL' - that means an extension of the syntax of language A by means of features already present in the language A. A lot of wizardry and hacking has been done with lisp macros. More on the [history of Lisp can be read here](http://www.dreamsongs.com/NewFiles/HOPL2-Uncut.pdf) . With Lisp you have the macroexpand and macroexpand-1 functions; macroexpand will show you the full result of expanding a macro, macroexpand-1 will show you the expansion of a macro to the next level (macro's within macros are not expanded with macroexpand-1).

-   The C preprocessor; The C preprocessor is less powerful by comparison than what has been, in fact C is a very minimal language by itself. One strong but often overlooked feature of the C preprocessor is that it facilitates the constructing of code generators that embed C statements; example of such systems are YACC/BISON compiler generator or various embedded SQL solutions for instance (Oracle PSQL for example); The higher order language can include sections written in C that do something; now this text is translated into C; but the debugger knows exactly how the embedded C statement maps back to the original higher order language definition. This is done by means of [the \#line directive](http://gcc.gnu.org/onlinedocs/cpp/Line-Control.html) - the \#line directive tells the compiler that the following section of code should be marked in the debugger line table as coming from a different source; the debugger is looking at the line table and will look for the source of this goodness in the higher order language definition.
    Now with GCC and other compilers you can invoke the preprocessor as a separate process; this is done because most compiler stages are done as a processes that talk to next compiler stage via a pipes, so the preprocessor is just a command line program that pipes its output to the syntax checker / AST builder.

<!-- -->

-   Modern languages; Java, Scala most JVM languages - do not have a preprocessor, and they are proud of that (probably that is a feature that has been proudly inherited from Smalltalk). One of the better features of Java is it's very fast compilation speed and lack of linkage step, this is achieved by integrating the semantic verification step of the compiler with the class loading mechanism; when the compiler checks for a method signature of a given class, it just looks up the meta data of a class in the already compiled class file. I guess a preprocessor would slow compilation speed, so Java people would not like that feature. Of course you lose some better features - like the C preprocessor support for code generators, well I guess one can't always win; the C preprocessor is better integrating 'external DSL's' - so be it. In Scala one can still do an 'internal DSL' - that means an extension of the language syntax of language A by means of features already present in the language A has; Now interestingly modern language extension mechanisms are not founded on any macro facilities; instead on this, we are now supposed to overload operators or work with monad comprehension (some syntactic trickery of combining functions like Lisp functions named map, filter and fold - that's my explanation of monads : -)