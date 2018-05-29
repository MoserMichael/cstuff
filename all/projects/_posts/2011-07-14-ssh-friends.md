---
layout: post
title: Script to set up SSH trusted keys
---

{{ page.title }}
----------------

<p class="publish_date">
20 July 2011

</p>
If you ever have to type the same sequence of three commands again, then turn this into a script - that's a general rule on UNIX, or any other systems for that matter were the command line rules.

If you have to run process or copy files to another machine, than you are probably working with [OpenSSH](http://www.openssh.com/) . One can establish a trust relationship between computer A / account A and Computer B / account B, so that ssh will not ask for a password when logging on to Computer B /account B while you are on computer A / account.

This effect is achieved by means of sharing a public / private key pair - hence the name trusted key; Computer A owns the private key, computer B receives the public key; During the logon process, computer A encrypts a message with its private key (only computer A is supposed to have this private key); computer B can use it's public key in order to decrypt and verity the message, so that computer B knows that its computer A who is calling, and not some imposter.

Unfortunately it takes quite a long sequence of commands to set this up correctly, also most of the guides on the Internet that describe the process get it wrong. So it's time for some scripting magic, the script will automate set up of trusted key. First of all the script can be [seen here](https://github.com/MoserMichael/cstuff/blob/master/scripts/maketrust.sh)

    ./maketrust.sh  -m <hostname> -u <user> [ -p <password> | -P | -d | -h]

    This script automates setup of trusted key between the currently logged on
    user account on the current host on the one hand, and the 
    remote <hostname> and <user> account on the other hand.

    If the command succeeds then SSH/SCP/SFTP commands will not require 
    a password when logging on to <user>@<hostname>, while your are logged 
    in on the current user on current host.

    Command line options
        -m Remote host name
        -u Remote user
        -p password of remote machine (-P option is preferable, if you do not want others to 
           see your password with ps command, while this script is running).
        -P prompt the user to enter command via terminal.
        -d (optional) debug option. Creates log files maketrust.log and maketrust.log.expect.

So the process is as follows. Lets say
o $HOSTNAME and $USERNAME are the local host and user names.
o $RHOSTNAME and $RUSERNAME are the remote host and user names.

\* On the local computer the public and private key pair is created

      ssh-keygen -t dsa -f id_dsa -P ''"
             

\* On the local computer installs the public and private key pair

    mkdir -p ~/.ssh/${USERNAME }@${ HOSTNAME}/${RUSERNAME}@${RHOSTNAME}
    mv id_dsa ~/.ssh/${USERNAME }@${ HOSTNAME}/${RUSERNAME}@${RHOSTNAME}
     mv id_dsa.pub ~/.ssh/${USERNAME }@${ HOSTNAME}/${RUSERNAME}@${RHOSTNAME}
        

\* On the local computer: edit the local machine's SSH configuration so that it
will pick up the correct public/private key pair.
Add the following line to ~/.ssh/config file

    IdentityFile ~/.ssh/ident/%u@%l/%r@%h/id_dsa
               

Now the last two steps were very important: SSH will now look for the keys in specific location, and you can install multiple keys without them interfering with one another; This IndentityFile definition will instruct OpenSSH to look for each trusted key in its separate directory: <local user name>`<local host name>/<remote user name>`<remote host name> ; the local host and user name is used here because the home directory may be mounted on an NFS disk, so the same home directory can be used while logging on from different systems (in the general case).
Correction: this trick works only from version 4.4 of OpenSSH on; on previous versions (that is beginning with version 2.5.1.p1 until version 4.4) all you can do is add multiple IdentityFile entries, one for each private key owned by computer A. For these older versions we can have multiple definitions of IdentityFile directive, each one lists a separate trusted key. Now the OpenSSH client will try out each trusted key in sequence, until it either finds a matching key or gives up. Interestingly Redhat keeps old versions of the OpenSSH package even in recent releases; maybe this policy is supposed to mirror that of the Debian stable release - but what am I to know about 'Enterprise software' in general.
Kindly the [setup script](https://github.com/MoserMichael/cstuff/blob/master/scripts/maketrust.sh) takes care of all these details.

\* Set correct permissions (only the current user should be allowed to read from .ssh)

    chmod 0700  ~/.ssh 
    chmod 0700  ~/.ssh/${USERNAME }@${ HOSTNAME}
    chmod 0700  ~/.ssh/${USERNAME }@${ HOSTNAME}/${RUSERNAME}@${RHOSTNAME}
    chmod 0700  ~/.ssh/${USERNAME }@${ HOSTNAME}/${RUSERNAME}@${RHOSTNAME}
    chmod 0600 ~/.ssh/config
    chmod 0600 ~/.ssh/${USERNAME }@${ HOSTNAME}/${RUSERNAME}@${RHOSTNAME}/id_dsa
    chmod 0600 ~/.ssh/${USERNAME }@${ HOSTNAME}/${RUSERNAME}@${RHOSTNAME}/id_dsa.pub

\* On the remote machine
Copy id\_dsa.pub to the remote machine
Append content of id\_dsa.pub to ~/.ssh/authorized\_keys2

    cat id_dsa.pub >>~/.ssh/authorized_keys2

Create the symbolic link

    ln -s ~/.ssh/authorized_keys2  ~/.ssh/authorized_keys

Again set all permissions

    chmod 0700  ~/.ssh 
    chmod 0600  ~/.ssh/authorized_keys
    chmod 0600  ~/.ssh/ authorized_keys2

### Scripting magic

Basically it's a shell script generates the keys and installs them on local computer, then it that writes an installation shell script that contains the public key, later it writes two expect script to copy & run the installation script on another machine. Sort of [I'd rather write programs to write programs than write programs](http://dictionary.die.net/toolsmith)
By making a script out the long wound process, I would see the error that is apparently hidden in others tutorials that cover this subject.

### Portability of shell scripting

Once upon a time you could rest assured that each Linux installation has a bash script shell; now that is no longer true. Ubuntu for instance gave up bash in favor of the dash shell.
Well, I am still sticking with bash though; but any software sort of dies due to sudden changes of its environment; I guess this will true as long as you do not have [real talking and improvising robots](http://en.wikipedia.org/wiki/WALL-E) (Also see my [Wall-E test of intelligent systems](http://mosermichael.github.com/cstuff/all/ramblings/2011/06/18/semantic-nets.html#we) ).