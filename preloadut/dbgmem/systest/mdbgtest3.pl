#!/usr/bin/perl

use POSIX ":sys_wait_h";


test_engine("simple");
test_engine("check");

sub test_engine
{
    $rdir = $ARGV[0];
    $eng  = shift;
    $bdir = $ENV{'BIN_ROOT_DIR'};
    $log  = "report-${eng}.log";

    $pid_file="test3.pid";
    system("rm -f $pid_file report-${eng}.log");

    $cmd = "${rdir}/run -v -d $eng -f ${log} ${bdir}/test/mdbgtest3";

    my $__FILE__ = __FILE__;

    print <<EOF

***
Tst: $__FILE__
Eng: $eng
Log: $log
Cmd: $cmd
***

EOF
;
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
    print("*** test eof, see $log ***\n");

    unlink($pid_file);
}

