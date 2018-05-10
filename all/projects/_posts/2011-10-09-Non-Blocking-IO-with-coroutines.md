---
layout: post
title:  Non blocking IO with co-routines
---

{{ page.title }}
----------------

<p class="publish_date">
9 October 2011

</p>
This project extends the co-routine library and so that it will be useful with non blocking networking.

There is the long standing debate on how to write a server, the basic choices are

-   The server maintains an operating system thread per network connection connection.
-   One operating system thread can handle multiple network connections. The thread watches a set of non blocking connections for 'read events'; Every now and then a particular connection will receive new data over the network, this constitutes a 'read event'; The connection has to handle the read event by consuming all available data, this way the the read event shall be cleared; when a response is ready, it is sent over the network connection, it may happen that the response is larger than what can be transmitted immediately; in this event the send system call fails, but indicates that it can be resumed later on; the connection must now register interest in receiving the 'write event' - once received it will signal that more data can be sent over the network connection. Once all data has been sent, interest in the 'write event' must be cleared.

Now implementing non blocking servers is tricky; usually there is a 'event loop' or 'Reactor' component that handles the details of watching network connections for events; now when a new event must be handled a callback functions is called by the 'event loop'/'Reactor'.

As stated earlier in the co-routine article, chopping up the server logic into a state machine that fits the callback functions can turn into many bugs, sweat and tears. So this project integrates the [co-routine package](http://mosermichael.github.com/cstuff/all/projects/2011/10/05/Co-Routines.html) with the [libevent](http://libevent.org/) event loop.

Here are the abstractions that make up the EVCOROTHREAD library

-   EVLOOP - the event loop / reactor, once this object is initialized, the EVLOOP\_run method is called, from this point on the event loop will continuously listen for io events and service io events.

<!-- -->

-   EVTHREAD - the co-routine thread that handles one or more network connections. The application can also explicitly create a EVTHREAD, a co-routine thread owns one or more EVSOCKET objects, when the co-routine thread exits, then all EVSOCKET objects owned by it are closed. The subroutine thread can call services of EVSOCKET object, while it is running.

<!-- -->

-   EVSOCKET - the network connection, each network connection belongs to an EVTHREAD co-routine, this objects has methods to connect / read / write a EVSOCKET object; when each one of these methods blocks - that means it cannot be completed immediately, then the current co-routine thread is suspended, and control returns to the event loop thread; the event loop will then pick another co-routine thread where one of its sockets have a pending event, and run it.

<!-- -->

-   EVTCPACCEPTOR - the object that accepts new incoming connection and wraps it into a EVSOCKET object; a co-routine thread is created per connection and is started immediately.

### The abstractions in more detail

In this section each class will be covered in more detail, and a TCP echo server is also presented in the process.

#### Initializing the event loop

    01: #include <corothread/evthread.h>
    02:
    03:  STACKS stacks;
    04:  EVLOOP *loop;
    05:  
    06:  CTHREAD_libinit() 
    07:  
    08:  STACKS_init( &stacks, 100 , 5 ) );
    09:
    10:  loop = EVLOOP_init( &stacks );

Line 6 initializes the co-routine library; line 8 initializes the allocator of co-routine stacks; here a limit of 100 co-routines each with 4 usable memory pages each can be created; there cannot be more than 100 co-routine threads serviced in this example, this is a convenient way to impose a limit / throttle the number of connections that can be created ; very important feature.
Line 10 creates the event loop object - the stack allocator is passed to it.

#### Creating socket listener for incoming connections

The EVTACCEPTOR class encapsulates a passive socket that is listening for incoming connections.

    01:  EVTCPACCEPTOR *acceptor;
    02:  IPADDRESS addr;
    03:  SOCKADDR saddr;
    04:
    05:  IPADDRESS_any( &addr, AF_INET );
    06:  SOCKADDR_init( &saddr, &addr, 9999 );
    07:
    08:  acceptor = EVTCPACCEPTOR_init_ex( loop, &saddr, 30, echo_thread_factory, 8192, 8192 );

Lines 5 and 6 initialize the socket address that will listen for incoming connections at any interface on port 9999.

Line 8 creates the listener object - EVTCPACCEPTOR .
the first parameter loop is a reference to the event loop
parameter saddr is the address that will be listened at
30 - the sizeof the backlog parameter passed to listen system call.
echo\_thread\_factory - once a new connection has been accepted, this callback function is called ; it will determine the thread procedure of the newly created thread, and will also create a context pointer that holds the data of the new thread procedure.
8192, 8192 - respectively the size of the socket receive and send buffers; a value of -1 says that the default size is not changed.

    01: int echo_thread_factory (int fd, EVTHREAD_PROC *proc, void **ctx )
    02: {
    03:   *proc = echo_thread;
    04:   *ctx = 0;
    05:   return 0;
    06: }
    07:
    08: void echo_thread( EVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx)

the echo\_thread\_factory sets the thread procedure value to echo\_thread. The context value is set to 0, meaning that user\_ctx parameter of echo\_thread will be zero.
If echo\_thread\_factory returns non zero value, then the accepted socket is closed immediately.

#### The co-routine thread

Once a new connection has been accepted, a co-routine is started and the echo\_thread procedure is invoked.

    01: static struct timeval TIME_OUT = { 3 , 0 };
    02: 
    03: void echo_thread( EVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx)
    04: {
    05:   char *buffer;
    06:   int rt;
    07: 
    08:   buffer = malloc( BF_SIZE);
    09: 
    10:   while ( (rt = EVSOCKET_recv( socket, buffer, BF_SIZE, 0, TIME_OUT)) > 0) {
    11:     if ( EVSOCKET_send( socket, buffer, rt, 0, TIME_OUT ) != rt ) {
    12:       break;
    13:     }
    14:   }
    15: 
    16:   free(buffer);
    17: }

The echo\_thread procedure receives the following arguments

-   thread - pointer to EVTHREAD the current co-routine thread.
-   socket - if a socket is initially attached to the thread, as is the case when receiving a new connection via EVTCPACCEPTOR, then this value points to the connection object that has been created by EVTCPACCEPTOR
-   user\_ctx - pointer to user context as returned by echo\_thread\_factory procedure.

At line 10 we are actually reading some data from the connection. EVSOCKET\_recv is the function to do it; it also receivess a TIME\_OUT (struct timeval) parameter that denotes the read timeout.

EVSOCKET\_send sends the return back via the connection, note that it too receives a timeout parameter.

Note that the buffer used here is allocated from the heap; great care should be taken with stack allocations, especially with 32 bit code, the stack is usually much smaller than what you would expect of pthread threads; (see the co-routine article for more details).

#### delays and timing

    01: void evthread_proc( EVTHREAD *thread, EVSOCKET *socket, void *user_data ) 
    02: {
    03:   struct timeval tv;
    04: 
    05:   tv.tv_sec = 3;
    06:   tv.tv_usec = 0;
    07: 
    08:   EVTHREAD_delay( thread, tv );  
    09: }

EVTHREAD\_delay will pause the current co-routine for three seconds; during this time other co-routine threads will be active.

### Download instructions

Get [My stuff](https://github.com/MoserMichael/cstuff)
cd build
./build-evcorothread.sh
(For platform specific stuf ./build-coroutine.sh ARCH=x86\_64)
For tutorial see <root direcotory>/corothread/tutorial

Set inlclude search path to <root directory>

Add the following libraries
-lcutils -lbutils -lnutils -lcorothread -levcorothread -pthread

If one wants to use shared libraries add
-lcutilssh -lbutilssh -lnutilssh -lcorothreadsh -levcorothreadsh -pthread

### limitations of this approach

Co-routines can simplify development of non blocking servers; however there are situations when this technique can't be used. Note that for a connection we will either receive data (SOCKET\_recv) or we will be writing data to it (SOCKET\_send) ; we can't do both at the same time. So you will not be able to do applications that stream data in both directions - from server to client and from client to server.

### what comes next

In the next part of this series we will build a small HTTP server, and do so with three paradigms

-   Operating system thread per connection
-   Co-routine thread per connections
-   regular libevent - multiple connections per thread with event callbacks / Reactor pattern.

The HTTP server will be embeddable - an application program will be able to run the WEB server as a library. It will then be interesting to study performance of these three models.
Apart from that it will be a good tool in order to test and debug the library.
