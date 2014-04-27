

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


function check_make
{
  local ver

  ver=`make -v 2>&1 | head -n 1`

  if [ "x$?" != "x0" ]; then
     echo "make not installed"
     exit 1
  fi

  echo $ver | grep -F "GNU Make"  2>&1 >/dev/null
  if [ "x$?" != "x0" ]; then
      echo "make must be GNU Make"
  fi

  local cur_major=`echo "$ver" | sed -e 's/GNU Make \([0-9]*\)\.\([0-9]*\).*/\1/'`
  local cur_minor=`echo "$ver" | sed -e 's/GNU Make \([0-9]*\)\.\([0-9]*\).*/\2/'`

  local major="3"
  local minor="81"

  cur_major=`printf "%d" "$cur_major"`
  if [ $cur_major -lt $major ]; then 
    echo "GNU Make version 3.81 or later expected, current version ${cur_major}.${cur_minor} . Sorry"
    exit 1
  fi

  if [ $cur_major -eq $major ]; then
    cur_minor=`printf "%03d" "$cur_minor"` 
    if [ $cur_minor -lt $minor ]; then 
      echo "GNU Make version 3.81 or later expected, current version ${cur_major}.${cur_minor} . Very Sorry"
      exit 1
    fi 
  fi
}


check_make

make_file $TARGETS

cd ..

if [ "x$1" != "x" ]; then
  make MODE=$1 $@
else
  make cleanall $@
  make MODE=releaseBuild test $@
fi
