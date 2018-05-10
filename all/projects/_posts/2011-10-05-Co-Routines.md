---
layout: post
title:  Simple co-routine package in C
---

{{ page.title }}
----------------

<p class="publish_date">
5 October 2011

</p>
This project presents a simple user mode threading library, it turns out that this concept has many names

-   [co-routine package](http://en.wikipedia.org/wiki/Coroutine)
-   [continuations](http://en.wikipedia.org/wiki/Continuation) .
-   In the Windows world this concept is also called a [Fiber](http://en.wikipedia.org/wiki/Fiber_(computer_science))
-   Also known as user mode threading
-   [cooperative multitasking](http://en.wikipedia.org/wiki/Computer_multitasking#Cooperative_multitasking.2Ftime-sharing)

It is a small wonder how so many computer science terms all denote the the very same concept. Usually a confused language is either a sign that the same concept has been reinvented several times, or that something is not very well understood, this fact is known since the times of Babylon. Lets see.

Software engineers try to create complex systems out of simple parts; each simple part implements some sequence of commands - a process of sorts. Let's look at a simple example; process (A) creates a sequence of Fibonacci numbers, and process (B) receives each Fibonacci number and computes the sum of the square roots of each Fibonacci numbers.

Now one has two choices, either process (A) and process (B) share the same stack, or they do not share the same stack.

    void make_fibonacci_sequence( uint64_t *fib, size_t num_fib ) 
    {
      uint64_t a=0, b=1, n;
      size_t i;

      for(i=0; i < num_fib; i++, a = b, b = n) {
        n = a + b;
        fib[ i ] = n;
      } 
    }

    double sum_of_roots( uint64_t *num, size_t num_size ) 
    {
      size_t i;
      double sum_roots = 0;

      for( i = 0; i < num_size; i++) 
        sum_roots += sqrt( (double) num[i] );
      return sum_roots;
    }

    int main()
    {
      uint64_t num[MAX_NUM];

      make_fibonacci_sequence( num, MAX_NUM );
      printf("Sum of roots of fibonaci numbers: %f\n", sum_of_roots( num, MAX_NUM ) );
    }

In this example both steps share the same stack, first Fibonacci numbers are computed and stored in an array by the first procedure, and then the result is passed to the second procedure that computes the sum of the square roots.

Now what if the array of results is very large and we are short on computer memory ? What if one has to know the current sum of square roots immediately upon receiving a new Fibonacci number / react to the event of a new Fibonacci number?
In this situation both processes have to share the same stack, so that each new Fibonacci number is passed to a callback function [callback function](http://en.wikipedia.org/wiki/Callback_(computer_programming))

    typedef void (*FIBONACCI_CB) (uint64_t num, void *ctx);

    void generate_fibonacci_sequence( size_t num_size, FIBONACCI_CB callback, void *ctx ) 
    {
      uint64_t a=0, b=1, n;
      size_t i;

      for(i=0; i < num_size; i++, a = b, b = n) {
        n = a + b;
        callback( n, ctx );
      } 
    }

    void fibonacci_cb( uint64_t fib_num, void *ctx) 
    {
       double *fibb_sum = (double *) ctx;
       *fibb_sum += sqrt( (double) fib_num );
    }

    int main()
    {
      uint64_t num[MAX_NUM];
      double result = 0;

      generate_fibonacci_sequence( MAX_NUM, fibonacci_cb, &result );
      printf("Sum of roots of fibonaci numbers: %f\n", result );
    }

.
This mode of doing things has a marked disadvantage: The procedure of computing the sum of roots is now chopped up into multiple calls of the callback function fibonacci\_cb ; on each call this function has to retrieve the current state of the computation from the argument 'context parameter' , perform a processing step and then update the result back into the 'context parameter'. A more complex process also must be partitioned into a state machine, the state is maintained extra in the 'context parameter'. No fun.

So one need sto have both processes running one after the other; first a number is produced , and then one wants to switch to consuming it; alas each process should be able to run in parallel - as if it were a sequence of steps that does not bother of the other process at all.

So each procedure needs its own stack. Now the same basic problem is being solved in a multitude of ways; each solution solves the problem for some specific context,

-   multi processing: Two processes are connected via a pipe; the Fibonacci number generator writes its result to the pipe; the square root adder reads each number off the pipe.
-   Multi-threading with shared state;
-   actor model - several threads communicate via known queues; there is no shared state between the threads other than the queues; all communication between the threads is strictly via the queues; each message is not modifiable.
-   Python / Perl generator functions (co -routines implemented by the interpreter run-time).

Now co-routine will simply maintain multiple stacks per procedure; the Fibonacci number generator function has its own stack, the function that consumes Fibonacci numbers has its own stack. When a new number is computed, Fibonacci number generator switches the stack to the consumer of numbers, when the consumer is ready and wants to get a new number then it switches the stack to the generator function.

There is a significant difference - with multiple threads or multiple processes it is the operating system that decides which task will run next; with co-routine the application decides when to switch between stacks.

<a name="pf" ></a>

### Performance advantages

Another motivation for co-routines is performance. Lets say we have the following example that creates 10000 operating system threads with the pthread package, where all threads are put to sleep.

With Linux (2.6.38-8) you will see that responsiveness of the system will decrease dramatically; The top utility will display a CPU usage of 30% and more and the load average (a somewhat obscure statistic that sums up the system utilization) will increase dramatically, all within a very short time.

How could that be explained? The Linux scheduler maintains several run queues - each run queue is a data structure (not necessarily a queue in that sense) that maintains each tasks (processes or threads) of a given priority class; overall there are 255 priority classes. Now on Linux a run queue holds all tasks; those that are runnable and those that are sleeping; this implies that at the time when the operating system scheduler is deciding which task to run next, it has to consider all tasks of a given priority class. That implies that the Linux kernel is probably not too good at scheduling a large number of threads;

Another issue is that with cooperative multitasking a switch of context occurs when the application decides that such as switch must be done; so a context switch is performed when one is needed; On the other hand, a preemptive scheduler has to maintain a lot of other consideration like fairness of tasks, system throughput, responsiveness of the system, etc. etc. so the decision to switch contexts will be taken at a different time; not at exactly the same spot when one is required by the task at hand.

Please note that an operating system that maintains separate queues for sleeping tasks as opposed to runnable tasks will probably be much better at the task of scheduling a huge number of threads; BSD appears to do so, alas I currently don't have access to the required hardware / software in order to do sufficient benchmarks; etc. etc.

Historically high performance systems where all done with user mode scheduled threads; CICS does so - which is of course a very important system on the Mainframe; and this system is of course still very much in use for certain tasks.

Some more obscure details on running the test that creates 10000 threads via pthreads.
make sure that the system wide limit on the number of tasks does not bother this test; you can display the current limit by running ''cat /proc/sys/kernel/threads-max"; the root/superuser can change this value at run time, one does not have to restart the system for that. An enterprise system
Make sure that the current soft limit does not limit the number of tasks that can be created; the command "ulimit -u" will display the current value of a shell;
the test application creates a short stack for each thread, this way a 32 bit process will not run out of virtual memory address space (by default a pthread stack has 10 MB).

### The CTHREAD package

The remainder of this article will introduce a cooperative user-mode threading package; which is implemented as a library of functions callable by any C program on Linux, alas portable to any POSIX compliant operating system;
This package will be named the CTHREAD package.

### How to pass a typed set of values around.

This library will be passing parameters from and to the co-routine thread, so here is a utility class in C for packaging of a typed set of values.

    #include <corothread/val.h>

    VALUES values; 

    uint8_t n8 = 250, _n8;
    uint16_t n16 = 65000, _n16;
    int32_t  n32 = 42, _n32;
    int64_t n64 = 0xBEEFBEEFBEEFBEEF, _n64;
    char *sval = "aaa", *_sval;

    VALUES_init( &values ) 

    VALUES_print( &val, "%hhu%hd%d%qd%s", n8, n16, n32, n64, sval );

    VALUES_scan( &val, "%hhu%hd%d%qd%s", &_n8, &_n16, &_n32, &_n64, &_sval );

### More on stacks

A co-routine stack consists of a continuous number of memory pages; now at the top of the stack there is a memory page that is not accessible, any attempt to read or write to this memory page will result in a SIGSEGV - access violation signal. This turns out to be a very nice trick, when the co-routine stack grows out of proportion / overflows, then one will immediately get to know that something got wrong.

There are times when an application will want a very large number of co-routines - in the order of tens of thousands; Now on 32 bit machine architectures one will choose a very small stack, otherwise one will run out of virtual memory to address;

Please note that with small co-routine stacks one should not allocate big arrays on the co-routine stack.

The CTHREADS package implements a pool of stacks, so that creation of co-routine will be very fast and will usually not even require even a single system call. With this purpose there is a STACKS reserves one very big continuous are of virtual memory addresses and carves the application stacks out of this memory are. The STACKS object has to be passed to co-routine thread creation function.

### Fibonacci example with co-routines

The same example that computes a sequence of Fibonacci numbers now by using the CTHREAD co routine package. The co-routine make\_fibonacci\_sequence now generates a number and calls CTHREAD\_yield to switch the stack back to the caller of the co-routine, and to pass the computed value to the caller.

    #include <corothread/cthread.h>
    #include <stdio.h>
    #include <assert.h>
    #include <math.h>

    #define NUM_STACKS 10000 
    #define MAX_NUM 90


    // -----------------------------------------------------------------
    void  make_fibonacci_sequence(VALUES *values)
    {
      int i, max_num;
      void *val;
      uint64_t a,b,n;

      // retrieve argument passed to co-routine - how many iterations to run.
      VALUES_scan( values, "%d", &max_num );


      for(a = 0, b = 1, i = 0; i < max_num; i++) {
        n = a+b;

        // now that a new Fibonacci value has been computed, it can be passed to the caller;
        // the next function switches stacks from co-routine back to the caller.  
        CTHREAD_yield( 0, "%qu", n );

        a=b;
        b=n;
      }

    }

    double sum_of_roots( size_t max_num, STACKS *stacks ) 
    {
      CTHREAD *th;
      VALUES *values;
      uint64_t fib;
      int i;
      double sum_roots = 0;

      // initialize the co-routine thread
      th = CTHREAD_init( stacks, make_fibonacci_sequence );
      assert( th != 0 );

      // start coroutine. pass the value of max_num to the coroutine.
      assert( CTHREAD_start( th, &values, "%d", max_num ) == 0 );

      // on return from CTHREAD_start, the return value from the coroutine is now stored in the values object.

          for( i = 0 ; i < max_num && CTHREAD_state(th) != CTHREAD_STATE_EXIT; i++) {

         VALUES_scan( values, "%qu", &fib );

         sum_roots += sqrt( (double) fib );

         // resume the co-routine thread, switch stack to co-routine thread.  
         CTHREAD_resume( th, &values, 0 );

         // on return from CTHREAD_resume, the return value from the co-routine is now stored in the values object.

      }

      // free co-routine object.
      assert( ! CTHREAD_free( th ) );
      return sum_roots;
    }


    int main()
    {
      STACKS stacks;

    // library first time initialisation
      assert( !  CTHREAD_libinit() );

      // initialize the pool of stacks - here one stack that takes five memory pages is allocated.
      assert( !  STACKS_init( &stacks, 1 , 5 ) );

      printf("Sum of roots of Fibonacci numbers: %f\n", sum_of_roots( MAX_NUM, &stacks ) );

      // release pool of stacks
      assert( ! STACKS_destroy( &stacks ) ); 

      return 0;
    }

### Download instructions

Get [My stuff](https://github.com/MoserMichael/cstuff)
cd build
./build-coroutine.sh
(For platform specific stuf ./build-coroutine.sh ARCH=x86\_64)
For tutorial see <root direcotory>/corothread/tutorial
Add the following libraries -lcorothread -lcutils -lnutils -lpthread
If you want to use shared libraries then use -lcorothreadsh -lcutilssh -lnutilssh -lpthread

### What comes next

The next article in this series will show how to handle IO events / non blocking sockets with co-routines; For this purpose a library will be presented that invokes a co-routine thread per new incoming connection; the co-routine will use blocking primitives in order to receive data from and write to the connection; when a connection blocks ; the library will switch the stack and handle an IO event with another co-routine.
