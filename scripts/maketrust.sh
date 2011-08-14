#!/bin/bash

LOG_FILE="/dev/null"


function redirect
{
  exec 6>&1
  exec > ${LOG_FILE}  2>&1
  set -x
  HAS_REDIRECT=on
}

function remove_password_from_log
{
  sed -e s#${PASSWORD}#XXXX#g <${LOG_FILE} >${LOG_FILE}.1
  mv -f ${LOG_FILE}.1 ${LOG_FILE}
}


function undo_redirect
{
  if [ "x$HAS_REDIRECT" = "xon" ]; then
    set +x
    exec 1>&6 6>&-
    #remove_password_from_log
   fi
}

function usage
{
  local msg=$1

  undo_redirect
  if [ "x$msg" != "x" ]; then
     cat <<EOF

Error: $msg

EOF
  fi

cat <<EOF
./maketrust.sh  -m <hostname> -u <user> [ -p <password> | -P | -d]

Script for creating automatic login relationship between current account and
account <user> specified by option -u on host <hostname> specified by option -m

If the command succeeds then SSH/SCP/SFTP commands part of OpenSSH will not require a password on host <hostname> when used from current user on this machine.

The password:
 can either be entered by value with -p <password> parameter;
 if -P specified then it is read from stdandard input.


This script automates the process described here
http://www.csua.berkeley.edu/~ranga/notes/ssh_nopass.html
and

EOF
   exit 1
}

function copy_to_remote_host
{
   local ssh_uh=$1
   local local_file=$2
   local remote_file=$3

   cat <<EOF | expect 
set timeout -1
$EXPECT_LOG

spawn scp -o StrictHostKeyChecking=no ${local_file} ${ssh_uh}:${remote_file}

expect "assword:" {
      send -- "${RPASSWORD}\r"

      expect -re  "100%" {
          exit 0
      }
}
EOF

  return $? 
}


function run_ssh_cmd 
{
   local ssh_uh=$1
   local rdir=$2
   local cmd=$3
   local eof_pattern=$4

   cat <<EOF | expect 
set timeout -1
$EXPECT_LOG

spawn ssh -o StrictHostKeyChecking=no ${ssh_uh} 

expect "assword:" {
      send -- "${RPASSWORD}\r"

      expect -re  "(.*)\n" {
         sleep 3

         send -- "cd ${rdir};${cmd};RT=\\\$\?;echo '${EOF_PATTERN}';echo \"rt: \\\$RT\"\r"

         expect "${EOF_PATTERN}" {

            expect -indices -re "rt: (\[0-9\]+)" {

	       set rval "\$expect_out(1,string)"
               set nval [expr \$rval]
	       exit \$nval
            }
	 }    
      }
}
EOF

  return $? 
}

function add_line_to_config
{
  local file=$1
  shift
   
  if [ ! -f $file ] ||  !  grep -F "$@" $file  ; then
    cat <<EOF >>$file

$@
EOF
fi

}

function trust_me
{

  local local_install_dir=~/.ssh/ident/${USERNAME}@${HOSTNAME}/${RUSERNAME}@${RHOSTNAME}
  local lfile=$local_install_dir/id_dsa

  if [ -f $lfile ]; then
    usage "it seems that a key already exists for access from ${USERNAME}@${HOSTNAME} to ${RUSERNAME}@${RHOSTNAME}"
  fi

  local tmp_dir=.tmpdir.$$
  
  mkdir $tmp_dir
  pushd $tmp_dir
  
  KEY_PAIR_NAME="id_dsa"

  ssh-keygen -t dsa -f ${KEY_PAIR_NAME}  -P '' || usage "Failed to generate dsa key pair"
  if [ "x$?" != "x0" ]; then
      usage "Failed to generate ssh key (ssh-keygen failed)"
  fi
   
  TMP_FILE=".tmpfile_$$.sh"

  local pkey

  pkey=`cat ${KEY_PAIR_NAME}.pub`
  
  cat >$TMP_FILE <<EOF
#!/bin/sh 

if [ ! -d ~/.ssh ]; then
   mkdir ~/.ssh
fi

chmod 0700 .ssh
cd ~/.ssh
cat <<EOFF >>authorized_keys2
$pkey
EOFF
if [ ! -f authorized_keys ]; then
   ln -sf authorized_keys2 authorized_keys
fi
chmod 0600 authorized_keys2

#rm -f ${TMP_FILE}
echo "trustMe"
EOF
 
  copy_to_remote_host ${RUSERNAME}@${RHOSTNAME} $TMP_FILE  || die "Failed to copy installation script to remote machine"
  
  run_ssh_cmd ${RUSERNAME}@${RHOSTNAME} "~" "/bin/sh $TMP_FILE" "trustMe" || die "failed to run installation script"
 

  mkdir -p $local_install_dir
 
  cp -f id_dsa*  $local_install_dir
 
  local_install_dir=`dirname  $local_install_dir`
  chmod 0700 $local_install_dir
  local_install_dir=`dirname  $local_install_dir`
  chmod 0700 $local_install_dir
  local_install_dir=`dirname  $local_install_dir`
  chmod 0700 $local_install_dir
  local_install_dir=`dirname  $local_install_dir`
  chmod 0700 $local_install_dir


  pushd ~/.ssh
  add_line_to_config config  "IdentityFile ~/.ssh/ident/%u@%l/%r@%h/id_dsa"
  add_line_to_config config  "IdentityFile ~/.ssh/id_rsa"
  add_line_to_config config  "IdentityFile ~/.ssh/id_dsa"

  chmod  0600 config
  popd

  popd
  #rm -rf $tmp_dir

}



while getopts "hdPp:u:m:" OPT 
do
	case $OPT in
	    h)
		usage
		;;
	    u)
		RUSERNAME=$OPTARG
		;;
	    p)
		RPASSWORD=$OPTARG
		;;
	    P)
		read RPASSWORD
		;;
	    m)
		RHOSTNAME=$OPTARG
		;;
	    d)
		LOG_FILE="./maketrust.log"
		EXPECT_LOG="log_file -a \"${PWD}/${LOG_FILE}.expect\""
		;;
	 esac
done

if  [[ -z $RUSERNAME ]] || [[ -z $RPASSWORD ]] || [[ -z $RHOSTNAME ]]
then
     usage "Missing command line argument"
fi

redirect


trust_me

undo_redirect

echo "*** Completed. try to ssh ${RUSERNAME}:${RHOSTNAME} ; should complete without password ***"

