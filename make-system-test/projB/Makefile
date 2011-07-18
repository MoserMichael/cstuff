TOPDIR=../..



# - declare build targets. (built with make)

TARGETS:=shlib slibuser

# - slib target is a static library -

shlib_TYPE=lib

shlib_SRC=slib.c


# - slibuser target is a executable using slib -


slibuser_TYPE=exe

slibuser_SRC=slibuser.c

slibuser_LIBS=shlib


# - declare test targets. (built and run with make test)

TESTS:=tlib texe


tlib_TYPE=lib

tlib_SRC=tlib.c


texe_TYPE=exe

texe_SRC=texe.c

# - list of dependent static libraries of target texe
texe_LIBS=tlib shlib 



include $(TOPDIR)/rules.make


