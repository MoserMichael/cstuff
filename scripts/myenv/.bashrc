

# If not running interactively, don't do anything
[[ "$-" != *i* ]] && return

PATH=$PATH:/var/lib/gems/1.8/bin/

if [[ `uname -o` == "Cygwin" ]]; then
  # Windows has its own find.exe , so i rename the cygwin exe into findu
  FIND_EXE=findu
else  
  FIND_EXE=find
fi  


export EDITOR=vim

alias diffsrc="diff -rpu"
alias m=make
alias e=vim 


# followed by branch name (master)
function gitarchive()
{
  local bname
  local fname

  
  fname=$1
  if [[ "x$fname" == "x" || "x$fname" == "x-h" ]]; then
cat <<EOF
Usage: gitarchive <pathofzip>
Archives git archive with current branch into <pathofzip> as zip file.
EOF
  else
    set -x
    bname=$(git branch | grep -F "*" | sed -e 's#^*##')
    git archive --format zip --output $fname $bname
    set +x
  fi
}

alias gitls="git ls-files"


###
# search among source files
###
function s()
{
    local arg=$1

    if [[ "x$arg" == "x" || "x$arg" == "x-h" ]]; then
      cat <<EOF
Usage: s <search> 
case insensitive search for term in C/C++ sources and headers
beginning with current directory downward
EOF
    else 
      $FIND_EXE . -name '*.c' -o -name '*.cxx' -o -name '*.cpp' -o -name '*.h' -o -name '*.hxx' -o -name '*.hpp' 2>/dev/null | xargs grep -ni "${arg}" 
    fi

}

###
# Grep among header files only
###
function hgrep()
{
    local arg=$1
    
    if [[ "x$arg" == "x" || "x$arg" == "x-h" ]]; then
      cat <<EOF
Usage: hgrep <search> 
case insensitive search for term in C/C++  header files
beginning with current directory downward
EOF
    else 
      $FIND_EXE . -name '*.h' -o -name '*.hxx' -o -name '*.hpp' 2>/dev/null | xargs grep -ni ${arg} 
    fi
}


###
# tar sources in dir
###
function tarsrc()
{
    local arg=$1
    
    if [[ "x$arg" == "x" || "x$arg" == "x-h" ]]; then
      cat <<EOF
Usage: tarsrc  <archivename>
create tar archive of all sources from current directory downward
EOF
    else 
	# takes care of spaces in file names
	#   find -print0    - list of files in 0 terminated
	#   tar -T - --null - -T - take list of file from stdin --null asume 0 as null separator
	$FIND_EXE . \( -name '*.h*' -o -name '*.c*' -o -name 'Makefile*' -o -name '*.rules' -o -name '*.sh' -o -name '*.p*' -o -name '*.l' -o -name '*.y' \) -a -print0 | tar czvf ${arg} -T - --null
    fi
}

function gpush()
{
  if [[ "x$1" == "x-h" ]]; then
    cat <<EOF
Usage: gpush
git: for current branch: push changes to remote branch;
EOF
  else 
    local bname=$1
    set -x
    bname=`git rev-parse --abbrev-ref --symbolic-full-name @{u} | sed -e "s#/# #"`
    git push ${bname}
    set +x
  fi
}

###
# searching where symbol is defined (in directory with static libraries)
###
function nmgrep()
{
    local sym=$1
    local dir=$2

    if [[ "x$dir" == "x" ]]; then
      dir="."
    fi

    if [[ "x$sym" == "x"  || "x$sym" == "x-h" ]]; then

cat <<EOF
Usage: nmgrep <sym> <dir>
Searches for definition of <sym> in object files and static libraries in dir
EOF
    else
       $FIND_EXE ${dir} -name '*.o' -o -name '*.a' | xargs nm --print-file-name 2>/dev/null | grep $sym | grep -F 'T ' | less
    fi
}

###
# searching where symbol is used (in directory with static libraries)
###
function nmgrepu()
{
    local sym=$1
    local dir=$2

    if [[ "x$dir" == "x" ]]; then
      dir="."
    fi

    if [[ "x$sym" == "x"  || "x$sym" == "x-h" ]]; then

cat <<EOF
Usage: nmgrepu <sym> <dir>
Searches for reference of <sym> in object files and static libraries in dir
EOF
    else
       $FIND_EXE ${dir} -name '*.o' -o -name '*.a' | xargs nm --print-file-name 2>/dev/null | grep $sym | grep -F 'U ' | less
    fi
}
###
# list directories from current directory.
###
alias lsd="ls -al | grep ^d"

# list all files
alias lsa="ls -al"

# show aliases and functions
function manloc
{
  #typeset -F
  #alias

  typeset -F | grep declare  | awk '{ print $3 }' | grep -v command_not_found | grep -v '^_' | grep -v manloc | xargs  /bin/bash -i -c "for f in \$* ; do \$f -h; echo \"*****\"; done"  args 
  
  alias
}

function errno()
{
   local arg=$1

  if [[ "x$arg" == "x-h" ]]; then
cat <<EOF
Usage: errno <num>
Prints text that describes errno error number
EOF
  else
   set -x
   pushd /usr/include
   $FIND_EXE . -name "errno*.h" | xargs grep   "[[:space:]]${arg}[[:space:]]"
   popd
   set +x
 fi
}
 

# In putty: Ctrl-S does stopa echoing of characters as they are typed, Ctrl-O turns it on again;
# these commands turn of this nonsense. http://raamdev.com/2007/recovering-from-ctrls-in-putty/ 
# also in bash Ctrl-S does forward search in command history, however nobody knows that because
# by default terminal blocks this for this nuisance ;-)
stty ixany
stty ixoff -ixon
