TOPDIR=..

POSTBUILD_SUBDIRS=webbyimpl test 

TARGETS:=webbylib webbylibsh 


# --------------------------------------------:w

webbylib_TYPE=lib

webbylib_SRC_EXTENSION=c

# --------------------------------------------:w

webbylibsh_TYPE=shlib

webbylibsh_SRC_EXTENSION=c

webbylibsh_LIBS=$(webbylibsh_LIBS-$(OS_TYPE))

webbylibsh_LIBS-Cygwin=hutilssh cutilssh butilssh nutilssh pthread 

webbylibsh_LIBS-GNU-Linux=  


include $(TOPDIR)/rules.make
