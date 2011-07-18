#!/usr/bin/perl

use strict;

my ($error_log,@shared_sections,@frames,$state,@errors);

$error_log = $ARGV[0];

parse_log_file();


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

      if (hex($ip_start) < hex($ip) && hex($ip_end) > hex($ip)) {
        $have_source_of_ip = 1;
        print "ip $ip from $path\n";
    	
	if (-f $path) {
	  my $addr = sprintf("%lx", hex($ip) + hex($offset) );
#	  my $cmd = "addr2line -e $path -C -i -p -f -s $addr"; 
	  my $cmd = "addr2line -e $path -C -i -f -s $addr"; 
	  my $res = `$cmd`;
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


