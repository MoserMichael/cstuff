TOPDIR=../../..


TESTS=makecore


TEST_TOOL=true

#---

makecore_TYPE=exe

makecore_SRC=makecore.c

makecore_CFLAGS=-I..

makecore_POSTBUILD=/bin/sh -c "set -ex ; rm -f memerrors.log core || true; ulimit -c unlimited ; $(ROOT_DIR)/scripts/run $(BIN_ROOT_DIR)/test/makecore || true ; gdb --batch -ex='core-file core' -ex='source $(ROOT_DIR)/scripts/dbgmems-read-core.py'  $(BIN_ROOT_DIR)/test/makecore; cat memerrors.log  "

 
 



include $(TOPDIR)/rules.make
