#!/usr/bin/perl

use POSIX ":sys_wait_h";

print("\n*** mdbgtest3.pl ***\n");

$rdir = $ARGV[0];
$bdir = $ENV{'BIN_ROOT_DIR'};

$pid_file="test3.pid";
system("rm -f $pid_file report.log");

$cmd = "${rdir}/run -v -d simple -f report.log ${bdir}/test/mdbgtest3";  
#system($cmd);
#exit(0);


$pid = fork();

if ($pid == 0) {
    print("Running: $cmd\n");
    exec( $cmd );
    print("!Exec failed!\n");

}

while(! -f $pid_file) {
}

sleep( 3 );

system("kill -2 `cat ${pid_file}`");
system("rm -f ${pid_file}");
print("*** wait for report to be written ***\n");
waitpid($pid,0);
print("*** test eof, see report.log ***\n");

