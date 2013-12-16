TOPDIR=..


TARGETS=stack tst tstop jmp ex pth


jmp_TYPE=exe

jmp_SRC=longjmp.c

jmp_CFLAGS=-finstrument-functions 

jmp_LIBS=c

stack_TYPE=shlib

stack_SRC=stack.c

stack_LIBS=c


ex_TYPE=exe

ex_SRC=exception.cpp

ex_CXXFLAGS=-finstrument-functions 

ex_LIBS=stdc++


tst_TYPE=exe

tst_SRC=test.c

tst_CFLAGS=-finstrument-functions 


tstop_TYPE=exe

tstop_SRC=test.c

tstop_CFLAGS=-finstrument-functions -O2 -fno-omit-frame-pointer





pth_TYPE=exe

pth_SRC=pth.c

pth_LIBS=pthread



include $(TOPDIR)/rules.make
