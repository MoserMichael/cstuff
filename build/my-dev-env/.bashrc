# base-files version 4.0-4
# ~/.bashrc: executed by bash(1) for interactive shells.

# The latest version as installed by the Cygwin Setup program can
# always be found at /etc/defaults/etc/skel/.bashrc

# Modifying /etc/skel/.bashrc directly will prevent
# setup from updating it.

# The copy in your home directory (~/.bashrc) is yours, please
# feel free to customise it to create a shell
# environment to your liking.  If you feel a change
# would be benificial to all, please feel free to send
# a patch to the cygwin mailing list.

# User dependent .bashrc file

# If not running interactively, don't do anything
[[ "$-" != *i* ]] && return

# Shell Options
#
# See man bash for more options...
#
# Don't wait for job termination notification
# set -o notify
#
# Don't use ^D to exit
# set -o ignoreeof
#
# Use case-insensitive filename globbing
# shopt -s nocaseglob
#
# Make bash append rather than overwrite the history on disk
# shopt -s histappend
#
# When changing directory small typos can be ignored by bash
# for example, cd /vr/lgo/apaache would find /var/log/apache
# shopt -s cdspell

# Completion options
#
# These completion tuning parameters change the default behavior of bash_completion:
#
# Define to access remotely checked-out files over passwordless ssh for CVS
# COMP_CVS_REMOTE=1
#
# Define to avoid stripping description in --option=description of './configure --help'
# COMP_CONFIGURE_HINTS=1
#
# Define to avoid flattening internal contents of tar files
# COMP_TAR_INTERNAL_PATHS=1
#
# Uncomment to turn on programmable completion enhancements.
# Any completions you add in ~/.bash_completion are sourced last.
# [[ -f /etc/bash_completion ]] && . /etc/bash_completion

# History Options
#
# Don't put duplicate lines in the history.
# export HISTCONTROL=$HISTCONTROL${HISTCONTROL+,}ignoredups
#
# Ignore some controlling instructions
# HISTIGNORE is a colon-delimited list of patterns which should be excluded.
# The '&' is a special pattern which suppresses duplicate entries.
# export HISTIGNORE=$'[ \t]*:&:[fb]g:exit'
# export HISTIGNORE=$'[ \t]*:&:[fb]g:exit:ls' # Ignore the ls command as well
#
# Whenever displaying the prompt, write the previous line to disk
# export PROMPT_COMMAND="history -a"

# Aliases
#
# Some people use a different file for aliases
# if [ -f "${HOME}/.bash_aliases" ]; then
#   source "${HOME}/.bash_aliases"
# fi
#
# Some example alias instructions
# If these are enabled they will be used instead of any instructions
# they may mask.  For example, alias rm='rm -i' will mask the rm
# application.  To override the alias instruction use a \ before, ie
# \rm will call the real rm not the alias.
#
# Interactive operation...
# alias rm='rm -i'
# alias cp='cp -i'
# alias mv='mv -i'
#
# Default to human readable figures
# alias df='df -h'
# alias du='du -h'
#
# Misc :)
# alias less='less -r'                          # raw control characters
# alias whence='type -a'                        # where, of a sort
# alias grep='grep --color'                     # show differences in colour
# alias egrep='egrep --color=auto'              # show differences in colour
# alias fgrep='fgrep --color=auto'              # show differences in colour
#
# Some shortcuts for different directory listings
# alias ls='ls -hF --color=tty'                 # classify files in colour
# alias dir='ls --color=auto --format=vertical'
# alias vdir='ls --color=auto --format=long'
# alias ll='ls -l'                              # long list
# alias la='ls -A'                              # all but . and ..
# alias l='ls -CF'                              #

# Umask
#
# /etc/profile sets 022, removing write perms to group + others.
# Set a more restrictive umask: i.e. no exec perms for others:
# umask 027
# Paranoid: neither group nor others have any perms:
# umask 077

# Functions
#
# Some people use a different file for functions
# if [ -f "${HOME}/.bash_functions" ]; then
#   source "${HOME}/.bash_functions"
# fi
#
# Some example functions:
#
# a) function settitle
# settitle () 
# { 
#   echo -ne "\e]2;$@\a\e]1;$@\a"; 
# }
# 
# b) function cd_func
# This function defines a 'cd' replacement function capable of keeping, 
# displaying and accessing history of visited directories, up to 10 entries.
# To use it, uncomment it, source this file and try 'cd --'.
# acd_func 1.0.5, 10-nov-2004
# Petar Marinov, http:/geocities.com/h2428, this is public domain
# cd_func ()
# {
#   local x2 the_new_dir adir index
#   local -i cnt
# 
#   if [[ $1 ==  "--" ]]; then
#     dirs -v
#     return 0
#   fi
# 
#   the_new_dir=$1
#   [[ -z $1 ]] && the_new_dir=$HOME
# 
#   if [[ ${the_new_dir:0:1} == '-' ]]; then
#     #
#     # Extract dir N from dirs
#     index=${the_new_dir:1}
#     [[ -z $index ]] && index=1
#     adir=$(dirs +$index)
#     [[ -z $adir ]] && return 1
#     the_new_dir=$adir
#   fi
# 
#   #
#   # '~' has to be substituted by ${HOME}
#   [[ ${the_new_dir:0:1} == '~' ]] && the_new_dir="${HOME}${the_new_dir:1}"
# 
#   #
#   # Now change to the new dir and add to the top of the stack
#   pushd "${the_new_dir}" > /dev/null
#   [[ $? -ne 0 ]] && return 1
#   the_new_dir=$(pwd)
# 
#   #
#   # Trim down everything beyond 11th entry
#   popd -n +11 2>/dev/null 1>/dev/null
# 
#   #
#   # Remove any other occurence of this dir, skipping the top of the stack
#   for ((cnt=1; cnt <= 10; cnt++)); do
#     x2=$(dirs +${cnt} 2>/dev/null)
#     [[ $? -ne 0 ]] && return 0
#     [[ ${x2:0:1} == '~' ]] && x2="${HOME}${x2:1}"
#     if [[ "${x2}" == "${the_new_dir}" ]]; then
#       popd -n +$cnt 2>/dev/null 1>/dev/null
#       cnt=cnt-1
#     fi
#   done
# 
#   return 0
# }
# 
# alias cd=cd_func

if [[ `uname -o` == "Cygwin" ]]; then
  # Windows has its own find.exe , so i rename the cygwin exe into findu
  FIND_EXE=findu
else  
  FIND_EXE=find
fi  

###
# common
###

export EDITOR=vim

alias m=make
alias e=vim 
alias lsd='ls -al | grep ^d'
alias lsa="ls -al"

alias diffsrc="diff -rpu"

###
# git
###

alias gitls="git ls-files"

alias gitdiffvim="git difftool --tool=vimdiff --no-prompt"

alias gitmerge-in="git merge -X ignore-all-space --squash "

alias gitmerge-out="git merge -X ignore-all-space "

alias gitreset="git reset --hard HEAD"

alias gitcherry="git cherry-pick -X ignore-all-space -x"

alias glog="git log --decorate=full"

alias gb="git branch"

alias gl="git log --pretty=format:\"%h %s\" --graph"

function gpush()
{
  if [[ "x$1" == "x-h" ]]; then
    cat <<EOF
Usage: gpush
git: for current branch: push changes to remote branch;
EOF
  else 
    set -x
    local bname=`git rev-parse --abbrev-ref --symbolic-full-name @{u} | sed -e "s#/# #"`
    git push ${bname}
    set +x
  fi
}

function gpull()
{
  if [[ "x$1" == "x-h" ]]; then
    cat <<EOF
Usage: gpull
git: for current branch: pull changes from
EOF
  else 
    set -x
    local bname=`git rev-parse --abbrev-ref --symbolic-full-name @{u} | sed -e "s#/# #"`
    git pull ${bname}
    set +x
  fi
}

function grename()
{
  if [[ "x$1" == "x-h" || "x$1" == "x" ]]; then
    cat <<EOF
Usage: grename <new_branch_name>
git: rename current branch to <new_branch_name> and set upstream tracking info to origin/<no_branch_name>
EOF
  else 
    set -x
    git branch -m $1
    git branch --set-upstream origin/$1
    set +x
  fi
}


function garchive()
{
  if [[ "x$1" == "x-h" || "x$1" == "x" ]]; then
    cat <<EOF
Usage: garchive <archive-name>
create zip archive of the current branch into <archive-name>
EOF
  else 
    local oname=$1
    set -x
    local bname=$(git branch | grep -F "*" | sed -e 's#^*##')
    #local bname=`git rev-parse --abbrev-ref --symbolic-full-name @{u} | sed -e "s#/# #"`
    git archive --format zip --output ${oname} ${bname}
    set +x
  fi
}



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

# show aliases and functions
# might not work find because of all the functions declared before .bashrc 

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

#no bell on completion: the next line must be in ~/.inputrc - does not work otherwise
#set bell-style none

PATH=$PATH:/var/lib/gems/1.8/bin/
