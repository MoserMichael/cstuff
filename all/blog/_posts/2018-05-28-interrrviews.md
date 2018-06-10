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

Interesting to note that in some companies the design questions are harder than the coding questions!

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

Features of DRM system:

* Main principle: the system specifies how media items can be used and who can use them.
* Examples: DRM service for books (can print, can copy, can display); distributed book can only be viewed on a specific book viewer device.
* access restriction is usually achieved by encrypting the content, so that it cant be used without decryption key, where only the specific customer device has the means to decrypt the content.

Entities:

* there are media files M1....Mn that are being sold, more exactly access to the file is being sold.
* an owner of a device Di can purchase access rights for a media item Mj.
* only the device of the client can access the purchased media item	
  There are devices d1... dn.
* the purchased media item also defines usage scenaroi for the item (for example ebook has verbs like 'can print', 'can copy', etc).
* These ends are achieved by passing a Ticket - an encrypted entity with the definition of rights + symetric key for decryption of media file; this Ticket is tied to the identity of the client device (also by means of encryption)
 
* there is a content server that 
    * creates Ticket object (per sale)
    * prepares media files so that they can be used with a sale 


Processes

there are media files M1....Mn

each media file is sold multiple times, must be encrypted, but don't want to do that for for each sale - therefore media item must be 'prepared' : a symetric encryption key k1 is generated per media file and the media file is encrypted with that key all this data is stored on the DRM server.

E(k1)(M1) - the media file content will be distributed in encrypted form. - also stored on the DRM server.

`So we have a table := <media item id> <description> <encryption key> <ref to encrypted file>`

what about the symetric key? can't send the encryption key as plaintext (would allow everyone to use the file). so there is a trick that must
bind it to the identity of the device.

There are devices d1....dn
each device has a Private key d1-Priv1.
The private key is established when the device is registered.
It ensures the identity of the device

`Device table := <device id>, <device private key-stored at device>, <device public key - used by DRM server to prepare ticket>`

The server: once a media item is purchased for device d1 - the server looks up the PubKey(d1)

`MessageToDevice := RsaEnc(PubKey(d1)) (DRM-access-token + k1) . E(k1)(M)`

- only the device must decrypt the access token + k1.
- now the device can decrypt the media file.''

--- 

### Me: design an NoSQL like cache 

* hosts in memory key to value maps / does the server host multiple instances of hash objects? (most likely)
* Types of maps
    * hash map! problem: time to rebuild map when it is filling up (must specify capacity & load factor)
    * Btree index? 
    * Balanced search tree?
* multiple map instances (schema or dynamic create request?)
    * createMap : (numBuckets, kindOfMap) -> tableID # for create request
    * findMap : tableName -> tableID # for schema

* data model: a map
    * lookup: tableId, key -> Maybe value 
    * insert: (tableId, key, value) -> Bool
    * atomictestAndSet: (tableId, key, oldValue, newValue) -> Bool

* types of key: (string? other basic types?)
* types of value upon insert: (string?, sets?, list?, reference to other map instances?)
    * type enforcement? (in this case type must be specified in type definition of table)
    * can we modify value in place? (while entry is within a map?)

Questions on data model:
* map: multipmap? single key map?
* different kind of hashes? (open addressing bucket hash)? (probably not)	

Operational requirements:
* what is the kind of external interface of the server? (RPC, Http requests, UDP requests, in process)
* hash needs to be rebuild when it becomes full (near full); object may not be available when this is happening (but server must remain responsive)
* expiration of old keys; throw out stuff that has not been accessed for some time
* cleanup policy a property of a hash object? - background task/check upon access/upon running out of space

Enterprise usage 
* redundancy: duplicate sets of keys among servers (so that if one server fails there will be a backup copy)
* data set partition: ability to add servers to serve a larger data set (larger than can be handled by a single server).
  partition the key space among servers (sharding)
* load balancing

Server model:
* event based


## performance evaluations

Some data items that can be useful during discussions of performance:

Table of performance (relative) - they like you to memorize the key facts for performance evaluations:

(also: always remember your big O stuff you learned in school!)

https://blog.codinghorror.com/the-infinite-space-between-words/

* 1 CPU cycle			0.3 ns	 (ns - nanosecond = 1/1000.000.000 second)
* Level 1 cache access		0.9 ns	
* Level 2 cache access		2.8 ns	
* Level 3 cache access		12.9 ns	
* Main memory access		120 ns	   
* Solid-state disk I/O		50-150 μs (μs - microsecond = 1/1000.000 second)	
* Rotational disk I/O		1-10 ms
* Internet: SF to NYC		40 ms	
* Internet: SF to UK		81 ms	
* Internet: SF to Australia 	183 ms	 (millisecond = 1/1000 second)
* OS virtualization reboot   	4 s	
* SCSI command time-out		30 s	
* Hardware virtualization reboot	40 s	
* Physical system reboot	        5 m	

Main takeaways: 
 
* L3 cache hit is ten times slower than L1 cache hit; 	
* Main memory access 10 times slower than L3 cache hit
* SSD disk access is 10-50 times slower than main memory access (the memory bus that talks to external devices is of realy low priority) 
* Spinning disk acccess is 100 times slower than SSD disk.
* Local network access is about the same time as spinning disk (?)


## Service oriented architecture

there are a lot of Service oriented architecture companies, so some exposure to SOA design patterns may be advantages: the problem - unlike the Gang of Four book (OOP design patterns) there is no one defining books, instead there are lots and lots of patterns that do mostly obvious stuff and no standardized vocabulary of design patterns, Still there are some principles:

* again: always start your discussion with a primitive solution that consists of a series of SQL like tables, then try to scale it up.
* messages should be indempotent (that means stateless as far as possible). State is bad because handling connections is complicated, and load balancers need to do sticky sessions (route related requests to the same machine), etc.
* scaling: tricks for horizontal scaling like [sharding](https://en.wikipedia.org/wiki/Shard_(database_architecture))
* don't forget to put in load balancers for high availability
* when in doubt: delegate to another proxy (lots of small services win).
* [eventual consistency rules](https://en.wikipedia.org/wiki/Eventual_consistency)
* wherever you can put in a redis in memory cache that is fed by multiple related data sources.
 
  
--------

Some important papers that can help to understand the thinking behind these large systems (hint: it helps a lot to understand some basic infrastructure components, as these are providing the terms and definition used later on, also it explains the context of things and operations)

## Google file system 

http://static.googleusercontent.com/media/research.google.com/en//archive/gfs-sosp2003.pdf

### Principles

* done as user mode library (not in kernel); does not do POSIX file semantics (even can have read from stale regions, temporarily - no operations on directory tree - it is maintained implicitly ... read on)
* components fail frequent; resilience and monitoring are very important (commodity hardware) must do replication.
* dataset: large files (not many small files); this means block size and IO size must be tuned to this (no optimizations for small files!)
* mostly appending files; almost no overwrites of existing data.
* most read operations are sequential (read 1MB or more) (because most programs in google are data analysis / passes on the whole data set); (some reads are from random offset - small in size : applications often batch and sort small reads - to avoid performance problems (minimize seek times?)) 
* caching is not important - (that doesn't help with sequential access).
* how do application considerations influence priorities? - they added 
 append so that multiple clients can append simultaneously (for map/reduce, apparently), consistency model also designed per requirements of applications 

Services

* files are subdivided into equal sized chunks (64 mb) identified by 64bit handle. and multiple replicas of a chunks are maintained.
* file: operations create/delete/open/close/read/write
* snapshot create (fast copy of file or directory)/record append (allow to append concurrently - atomically)
* file renaming atomically (as there is one master server that holds all metadata)

### Architecture:

Actors:

Master - holds metadata
Chunk server - holds chunk data (chunk is fixed sized range of 64MB)
Client - talks to Master to get lease info, then with that info does IO on chunk via Chunk server

#### master 

* single instance
* has metadata on all files hosted (file names, subdivision into chunks, where chunks are stored (set of chunk server addresses));
* each file is divided into fixed sized chunks (64 MB size), each chunk is identified by a 64 bit chunk id
* one master instance! no data is read through master (so that it does not become bottleneck, only metadata is handled); 
* all metadata in memory (no paging).
* master periodically checks each chunk server for status (HEARTBEAT messages - the response includes which chunks are hosted by the chunks server instance (that makes relatively big messages!))
* master handles replication of cached copies (of chunks among different chunk server instances).
  (may start to re-replicate chunks upon to meet high loads of particular chunks)
* master creates log of metadata changes (so that it can restore its state in event of master server crash)
* the logs do NOT have info on location of chunks (on what machine a chunk is hosted); The master asks the chunk servers on start-up and gets this information through HEARTBEAT checks (can't store this info persistently in master - a chunk server may get corrupted disk, or other permanent failures) chunk server join and leave the cluster anyway.
* the metadata record log is replicated on several machines (global consistency!); user gets ack on request only when the log for that change has been stored persistently - in all replicas.
* when log grows to large they put in a serialized dump of metadata (BTREE, this entry is called checkpoint) and start with the logs after that (deltas relative to checkpoint)
* they do not block updates upon metadata flush (writing of checkpoint); if metadata changes need to be done while checkpoint is being created and written, then this change is written into separate log file instance for this purpose!
  if incomplete checkpoint transaction then the tail is gathered from such a separate log file instance (but how is this temporal log file merged back into the main sequence when checkpoint has been completed?)
* server cleans out stale regions (each region info returned in HEARBEAT has version tag - can have region that becomes invalid after update (update completed while a chunk server was down)
* HEARTBEAT even has checksums on the data and tries to identify corrupted data so that it must replicate bad instances! (says so in 2.7.1) (? Limited guarantee - CRC checksums can go very bad with zero runs?)

#### chunk server 

* hosts chunk data 
* multiple computers that are communicate with master. 
* chunk server hosts several chunks, a client communicates with chunk server to read/modify data in a chunk. 
* responds to heartbeat messages sent by master to check chunk server status and send status of chunks)

#### Client 
* opens file with master; 
* obtains temporary leases on chunks from master (chunked = offset / CHUNK_SIZE); IO on chunk goes to chunk server;
* no file data is cached by client, client caches metadata (what are files? where is the chunk?) for a limited period.
* upon READ - translates offset to chunk id (= OFFET / FIXED_CHUNK_SIZE) - calls master for lease on chunk (and possibly several following chunks.
* the server replies with location of all chunk replicas; the client chooses the nearest replica instance for read access.

### Consistency model 

(now it gets complicated)

Consistent region := all clients see the same data on all replicas (failed mutation makes region inconsistent)

Defined region := (upon modification by client A) region must be consistent and all clients saw the modifications of client A (problem: concurrent modifications leave a region defined but not consistent - writes from different clients become intermingled)

? didn't they just say that only one client can get a write lease at a time?

Types of modifications:
* Write - the application says what offset within a region will change
* Append - an atomic 'record' append (? says at least once?) the offset of the write is determined by GPFS
* Regular append - a write at the end of the file (at offset that is believed to be the end of file)

A write has to be copied among all chunk servers to become 'Consistent'  (? When is this replication done? upon each write or is there an explicit action like 'release lease' that triggers this action?)

Now each region instance has version tag; (this is also used to deleted 'garbage'/'stale regions' that are out of date (chunk server down while another party modified the chunk) (? So the tag must be if hierarchic nature to detect such situations? not just a version number?)

Problem: client may temporarily read from stale replica (this problem is not solved, but read leases are temporary so that the problem is not quite as bad, at least stale replica can't get modified)

HEARTBEAT even has checksums on the data and tries to identify corrupted data so that it must replicate bad instances! (says so in 2.7.1) (? Limited guarantee - CRC checksums can go very bad with zero runs?)

Can have data loss if all replicas of a region are bad, but system identifies this situation.

### write flow

* for a file/chunk the client gets a write lease from server (granted to one client exclusively)
* server lease response includes list of addresses of all chunk servers that holds the chunk
* client writes the data to all chunk servers (that hold this chunk id); chunk servers buffers this data (no write yet - doesn't have serial id)
* when all acks are in: send write request to master server
* master assigns serial id for this write; (master log records written - apparently) - master sends write request to all chunk servers (with serial number)
* chunk servers respond with write status; the status is returned to the client; the write is considered failed by the client if at least some chunk servers had errors while writing the data to disk. In this event the write is repeated with the current lease (for a couple of times); If it failed for all retries then client retries the write op from from the beginning.

Problems: write that cross chunk boundaries - are done as separate IO requests (each can fail, servers fail), so the file may be left in interesting global state (consistent but undefined)

### write flow for record append

Now record append may have a situation where some chunk servers failed, in this situation the client does some record padding and retries the write at a different offset within the chunk; (padding may be up to a third of chunk size! - if the record is near the end of the chunk). If write crosses chunk boundary (tricky cases reported by some chunk server instances) then part of the data must be rewritten in the next chunk (wow, application that uses this stuff must be really complex)

### communication 

Actually the client is writing the chunk data to chunk server S1, S1 sends the data to chunk server S2 .... (each one chooses the closes neighbor (on the same link preferred, or smallest number of in-between switches). this saves bandwidth.
(data is pipelined, S1 forwards to S2 as the data is received!)

### snapshots 

copying files just creates a reference (comparatively fast, still involves writing record to log), the actual chunk data is copied upon modification (copy on write)
Has to do the following:
* invalidates all outstanding leases (next interaction with the master will give it a chance to cope)
* writes record to log

### Master operations 
many gory details here
#### namespace management
GFS has filenames like this aaa/bbb/ccc/ddd.file - it applies prefix compression, so that that common prefixes are represented as tree nodes, each tree node has a read/write lock.
Any operation (like obtaining a lease) needs to get a lock on every component of this implicit tree (therefore can't create name component(aka directory) while a file of the same name is being written)
#### where are new replica of chunks placed? 
policy must maximize both bandwidth utilization and reliability.

* Can't place all replica on the same network link (rack) - if the switch is down then all replica are not available.
* but some are on the same net link (this maximize bandwidth because data is routed preferably between instances on the same link)

When creating new chunk:
* choose a machine with low disk utilization (to even things out)
* limit number of recently created chunks on same machine (to prevent hot spots)

Re-replication: copy existing replicates when some replicated data has become corrupted and we need to get target number of replicates.
* if a chunk has lost two replicas than it is in higher priority in the queue of pending replications than one that has lost one replica
* chunks belonging to recently modified or active files is in higher priority for replication.
* chunks 'blocking client progress' are in highest priority if replication needed.
Also rebalancing: moving chunks with aim of evening disk utilization/improving network utilization/



#### space reclamation

when file is deleted
* master writes metadata record
* space is not reclaimed immediately! chunk server leaves the chunk data (may not be read via new leases - as the master has no metadata); chunk data is renamed to be a 'hidden file'
* space is freed by 'garbage collection': after 3 days the 'hidden file' get deleted and space can be reused.
* during HEARBEAT: in each message the chunk server reports a subset of its chunks and the status/version; the MASTER reply contains the ids of chunk that are superseded by new version numbers, or where no metadata exists (orphaned). chunk server deletes these files

### fault tolerance & diagnostics


#### fast startup

both master & chunk servers start fast. no special abnormal termination scenario - all exits are equal.

#### reliability

chunk data is replicated, master log too.

there is a shadow executable of the master that only reads logs + is ready any time. If current master crashes, then the DNS server sets its record to the shadow copy of master. (new access to master via DNS lookup)
(? still it doesn't know where the replica are - this info it gets when HEARBEAT messages arrives?)

On disk data corruption: chunk divided into 32k blocks, each block has its checksum (multi bit failures?)
chunk server checks this checksum against the checksum computed over data that is about to be returned.
write of new data is easy, overwrite of existing data needs to check the checksum of old data, overwrite then check checksum of newly written data (!)



---------------

## Map-reduce

https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf


problem: working on large data sets that are distributed among a huge set of machines: each program needs to deal with node failures and distribution of data and load balancing; that results in lots of duplicated work; Instead use inversion to have a general mechanism to solve these problems in generalized manner.

Simplified model: most computations are two-step
* perform a map function - it passes over input records and fill output multimap hash table (key can have multiple values) 
* apply reduce function - it passes over intermediate multimap and combines values with the same key into output value; this output value is passed to a Reduce function as input)

Implementation principles
* works over data stored on google file system
* the job is subdivided into set of splits (splits processed in parallel on different machines)
* one process applies the map function over a single split 
* for each split the reduce function is subdivided: reduce can be subdivided into sub sets by applying processor-Index = hash(Key) % NumSets  

Process

* input data is split into distinct sets - splits (one split is 16MB)
* one process is the master (coordinator) - it assigns an idle processing not to do a work unit (either MAP on split or REDUCE on subset of keys of a map produced by split)
* the output of the map stage is buffered in memory.
* when the map output buffer is written to disk (flushed): the output is partitioned into disjoined sets by hash(key) % R (prepare for the next reduce action)
* master then selects idle processor for reduce step - the input file has already prepared by previous stage
* reduce step: it first sorts the value set of a single key value (in order to remove duplicate values)
* reduce step each writes its own output file (why not a global output file?)

Output: multiple files (one for each invocation of reduce step); - no need to combine into a global file: most often this is the input for another map/reduce program.

Data structures

* for each task (either map or reduce) keep the state (idle, in-progress, complete), for completed jobs keep the result file (intermediate file) and its size
* master pings each worker machine, if it fails then running tasks (or completed tasks - the result is kept on local machine!) are reset to idle state and resubmitted.
* handles dependencies (reduce task depend on data task) given that the intermediate file may be gone due to worker machine failure.
* a single master process (still writes check points so that a failure will be apparent from the output file)
* bandwidth is conserved by use of GPF (google file system) feature - storage of chunks locally on the same machine as the worker process.
* backup tasks: some workers are very slow (bad disk causes long IO times/machine is loaded) so a few machines are very slow (few tasks at the end of a batch run take very long time to complete); therefore, at the end of a run they duplicate remaining tasks (often that results in significant speed up).

* the result of each map subtask is subdivided into R reduce tasks, so we have to assign the intermediate result (key, values) to a particular reduce task. This is often done by applying hash on key value (hash(key) % R = indexofreduceset)
  However, if key is URL then they may want to group all results of the same host, so they would need (hash(hostname(uri-key)) % R = indexofreduceset) as hash function (user may specify custom function)

* combiner function: may have additional step; function for doing reduce on local machine (before sending it to reduce task, to reduce bandwidth (like combining away intermediate results)

* reader: standard modes for interpreting input: key/value pairs, treat each line as input record, etc. cam specify additional optional component for customized parsing of input file.

-------

## TAO: Facebook distributed data store for social graph 
https://www.usenix.org/system/files/conference/atc13/atc13-bronson.pdf
https://www.usenix.org/conference/atc13/technical-sessions/presentation/bronson

* First they used SQL DB (mySQL) and Memcache as lookaside cache
* walking the local environment of a graph is probably faster than ploughing through an SQL table in search of every relevant table entry
  so they came up with a graph oriented database.
* TAO still uses mySQL as persistent storage, but for the cache they now have a graph abstraction; (TAO is mostly read access, infrequent writes)

### Design goals:

Optimized for reads / favor efficiency reliability over consistency


All objects are nodes in a graph; different object types - user node (each instance is a different user); checked-in-at (event type node); a comment is a node of type COMMENT.

### TAO concepts:

TAO objects are typed nodes in a graph;

TAO objects := each object has a unique 64 bit integer id, object type (otype)

Tao associations - typed direct edges in a graph (unidirectional link)

Tao associations := source object (id1), target object (id2), association type (atype)

type - determines what attributes 
a object or associations can hold (there is a schema (type definition) that defines the set of attributes + default values)

All associations have a 32 bit time value (the schema for all associations has a time value)

Object: (id) -> (otype, (key  value)?)

Assoc.: (id1, atype, id2) -> (time, (key  value)?)

## Tao interface:

* creation of new object/find & delete object by id 
* creation of link (also provides automatic creation/update/deletion of an inverse relationship instance - if inverse relationship is asked for/defined in schema)
* no atomic compare and set (they do eventual consistency)
* manipulating associations
    * assoc-add(id1, atype, id2, time, (k->v)*)
    * assoc-delete(id1, atype, id2)
    * assoc-change-type(id1, atype, id2, newtype)

### Query interface

Association list for object with id := list of edges (associations) that connect to object id, sorted by creation type

Association List: (id1, atype) -> [ a - association (edge) | anew . . .aold]	

now they got queries to ask for the Association list given that you have an id (and other constraints: like link type and optionally a time filter.

The API only returns up to a maximum of objects - if there are more than this they need repeated queries.

* assoc get( /*in*/ id1, /*in*/ atype, /*out*/ id2set, /*out*/ high?, /*out*/ low?)
* assoc count( /*in*/ id1,  /*in*/ atype)

## Architecture
* use SQL DB (mySQLl) as backing store.
* (important for such thing as backups / bulk updates)
* use shards (DB is too big); each object id includes the ID of the shard that holds it (? can't add any new shards?)
* links don't have a shard ID in their object id (they are tied to the shard of the object) links are stored on the shard of obj1 (source of edge)
  (bidirectional links may need to update two shards (if id1 and id2 are in different shards) (No atomicity: if the backward edge was not written then this edge will be cleaned up by background task).


### cache layer

set of cache servers form a tier - a tier can handle any TAO request. Individual servers within a tear are sharded (similar to the DB sharding)
cache eviction - Least recently used entries get thrown out.

For each tear there is a cache leader server (cache server that does updates to from mySQL DB
There are second tear servers (followers)  that are only in memory store, if cache miss then this one talks to leader server.

When leader fails: follows read directly from DB; for writes it sets a temporary leader (from among the followers)

when followers fail: For each tear there is a backup tier; When request to a shard server within a tear fail then request is still sent to corresponding shard server in backup tier.

Problems: consistency: an update in leader needs to send cache invalidation messages to each instance of following leader (change eventually will propagate - eventual consistency)
Each cached item has version, when leader gets modify request it sends update with version number (important: follower cache must check if its current data is not stale relative to this version number) The follower cache update must be completed before the write request returns.

Advantage: can add following leaders as much as required.

Each geographical region has its own replica of DB + leader + follower caches.
Now DB must be replicated across regions(!)
Leader server of each region can also talk directly to leader of each other region (!)
(says they have millisecond latency for communication between regions (!)


### Caching server - modified Memcache

* manages LRU (least recently used) for objects of the same size, uses SLAB allocator (set of pools, each pool of fixed size allocations)
* memory is divided into Arenas (some object+association types get into the same arena) - this is for 'better isolation: allows to have different cache lifetime for important object types)

* refrain from keeping small items in separate hash bucket; here they have a dynamic array per bucket.
* active pairs of (id, atype) are represented by a 16 bit integer index.

### MySQL schema
* all attributes of an object are in the same data column (do not have a separate table per object type)
* for associations: must have range queries to that association table has an index based on (id,atype,time)
* cache Sharding? (multiple follower caches for each Shard)

