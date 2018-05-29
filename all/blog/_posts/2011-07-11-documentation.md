---
layout: post
title: My take on software (Re)use
---

{{ page.title }}
----------------

<p class="publish_date">
11 August 2011

</p>
[Code reuse](http://en.wikipedia.org/wiki/Code_reuse) is like a [Holy Grail](http://en.wikipedia.org/wiki/Holy_Grail) of sorts for software engineering; Magical Powers of productivity are attributed to code reuse, however the concept has proven to be somewhat illusive in practice. Most attempts to achieve this aim also tend to lead to doubts and frustration. Yosefk has [a long and thorough piece about that](http://www.yosefk.com/blog/redundancy-vs-dependencies-which-is-worse.html) - the argument here is that by using a library one adds dependencies, managing dependencies adds overall complexity (paradoxically) - this complexity is created by the need of managing those dependencies, requires one to adapt to the libraries interface, creates politics from the need to interact with the author / origin of the said library etc. etc. As usual with software there are tradeoffs to make; as usual the resulting choice is often determined by the character of the person that makes it ;-)

By the way, the stuff here for [my projects](https://github.com/MoserMichael/cstuff) that might be usable to others is document [right here](http://mosermichael.github.com/cstuff/docs.html) .

I would like it simpler, so here is my tale: Once upon a time I have worked as a free lance programmer/consultant; the problem here is that the software consultant (me that is) had to determine up front the exact amount of work required for each job. My customers somehow always needed to know how long it takes / how much money they would have to pay me. So after one or two miscalculations I have come up with the following rules.

-   Each entity of work basically needs at least three iterations
-   First iteration 'does something'; it basically works according to how I understand the problem. The main mistake in consulting is to mistake the result of first iteration for the end result.
-   The second iteration comes nearer
-   The third iteration must then be according to customer expectations

Basically this knowledge translates into the rule of thumb where everything takes at least three times as longer than expected.
So in my definition a software component is

-   A software entity that has some defined boundary
-   It requires at least three iterations to get it reasonably right.
-   You have a 95% chance of getting it wrong the first time when you do all by yourself

This definition is somewhat vague; however it should be applied as a test to see if there is a justification for creating or using another component. If you see an entity in your 'problem domain' and are certain that these three rules are true, then a programmer should consider to look out if there is a software component already written by somebody else that covers the 'problem domain'.

P.S. If the reader of this text somehow happen to be a manager of sorts, then please be aware that assigning junior people to reusable stuff is a sure receipt for disaster. This approach can be characterized by the phrase 'Infrastructure is not a real project' which is a quite funny phrase if you think of it.