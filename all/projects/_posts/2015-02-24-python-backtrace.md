---
layout: post
title:  Python debugging tools
---

{{ page.title }}
----------------

<p class="publish_date">
24 February 2015

</p>
I have been learning python lately; So far i would use Perl 5 as my scripting language of choice, but at work they start to phase out usage of Perl. This article presents some debugging tools that I wrote in the process

------------------------------------------------------------------------

### better python stack trace

Learning a new programming language is always an aggravating business; I did not quite like is the format of the stack backtrack that python 2.7 produces when it encounters a run-time error.

So I wrote a module that produces a better backtrace that looks like the one that gdb produces with gdb commands 'bt' and 'bt all',

Here is an example stack trace:

<blockquote>
    <code>

    #1 def fact(n = 1) at test_pd.py:10
    Local variables:
    loc = 2
    loc2 = [0]
    Calls next frame at:
        pd.print_stack_ex() at: test_pd.py:10

    #2 def fact(n = 2) at test_pd.py:8
    Local variables:
    loc = 4
    loc2 = [0, 1]
    Calls next frame at:
        return n * fact( n - 1 ) at: test_pd.py:8

    #3 def fact(n = 3) at test_pd.py:8
    Local variables:
    loc = 6
    loc2 = [ 0, 1, 2 ]
    Calls next frame at:
        return n * fact( n - 1 ) at: test_pd.py:8

    #4 def fact(n = 4) at test_pd.py:8
    Local variables:
    loc = 8
    loc2 = [ 0, 1, 2, 3 ]
    Calls next frame at:
        return n * fact( n - 1 ) at: test_pd.py:8

    #5 def main() at test_pd.py:36
    Local variables:
    Calls next frame at:
        print fact(4) at: test_pd.py:36

    #6 def <module>() at test_pd.py:53
    Calls next frame at:
        main() at: test_pd.py:53

    </code>

</blockquote>
This module is called pd and that stands for python debugging module. It is similar to the standard traceback module that comes with the python libraries.

The pd module also exports the die and die2 functions, these print an error message and exit the program, they also print out a stack trace (die2 with a more detailed stack that prints out object references)

So these are similar to the built in die function in Perl;

### how to install the pd module

you can obtain the module from pypi

<code>
sudo pip install pd
</code>

quite easy if it works.

An alternative is to get the sources from the github repository:

<code>
git clone https://github.com/MoserMichael/cstuff.git
</code>

One can then install it from the sources

<code>

cd cstuff/python-pypi/pd

on unix/linux:
sudo python setup.py install

on windows:
python setup.py install
</code>

### python tracing with ntrace

The python program trace.py runs a program and displays source lines before they are executed (it can also do basic coverage report, but my ntrace.py module does only tracing)

The ntrace.py program presented here can also print out additional information:
- it can show function arguments and return values
- it can display variables when they are modified
- it can display the value of accessed variables

(Note: the ntrace program needs an extension module for part of its functionality; i have tested that on python2.7 only)

### how to install the pd module

First check that the python headers are installed on the system; for debian based systems that is (this program has its own extension module, so installation wiill compile the module)

<code>
apt-get install libpython2.7-dev
</code>

also you would need a c compiler installed on the system

<code>
apt-get install gcc
</code>

now you can obtain the module from pypi

<code>
sudo pip install ntrace
</code>

quite easy if it works.

An alternative is to get the sources from the github repository:

<code>
git clone https://github.com/MoserMichael/cstuff.git
</code>

One can then install it from the sources

<code>

cd cstuff/python-pypi/ntrace

on unix/linux:
sudo python setup.py install

on windows:
python setup.py install
</code>

### how to use the tracer

after installing the module you can invoke the command line program as a module

<code>

python -m ntrace/trace

</code>

this will give you the following help message:

<blockquote>
    <code>

    Usage: /usr/local/lib/python2.7/dist-packages/ntrace-0.0.1-py2.7-linux-i686.egg/ntrace/trace.py [OPTIONS] <file> [ARGS}

    Traces execution of python program; 

    By default it shows function parameters and function return values

    Options:
        -f          trace function parameter calls (default: on)
        -F          do not trace function parameters

        -r          trace function return values  (default: on)
        -R          do not trace function return values

        -l          trace source lines before they are executed (default: on)
        -L          do not trace source lines before they are executed

        -s          trace setting of variablesa (default: off)
        -S          do not trace setting of variables

        -g          trace getting of variables (default: off)
        -G          do not trace getting of variables

        -o          print out object content (default: off)
        -O          do not print out object content

        -t          show the current thread id in prefix for each source line (default on)
        -T          hide the current thread id  

        -n          show the nesting level, how many levels down the call stack (default on)
        -N          hide the nesting level

    By default the following options are on: -l -f -r

    </code>

</blockquote>
The functionality can also be accessed from an class: here the Ntracer class is used to run the program test\_ntrace2.py with tracing.

<blockquote>
    <code>

    import ntrace

        t = ntrace.NTracer(ntrace.TO_FUNCTION_ARGUMENTS | ntrace.TO_FUNCTION_RVALUES | ntrace.TO_TRACE_SRC_LINES )
        t.trace( 'test_ntrace2.py' , False)

    </code>

</blockquote>
### how it works

One can start another python program; first the build in python function [compile](https://docs.python.org/2.7/library/functions.html#compile) is used to compile source code has to be compiled into an abstract syntax tree (AST); next the [exec statement](https://docs.python.org/2/reference/simple_stmts.html#grammar-token-exec_stmt) takes this internal representation and runs it.

Python debuggers (and tracers) add the installation of a callback function between these two step by means of [sys.settrace](https://docs.python.org/2/library/sys.html#sys.settrace) ( it is installed prior to invoking the exec statement); the supplied callback is called upon a python function call and prior to returning from a function.
leaving

How then can we trace the assignments of variables in python?
For access to global variables this is relatively easy: the exec statement returns a dictionary that represents the global namespace - setting a global variable with the name aaaa will result into insertion of the key 'aaaa' into this dictionary. Luckily we can subclass a custom dictionary class, this subclass that will override insertion and lookup of entries. The overriden insertion/lookup function are now able to trace global variable modification/access.

Local variables are more tricky: prior to calling python method the python function, the python interpreter must create a stack frame object . This stack frame object has a dictionary for the local namespace; again, setting a local variable with the name bbbb will result into insertion of the key 'bbbb' into this dictionary that is owned by the frame object. The solution for local variables will again substitute the python dictionary with an object that is derived from class Dict.

Unfortunately we can't modify or substitute this dictionary from python code, but of course we can do so from a native library. That's exactly what is being done by the ntrace package.

(PS: Personally I think that it is a bit wasteful that Python is handling all variable accesses as hash lookup by variable name; for example the [Pooh programming language](http://mosermichael.github.io/cstuff/all/pooh-lan/2012/12/11/pooh-intro.html) translates variables into indexes on the stack/heap objects. this use of hash table for variable lookup is really inefficent, i find it quite amazing. Just prooves again that speed is not one of the priorities of scripting languages; people are ready to trade that for an improvement of preceived expressiveness)

------------------------------------------------------------------------

### comparing Perl and Python

Now here comes some subjective ranting on the subject of Perl vs. python.

I did like the following in python:

-   it is not allowed to access an undefined variable (unlike ruby where this is allowed);
-   there is lots of support for python on sites like [stack overflow](http://stackoverflow.com) ; whenever i had a question there was an answer ready.
-   also lots of libraries are available in python, and using the pypi installer is quite easy.
-   doing classes in python is mostly intuitive
-   in python every object is a reference, (in Perl you have values and references); i guess that makes it easier to use.

here are some bad parts of python and things i miss from Perl; in any event it took me some time to get used to these features.

-   the syntax of python is a bit aggravating - writing a colon at the end of some statements (like if, for, def, try, etc); the problem is that this is not a universal feature - assignments do not need a colon at the end, so i often omit the colon by error when it is needed.
-   when an undefined (not assigned) variable is used this is a run time error, this means that the program has to run until it encounters this error; in Perl Strict mode this is a check that is done while parsing the script, so that these kinds of errors are reported much earlier.
-   when appending an integer value to a string we do have to convert it to string format with the str built in.
-   in PERL i miss the way that online documentation is structured; especially perlfunc [here](http://perldoc.perl.org/perlfunc.html#Perl-Functions-by-Category) where links to specific functions are clustered around the way (similar to headwords in Roget thesaurus)
-   i am sceptic about exceptions in scripting languages - i still think that doing exceptions for scripting is an overkill.
-   the way they do getter and setter functions in classes by means of property declarations is a bit non intuitive
-   Perl has a better integration of regular expressions into the languages; in python we have to get the match object, and then extract the match groupings; in Perl you just write $1 and there you have the sub group.
-   [cpan](http://www.cpan.org/) is better organized than the [pip](https://pip.pypa.io/) archive.
-   in Perl they have references, i guess that makes it less easy to use, still this feature is powerful.

it seems to be a longer term trend: each generation of programmers has a favorite scripting language, and so I guess that in another twenty years we will see a different prevalent scripting language.

Lets hope i will still be around to witness the next change. I guess that this future scripting language will also have its good points and also have some aggravating details.