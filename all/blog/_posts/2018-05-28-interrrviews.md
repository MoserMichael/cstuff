---
layout: post
title: On the dreaded subject of programming interviews

---

{{ page.title }}
----------------

<p class="publish_date">
28 May 2018

</p> 


### Hiring rituals of high tech companies

Software companies invest a lot of thought into the interview process - it is supposed to determine the character of the workforce and by extension to shape the culture of a company. On the receiving side: the nature of the questions may give the interviewee some hints about the company / work culture at the place; The way they ask questions may also provide for some hints on the character of an interviewer; However, be warned: this guesswork may also lead to exactly the wrong conclusions - for example in larger companies one may not end up working for an interviewer, however a lot may depend on the personality of your future supervisor.

In any event: beware, all the information in this text should be taken with a grain of salt: people are imperfect - it may be that your interviewer is a swell person and the job is the job of your dreams; it's just that they don't know any better. In any event take the job that pays your bills and don't believe any advise you read on the internet.

Approaches: there are potentially different priorities at evaluating prospective employees
* testing knowledge
* focus on testing particular skills needed for the job
* some say that skill/knowledge is secondary, but general problem-solving skills + soft skills is primary.
* background - how do colleagues assess your performance at previous companies?
* some want all of the above, and want it now (then they might become upset there are very few qualified enough to fill in vacancies)
 
I think that the focus can either be on testing for knowledge/skills or on testing for problem solving/soft skills + background checks.

Now of course there are lots of different means to test a candidate -

## Coding questions

### whiteboard interview: simulation of coding and code review processes

Basically this is a reasonable simulation of the work process - the candidate gets a problem that is not entirely defined, the first task is to clarify details (they listen and evaluate that); Then you have to explain the algorithm and define some examples where edge cases are worked out, then you need to code the stuff (while talking and answering review questions); finally you need to make sure that the darn thing works (code review) - explicitly they watch for how you react and incorporate feedback, etc.

Often the questions do involve coding a task with a graph or a tree; some recursion and some dynamic programming; I guess this is designed to give people out of university a good chance, and in order to test how good a person is at learning stuff (relearning stuff if you are long out of college).

These questions can be solved by applying the following pattern:
* clarify the conditions
* make up several examples (example need to be not too trivial - so that edge cases appear)
* come up with a naive recursive solution
* dynamic programming problems: the recursive solution can be refined by applying memorization (storing intermediate results and using them where appropriate)
* if requested: find an iterative solution (trick question)
  
For me: If the interviewer wants to trick you too much during the process than that is a very bad sign, may be a personal attitude problem or speak about the organizational culture at the place.

### code us a known problem (hash table, runlength encoding, etc.) on paper and explain the result.

Attention to detail is checked - better think of the algorithm and as many examples and edge cases before starting to code, these pesky edge cases will come back to haunt you when the answer is being reviewed.

To me a question like this signals that the company has a clear distinction between designers and coders - and that the ideal worker is a coder with a limited set of responsibilities (that's also not true universally - because this way the organization has a hard time to struggle with its tasks, however there is a tendency?). One should try to figure out to what extent personal initiative is asked for at this place.

### code in an IDE environment; the tasks are well defined, and you are expected to produce the exact same output as specified within the specified time frame

That one is bad - they want to signal that we are sharp and mean and that we really know how to drive people, often these are probably not the most pleasant people to work for.

### given a code listing find some glaring problems in the code by looking at it.

Actually, this is a very infrequent question, but I think it is a very good one - it shows if a person is not just good at writing code, but also inclined to maintain an existing code base. However, it speaks of testing for existing skill rather than testing for problem solving skills, so this methodology is infrequently used.

## Talking questions

### open ended design question

First part of the interview - one is supposed to figure out the scope of the design question; key features of a system; provide for a simplified solution then refine gradually refine it.

The question is sort of a background check - one is often asked to design a system that is similar to your previous work experience, or one that is similar to the industry of the company that is hiring. Practically this favors people with detailed domain knowledge (but is not supposed to be this way).

### tell us of a successful project of yours

Now it is important to get your story ready for this task: this test communication skills; It also checks to what degree one is in tune with the culture of the place (one really needs to get some background information on the organization that you apply for),

### broad questions designed to test problem solving approaches

Hard to test these in an effective manner, often stupid open-ended questions are asked here ('why are manholes round') or you are asked to extrapolate (assess and explain how many gas stations are in the USA). I think these questions are not so good because it is possible to memorize the right answers here.

### knowledge quiz on some technical question

Often asked at screening interviews - sort of a substitute for technical background check - designed to figure out if you really had some hands-on experience with the technologies of interest?

### open ended discussion of some technical questions

this starts with some stuff and gradually gets deeper (depending on the knowledge of the interviewer - they often like to show off with their knowledge). Don't know what that says about the interviewer (I am a bit like that too ;-)

## Soft interview

### tell us about your work experience.

Very important to get your story right: concise description of you and your winning traits, followed by brief review of your jobs - then longer coverage of each job.

### simulated discussion among a group of job applicants

This is rare - designed to figure out if you fit into a leadership role - key question: who is the one who starts to take notes and starts to organize the discussion? Who is able to dominate the discussion?

### IQ tests

These are very rare - only very bureaucratic organizations do that nowadays.  I think these tests are mainly bullshit - but there are some believers left, Organizations where HR has a disproportionate deal of influence might consider this course of actions.

---
 
The reminder of this document reviews some interesting material that can be used as preparation for the interview process. Nowadays there are lots of interesting movies on YouTube, lots of books that cover the issue, lots of sites with interview questions in use by the major companies (those big three guys that are the pride of the industry and emulated by most places of interest).

### open ended design questions

There are several great guys who are offering their service as coaches on interviews, and so it goes that they offer their talks on youtube with great passion: Ramon Lopesz - his youtube channel [Success in Tech](https://www.youtube.com/channel/UC-vYrOAmtrx9sBzJAf3x_xw) is a great place to learn about design interview questions 

Talk: Success in tech: [Design a parking lot](https://www.youtube.com/watch?v=DSGsa0pu8-k)


Task: interviewer wants to know how you handle ambiguity

Q: is it a system design/class design/coding question?

Task: interviewer wants to know if you approach things systematically. Ask for details

* How is this designed? Open space - multiple buildings / are there floors / how to move between floors / multiple entrances?
* optimize to fill certain areas first? fill in floors - priorities? Display how many free spots are in a given floor?
* different sized slots? Different sized cars? Different prices for each size slot?
* once a car is let in: assign a specific spot with directions on how to get there or let driver figure out?
* premium parking spots? reserved slots for drivers with disabilities?


Important: on open ended tasks many questions? Shows your grasp of the problem and that customers matter.

Start:

four sized slots = (each different size class of vehicle: motor cycle / car / lorry / bus).

Q: Can we put a car into a slot of a lorry?


Class hierarchy question?

start with vehicles:

Vehicle base class : license plate (for id)

Four derived classes - each one representing a vehicle size class.

class for ParkingLot
	- attribute zipCode (to identify several parking lots)

`class ParkingSpot
	id
	enum SizeClass`

the service `ParkingLot::placeVehicle(Vehicle *v)` assigns and attaches a ParkingSpot from the lot

---

Talk: Success in tech: [Design a messaging service like whatsapp](https://www.youtube.com/watch?v=5m0L0k8ZtEs)

Identify features (scope):

- one to one communication of text messages.
- indication of status of message (sent/delivered/read).
- push notifications

- need a server to store messages - if receipient of message is not curently online then message cant' be delivered, so it has to be stored
centrally until it can be delivered.

- Message delivery flow:

Bob->Server<-->Alice

- Server needs to maintain a queue per client for his outgoing messages. maintained until message is delivered (for space efficiency)

- horizontal scaling (sharding) on user id as primary key.


Problems: messages can arrive out of order. consecutive messages got into queues on different machines and were fetched by the target (Alice) in
different order. (Can solve this by client maintaining an incremented message id (per user))


- how to do indications of status of message:

  Once message
 has been sent you get an ack message (sent status will be updated on client)

  delivered message -> server can connect to sender and send this indication (or can be reply message that is queued like regular message)

  read message -> must be a return message that is queued on server, is totally asynchronous

- Push notifications: can be handled with lesser priority than one to one text messages. Can be handled when server capacity is available.

Questions of representation:
- SQL table for persistence or in memory list? How to get first suitable spot? Several lists in memory or one ?


---

Talk: Success in Tech: [How to design twitter](https://www.youtube.com/watch?v=KmAyPUv9gOY)


+ not expected to design the whole frigging thing; identify a few key features and talk about them.
- ask clarifying questions; allow the interviewer to guide your search (he wants to find out about you!)

Features:
1) sending a tweet,
2) see your home timeline: with the post of those that you follow, your own posts, and a few others (that you are not subscribed too); all in
chronological order.
2a)see your user timeline: only your own posts.


Naive solution: Use relational databases (good for developing an initial model)
`User table :== <user_id>,<List followed by this user>`

`Tweet_table:=  <Tweed_id_primary_key>, <user_ref>
,<date when posted>,<text of tweet>`

Home timeleline of user u1 : `SELECT * from  TweetTable WHERE user_id =<user id followed by user u1> AND user_id =<user id followed by user u1>`
...

Problem: this is too slow, not scalable; what to do now?


Features of optimized solution:

* Know that twitter is read heavy with fewer writes (by an order of magnitude) - need to optimize for many many reads.
* Availability - no interruption in service, ability to handle peak loads.
* Consistency - need to see all tweets of others
* Priorities: care most about availability - consistency is important but this is a less important concern (it is possible that your tweets will
appear on another's home timeline with delays) aka eventual consistency.


Solution: every time a new tweet is received for user u1 the user timeline of all the users subscribed to u1.

On submitting a tweet - http requests hits load balancer; and updated timeline is stored in redis cluster (in memory database) (each cluster has
three machines - a tweet is triplicated)

Problem: users with a lot of followers will generate a lot of update action when they tweet.
Optimization: if a user has not been active for a few dates, then his timeline will not be updated/cached (first visit after break takes longer!)


Edge cases: follow up question - what are problems with this architecture?
Long replication time for users with millions of followers
Also u1 and u2 follow user u3 with millions of followers; update takes a long time so u1 is updated first; u2 sees the reaction of u1 before he
gets original tweet.

Possible solution: for user u1 with millions of followers - do not update each followers timeline; when follower accesses his timeline then
perform SQL query to get the heavy ones update on demand and merge it with each follower's timeline on demand.

Horizontal scalability: have a set of thousands of redis servers, need to know the three machines that store the timeline.
Here use sharding: take user id and apply a hash -> drive the three servers that store the timeline from the has of the user id.


Possible things to talk about: search, push notifications, advertising.

---

Me: How to design a DRM system.

Entities:
* there are media files M1....Mn that are being sold, more exactly access to the file is being sold.
* an owner of a device Di can purchase access rights for a media item Mj.
* only the device of the client can access the purchased media item	
  There are devices d1... dn.
* the purchased media item also defines usage scenaroi for the item (for example ebook has verbs like 'can print', 'can copy', etc).
* there is a content server that prepares a media items upon purchase so that it can only be viewed per device.

Features of DRM system:

* Main principle: the system specifies how media items can be used and who can use them.
* Examples: DRM service for books (can print, can copy, can display); distributed book can only be viewed on a specific book viewer device.
* access restriction is usually achieved by encrypting the content, so that it cant be used without decryption key, where only the specific
customer device has the means to decrypt the content.


Specifics:

there are media files M1....Mn

each media file is sold multiple times, must be encrypted for each sale - therefore media item must be 'prepared' : a symetric encryption key k1
is generated per media file. all this data is stored on the DRM server.

E(k1)(M1) - the media file content will be distributed in encrypted form. - also stored on the DRM server.

`So we have a table := <media item id> <description> <encryption key> <ref to encrypted file>`

what about the symetric key? can't send the encryption key as plaintext (would allow everyone to use the file). so there is a trick that must
bind it to the identity of the device.

There are devices d1....dn
each device has a Private key d1-Priv1.
The private key is established when the device is registered.
It ensures the identity of the device

`Device table := <device id>, <device private key-stored at device>, <device public key - used by DRM server to prepre ticket>`

The server: once a media item is purchased for device d1 - the server looks up the PubKey(d1)

`MessageToDevice := RsaEnc(PubKey(d1)) (DRM-access-token + k1) . E(k1)(M)`

- only the device must decrypt the access token + k1.
- now the device can decrypt the media file.''


## Service oriented architecture

there are a lot of Service oriented architecture companies, so some exposure to SOA design patterns may be advantages: the problem - unlike the Gang of Four book (OOP design patterns) there is no one defining books, instead there are lots and lots of patterns that do mostly obvious stuff and no standardized vocabulary of design patterns, Still there are some principles:

* again: always start your discussion with a primitive solution that consists of a series of SQL like tables, then try to scale it up.
* messages should be indempotent (that means stateless as far as possible). State is bad because handling connections is complicated, and load balancers need to do sticky sessions (route related requests to the same machine), etc.
* scaling: tricks for horizontal scaling like (sharding) [https://en.wikipedia.org/wiki/Shard_(database_architecture)]
* don't forget to put in load balancers for high availability
* when in doubt: delegate to another proxy (lots of small services win).
* eventual consistency rules https://en.wikipedia.org/wiki/Eventual_consistency
* wherever you can put in a redis in memory cache that is fed by multiple related data sources.
 

