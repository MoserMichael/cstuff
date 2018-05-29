---
layout: post
title:  HTTP server with co-routines
---

{{ page.title }}
----------------

<p class="publish_date">
25 October 2011

</p>
The [previous article](http://mosermichael.github.com/cstuff/all/projects/2011/10/09/Non-Blocking-IO-with-coroutines.html) describes a co-routine library integrated with the [libevent](http://libevent.org) asynchronous event notification library, this article now presents WEBBY - an HTTP server that comes in two implementations, one that uses the co-routine library, and another one that handles each connection in a different operating system thread. WEBBY is an [embedded HTTP server](http://en.wikipedia.org/wiki/Embedded_HTTP_server) , that means it should be easy to integrate the server with an existing application, and its main focus is on creating dynamic WEB content.

There are many tools that promise to do non blocking network communication properly; most of them come with a HTTP server. There are many reasons for this, first of all doing a WEB server is a means to test the tool. The author of a tool is then the first one to discover shortcomings of said tool, the proper computer jargon for this process is [eating ones own dog food](http://en.wikipedia.org/wiki/Eating_your_own_dog_food) , and so we have a valid usage of 'dog-food' as a noun, alas it is limited to the practice of programming a computer.

Also of course HTTP is the 'little protocol that could', nowadays all sorts of application are built on top of HTTP; web services, video and audio streaming, of course it serves this web page as well; so by implementing an HTTP server one states that the tool is at least good for serving web pages;

### The many functions of a WEB server and its design by filters/modules.

The HTTP protocol consists of a set of non overlapping features that complement each other without interfering with each other, this state is called [Orthogonal set of features](http://en.wikipedia.org/wiki/Orthogonality#Computer_science) . For example the protocol has the option of [compressing the request/response body](http://en.wikipedia.org/wiki/HTTP_compression) , which does not bother the feature of [HTTP Basic Access authentication](http://en.wikipedia.org/wiki/Basic_access_authentication)

The system is therefore implemented as a [pipeline of handlers](http://en.wikipedia.org/wiki/Pipeline_(computing)) where each feature is implemented by a dedicated handler, the output of one handler is automatically used as the input to the next, following handler (very common theme with programming ;-). Each such handler is called a filter, all usable HTTP servers have this concept ([Apache](http://httpd.apache.org) and [nginx](http://nginx.org/) it
is called a 'module')

Each filter must be able to alter a particular aspect of the HTTP request or response, so each filter will have a chance to alter one of the following events

-   The event that a [http header has been parsed](http://mosermichael.github.com/cstuff/docs/html/webby/a00019.html#ga27407d3eb9a137407a8432603325b5b0) ; an object representing the parsed [HTTP request](http://mosermichael.github.com/cstuff/docs/html/hutils/a00024.html) header in memory
-   The event that [http request data has been parsed](http://mosermichael.github.com/cstuff/docs/html/webby/a00019.html#ga07fb25acde5fb71c0bbcad748331e462) ; zero or more blocks of data that make up the HTTP request body (without [chunk headers](http://en.wikipedia.org/wiki/Chunked_transfer_encoding))
-   An event that signals that [HTTP request has been parsed completely](http://mosermichael.github.com/cstuff/docs/html/webby/a00019.html#gaf6f5ee2b34c30cd71ddeccf8a8cc98c8)

These events make up one direction, the processing of the request, at some stage a filter will generate a response, which will traverse the same sequence of filters backwards. Now each filter will have a chance to alter one of the following events

-   The event that a [HTTP response has been generated](http://mosermichael.github.com/cstuff/docs/html/webby/a00019.html#gace404adfffdcfe00660631c1b74d141e) with an object representing the [HTTP response header](http://mosermichael.github.com/cstuff/docs/html/hutils/a00025.html)
-   A response data block or chunk [has been generated](http://mosermichael.github.com/cstuff/docs/html/webby/a00019.html#ga3fc7467c414925853cc55e085bef5ddc) , one or more blocks of data that make up the HTTP response body, or a single object representing a chunk of the response, in case that chunked transfer encoding is used for the response.
-   An event that signals the [end of the HTTP response](http://mosermichael.github.com/cstuff/docs/html/webby/a00019.html#ga3424cf82a3e72ff1a900de9383c52e61)

The more or less detailed documentation of this interface can be [found here](http://mosermichael.github.com/cstuff/docs/html/webby/a00019.html).

Now for each event, the filter object will process a different callback function. Many frameworks such as [Apache Mina](http://mina.apache.org/) or [JBOSS Netty](http://www.jboss.org/netty) will ask you to implement a generalized 'protocol layer' that passes around a generalized message object, this object has to fit all cases (The [Scala](http://www.scala-lang.org/) language even has the construct of the [case class](http://www.scala-lang.org/node/107) very big abstractions come out of it). I think that this is an over generalization. One of the virtues of doing C with classes is that it encourages the programmer to choose an abstraction that is not too far removed from the actual task, this is of course the result of the programmer being reluctant to type too much, because it is not too easy to declare a new class ;-)

### Virtual hosts.

Each HTTP/1.1 request includes a Host header, the value of this protocol header is a domain name + optional port number; this allows the web server to implement 'virtual hosting';

WEBBY can work with two scenarios:

-   No virtual hosts are defined, all filters form one processing chain; in this case the server checks for the presence of the Host header, but accepts any values.
-   There are virtual hosts defined. In this case the Host header has to mach one virtual host definition. Also each virtual host can have a different sequence of installed filters.

### The concept of the Servlets

So where is the actual response generated?

A Http response can be generated by a filter, but many WEB servers offer a simplified interface for generating (dynamic) content, a Servlet object has one callback that receives the request header object, and all of the request data.

Java Servlets have the servlet; IIS has the ISAPI application ; and WEBBY will have its own HTTP\_SERVLET interface.

The interface of the HTTP servlet is [documented here](http://mosermichael.github.com/cstuff/docs/html/webby/a00020.html) , an example usage of WEBBY the web server with an example Servlet [is here](https://github.com/MoserMichael/cstuff/blob/master/webby/webbyimpl/test/embed_example.c)

### WEBBY as a standalone application / dynamic loading of filters and Servlets

WEBBY can also exist as a standalone application; In this case it will load the filters and servlets dynamically from shared libraries. The shared library is expected to export a function that initializes the HTTP\_FILTER object and returns it from the shared library. Filters are supposed to export the load\_filter function and servlets export the load\_servlet function. An example of a servlet as a shared library [is here](https://github.com/MoserMichael/cstuff/blob/master/webby/webbyimpl/test/helloworld.c)

The web server executable:
webycoro - the web server implementation with co-routines
webythread - the web server implementation with pthreads

Both programs have the same command line, which is [explained here](https://github.com/MoserMichael/cstuff/blob/master/webby/webbyimpl/webbymain_blurb.txt)

### What comes next.

Plan A) Obviously I should do load testing that compares the performance of pthread implementation versus performance of co-routine server with events. Unfortunately this requires access to some costly hardware, which I currently do not have; If somebody can kindly donate 5000-10000$ to me for this noble purpose? So for the meanwhile this task has to wait.

Plan B) Of course an area where the co-routine server would shine is web applications with database back end access. This would require some hacking - the database access libraries for various SQL database clients (namely Postgress/Mysql/Oracle) would have to be adapted to suite the EVCOROTHREAD package. Then, further this would have to be integrated with an embedded scripting language, [Lua programming language](http://www.lua.org/) would fit the purpose. Anybody wants to server 10000 connections simultaneously, that do all require SQL queries/updates ?

So paradoxically such a system would have commercial potential. The only viable business model here would be dual Open source / commercial support option; The open source component would have the purpose of establishing trust / generating interest and is absolutely required. The problem of course if anybody would be prepared to shell out money for an HTTP server / application server in 2011 - given all the free solutions available; of course scalable back end access would be a strong selling point here.

Technically there is a risk here, does the [Lua](http://www.lua.org/) runtime / LUA JIT allow for suspend / resume model or for low overhead JIT per request ? The [Erlang programming language](http://www.erlang.org/) is supposed to have low overhead interpreter instances, but a functional programming language will probably not be too popular with developers, and also Erlang does not seem to have a JIT. Is Lua managing with small stack sizes, or does it need a huge stack?

What if non blocking event driven server does not have a sufficient practical advantage over connection per operating system thread model ? So at least some level of stress testing is required, before plan B is pursued. One thing I do strongly dislike of is to speculate on the subject of performance; so that's exactly the reason why WEBBY has multiple implementations.

### Still further deliberations on the same subject

I guess Lua would not be the right language ; for whatever reasons it is popular with embedded applications and games; also very special hacker applications like nmap and wiresharks use Lua as a scripting language. The rather conservative world of WEB development sticks with Java; Another reason is that Lua already has its own co-routines; and we have little to offer here.

There are of course, some lesser used but still wide spread tools like PHP, ColdFusion or ASP. One possibility is to implement such a niche system from scratch and to market a commercial solution for any of these tools; Such a product would receive only limited attention of a niche system, and in the long term it would probably be futile to invest in a fading platform; alas people would probably be more prepared to spend money on it.

Of course we could do a version for Java; I guess the most general solution here would be to create a customized JVM starting with the OpenJDK sources; the JVM would introduce the concept of co-routine threads;

So a regular Java thread will be able to host multiple co-routine, cooperatively scheduled threads;
A regular thread object will then have a field that says if a co-routine is currently executed.

Now when the application may well call one of the java monitor synchronization primitives of class java.lang.Object wait/notify/notifyall; if a co-routine thread is currently executed, then a special version of these function has called, one that does not block the co-routine thread completely, wait would yield control, notify would resume the co-routine; of course both wait and notify have to be called from a co-routine that is invoked as part of the current operating system thread.

Also specialized versions of java.net.Socket and their nio counterparts will be implemented, those will suspend the current co-routine thread when the socket operation blocks; just like the [EVCOROTHREAD](http://mosermichael.github.com/cstuff/all/projects/2011/10/09/Non-Blocking-IO-with-coroutines.html) package does.

This is a fine design, but I am not quite sure if I want to donate my effort to [Mr. Ellison](http://en.wikipedia.org/wiki/Larry_Ellison) for free, without charge, after all the guy already has a [very big yacht](http://en.wikipedia.org/wiki/Rising_Sun_(yacht)) in the world.