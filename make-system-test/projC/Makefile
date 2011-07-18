TOPDIR=../..


# - list build targets
TARGETS:=shlibso slibexe

# - slibso target is a shared library -
shlibso_TYPE=shlib

# - list of files for shlibso target -
shlibso_SRC=slib.c


# - slibuser target is a executable that depends on the shlibso library -
slibexe_TYPE=exe

slibexe_SRC=slibuser.c

slibexe_LIBS=shlibso

# - declare test targets. (built and run with make test)
TESTS:=tshexe

tshexe_TYPE=exe

tshexe_SRC=texe.c

# - list of dependent static libraries
tshexe_LIBS=shlibso

include $(TOPDIR)/rules.make