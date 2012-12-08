TOPDIR=../..


ifeq "$(shell uname -o)" "Cygwin"
TESTS=corothreadjmptest  
else
TESTS=corothreaductxtest 
endif

#---

corothreadjmptest_TYPE=exe

corothreadjmptest_SRC_EXTENSION=c

corothreadjmptest_LIBS=corothrjmp cutils butils vtest pthread

#---

corothreaductxtest_TYPE=exe

corothreaductxtest_SRC_EXTENSION=c

corothreaductxtest_LIBS=corothructx cutils butils vtest pthread

#---




include $(TOPDIR)/rules.make
