#!/usr/bin/perl

use strict;


my %options;

sgetopt( [ 'h', 'a', 'b', 'c', 'd:', 'e:', 'f:' ],  \%options );


if (defined $options{'h'} ) {
  print <<EOF
test command line parser equivalent to Std::getopt('habcd:e:f:',\\%result);
The problem here is that not all installations of perl have Std module installed.
so we do a quick equivalent here.

instead of getopt('habcd:e:f:',\\%result) write

sgetopt( [ 'h', 'a', 'b', 'c', 'd:', 'e:', 'f:' ], \\%result);

EOF
;
  exit(1);
}

my $opt;
foreach $opt (keys %options) {
  print "Option $opt value: " . $options{ $opt } . "  \n";
}

# ----------------------------------------------------------------------------
sub sgetopt
{
   my $opts = shift;
   my $resref = shift;
   my ($i,$argval,$optval,$has_value);

   for($i=0; $i < scalar(@ARGV) ;  ) {
      $argval = $ARGV[ $i++ ];
      next if (! substr($argval,0,1) eq "-" && length($argval) == 2);
      $argval = substr($argval,1);
      $has_value = 0;
      foreach $optval (@{ $opts }) {
	   if ($argval eq substr($optval,0,1)) {
	      $has_value = 1;
	      if (index($optval,':') != -1) {
		 if ($i >=  scalar(@ARGV)) {
	           print "ERROR: option -$argval must have a value\n";
		   exit(1);
		 }
		 $has_value = 1;
	         ${ $resref } { $argval } = $ARGV[ $i ++ ];
	      } else {
                 ${ $resref } { $argval } = 1;
	      }
	   }
       }
       if (! $has_value) {
	 print "ERROR: Command line option -$argval is not defined\n";
	 exit(1);
       }
   }
}

