---
layout: post
title: PERL my getopt command line parser
---

{{ page.title }}
----------------

<p class="publish_date">
17 July 2011

</p>
Usually I want my PERL scripts to work on all machines that have PERL installed.
Now a script usually parses the command line, in perl you have [Getopt::Std](http://perldoc.perl.org/Getopt/Std.html) to does that for you.
Now this command line parser is in a core module, however this does not mean that it is available on each PERL installation, for example the default installation of RedHat Enterprise Server installs PERL without the core modules; so we can't use Getopt::Std for parsing command lines here.

Fortunately we can roll our own in 30 lines, so there is no problem in copying this function over into other scripts; as we all know copying code is an awful engineering practice, that should be punished by decapitation of the offender (in most cases the offender is acting without a head to begin with, so this measure becomes impractical ;-)
That does not mean that we should [Screw good practices](http://xkcd.com/292/) - punishment is never [far behind](http://xkcd.com/292/)

Here we can justify copy-paste by our desperate goal of running a PERL script on all installations.
Now [here is the script](https://github.com/MoserMichael/cstuff/blob/master/scripts/m-getop.pl)
As you can see, the getopt function receives an array, where each entry describes an option, this simplifies the function to the extent that we don't have to parse the argument specification.
Otherwise it is all quite similar to the original [Getopt::Std](http://perldoc.perl.org/Getopt/Std.html)