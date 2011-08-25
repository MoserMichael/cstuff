#!/usr/bin/perl


use strict;
use Getopt::Std;

my (%options,$in_file,$out_file);

getopts("hi:o:A:B:",\%options);


if (defined $options{'h'}) { 
  printHelp();
}

if (!defined $options{'i'}) {
  printHelp("Missing -i options");
}
$in_file = $options{'i'};

if (!defined $options{'o'}) {
  printHelp("Missing -o options");
}
$out_file = $options{'o'};





filterIt();


sub filterIt
{
   my ($tmp, $state, $has_end_t,  @start_t, @end_t);

 

   if ( defined $options{'A'}) {
  

     $tmp = $options{'A'};
     if (! $tmp =~ /(\d\d):(\d\d):(\d\d)/ ) {
       printHelp( "-A option. wrong time format, shoud be HH:MM:SS");
     }

     $tmp =~ /(\d\d):(\d\d):(\d\d)/;
     @start_t = ( $1, $2, $3 );

  
     $state = 0;

   } else {
     $state = 1;
   } 
   
   $has_end_t = 0;
   if ( defined $options{'B'}) {
   
     
     $has_end_t = 1;

     $tmp = $options{'B'};
     if  (! $tmp =~ /(\d\d):(\d\d):(\d\d)/ ) {
       printHelp( "-A option. wrong time format, shoud be HH:MM:SS");
     }
     
     $tmp =~ /(\d\d):(\d\d):(\d\d)/;
     @end_t = ( $1, $2, $3 );
     

   } else {
     if ($state == 1) {
	printHelp("At leas either -A or -B option must be defined");
     }
  }
 


   open(INFILE,"$in_file") || printHelp("Can't open file $in_file, wrong -i option");
   open(OUTFILE,">$out_file") || printHelp("Can't open file $out_file ");

   my (@toks,$hour,$min,$sec,$msec,$line);

   while(<INFILE>) {

      $line = $_;
      
      @toks = split(" ",$_);
    
      if ($toks[1] =~ /(\d\d):(\d\d):(\d\d)\.(\d+)/) {
          $hour = $1; 
          $min  = $2;
          $sec  = $3;
          $msec = $4;
      }           	


      if ($state == 0) {
	 # throw out all what is less then $start_t time.
         next if (int($hour) < int($start_t[0])); 
	 next if (int($min)  < int($start_t[1]));
         next if (int($sec)  < int($start_t[2]));
	 
	 $state = 1;	
     }

     if ($state == 1) {
         if ($has_end_t == 1) {

	   last if (int($hour) > int($start_t[0]));
	   last if (int($min)  > int($start_t[1]));
           last if (int($sec)  > int($start_t[2]));
         }


	
	print OUTFILE $line;
     }
	
   }
   close(OUTFILE);
   close(INFILE);

}


sub printHelp
{
  my $error = shift;

  print "\nERROR: $error\n\n" if (defined $error);

   print <<EOF
./strace_filter.pl [-A <start time>] [-B <end time>] -i <input_strace_log> -o <output_strace_log>

The utility takes an strace log produced by strace, and filters out all entries
that fall into a given time range. (Either -A or -B option must be specified).

Strace must be used with both -f and -tt options (!!!) , if this script is to work.




-i <input strace log>
    Input file - the large log produced by strace with -tt options'

-p <output strace log>
    output file.

-A <start time>

    Saves only the lines whose timestamp is on or after start time. The time is given in the following format HH:MM:SS
-B <stop time>

    Saves only the lines whose timestamp is before stop time. The time is given in the following format HH:MM:SS
-h

EOF
;
  exit(1);

}
