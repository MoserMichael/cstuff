#!/usr/bin/perl
#

test_engine("simple");
test_engine("check");


sub test_engine
{
    $rdir = $ARGV[0];
    $bdir = $ENV{'BIN_ROOT_DIR'};
    $eng =  shift;
    $log = "srvreport-${eng}.log";
    
    $port =  find_free_port(); 
    $cmd = "${rdir}/run -v -d simple -f ${log} -leak 50 51 ${bdir}/test/mdbgleaksrv 127.0.0.1 $port";

    my $__FILE__ = __FILE__;

    print <<EOF

***
Tst: $__FILE__
Eng: $eng
Cmd: $cmd
Log: $log
***

EOF
;
    $pid_file="mdbgleaksrv.pid";
    unlink($pid_file);


    $pid = fork();

    if ($pid == 0) {
	print("Running: $cmd\n");
	exec( $cmd );
	print("!Exec failed!\n");
    }



    # wait for server to start up
    while(! -f $pid_file) {
    }

    # get pid of server
    open(IN,$pid_file);
    $srv_pid = <IN>;
    chomp($srv_pid);
    close(IN);


    # tell it to advance generation
    kill(50, $srv_pid);


    #send some messages

    system("echo 'abcdefg' | nc  127.0.0.1 $port");
    system("echo 'abcdefg 1234 234 34' | nc  127.0.0.1 $port");
    system("echo 'alj ashdf bma tiubfdk bd' | nc  127.0.0.1 $port"); 
    system("echo 'aaaaaaaaaa' | nc  127.0.0.1 $port");
    system("echo 'baaaaaaaaa' | nc  127.0.0.1 $port");
    system("echo 'caaaaaaaaa' | nc  127.0.0.1 $port");
    system("echo -n 'exit' | nc  127.0.0.1 $port");

    print("*** wait for report to be written ***\n");
    waitpid($pid,0);
    print("*** test eof, see ${log} ***\n");

    unlink($pid_file);

}



sub find_free_port
{
    # find listening port
    $listenaddr = `netstat -tln | tail -n +3 | awk '{ print $4 }'`;
    
    $port = 10000;

    while(1) {
      if (index( $listenaddr, ":".$port ) == -1) {
	last;
      }	
      ++$port;
    }

    return $port;
}




