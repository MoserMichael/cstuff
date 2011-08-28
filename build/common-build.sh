

#set -x
set -e


function make_file
{

cat <<EOF >../Makefile 

TOPDIR?=.

PREBUILD_SUBDIRS=$@

include \$(TOPDIR)/rules.make

EOF

}

make_file $TARGETS

cd ..

make cleanall

make test $@
