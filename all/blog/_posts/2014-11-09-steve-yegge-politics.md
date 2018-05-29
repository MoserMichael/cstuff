---
layout: post
title: <notextile>Steve Yegge on software politics</notextile>
---

{{ page.title }}
----------------

<p class="publish_date">
9 November 2014

</p>
Steve Yegge [says here](https://plus.google.com/110981030061712822816/posts/KaSKeg4vQtz) that there are two types of approaches to software development.

The 'conservative' (risk averse) approach: bugs are not allowed to happen, quality comes first; Statically type checked languages are preferred

The 'liberal' approach (flexible design and speed to change come first): bugs are a fact of live so lets face them as they happen; Dynamic languages are tolerated. Are real world politics connected to this? Not significantly; the author says that more important are colleges and teachers + early (formative ?) experience as a programmer.

Software 'conservatives' say:

-   bugs must be fixed before launch (like software specs, UML modeling and unit tests)
-   limit syntax to 'safe' features ('central committee' approves style guide that forbids code generation/internal DSL, dynamic RTTI, exceptions, etc).
-   static type checking a must
-   data must be stored according to well defined schema (in third normal form)
-   no debugging features on production system (to avoid back-doors)
-   fast is better then slow

software 'liberals' say

-   bugs are not a big deal; fix them as they happen
-   fewer rules, so that there is a short learning period for new programmers (not one year like in Google)
-   don't need protection from language features;
-   code should be small (shortness conveys expressiveness / 'power' of an abstraction )
-   requirement of data schema slows down development. Schema should follow the code - when nature of data is better understood.
-   interfaces: Modeling is just guessing how interface will look like. Possibility to evolve an interface is valued (also backward and forward compatibility
-   Public interfaces should always do the simplest thing that could possibly work, and grow only as needed. '
-   System flexibility can mean the difference between you getting the customer (or contract) vs. your competitor nabbing it instead. Security and safety risks in run time production systems can be mitigated and controlled by logging, monitoring and auditing. There are plenty of existence-proofs of large systems with root-access backdoor and shells (e.g. RDBMS, online game servers) whose risk is controlled while still giving them world-class run time flexibility.
-   Premature optimization is the root of all evil. Get the code working first.

Examples of 'software conservative' companies: Google and Microsoft (very conservative)
Examples of 'liberal' companies: Facebook and Amazon

------------------------------------------------------------------------

I think this analysis is very interesting: it tries to characterize organizations in a way that is independent of the size of an organization: there can be software conservative start-ups and software liberal big mega corporations;

Now i think that its business that is pushing a companies into the camp of 'software liberals' or 'software conservatives'. A company where the user experience is important will need to evolve the UI quickly; sometimes the feature set/time to market is more important than software quality. Such a setting will lead to a 'softaware liberal' approach.

For example Facebook had to climb to the top of the very dynamic 'social' heap, this extensive growth model made them very software progressive. At an early stage it was probably very important to evolve the product speedily; the winner would need a more dynamic setting where good ideas would be adopted and bad ones discarded; Here quick turnaround of features was very important.

(well they need to stay on top of the 'social heap' in order to stay relevant, a big established platform has often other requirements, but apparently the founders are still dominant here)

Google and Microsoft both do stuff that must work reliably in complex settings, so they are not likely to experiment with dynamic languages (at least not for the 'core' product). Also both companies must preserve their relative positions, so they are more defensive and have more to loose/fear to loose their relative positions, this makes them more 'software conservative'. (both Google and Microsoft are perceived to have this one great product that everything else depends on)

Also Microsoft (or IBM) products have a strictly defined and managed product cycle; this makes for less room for experimentation; bugs have to be avoided and can't be accepted as a fact of live; Google had to be good enough to do without much customer support, so this requires a high level of quality up-front (all for different reasons, see below); strong quality requirements make them much less tolerant of bugs.

An interesting question is if a 'software conservative' company has a more complicated management structure than a 'software liberal' company.

------------------------------------------------------------------------

It is often fairly limiting to evaluate real world politics according to a one-dimensional scale of left versus right. One way to look at things is to add additional dimensions: a second scale in addition to left&lt;-~~<span style="text-align:right;">right is authoritarian&lt;</span>~~&gt;libertarian.

Let's see if this second dimension can be added to the analysis of software companies, I think it is independent of the political dimensions, there can be strictly top-down managed software liberal and software conservative companies.

-   authoritarian shops would be strictly hierarchic organization where business decisions are made by the top level, design documents and schedules formalized by middle level management and then passed down to be executed by grunt programmers. Even this approach has gradations: during design stage: is feedback encouraged from lower levels, or is it the norm to brush such feedback away? Is criticism tolerated? On the one hand these factors are determined by the personalities of managers within an organization, on the other hand a manager with a style that does not fit into the prevailing management culture is not likely to stay there for a very long time. How is information shared within the company? Are decissions just passed down the hierarchy or does management care to share contextual information about decissions and feature schedules (to be updated on a need to know basis is always less than what is required for a full understanding) ? The amount of information that is shared within an organization is an important factor. informed and constructive feedback from below would have to be based on some real factual knowledge. Important questions here are: does management try to act as an effective shield of information, are lower ranks informed on a need to know basis? What is the amount of interaction between peers? Is there a culture that encourages information sharing between peers or is it of benefit to hide knowledge in order to maximize job security? Hierarchy is often enforced by subtle gradations: so authoritarian shops are more likely to stress distinctions and boundaries between ranks. It is interesting that large authoritarian organizations do have research divisions that are organized with a reduced measure of hierarchic control.
-   Goggle (pre Google*) is said to have been a very non authoritarian work place. Initiative from below was encouraged, often big initiatives started as personal projects (the 10% project). Unfortunately this could only have worked when they had this playground where they could try out new things: Social networks like facebook were recognized as threats to the business model and Google decided to consolidate around the Google* platform. A platform means that every feature has to fit in with the rest of the system, so there is less room to try out new thing, this change also comes with more hierarchic style of product management; is also slashes the digital playground - a platform does not tolerate contradictions/overlap between features. I would guess that information sharing also runs into problems as a company grows into a mega corporation. It is interesting that Google's lack of customer support is what enabled this style in the first place: (google products are a take-it or leave it proposition). For developers there is much less time for trying out new things when one is frequently interrupted by customer problems.

------------------------------------------------------------------------

On the personal side there is one major trade off:

-   Working for bigCorp has its benefits - the main benefit is stability, the downside is that one has to put up with a log of hierarchic crap.
-   Start-ups have less hierarchy but you have to put up with less stability (political infighting between founders, question of funding, question of direction)

Nothing is perfect; coding is only part of the problem when working in software ;-)

i am not the only one to suspect that half of the complexity of any given software product is due to internal politics [here](https://en.wikipedia.org/wiki/Conway%27s_law) - in the end its always the guy who is handling grep and ctags and who has to figure this shit out ...

<center>
<img src="/cstuff/positive.jpg"/>
</center>