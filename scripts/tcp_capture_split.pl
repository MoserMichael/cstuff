#!/usr/bin/perl

use strict;
use Getopt::Std;
use File::Path qw(make_path);

my ($VERBOSE,$INCAPTURE,$OUTDIR,$START_TIME,$END_TIME);


check_args();
check_env();
do_filter();    


sub print_help
{
    my $msg = join(' ', @_);
 
    $msg = "\nERROR: $msg\n\n" if ($msg ne "");

	

    print <<EOF
$msg

tcp_capture_split.pl [-s <start_date>] [-e <end_date>] -f <capture file> -o <output directory>

This utility helps with analysing Wireshark and TCPDUMP capture files (libpcap format)

Each TCP connection is dumped into a separate file in the output directory.
Prior to this we can optionally limit this analysis to specfied time range.

Please note that this utility works only if the wireshark product is in the current PATH;
tshark and editcap programs are used here.

Please also note that tcpdump/wireshark packet sniffers are not guaranteed to capture -all- traffic.

Options:

  -f <capture file>	input capture file
  -o <output dir>	output directory

  -v			verbose output

  -s <date/time>	filter out packets that arrived later than this date/time (optional)
  -e <date/time>	filter out packets that arrived earlier than this date/time (optional)

  Date time format:
	YYYY-MM-DD HH:MM:SS
	Please note that all dates are in UTC time zone.



EOF
;

    exit(1);
}


sub check_args
{ 
  my %options;


  getopts("hvf:s:e:o:",\%options);

  print_help() if(exists $options{'h'});

  print_help("Input capture file $options{'f'} does not exist") if (! -f $options{'f'});

  if (! -d $options{'o'}) {
    #`mkdir -p $options{'o'}`; || print_help("Can't create output directory $options{'o'}");
    make_path( $options{'o'} );
  }

  $VERBOSE=1 if (exists $options{'v'});

  $INCAPTURE=$options{'f'};
  $OUTDIR=$options{'o'};

  $START_TIME=$options{'s'};
  $END_TIME=$options{'e'};
}

sub check_env
{
      system("which tshark");
      if ($? != 0) {
	print_help("tshark is not in the current path - please install wireshark");
      }

      system("which editcap");
      if ($? != 0) {
	print_help("editcap is not in the current path - please install wireshark");
      }
}

sub do_filter
{
       my $tmp_file_capture = "$OUTDIR/capture.dump";
       my $tmp_file_conv = "$OUTDIR/conv.txt";

       my $opts;

       if ($START_TIME ne "") {
	 $opts=" -A $START_TIME ";
       }
       if ($END_TIME ne "") {
	 $opts="$opts -B $END_TIME";
       }

       run_cmd("editcap $opts $INCAPTURE $tmp_file_capture");
       run_cmd("tshark -r $tmp_file_capture -q -z conv,tcp -n >$tmp_file_conv");

       my $cnt;
       $cnt = 1;

       open(IN,$tmp_file_conv);
       while( <IN> ) {
	 my $line = $_;
	 chomp( $line);

	 my @tok = split( " ",$line);
	 next if ($tok[1] ne "<->");

	 my ($src_ip,$src_port,$trg_ip,$trg_port,$cmd);
	 

	 if (  $tok[0] =~ /([^:]*)\:(\S*)/ ) {
	   $src_ip = $1;
	   $src_port = $2;
	 } else {
	   next;
	 }

	 if (  $tok[2] =~ /([^:]*)\:(\S*)/ ) {
	   $trg_ip = $1;
       $trg_port = $2;
     } else {
       next;
     }

     $cmd="tshark -r $tmp_file_capture -w $OUTDIR/tcp_${cnt}.dump -R \"ip.addr==${src_ip} && tcp.port == ${src_port} && ip.addr==${trg_ip} && tcp.port==${trg_port}\"";

     run_cmd($cmd);
     
     $cnt += 1;	
	
   }
   unlink($tmp_file_capture);
}


sub run_cmd
{ 
  my $cmd=join(" ",@_);
  system($cmd);
  if ($? != 0) {
    print "Command failed, status $?\nCommand: $cmd";
    exit(1);
  }
  print ">$cmd\n" if ($VERBOSE);
}
