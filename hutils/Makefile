TOPDIR=..

POSTBUILD_SUBDIRS=test

TARGETS:=hutils hutilssh


# - sexe target compiles all files in current directory with extension *.c

hutils_TYPE=lib

hutils_SRC_EXTENSION=c


hutilssh_TYPE=shlib

hutilssh_SRC_EXTENSION=c

hutilssh_LIBS=$(hutilssh_LIBS-$(OS_TYPE))  

hutilssh_LIBS-Cygwin=cutilssh nutilssh
hutilssh_LIBS-GNU-Linux=



include $(TOPDIR)/rules.make
