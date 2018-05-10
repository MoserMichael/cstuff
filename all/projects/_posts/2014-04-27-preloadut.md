---
layout: post
title:  Memory leaks revisited
---

{{ page.title }}
----------------

<p class="publish_date">
27 April 2014

</p>
### Memory leaks

Leaking memory is a problem that occurs frequently in C/C** programs, Any limited resource will become scarce if it is not properly used; dynamically allocated memory memory must be properly freed up, otherwise a program will run out of it.

A tool like [valgrind](http://valgrind.org/) is a very good at solving this problem for smaller programs; if you ask me then this is an important argument in favor of unit testing - checking out unit tests is the stage where valgrind can be used with great efficiency. Actually the [UNIX Philosophy](http://en.wikipedia.org/wiki/Unix_philosophy) favors small programs over big monolithic systems - big systems can be composed from little independent pieces; the rational is that it is easier to debug a bunch of small tools rather than that one big monolithic system. Of course this approach is not always doable; however unit testing brings back the advantages of the UNIX Way for larger systems (again for most bugs in most cases).

Debugging large programs is not quite as simple; comprehensive tools like valgrind either slow down the program significantly, or they have a large memory footprint.Some projects start to manage their resources by means of reference counting - the assumption here is that this is easier to trace and validate; still it does not solve the problem.
Other projects like SAMBA have a hierarchy of specialised pooled allocators ( [here](http://talloc.samba.org/talloc/doc/html/libtalloc__tutorial.html) ) ; one for the session, one for the request - this approach is similar to reference counting, the problem is not solved, it is moved into a different plane.
Still other managed languages like Java have garbage collection, still you can have a list of object that grows indefinitely without cleanup; the result is the same - the program eventually runs out of memory; so the same problem comes up again, each time it gets a different name.

This article presents some other tools that might help in some situations. Remember that there is no one solution; the requirements of each tool may make its use impractical in some situations.

### how to download the sources & build the tools

Sources can be downloaded as [zip archive](https://github.com/MoserMichael/cstuff/zipball/master) or downloaded by the git program.

        git clone https://github.com/MoserMichael/cstuff.git

The project can be build on Linux (checked 32 or 64 bit Intel/Amd); it will not work on Cygwin.

Build and install into /usr/local/ directory

        cd build
        ./build-dbgmem.sh install

Build and install into /usr/alt directory a

        cd build
    `   ./build-dbgmem.sh install INSTALL_PREFIX=/usr/alt

### Common technique used here

The tools here use a common trick: they install a specialised version of memory allocation functions; the shared library loader is now tricked to call these custom functions instead of the GLIBC memory allocation function
(The substituted functions are: malloc, free, realloc, posix\_memalign, valloc; c** also adds new, new\[\], delete, delete\[\])

The trick is to instruct the shared library loader to load our specialized shared library first, before loading all other libraries (this is done by setting environment varialble LD\_PRELOAD to the full path of our shared library file).
Our special library now implements the memory allocation functions (malloc and free); The trick is that all other shared libraries and the executable will now call our version of malloc and free !
This magic is due to the fact that the Unix shared loader resolves shared library function calls upon first call; when any other shared library, or the executable call malloc, the shared library loader resolves this to the first known implementation.

Now this trick does not work on CYGWIN - CYGWIN is using the Windows shared library loader, and Windows does not support this trick.

An example of this trick is the INITMEM project ( [see here](https://github.com/MoserMichael/cstuff/tree/master/preloadut/initmem) )- a very simple tool that sets all newly allocated memory to the value 0xDD and sets all freed memory to the value 0xEE (actually GLIBC does that if you set the environment variable MALLOC\_PERTURB\_ ). All by itself it is not quite useful, however it shows the concept.

### Overview of the tools

#### TRACEMEM - tracing memory allocations

One approach is to trace all memory allocations as they happen; a shared library intercepts all call to the memory allocation functions; it directs them to GLIBC memory allocator, however a log entry is written to a file for each call to malloc/free/etc. The log file entry contains pointer value allocated or freed, the size of the memory block, and a few stack frames that describe what part of the program caused the logged even. This is the raw information, when the process exists this information is analyzed, if a memory allocation is not paired with a call to free then this event is reported as a leak.

This trick has a very low memory overhead - nothing is added on top of the memory allocation; however logging each memory allocation takes quite some time, the program will therefore run significantly slower.

GLIBC supports this trick - [see here](http://www.gnu.org/software/libc/manual/html_node/Tracing-malloc.html) ; however it is implemented by means of [malloc hooks](http://www.gnu.org/software/libc/manual/html_node/Hooks-for-Malloc.html#Hooks-for-Malloc) ) - a terrible interface that results in critical section lock for every allocation.
TRACEMEM does not add any locks on its own. Another point is that mtrace only records one stack frame - the direct caller of the memory allocation function, whereas TRACEMEM can log several stack frames, so that the context of the memory leak is easier to understand.

#### DBGMEM - memory debugger (simple mode)

this tool again implements its own version of the memory allocation functions - the DBGMEM version of malloc asks asks for more memory than what has been requested by the actual caller of malloc; this added memory is used for a header - this header placed right before the pointer of user memory that is returned by malloc.

This approach has a higher memory overhead if compared with TRACEMEM; however each memory allocation takes less time, as no log record is written to a file. Please note that the current implementation adds a critical section lock; allocations are tracked by means of a double linked list. When the program exits, all remaining entries in this list are classified as leaks.

DBGMEM in simple mode also adds a word of memory before and after the memory block returned to the user; free and delete is therefore able to detect if the memory block has been overwritten.


            linked list of all allocations
      ----------------------------------------------------------------
      |                              |
      V                                                              V
    [DBGMEM header][guard word][user memory][guard word] [DBGMEM header][guard byte][user memory][guard byte]
    [-- memory returned by glibc ----------------------] [-- memory returned by glibc ----------------------]

#### DBGMEM - memory debugger (check mode)

This tool adds an even larger memory overhead on top of the memory and runtime overhead on top of the GLIBC allocator.
This tool does not place a memory header adjacent to the memory block returned to the user, as this header can be corrupted by a bug in the application. Instead the memory header that maintains its additional information in a Radix tree; the pointer value returned to the application is the key in this Radix tree.

This schema has an additional benefit: for pointers that are located inside an allocated memory block it is relatively easy to look up the information that describes the memory block.
It is possible to add checks for functions like strcpy/memcpy and friends, and check if they overwrite a dynamically allocated memory block;this trick does not always work: for instance the GCC compiler inlines memcpy/strcpy on most occasions.

### How to use the tools

#### Using TRACEMEM

There is a helper script - runtracemem; it is used to execute the application that is checked; the script waits until the program exits; on completion of the test run it processes the output log file and generates an html report.

The runtracemem script accepts the following command line;

    [-f reportfile] [-s NUMBER] [-a on|off] [-r <sig_num>] <cmd-to-debug>

    Starts a process with TRACEMEM memory allocation tracer. Each memory allocation is traced.

    Options:

    -f reportfile       set name of report file. if not set then the name is $FNAME

    -s NUMBER           number of stack frames displayed. (default 3)

    -a on
    -a off              if tracing is enabled when process starts (on) or disabled (off); (default on)

    -r <sig_num>        sets a signal handler; the signal handler toggles tracing mode; (example: if tracing was on then it stops upon signal)

    [cmd-to-debug]      command line of process that will be started and debugged.

    Note: this debugged process will set its core limit to unlimited.  

    Note: this is a safer way to do what glibc mtrace is doing; also it does not introduce any locks.

You can start the process while tracing is disabled, this allows you to skip allocations that are performed once during program start up; tracing can later be turned on once the system has completed its initialisation sequence.
To turn on tracing you can either

1.  install a signal handler that will turn on tracing upon reception of signal (option -r <sig_num>)
2.  from the application: call mallopt(1004,1) to turn on tracing; call mallopt(1004,0) to turn off tracing.

Please note that the generated HTML is best opened with Chrome.

#### Using DBGMEM

this topic has its own help file: please see [here](/cstuff/dbgmem/Introduction.htm)

#### Using DBGMEM (simple allocator) to debug cores

a core file that is produced while using DBGMEM with the simple tool can be analysed for memory corruptions.
This check follows the chain of all alocations and checks if the magic marker at the start and end of the allocation have been overwritten.
In case that a corrupted memory block has been detected, a stack trace that describes where the block has been allocated is shown.

This magic is done by scripting gdb (requires gdb to be compiled with python support). Please be patient, this check takes a lot of time for larger core files.

        gdb <executable file> <core file>

        (from gdb promt)

        source /etc/dbgmems-read-core.py  

in one command that would be as follows:

    gdb --batch -ex='core-file <path to core file>' -ex='source /etc/dbgmems-read-core.py' <executable program path>
