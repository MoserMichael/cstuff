---
layout: post
title:  The make file to end all make files
---

{{ page.title }}
----------------

<p class="publish_date">
17 July 2011

</p>
One of the things I do for living is fiddling with make files with [GNU Make](http://www.gnu.org/software/make/) . Usually this leaves me with the vague impression that there must be a better way to do this and that. And so it's time to do it right, even if it is only for my own stuff.
So each piece of software should start with a list of requirements, here is my list for the make file

-   Support make/test/install/clean/cleanall targets.
-   Make script runs on multiple operating systems (Linux and Cygwin for a start). It is possible to override options per OS / OS version / processor type.
-   All build results (objects, executables, libraries) are placed under one root directory. Root directory name contains OS name + OS version + processor type.
-   Multiple targets of different type can be declared in one make file; if one has forgotten declaration required for target, a human readable error is displayed.
-   The object files of each target are placed in separate object directory, this way we can set separate compiler option for intersecting source file sets.
-   A build target can be of type exe/shared library/static library.
-   A build target either includes subset of files or all files in directory.
-   A build target can add its specific compiler/linker settings.
-   Track source file dependencies; use trick that avoids reparsing of make file.
-   A target can declare dependent libraries;
-   Each make file has optional pre build and post build steps that can do code generation for instance.
-   Display banner on building of each target; (what is the target, type of target + directory)
-   Ability to add compiler and linker options from environment to each target (environment variables EXTERNAL\_CFLAGS , EXTERNAL\_CXXFLAGS , EXTERNAL\_LDFLAGS ).
-   Test target: all test programs and test libraries are put into separate directory $(BIN\_ROOT\_DIR)/test
-   Try to minimize number of shells invoked; try to minimize number of make file rereads/re reads. This is important for Cygwin support + compilation speed.
-   Like all software, the build script has a set of tests.

The make system magic works by reusing rule definition files all over each Makefile.
Each directory that builds a target has its own Makefile, this file has to includes the skeleton rule file.

The skeleton rule files are placed in the root directory and those are

-   [rules.make](https://github.com/MoserMichael/cstuff/blob/master/rules.make)
-   [rules-GNU-Linux.make](https://github.com/MoserMichael/cstuff/blob/master/rules-GNU-Linux.make)
-   [rules-Cygwin.make](https://github.com/MoserMichael/cstuff/blob/master/rules-Cygwin.make)
    You also get it with my [GIT repository for stuff in c](https://github.com/MoserMichael/cstuff)

### Basic usage + defining build targets

    1: TOPDIR=../..
    2:
    3: # - declare build targets. (built with make)
    4: TARGETS:=shlib slibuser
    5: 
    6: # - slib target is a static library -
    7: shlib_TYPE=lib
    8: shlib_SRC=slib.c
    9:
    10:
    11: # - slibuser target is a executable using slib -
    12: slibuser_TYPE=exe
    13: slibuser_SRC=slibuser.c slibuser2.c slibuser3.c
    14: slibuser_LIBS=shlib
    15:
    16: include $(TOPDIR)/rules.make

Each Makefile has the TOPDIR variable that points to the root directory, in the root directory we have the skeleton rule files. Line 16 includes the rule.make - the skeleton rule file.
The List of build targets is declared in line 4; the TARGETS variable lists the name of each target; in this case targets shlib and slibuser are declared.
Each build target defines its type
slibuser\_TYPE=exe
This means that build target slibuser builds a executable.
slibuser\_TYPE=shlib
This means that build target slibuser builds a shared library.
slibuser\_TYPE=lib
This means that build target slibuser builds a static link library.
Each target is built from sources, so either it lists all the source files:
slibuser\_SRC=slibuser.c slibuser2.c slibuser3.c
Says that build target slibuser is built from the source files slibuser.c slibuser2.c and slibuser3.c

    1: TOPDIR=../..
    2:
    3: # - declare build targets. (built with make)
    4: TARGETS:=shlib 
    5: 
    6: shlib_TYPE=shlib
    7: shlib_SRC_EXTENSION=c
    8:
    9
    10: include $(TOPDIR)/rules.make

shlib\_SRC\_EXTENSION=c
This declaration says that the build target shlib is built from all files with file extension \*.c in the current directory.

### build results

The build result directory is a sub directory of the root directory (a sub directory of $(TOPDIR); The name of the build result directory is made up of the following components
- Operating system name (based on uname ~~o, sanitized by replacing / ( ) characters with~~ )
- Kernel release (based on uname ~~r, sanitized by replacing / ( ) characters with~~)
- Machine architecture (based on uname ~~m, sanitized by replacing / ( ) characters with~~)
For my current system the name is GNU-Linux-2.6.38-8-generic-i686 ;

For each executable build target the result is placed into $(BUILD\_RESULT\_DIRECTORY)/bin, static or shared libraries are placed into $(BUILD\_RESULT\_DIRECTORY)/lib
Test programs and libraries are placed into $(BUILD\_RESULT\_DIRECTORY)/test

### adding specific compiler / linker options to a project.

    1:  TOPDIR=../..
    2:
    3:  # - declare build targets. (built with make)
    4:  TARGETS:=shlib
    5: 
    6:  # - slib target is a static library -
    7:  shlib_TYPE=shlib
    8:  shlib_SRC_EXTENSION=c
    9:  shlib_CFLAGS=-finstrument-functions -O2 -fno-omit-frame-pointer
    10: shlib_LIBS=pthread cutils
    11:
    12: include $(TOPDIR)/rules.make

shlib\_CFLAGS=-finstrument-functions
This declares that compiler option -finstrument-functions is added specifically to target shlib; the option is specifically added to compilation of C files.
In order to add options to C** command line one should have set shlib\_CXXFLAGS variable.
shlib\_LIBS=pthread cutils
This declares that the shlib target is linked with libraries pthread and cutils; the -lpthread and -lcutils is added to the linker options for this target.

### Recursing into subdirectories

    1:  TOPDIR=../..
    2:
    3:  PREBUILD_SUBDIRS=dir-a dir-b dir-c
    4:
    5:  # - declare build targets. (built with make)
    6:  TARGETS:=shlib
    7: 
    8:  # - slib target is a static library -
    9:  shlib_TYPE=shlib
    10: shlib_SRC_EXTENSION=c
    11: shlib_CFLAGS=-finstrument-functions -O2 -fno-omit-frame-pointer
    12: shlib_LIBS=pthread cutils
    13:
    14: include $(TOPDIR)/rules.make

PREBUILD\_SUBDIRS declares that prior to building target shlib, the make process would recurse into sub directories and invoke Makeefile in directories dir-a dir-b and dir-c
POSTBUILD\_SUBDIRS declares that after building target shlib, the make process would recurse into sub directories and invoke Makeefile in directories dir-a dir-b and dir-c

### Specifying test targets invoked by 'make test'

    1: TOPDIR=../..
    2: TESTS=cutilstest
    3: cutilstest_TYPE=exe
    4: cutilstest_SRC_EXTENSION=c
    5: cutilstest_LIBS=cutils vtest
    6: include $(TOPDIR)/rules.make

The TESTS variable lists name of test programs; these programs compiled and run by 'make test' command.

### Adding custom pre build step

Each target can execute a pre build step - a script/program that is run before making any source files. This can be used either to

-   Generation of source code
-   A self configuration script that tests current environment and writes a header file that defines all sort of options.
    The pre build test program receives the names of all source files of this target as command line parameters.
    A pre build step is defined by the following declaration
    <target name>\_PREBUILD=<prebuild script path>
    h3. Adding a custom post build step
    Each target can execute a post build step - a script/program that is run after making any source files. This can be used either to
-   custom packaging
-   Instrument the binary of the executable
-   Run the target binary yas part of complicated test script.

The first argument of the post build script is always the path name of the file that is built by the target.
A post build step is defined by the following declaration
<target name>\_POSTBUILD=<postbuild script path>

### Invocation of make

#### make

Running make will build all targets in debug mode;

make

This next command will build the code with full optimization, and with debug symbols; one issue is that executables and libraries are put in the same place for both debug and release builds; However the object files for release and debug builds are put in separate directories and do not intersect.

make MODE=release

This next command willl build build the code with less optimization (on Linux it uses gcc and -O2).

make MODE=release2

We can also specify the desired type of executable produced.

make ARCH=x86\_64

This will build a 64 bit executable (on the Intel platform that is)

make ARCH=i686

This will build a 32 executable.(on the Intel platform that is)

Also there are some other nice tricks in store, tricks that are sometimes very useful during development

make CPP\_LISTING=1

Now the source file directory will also have a file with the [C preprocessor output](http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html) for each compiled file.
For example the source file foo.c if compiled by target bar will have the file foo.bar.i with preprocessor output.
The preprocessor output can be priceless when you have to fix compiler errors that involve macros for instance.
The preprocessor listing is created prior to compilation of a source file, as it is usually needed when we try to fix a weird compiler error

Also the following invocation will produce the assembly listing for each compiled file.

make ASM\_LISTING=1

For example the source file foo.c if compiled by target bar will have the file foo.bar.S with assembly listing.
The assembly listing can be priceless if you have to understand what happens with advanced optimization modes, for instance.
The assembly listing is created after compilation of a source file, when we are sure that the stuff actually compiles.

The downside is that the listing files are not deleted by running 'make clean ' or 'make cleanall ' you have to remove them manually.

#### make test

Will first build all targets and then build all test targets
You can specify a prefix command that will run the test targets; for instance

make test TEST\_TOOL=valgrind

Here all test programs will be checked by [valgrind](http://valgrind.org/)
Running valgrind on unit tests is something that one definitely wants to do - on the one hand a unit test is supposed to be short, so that valgrind runtime overhead will not be that significant, on the other hand unit tests are supposed to cover a lot of functionality - so this tool will help a lot here.
Alternatively the following command will not run any tests at all;

make test TEST\_TOOL=true

Sometimes you really want to skip running tests, like when running a test is done by a post built step, or if you build with a cross compiler, for instance.

#### make clean

Deletes all object files and executable targets

#### make cleanall

Removes the whole build result directory tree; everything that is built is hereby removed in one simple step. Every file, with the noted exception of stuff that has been generated by scripts invoked prior to build process (pre-build step) or after build (post-build steps).

#### make install

Copies all built libraries into $(PREFIX)/lib and all built executables into $(PREFIX)/bin
The default prefix is /usr/local
make install INSTALL\_PREFIX=/opt/mystuff
Install to the very different prfix /opt/mystuff

### Limitation of make system

All this magic works with gmake 3.81 and upwards. It uses the built-in $(eval) and $(call) macro facilities; and these do not work properly on all versions of make.
Yes, I know that this statement is killing the punch line;-)

That seems to be a general rule; whenever one is using more sophisticated features of software product X the more you do rely on a specific version of the X product;
So the make file to end all make files does not work on older version of gmake.

Another major limitation is that I did not have to deal with the problem of cross builds; so there is no generic mechanism for dealing with cross build; I will have to devise a way that is compatible with other make systems. Automake tools do a lot in the respect; I still find it easier to run my own stuff;

### Debugging the make file

make SHOW\_RULES=1

This command will dump to the screen most macro expansions that are done behind the scene. It does not show all built rules though.
I think any macro system should have such an option built in - to dump the output to screen, but again, life is life.