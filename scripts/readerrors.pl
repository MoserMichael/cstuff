#!/usr/bin/perl

use strict;

my ($error_log,@shared_sections,@frames,$state,@errors);

$error_log = $ARGV[0];
if (! -f $error_log) {
  print_usage();
}

parse_log_file();


sub print_usage
{
  print <<EOF
readerrors.pl <error_report_file>

Parses an error report file and extracts stack trace reports; the stack traces
list stack frame addresses and shared module load addresses. The stack frames
are translated into function names.


A raw stack trace in the error report looks as follows

**** start stack ***
frame 0 ip: 0x8048520
frame 1 ip: 0x80485a0
frame 2 ip: 0x80485a0

08048000-08049000 00000000 /home/mmoser/archive/cstuff_bak/GNU-Linux-2.6.38-8-generic-i686/test/tstopf
40000000-4001c000 00000000 /lib/i386-linux-gnu/ld-2.13.so
4001e000-4001f000 00000000 [vdso]
40021000-40023000 00000000 /home/mmoser/archive/cstuff_bak/GNU-Linux-2.6.38-8-generic-i686/lib/libstack.so
40035000-4018f000 00000000 /lib/i386-linux-gnu/libc-2.13.so
*** eof stack ***


it is translated as follows:


ip 0x8048520 from /home/mmoser/archive/cstuff_bak/GNU-Linux-2.6.38-8-generic-i686/test/tstopf
crash_me
test.c:10

ip 0x80485a0 from /home/mmoser/archive/cstuff_bak/GNU-Linux-2.6.38-8-generic-i686/test/tstopf
fact
test.c:30


ip 0x8048633 from /home/mmoser/archive/cstuff_bak/GNU-Linux-2.6.38-8-generic-i686/test/tstopf
fact
test.c:38


EOF
;
exit(1);
}

sub parse_log_file
{

  my ($state, $line);

  open( INFILE, $error_log ) || die "can't open $error_log";

  $state = 0;
  while( <INFILE> ) {

    $line = $_;
    chomp($line);

    if ($state == 0) {
	 if ($line =~ /ERROR:/) {
	    push (@errors, $line );
	}


	    if (index($line,"*** start stack ***") != -1) {
	   $state = 1;
	}
    } elsif ($state == 1) {
	if ($line =~ /frame (\d+) ip: (\w+)/) {
	  my $frame = $1;
	  my $ip = $2;
	
	  push(@frames,$ip);
	} else {
	  $state = 2;
        }
    } elsif ($state == 2) {

	if ($line =~ /(\w+)-(\w+) (\w+) (\S+)/) {
           my $start_addr = $1;
	   my $end_addr = $2;
	   my $offset = $3;
	   my $path = $4;

	   push ( @shared_sections,[ $start_addr, $end_addr, $offset, $path ]);
	}
       
        if (index($line,"*** eof stack ***") != -1) {
	  process_stack_trace();
	
	  @shared_sections = ();
          @frames = ();
          @errors = ();
	  $state = 0;
	}
    }


  }
}

sub process_stack_trace
{
  my ($ip,$r);
  
  print "------------------------------\n";
  print join("\n", @errors);
  print "\n-----------------------------\n";

  for $ip (@frames) {
    
    my  $have_source_of_ip = 0;

    for $r (@shared_sections) {

      my $ip_start = @{ $r } [0];
      my $ip_end =  @{ $r } [1]; 
      my $offset = @{ $r } [2];  
      my $path = @{ $r } [3];  

      if (hex($ip_start) <= hex($ip) && hex($ip) < hex($ip_end)) {
        $have_source_of_ip = 1;
        print "ip $ip from $path\n";
    	
	if (-f $path) {
	  my ($addr, $cmd, $res);

          $addr = sprintf("0x%lx", hex($ip) );
	  $cmd = "addr2line -e $path -C -i -f -s -p $addr"; 
	  $res = `$cmd`;

	  if (index($res,"??") != -1) {
	    $addr = sprintf("0x%lx", hex($ip) - hex($ip_start) );
	    $cmd = "addr2line -e $path -C -i -f -s $addr"; 
	    $res = `$cmd`;
	  }	    

#	  print($cmd."\n");
	  if ($?) {
	    print "\tError: addr2line failed\n";
	  } else {
	    print "$res\n";
	  }
	} else {
	  print "\tError: file $path does not exist on machine\n";
	}
      } 
    }

    if (! $have_source_of_ip) {
       print "\tERROR: ip $ip is not contained in any shared section. sorry\n";
    }
  }

}


