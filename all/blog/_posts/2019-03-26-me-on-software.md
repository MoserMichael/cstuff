---
layout: post
title: Michael on Software

---

{{ page.title }}
----------------

<p class="publish_date">
26 March 2019
</p>

Everyone in this Industry seems to have a playbook

The big incumbents have the goal to stay on top of the heap;
They might follow the advise of the following articles by Joel Spolsky: [Smart companies try to commoditize their products’ complements](https://www.joelonsoftware.com/2002/06/12/strategy-letter-v/) ; another one of this is [“fire and motion”](https://www.joelonsoftware.com/2002/01/06/fire-and-motion/)
  And the upstarts have a playbook of their own; [disrupt from the low end from position as substitute](https://www.youtube.com/watch?time_continue=11&v=rHdS_4GsKmg) says Clayton Christiensen

Interesting that each article wants to promote itself as the big Truth; I think that each of them is looking at the same picture from a slightly different perspective.
Another thing is that these models look a bit apologetic, they describe the current state (that’s why the argument is quite convincing); It may by extension describe pattern of coming developments, but it might also fail to do so in the future.

I think that when any player is creating value, he is by extension also creating opportunities for other players. Its the law of unintended consequences that really counts – every action should be examined in terms of opportunities that it creates for other players, sometimes these unintended side effects turn out to be of major importance; 
Or: any ecosystem is creating opportunities for a whole range of actors, sometimes the effects af an action is not obvious at first, and the chain of events yields results that contradict the interests of the host/owner of the system.

examples: 
- Sun microsystem developed Java as a tool for [smart appliances](https://en.wikipedia.org/wiki/Java_(software_platform)#History); as a complementing product that would enable SUN to sell more hardware; several years later the major use case turned out to be enterprise software; now Java is a portable system, so that people started to use Java with cheap x86 servers instead of driving the sales of more expensive machines by Sun. 
- The Facebook API as a tool for third-party integration was used by [Cambridge Analytica](https://medium.com/tow-center/the-graph-api-key-points-in-the-facebook-and-cambridge-analytica-debacle-b69fe692d747), resulting into major fallout for Facebook. Of course users of this social network do get a lot of utility out of it; but the whole story did bring it to their attention that Facbook is not just making money by selling targetted adds; their data is part of the product and is being sold and repackaged for profit; this insight turned into quite a bit of resentment against facebook.
- I think very complex social networking sites are becoming hard to govern, because there is this multitude of players who also try to use the system to their own advantage.
- Thinking in terms of an ecosystem of different players is harder, but may become necessary when systems start to involve a lot of actors.

Another issue is that the industry is becoming a bit more complex and interdependent compared to what we had in the 90ies and turn of the century: this cloud thing is making things quite complex – if big cloud providers are turning enterprise software into ‘complements’; now what is Kubernetes/Docker? Is it a complementing product or will it lead for a push for hybrid clouds? Will hybrid clouds eventually cut into the profit of AWS/Azure/Google Cloud? 

This could make a bigger impact if combined with an adoption of ARM based servers, ARM processors need less cooling when compared to x86 processors, so you could possibly pack more systems into a box at a lower purchasing cost. However Mr. Thorvalds says that this is unlikely to happen: x86 became so important because devlopers could use the same platform for development and deployment  [here](https://www.realworldtech.com/forum/?threadid=183440&curpostid=183486). This could also change in the future; the [Kubernetes book](https://www.oreilly.com/library/view/kubernetes-in-action/9781617293726/) has a fascinating example where they build a playground cluster out of raspberry pis. Now what will happen when these raspberries grow up and if it becomes easier to set up a private cloud rather then paying a cloud provider for its services?

Another aspect is that every player in this game seems to have his own agenda and is pushing into a totally different direction. Also same thing can possibly be both a complement in one context and something completely disruptive if seen in a different context.

My conclusion: one might be analyzing the industry in terms of big players only, similar to the authors cited at the beginning of the article; but then it is easy to miss emerging trends and threats to these players; 

Another distrinct possibility is that I am misunderstanding the whole picture; I guess time will tell.

- - - - - - - - - - -


Notes on “Strategy letter V” by Joel Spolsky [link](https://www.joelonsoftware.com/2002/06/12/strategy-letter-v/)

Says the following concept comes from microeconomics – which is less abstract and of more use than macroeconomics:

* Every product has substitutes and complements;
    * substitute – a product that is similar to but cheaper than the original (chicken substitute of beef)
    * Complement – a product that goes with another one (gas is a complement of cars)
* Rule: demand for substitute increases when price of original product
* Rule: demand for product increases if the complement becomes cheaper.

Smart big companies strategy:
* push down prices of product complements – turn them into commodities “Smart companies try to commoditize their products’ complements” (all to create demand for the core product!)
* Example: IBM turned PC add-ins into commodity by documenting the architecture
* MS didn’t sell an exclusive MS-DOS license to IBM – MS succeeded in turning the PC into a commodity of MS-DOS.
* IBM invests in Open source systems and Linux: they want to commoditize the software environment to create more opportunities for their consulting business (and to drive demand for their own enterprise software and storage systems)
* Netscape makes navigator free: they want to sell more http servers and by turning the browser into a commodity they drive demand for http servers (hello apache ;-) And Microsoft make Internet explorer free – so that the web as a commodity would drive demand for Windows installations.
* Says Sun microsystems was acting strange/stupid by developing Java – which is portable. Due to this portability it did turn server hardware into a commodity (and server hardware was Sun’s core business)

Me: I don't quite agree with his diagnosis of Sun and java (but I got the advantage of looking back ;-)
I think the initial idea at Sun was to sell more ‘smart devices’ that run java, and that enterprise software was not the initial focus of the java platform, the end effect was something of an unintendended consequence due to the following reasons:
- Java required a lot of memory and cpu resources; At that time this was too much for the limited hardware of small devices; however a  decade later there was android and its apps are based on JVM technology! Now Google somhow got the profit from this technology as it was the company that defined the Android platform; but that is a different story.
- Java in the enterprise: Enterprise software turned into a major generator of revenue and here Sun was not a major player; however SUN tried hard to be one in middleware with EJB. This was not a success because EJB was hard to use and the emerging WEB architecture used a different path of development. It was a lost gamble, but that was not quite obvious at the time. 



-------


My notes on [Clayton Christensen: "Where does Growth come from?"](https://www.youtube.com/watch?time_continue=11&v=rHdS_4GsKmg)

<iframe width="560" height="315" src="https://www.youtube.com/embed/rHdS_4GsKmg" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

Questions:

- success is hard to sustain; good management causes companies to stumble (!); This creates despair for the author (as expert in management)
- growth, were does it come from?
- theories of management: many times they don't help at all! lots of aspects that explained by theory - example: no study of metrics.

Observations:
- says that good metrics would be great for testing theories - like the theory of disruptions? do these metrics exist?
- Recessions - hits bottom and need some timer before hiring new people ; for the first six recessions after WWII it took 6 months from end of recession until start of hiring wave. 1991 - 15 months; and 2001 - 39 months; 2008 - six years (!).
Longer gaps are due reasons related to finance (and availability of capital ?) - that is what's wrong about the economy.
- companies are not investing into innovations because of short term focus ; innovations are creating real growth.

types of innovations:

- potential innovations
- sustaining innovations
- efficiency innovations
- disrupting products

_potential innovations_ - potential products.
- only 15-25 of all innovations will be financially rewarding - we don't know which one, so there are a lot of tries;
- current marketing studies are crap - studying the customer is the wrong approach; they try to classify customers; and their properties (customer profiles) - which are not significant to the buying decision.
- what should be studied is the customer needs (customers not always good at articulating their own needs), a product is there to satisfy a need; always ask 'what job does this thing solve'? that approach is better at predicting a good product (so it is better to study customer behavior to understand their needs)

Research: how can McDonalds improve their milkshakes?
Wrong approach: build a profile of customers and ask them how to improve the product
Right approach: ask existing customers why they use the product? How can we fulfill your needs (the job) better?
Study: when are customers coming? What time? What are they doing with the product - take away or consume on the spot?
Turns out 50% bought it in the morning and drove away ? Asked them 'what caused you to hire this milkshake' 'what was your substitute (focus question)' - so that we are not hungry on a long drive/commute.
In the afternoon parents buy that for their kids - so they have something to talk over when the kids are consuming the milkshake. 

The Customer is wrong unit of analysis - look at the customer problem/situation; and customer needs - to solve the problem that the situation creates.
The task description is static (makes it easy to fix it) - but technology to solve it changes quite a lot;
every Job/customer problem has a functional/emotional/social aspect;

Then ask: what is the experiences are that will fulfill the job done? If these are associate with a brand/business then this would be great.

Ask:
- how will customers choose us and not the competition?
- what can we do that other people can't ?
- How will everyone know about us?

Ikea does a great job its because it is organized aroung the 'job to be done' (like Ikea);
customers like this approach because the alternative requires more work/effort to search parts for the customer.

_sustaining innovations_ - make an existing product better (most number).

don't create growth because they replace existing things; but are important to sustain existing companies.

_efficiency innovations_ - helps to produce more with less; keep us competitive, but reduce jobs and create cash flow;

Examples: Wallmarts, Toyota production process. ;

Abundance and scarcity: some things are scarce - like platinum; have to keep it , some things are abundant - sand, can waste it.
Finance: Capital used to be scarce - now it is abundant (? probably for big players ?) ; also finance now measures success in ratios (metrics) rather than whole numbers (of dollars)
Fractions should allow to performance of companies; (like metrics of 'return on net assets - RONA' / 'internal rate of return - IRR');
RONA can be gamed by outsourcing rather than doing innovating products;
IRR can be gamed by short range gains (long term investment is not good for it)
Says growth is lost in the pursuit of these metrics. Also companies have a lot of cash on hand;

Efficiency innovations pay off earlier (Pay off in a 6 months - 3 years) so they are prefered to disrupting products (these pay off in 5-10 years)

Japan was growing fast because companies invested in disruptive products; (lots of examples) Since the late eighties they use these metrics - Japan didn't do a disruptive product ever since.
USA is going the same way; result is that capital is used to create capital.

_disrupting products_ - these create growth

Trajectory of improvements that customers are able to utilize (doesn't change too much).
Trajectory of improvements that companies provide with better proucts - (lot larger than ability to utilize them) -> even not so good products are better than customer expectation.
Example: took a lot of time to utilize the PC;

Says that you can't unseat a current market leader in their own product (on their field);
But disruptive innovations can - new products that are more accessible and cheaper than existing ones.

Google disrupted advertising
DEC did minicomputers; in good times they sold computers for 200.000$ at 45% margins; Now DEC has disrupted mainframes that sold for a million at >60% margins.
Now the PC didn't matter for much of the eighties - DEC management cared about their current products, not the low starting PC that would disrupt them.

Innovators dilemma: focus your limited resources on attempt to improve your existing product or care for currently not-relevant but potentially disrupting upstart?

Other examples: Phones :: Blackberry -> Apple -> Samsung (and friends).
Disks :: Big disks -> small ones -> flash.
Cars :: Ford; GM -> Toyota (Corona was affordable to students and disrupted Ford; went after Corolla, etc; now they also sell Lexus cars) -> Hyundai, Chinese are now killing them at the low end.

_they all disrupt the existing leader from the low ends; the upstarts compete against non-consumption by making consumption more affordable._

Q: are there big companies that survived new disrupting products/competing upstarts ?

A: yes, Need to create a new business unit and new processes; like IBM that created new business units for mini computers; later new one for micro computers.

Q: what metrics are good ones for Google?

A: most metrics are concerned with the short term only; useless in the long run, so make up your own metrics.

Q: Google invests in AI: what will be the remaining jobs for humans (if this succeeds) ?

A: applications of AI will probably be pretty complicated; probably this will force us to hire more people.

Q: what is actually the disrupting effect of an innovation ? The technology/the application/the market effect ?

A: Word has a lot of connotations; the business model has to do with it, how it is deployed, and where (not the technology by itself)
Google is good at developing potentially disruptive technologies, but it is not good at creating business models that compete against non-consumption (?)

Q: certain products would not fit in with model of low end disruption: Uber/iPhone/Tesla started from the high end, not the low end ?

A: theory is that Tesla is a sustaining innovation; says that BMW and porsche will give Tesla some heat (or acquire them); Says the transforming innovation will be cheap/small electric cars from China.
Apple: they disrupted the laptop and succeeded. (didn't compete against blackberry; blackberry didn't have apps, that's what killed them); now Apple is being disrupted by cheap android phones.
Uber? taxis are asset intensive (car + medallion) + keep the cars running for the whole day to get a small profit; now Ueber has no assets and variable costs, taxis can't compete with that; so its also 'from the low end' - but in terms of business model

Q: measuring your life in a better way: how can employees do that? Why did you write that?

A: if he analysis personal stories then he sees that (in business terms) the resource allocation process/metrics causes people to pursue a direction that they didn't want in the first place.
Example: he sees his former friends at Harward reunions; with time fewer people appear at these events because they get divorced and don't want to talk about it.
The intention was to have happy families says they failed because of wrong metrics (wrong objectives) - on immediate/tangible signs of achievements;
careers get this; kids don't show this- kids have problems, so they don't invest in them (its a long term investment)
The book should set this right 'how to measure your life'

- - - - - - - - - - - - - - - - - - - - - - - - - - -

[Clayton Christensen On Truth, God and Personal Courage](https://www.youtube.com/watch?v=DfJ9ydhb53Q)


Q: what have you learned since (after many afflictions and illnesses) ?

A: Many attribute the speakers stand due to personal courage, he thinks differently: you do  need courage - if you have postponed work or undone work; that leads to fear when faced with danger.
Says he has no fear because he is convinced that he has led the right live according to G-d; so his approach is 'lets face another problem'

Q: what do you tell people who don't have faith?

A: atheism is similar to religion because its proposition is an unprovable statement (so it becomes kind of a religion); also both have common features: both Atheism and Religion do have a view of life/code of ethic;

says the conflict/distinction between science and religion is exaggerated - religious people don't want to face criticism by science, so they don't talk about Religion.

Says that concept of Greek deities (liake Zeus) is that of being outside of nature; so that science has nothing to do/is incompatible with the will of the deities.

Christiensen says that G-d is not outside of the universe, but within it and that he uses/is governed by natural laws in his actions.
In this view science is not incompatible with Religion, as the truth of science is part of the same realm as religion (or rather deal with the same concepts/have a shared language); as he sees Gods action as being subject to constraints of natural laws and science as studying these natural laws.

Q: who knows that our understanding doesn't change and will be invalidated in the future.

A: Can't know that - can hope that our knowledge is somehow converging with truth.


