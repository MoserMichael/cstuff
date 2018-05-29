---
layout: pooh
title:  The Pooh programming language for kids and grown ups.
---

{{ page.title }}
----------------

<p class="publish_date">
11 December 2012

</p>
The pooh programming language is an educational programming language. I think this means that the language should help to overcome difficulties when one tries to learn it. Many other programming languages are full with tricky tricks that are there to save a few keystrokes while typing long programs, these aids are very powerful, but people who are not already used to thinking in complicated patterns are put off by them. Learning to program with a complicated language becomes some sort of rite for geeks, who already like complicated things; Pooh thinks that the language that a person uses is important ; it makes a person accustomed to think the way he speaks, this goes on until many people are no longer used to think in any other ways. The computer language one uses as a programmer therefore sets ones mind to thinking in geeky ways (probably only when programming?). This thought is also known as the [Sapir-Whorf hypothesis](http://en.wikipedia.org/wiki/Linguistic_relativity#Programming_languages) ; maybe there is some truth in it with regards to computer languages. I think that a simple, yet powerful language would help here.

### Availability

Currently the project is available in source form only, It is released under the BSD Three clause license;
The copyright is owned by Michael Moser ( c ) 2012
Sources can be downloaded as [zip archive](https://github.com/MoserMichael/cstuff/zipball/master)
or downloaded by the git program.

        git clone https://github.com/MoserMichael/cstuff.git

The project can be build on Linux or on Windows with Cygwin. To build the program,we need the presence of bison, flex, gcc and a recent version of make.

Build and install into /usr/local/ directory

        cd build
        ./build-pooh.sh install

Build and install into /usr/alt directory

        cd build
        ./build-pooh.sh install INSTALL_PREFIX=/usr/alt

### Documentation

The [Introduction to Pooh](/cstuff/docs/html/pooh/pooh-lan.pdf) which is not complicated. (currently still working on it :-(
Here [Pooh library reference](/cstuff/docs/html/pooh/pooh-ref.pdf) which is at times short an more complicated
Here are [Some explanations of the design decisions](/cstuff/docs/html/pooh/designnotes.htm) should be interesting for people who like programming languages.

### Features

This section is interesting for people who already know a few programming languages.

-   The interpreter can produce a detailed execution trace with with the annotated values for all variables. Here is an example to run a program that computes the factorial of the number 4.
    !include tomd-include-0.txt
-   functions: all parameters are passed as [named parameters](http://en.wikipedia.org/wiki/Named_parameter) , order of parameters does not matter. Named parameters make the program much more readable.
-   All values defined in a function are of local scope; if you want to access a global from a function, then you have to add the <b> global . </b> prefix to the variable name.
-   strings, numbers, arrays and hashes are not objects, one does not have to introduce object oriented programming from the start on;
-   A variable is defined when a value is assigned to a variable name;.
-   The compiler forbids to reference uninitialized values (like 'use strict' in Perl and recently optional in Javascript); also forbidden to call of undefined functions ; forbidden to use of array or hash as scalar. I really like Perl strict mode; too much dynamic typing gets me confused.
-   In arithmetic operators one can use both number and string variables (strings are converted automatically); Comparison operators - there is a set of numeric comparison operators, where strings are interpreted as numbers, and string comparison operators, where both numbers and strings are allowed and interpreted as string.
-   data structures: has vectors and hash values for collections.
-   A simplified model of references to values is supported.
-   Does object oriented programming by [prototyping (prototype based programming)](http://en.wikipedia.org/wiki/Prototype-based_programming) ) ; access to data members via <b> this . </b> prefix to variable names
-   the concept of [closures](http://en.wikipedia.org/wiki/Closure_(computer_science)) is supported, one can have objects via closures. Non local variables references have the <b> outer . </b> prefix.
-   the concept of [co-routines / generators](http://en.wikipedia.org/wiki/Coroutine) is supported; for loops invoke a generator thread (similar to python)
-   has [higher order functions](http://en.wikipedia.org/wiki/Higher-order_function) like map/foldl/filter/sort
    [Multiple return values](http://rosettacode.org/wiki/Return_multiple_values) / [Parallel assignment](http://en.wikipedia.org/wiki/Assignment_(computer_science)#Parallel_assignment) is supported.
-   Strings are can span multiple lines and can embed expressions; very similar to like HERE documents; the language has one string constant format, not twenty.
-   It is possible to extend the language with shared libraries written in C.
-   It is possible to include other text, so it is compiled together with the program.
-   Compiler has detailed error messages. Invested quite some effort to make them clear.

Things left out:

-   No exceptions. Pooh programs either succeed or fail with a very detailed stack trace, its that simple.
-   No namespaces; another complication.
-   No meta programming; no pre-processor madness
-   No eval statement;
-   Currently no regular expression parsing; I would like something with fewer arcane rules, maybe BNF grammars like REBOL; Any ideas are and suggestions are really welcomed.
-   Currently no way to do unicode strings; Really Have To Fix That.
-   No auto-increment operators. One feature less to explain.

All this means pooh is not strongly typed, and also not a dynamic language. Hope you are still interested ;-) Because of the simplified reference model, the language probably does not translate into efficient machine code. Don't expect the current implementation to be fast, it is an AST walker; that's because it is easier to do the trace output with an AST walker; also I want to add a [Read-eval-print loop](http://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop) ( in a coming release.

### Tests / examples

This table lists some example programs, they make up the test suite of the project and are run on each build.

|                                                                                                       |                                                                                                                        |
|-------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|
| *Test name*                                                                                           | *Description*                                                                                                          |
| "01-add.p":/cstuff/docs/html/pooh/example/01-add.p.html                                               | mixing strings with numers is allowed in arithmetic operators                                                          |
| "01-copy-ref.p":/cstuff/docs/html/pooh/example/01-copy-ref.p.html                                     | with references, the := operator creates alias, left hand side refers to value returned by right hand side expression. |
| "01-function-call-by-ref.p":/cstuff/docs/html/pooh/example/01-function-call-by-ref.p.html             | function call by reference , call by value                                                                             |
| "01-hello-world.p":/cstuff/docs/html/pooh/example/01-hello-world.p.html                               | hello world                                                                                                            |
| "02-fact.p":/cstuff/docs/html/pooh/example/02-fact.p.html                                             | function: get factorial of a number (recursive)                                                                        |
| "02-loop.p":/cstuff/docs/html/pooh/example/02-loop.p.html                                             | loops: function computes the sum of squares over integer range                                                         |
| "03-int-range.p":/cstuff/docs/html/pooh/example/03-int-range.p.html                                   | function fills array with number range, array is the return value.                                                     |
| "03-matrix-mult.p":/cstuff/docs/html/pooh/example/03-matrix-mult.p.html                               | test work with two dimensional matrixes.                                                                               |
| "03-prime-while-loop.p":/cstuff/docs/html/pooh/example/03-prime-while-loop.p.html                     | array test: sieve of Eratosthenes gets all prime numbers                                                               |
| "03-print-array.p":/cstuff/docs/html/pooh/example/03-print-array.p.html                               | array test: print all elements of array, iterate with while statement.                                                 |
| "03-print-table.p":/cstuff/docs/html/pooh/example/03-print-table.p.html                               | iterate over table and print out values.                                                                               |
| "03-print-table2.p":/cstuff/docs/html/pooh/example/03-print-table2.p.html                             | iterate over table and print out values.                                                                               |
| "03-qsort.p":/cstuff/docs/html/pooh/example/03-qsort.p.html                                           | a quick sort written in pooh                                                                                           |
| "03-string-split.p":/cstuff/docs/html/pooh/example/03-string-split.p.html                             | test split library function - string split into array.                                                                 |
| "03-stringlib.p":/cstuff/docs/html/pooh/example/03-stringlib.p.html                                   | tests string library functions                                                                                         |
| "03-table-any-kind-of-key.p":/cstuff/docs/html/pooh/example/03-table-any-kind-of-key.p.html           | table with any kind of keys                                                                                            |
| "04-file-list.p":/cstuff/docs/html/pooh/example/04-file-list.p.html                                   | test listing files in directory.                                                                                       |
| "04-file-stat.p":/cstuff/docs/html/pooh/example/04-file-stat.p.html                                   | check properties of a file                                                                                             |
| "04-multi-assign.p":/cstuff/docs/html/pooh/example/04-multi-assign.p.html                             | test multi value assignment                                                                                            |
| "04-strace-test.p":/cstuff/docs/html/pooh/example/04-strace-test.p.html                               | show stack trace of deep nested function (showstack function)                                                          |
| "04-time.p":/cstuff/docs/html/pooh/example/04-time.p.html                                             | get current date / time; functions: localtime, gmtime, currenttime                                                     |
| "06-func-obj-call.p":/cstuff/docs/html/pooh/example/06-func-obj-call.p.html                           | function invokes argument callback over range of numbers; prints result as ASCII graph                                 |
| "08-object-prototype-finalizer.p":/cstuff/docs/html/pooh/example/08-object-prototype-finalizer.p.html | test where object has a finalizer - it is called when object is freed.                                                 |
| "08-object-prototype-point.p":/cstuff/docs/html/pooh/example/08-object-prototype-point.p.html         | 0bject oriented programming with prototypes: point class                                                               |
| "08-object-prototype-stack.p":/cstuff/docs/html/pooh/example/08-object-prototype-stack.p.html         | 0bject oriented programming with prototypes: stack class                                                               |
| "10-object-closure-point.p":/cstuff/docs/html/pooh/example/10-object-closure-point.p.html             | Object oriented programming with closures: point class                                                                 |
| "10-object-closure-stack.p":/cstuff/docs/html/pooh/example/10-object-closure-stack.p.html             | Object oriented programming with closures: stack class.                                                                |
| "11-file-pipe.p":/cstuff/docs/html/pooh/example/11-file-pipe.p.html                                   | run process and script send data to it via stdin and read stdout                                                       |
| "11-file-test.p":/cstuff/docs/html/pooh/example/11-file-test.p.html                                   | working with files                                                                                                     |
| "15-thread-range.p":/cstuff/docs/html/pooh/example/15-thread-range.p.html                             | co-routine threads: create thread via API, thread yields a range of numbers.                                           |
| "15-thread-range-1.p":/cstuff/docs/html/pooh/example/15-thread-range-1.p.html                         | create co-routine thread via api                                                                                       |
| "15-thread-range-fib.p":/cstuff/docs/html/pooh/example/15-thread-range-fib.p.html                     | co-routine threads: create thread via API, thread yields sequence of fibonaci numbers.                                 |
| "16-load-extension-library.p":/cstuff/docs/html/pooh/example/16-load-extension-library.p.html         | load an extension library written in C and call exported function                                                      |
| "17-for-lib.p":/cstuff/docs/html/pooh/example/17-for-lib.p.html                                       | co-routines / generators as used with for statement                                                                    |
| "17-for-range.p":/cstuff/docs/html/pooh/example/17-for-range.p.html                                   | co-routines / generators as used with for statement                                                                    |
| "17-for-range-lib.p":/cstuff/docs/html/pooh/example/17-for-range-lib.p.html                           |                                                                                                                        |
| "17-for-while-break-skip.p":/cstuff/docs/html/pooh/example/17-for-while-break-skip.p.html             | for loop ; test keywords: break - finishing the iteration and skip - continue to next iteration.                       |
| "18-higher-order-func.p":/cstuff/docs/html/pooh/example/18-higher-order-func.p.html                   | Higher order functions fold/filter/map implemented as script functions.                                                |
| "18-higher-order-func-lib.p":/cstuff/docs/html/pooh/example/18-higher-order-func-lib.p.html           | Higher order functions fold/filter/map - use built in library implementation.                                          |
| "18-higher-order-func-lib2.p":/cstuff/docs/html/pooh/example/18-higher-order-func-lib2.p.html         | Higher order functions fold/filter/map - use built in library implementation.                                          |
| "18-sort.p":/cstuff/docs/html/pooh/example/18-sort.p.html                                             | use standard library sort procedure - with callback for comparison                                                     |
| "18-sort-anonymous.p":/cstuff/docs/html/pooh/example/18-sort-anonymous.p.html                         | use standard library sort procedure - with anonymous function as callback for comparison                               |

</p>
The next table lists some libraries that can be included - all written in pooh

|                                                                         |                                                        |
|-------------------------------------------------------------------------|--------------------------------------------------------|
| *Test name*                                                             | *Description*                                          |
| "01-complex.p":/cstuff/docs/html/pooh/example/01-complex.p.html         | test complex number library - complex.inc              |
| "01-matrix.p":/cstuff/docs/html/pooh/example/01-matrix.p.html           | test matrix library - matrix.inc                       |
| "02-heap.p":/cstuff/docs/html/pooh/example/02-heap.p.html               | test heap data structure - heap.inc                    |
| "02-heap2.p":/cstuff/docs/html/pooh/example/02-heap2.p.html             | test heap data structure - heap.inc                    |
| "03-queue.p":/cstuff/docs/html/pooh/example/03-queue.p.html             | test queue data structure - queue.inc                  |
| "03-stack.p":/cstuff/docs/html/pooh/example/03-stack.p.html             | test stack data structure - stack.inc                  |
| "04-slist.p":/cstuff/docs/html/pooh/example/04-slist.p.html             | test linked list data structure - slist.inc            |
| "05-graphlist.p":/cstuff/docs/html/pooh/example/05-graphlist.p.html     | test adjacency graph data structure - graphlist.inc    |
| "05-graphmatrix.p":/cstuff/docs/html/pooh/example/05-graphmatrix.p.html | test adjacency matrix data structure - graphmatrix.inc |
| "06-graphutils.p":/cstuff/docs/html/pooh/example/06-graphutils.p.html   | test graph traversal class - graphutils.inc            |

### So what is next ?

So far I did not test the whole concept with real clients - nobody has ever used the Pooh language in order try to teach how to program. Well, it has to start with someone ?.

The concept of passing all function parameters as named parameters maybe a consistent and readable choice, but it is probably more difficult to write the program text this way. What may help is the addition of a special editing program - an IDE (Interactive developing environment) like Eclipse but with a user interface adapted for young people. Here the program parameter names will pop up as hints, together with lots of explanations about the function and its parameters. Now that is a lot of work.

Another important direction is addition of practical parsing engine ; similar to those of REBOL or Perl 6.

In the meantime I will continue to

-   Test the system under different configurations
-   Add missing library functions;
-   Complete the introduction text.

So maybe all we have is an eclectic set of features chosen by virtue of my personal preferences and not because of any objective and measurably better usability. Maybe that's the way programming languages are progressing; it is hard to compare approaches so that one can make a valid generalization; no tool is going to be the best in all cases, and anyway comparison are often between apples to oranges. Well enough quetching already, lets get some work done!

### And now for something completely different

The best pooh movies are here


<iframe width="420" height="315" src="http://www.youtube.com/embed/sqdiEUp6s4E" frameborder="0" allowfullscreen></iframe>

<iframe width="420" height="315" src="http://www.youtube.com/embed/qam9JBk5Oig" frameborder="0" allowfullscreen></iframe>
