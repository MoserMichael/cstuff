#!/usr/bin/perl

use strict;
use File::Find;
use Cwd 'abs_path';
use Getopt::Std;

our ($dir, @list_of_files, %digest_to_file, $DEBUG, $REMOVE);

getopts("hrd:");
our ($opt_h, $opt_d, $opt_r);


if ($opt_h || $opt_d eq "")
{
	print <<EOFFF
./findrepeatedfiles.pl -d <dir> [ -r ]

Searches for all files under directory <dir>; if there are two equal files then

give option -r , only the first file is kepts, all other repetitions are deleted.
without option -r , file names of repeated files are displayed.

i use it to remove repeting pictures in my picture archive.

Method: computes the sha1 digest of all files; if two digests are equal then compare files, 
this way it is known for sure that files are different (though collisions are supposedly unlikely)

Dependencies: uses openssl and diff utilities.

EOFFF
;
	exit(1); 	
}

$REMOVE = defined($opt_r) ? 1 : 0;
$DEBUG=0;

print "***find files***\n" if ($DEBUG);

list_files(".");

print "***processing: find repeated files***\n" if ($DEBUG);

process_files();

print "***finished***\n" if ($DEBUG);

sub add_file
{
  if (-f $_)
  {
    my $fullp=abs_path($_);
    print "${fullp}\n" if ($DEBUG);    
    push(@list_of_files, $fullp);
  }
}

sub list_files
{
  my $dir  = shift;
  find(\&add_file,$dir);
}

sub process_files
{
   my ($f, $res, $idx, %hash_to_file);

   for $f (@list_of_files) {

    	$res = `openssl dgst -sha1 $f`;
 	if ($? != 0) {
 	    print("Can't read file $f\n");
 	    next;
	}

	$idx = index( $res, ")=");
	if ($idx != -1) {
	   $res = substr($res, $idx + 3);
	}	
 	chomp($res);

	print " $f -> $res\n" if ($DEBUG);

	if (defined( $digest_to_file{ $res } )) { # found repetition of the same hash
          my ($isequal, $cmd, $second);

	  $second =  $digest_to_file{ $res };
	 
	  $cmd = "diff $f $second"; # double check that files are really different (no hash collision)
	  $isequal=`$cmd`;
	
 	  if ($isequal != 0) {
 	   	print "Bingo! the digest hash is of the same value but files differ: $f != $second sha1 $res\n";
	  } else {
		print "Equal files $f $second\n";
		if ($REMOVE)
		{
			unlink($f);		     
			print("$f deleted\n");
		}	
	  }
        }
	$digest_to_file{ $res }  = $f;	
   }
}

