---
layout: post
title:  Image archive organizer
---

{{ page.title }}
----------------

<p class="publish_date">
24 February 2014

</p>
### The problem

Lots and lots of images everywhere ... in the course of a few years we have managed to accumulate thousands of images from various cellphones and digital cameras, and we never had the time to organize a proper photo album.

It all looks like this [XKCD comics](http://xkcd.com/1360/)
<img title="Wow, ANIMORPHS-NOVEL.RTF? Just gonna, uh, go through and delete that from all my archives real quick." src="http://imgs.xkcd.com/comics/old_files.png"></img>

So its time to write a program that makes some sense out of this mess ...

### Defining the program

The program should be able to maintain a picture archive; here are my requirements:

-   The program organizes a picture archive; images are stored as files, the program also maintains some data on its own such a as a list of files, etc.
-   It should be able to exclude identical images; if you import two identical images twice then it should be able to skip copies.
-   It is very hard to work with thousands of image files all in the same directory (tried that); the program should be able to organize images by chronological order, within the archive we end up with a hierarchy of directories <year that image was shot>/<month image was shot>/<day image was shot> ; Image meta data includes the creation date ([exif format](http://en.wikipedia.org/wiki/Exchangeable_image_file_format) ), so the program should not rely on the file date alone.
-   Checking the archive: the program should be able to check the consistency of an archive. If a file has been corrupted then it must be thrown out of the archive.
-   Merging of two archives ; if you have two picture archives then it should be able to compare both of them and to add missing information from the other archive; all this would help with creating back ups of the archive.
-   The program should have a command line interface; good enough for me (but not enough to make money out of it ;-)

This program can be of use in the following scenarios

-   if you want to sort out a big stack of pictures then the first thing would be to organize them by date; you can begin to filter out the good pictures once each picture set is of manageable size, then you can decide on what to upload to picassa or to print on paper.
-   People like me who want to have their pictures stored locally on their own systems, not in some distant Cloud ™; I think end users are increasingly reluctant to trust the cloud with their own personal data - i personally see the cloud thing as a huge centralization of computing that inevitably comes with centralized political control (more on that [here](http://mosermichael.github.io/cstuff/all/ramblings/2013/06/08/post-1.html) )

### How to use the program

#### Creating an archive for the first time or adding of pictures to an existing archive.

A short description of that functionality:

<pre>
img-archive.pl -i -s &lt;dir> [ -s &lt;dir2&gt; ] -d &lt;dir&gt;   [ --noprogress ]
img-archive.pl --import --source &lt;dir&gt; [ --source &lt;dir2&gt; ] --dest &lt;dir&gt;

	import images from source directory (specified by -s &lt;dir&gt;) to archive (specified  by -d &lt;dir&gt;)
	Multiple source directories can be specified by repeating option -s

	Images are ordered into a directory hierarchy

	 &lt;year the picture was taken&lt;/&gt;month picture was taken&lt;/&lt;day picture was taken&gt;/&lt;file name&gt;

	The date of picture taken is extracted from the image file (where possible) - 
	this is enabled by the ExifTool, written by Phil Harvey.

	The script checks if the picture already exists in the destination directory;

	The option --noprogress suppresses display of progress indicator during copying.

	Script is part of my simple picture archive system.
</pre>

#### Backup / Merging to archives

A short description of that functionality:

<pre>
img-archive.pl -m -s &lt;dir&gt; [ -s &lt;dir2&gt; ]    [ --noprogress ]
img-archive.pl --merge --source &lt;dir&gt; [ --source &lt;dir2&gt; ] 

	Merge a number of archives; after merging every archive should have the same set of data;
	However the file names may differ.
</pre>

#### Verifying the validity of an archive

A short description of that functionality:

<pre>
img-archive.pl -c -s &lt;dir&gt; [ -s &lt;dir2&gt; ] [ --noprogress ]
img-archive.pl --check --source &lt;dir&gt; [ --source &lt;dir2&gt; ]  [ --noprogress ]
	
	checks an existing archive for consistency; 

</pre>

### How to install it

The program is written as a perl script (caugh caugh). It can be used on both Linux and Windows;

So on Linux you need

- the [img-archive.pl](https://raw.githubusercontent.com/MoserMichael/cstuff/master/img-archive/img-archive.pl) script

- perl installed

- perl package Exif, written by Phil Harvey. ([here](http://cpansearch.perl.org/src/EXIFTOOL/Image-ExifTool-9.60/html/ExifTool.html) and [here](http://www.sno.phy.queensu.ca/~phil/exiftool/ExifTool.html)

- jpeginfo program written by Timo Kokonnen ([here](https://github.com/tjko/jpeginfo) )
- openssl must be installed (comes with most distros default installation)

To simplify things you can just download and unzip [this archive](https://github.com/MoserMichael/cstuff/raw/master/img-archive/img-archive.zip) The archive includes:

- the program

- perl package Exif
- jpeginfo program compiled on windows for Cygwin (if that stops you can just build it by going to jpeginfo\_prog directory and run make).

You still need an installation of Cygwin with perl and openssl.

### A more detailed description of the program

For each image that is part of the archive we first compute the SHA1 signature/checksum of the image data - the digest is a function that takes the image data as input and that produces a large number as output; one might wonder if it will ever happen that two different pictures will yield the same signature value, however the likelyhood of such an event is considered to be very very low ([see here](http://en.wikipedia.org/wiki/Data_deduplication#Drawbacks_and_concerns) ) , surprise, surprise.

Each archive has an index file hash2file.lst - this file lists both the file name and its digest/hash number. First thing when working with an archive is to open the index file;
For each entry in the index file it checks if the file exists at all if the file does not exist then the index entry is discarded; In memory a hash map is mapping the digest to the file name; this hash map is used to check for duplicate images;

The different commands in more details:

#### Import of new images

-   The index file of the destination archive is read; checks if all files that are listed in the index file do exist; for each index entry a mapping is established in digest\_to\_file (maps digest to file name) and has\_file (maps file name to constant 1)
-   All files in directory of destination archive (other than the index file) are listed; if no checksum for this file exists, then the checksum is computed and an entry is added to the index.
-   All files in the source directories are listed; for each file the SHA1 digest is computed, if the digest does not appear in the archive index (no entry in digest\_to\_file\_exists) then the file is imported;
-   Import of a file
    -   Extract Exif meta data from the image file; get the CreationDate (if not available DateTimeOriginal, if not available then ModifyDate is extracted; if that is not available then the OS file creation time is assumed.
    -   Target directory is formed as <Year>/<Month>/<Day> under the target directory
    -   File name is that of the source file; take care not to overwrite an existing file, if a file of that name exists then pick a different one.
-   Save the modified index

#### Merging of archives

-   Check that all directories are valid archives (i.e. that each archive contains an index file) ; read in the index for each archive;
-   For each archive; enumerate the entries of the index; check if a file of the same digest exists in the other archives, if not then import the image into the respective archive
-   save the modified indexes.

#### Integrity check

-   The index file of the archive is read; checks if all files that are listed in the index file do exist; for each index entry a mapping is established in digest\_to\_file (maps digest to file name) and has\_file (maps file name to constant 1)
-   All files in directory of destination archive (other than the index file) are listed; if no checksum for this file exists, then the checksum is computed and an entry is added to the index.
-   For all entries of the index: compute the digest again, check if the digest has not been changed;
-   If the digest has been modified, then check if the image file is still valid; if it is valid then modify the existing index entry, for invalid files delete the index entry
-   Report Number of valid, number of modified and number of invalid files;
-   Save the modified index.

### An ode to Perl

Perl is like an old friend that is always there for you; whenever there is a problem it comes in with a huge set of libraries that seem to cover almost all areas, whenever there is a problem one can easily make up a program to automate the process. For example during the recession of 2001 it helped me to make up a program to spam prospective employees with my CV, the script would parse job boards for adds, build a database of adds and send out the CV; somehow that helped me and some of my friend;
Things like that create brand loyalty, at least for me ;-)

Perl the duct tape of more than the early internet !