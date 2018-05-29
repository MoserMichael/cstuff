---
layout: post
title: StackMirror tool for fixing of stack corruption problems
---

{{ page.title }}
----------------

<p class="publish_date">
19 July 2011

</p>
Debugging [buffer overruns](http://en.wikipedia.org/wiki/Buffer_overflow) is hard. Usually you have to solve these bugs with logs + apply divide and conquer techniques. Even [valgrind](http://valgrind.org/) can't help here !
GCC has the compiler options -fstack-protector-all , but it does not give accurate stack traces, it is more designed to shut down an application so as to avoid [remote exploit](http://insecure.org/stf/smashstack.html) of a buffer overflow error.

So a new open source tool is presented here that can help with most such cases; StackMirror to the rescue! It detects even the nastiest stack smashers and reports an accurate stack trace of the function that caused it.

First of all how to use it

- The tool works with gcc (versions or upward)

- The application must be re compiled with gcc compiler option -finstrument-functions

- During runtime the application has to load stack.so library, so either LD\_PRELOAD environment has to be set to the shared library, or application has to link with it.

- Application should not be compiled with Optimization (-O1 -O2 -O3 ) and frame pointer omission (Please use both -fno-omit-frame-pointer and -On in order to prevent frame pointer omission).
- I have tested the tool on both i686 and x86\_64; please look if it works on other platforms too.

### Download instructions

Get [my stuff](https://github.com/MoserMichael/cstuff)
chdir stack
make
(for specific platform builds do make ARCH=x86\_64 or make ARCH=i686 )
make install
the preceeding line will install to /usr/local
make install INSTALL\_PREFIX=/opt/stack
will install stuff to prefix /opt/stack

### Step by step usage instruction

Prior to running your program, please set the following environment variable
export LD\_PRELOAD=/usr/local/lib/libstack.so
Please note that value of LD\_PRELOAD must be the full path name of libstack.so

Run you program, if it smashes the stack and we are ok then an error message of the following form is displayed. Don't worry; it will be formatted in the next step.

<pre>

ERROR: Stack corruption detected
**** start stack ***
frame 0 ip: 0x80484e4
frame 0 ip: 0x80485c3
frame 1 ip: 0x804856f
frame 1 ip: 0x80485e7
frame 2 ip: 0x804856f
frame 2 ip: 0x80485e7
frame 3 ip: 0x804856f
frame 3 ip: 0x80485e7
frame 4 ip: 0x804856f
frame 4 ip: 0x80485e7
frame 5 ip: 0x804856f
frame 5 ip: 0x80485e7
frame 6 ip: 0x804856f
frame 6 ip: 0x80485e7
frame 7 ip: 0x804856f
frame 7 ip: 0x80485e7
frame 8 ip: 0x804856f
frame 8 ip: 0x80485e7
frame 9 ip: 0x804856f
frame 9 ip: 0x80485e7
frame 10 ip: 0x804856f
frame 10 ip: 0x804864e
frame 11 ip: 0x8048627
frame 11 ip: 0x80486c7
frame 12 ip: 0x8048670
frame 12 ip: 0x40049e37

08048000-08049000 00000000 /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
40000000-4001c000 00000000 /lib/i386-linux-gnu/ld-2.13.so
4001e000-4001f000 00000000 [vdso]
40021000-40023000 00000000 /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/lib/libstack.so
40033000-4018d000 00000000 /lib/i386-linux-gnu/libc-2.13.so
*** eof stack ***
</pre>

Save this error message to a file crash.log and run
/usr/local/bin/readerrors.pl crash.log

It should create the following message:

<pre>
-----------------------------
ERROR: Stack corruption detected
-----------------------------
ip 0x80484e4 from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
crash_me
test.c:10

ip 0x80485c3 from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
fact
test.c:36

ip 0x804856f from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
fact
test.c:30

ip 0x80485e7 from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
fact
test.c:38

ip 0x804856f from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
fact
test.c:30

ip 0x80485e7 from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
fact
test.c:38

ip 0x804856f from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
fact
test.c:30

ip 0x80485e7 from /home/mmoser/archive/cstuff/GNU-Linux-2.6.38-8-generic-i686/test/tst
fact
test.c:38
</pre>

### How does it work?

Compiler option -finstrument-function is usually used for building profilers; for each function the compiler inserts two function calls; on entering the function it calls \_*cyg\_profile\_func\_enter on leaving the function it calls*\_cyg\_profile\_func\_exit .
The tool implements these two functions; on entering an application function the tool records the function return address, and when it is time to leave this function it checks if that address has been modified; if that is the case then we know that the stack was smashed here right by this function.

In more detail: For each application thread a stack data structure is maintained and referenced by a thread local storage entry. For each function call this stack keeps an entry; this entry contains the function return address, the function frame pointer address and the value received from dereferencing the frame pointer address. Typically the value obtained from dereferencing of frame pointer address is the return address from the called function. The stack entry is created when \_*cyg\_profile\_func\_enter is called. When the function is left and*\_cyg\_profile\_func\_exit is called, we check that the value obtained from dereferencing of the frame pointer did not change. If it actually did change, then we know for certain that the stack was overwritten, just right now here by this function. We also have the accurate application stack trace, just by looking at the entries in the stack data structures. So when an error occurs, the return address of all functions is printed to standard error, in addition to that, we print the addresses of the text segment for all shared libraries + executable. This information is then converted (offline after the crash) to symbolic names by the readerrors.pl script ; we do not want to do this at crash time.

The tool keeps entries that shadow the application stack - hence the name StackMirror. Of course this shadow stack can also be overwritten by some pointer gone wild, but this is much less likely than occurrence of a buffer overflow error. Another limitation is that we only return address of the current function is checked. In some complex example an application function call could smash the return address of a calling function; we do not check for that here, (though an even more paranoid version of the tool could do that). We will still get there eventually, as the stack unwinds.

.h3 Other limitations

This library makes an effort to recover from situations with [setjmp / longjmp](http://en.wikipedia.org/wiki/Setjmp.h) trick; this function is a non local form of goto ; it undoes the stack up until some remembered levy.
However there is one trick that we can't cope with [makecontext / setcontext](http://en.wikipedia.org/wiki/Setcontext) If the application decides to do its own cooperative multitasking / user level threading or if it needs co procedures then all our efforts here are toast.
Note that it's becoming something of a habit here - for every trick find when it breaks ;-)