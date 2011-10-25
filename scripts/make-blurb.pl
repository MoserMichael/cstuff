#!/usr/bin/perl

$infile=shift;

$outfile=$infile;
$pos=rindex($infile,".");
if ($pos != -1) {
  $outfile = substr($outfile, 0, $pos );
}
$outfile="$outfile.c";


if ($infile eq "") {
  print <<EOF
Missing input file argument <infile>

Usage:
    The script takes a text file as argument (infile.txt for example) , and creates a C file infile.c
    The C file consists of the function print_help, which prints out contents of infile.txt
    Used to make the help blurb of command line programs.
EOF
;
  
 exit(1);
}


open(INF,"$infile") || die "Can't open $infile";
open(OTF,">$outfile") || die "Can't open $outfile";

print OTF <<EOF
#include <stdio.h>
#include <stdlib.h>

void print_help() {
  fprintf(stderr,"%s",
EOF
;

while( <INF> ) {
   $line=$_;
   chomp($line);
   $line = rtrim($line);

   if (trim($line) eq "") {
     $line = "";
   }

   $line =~ s/\"/\\\"/g;

   print OTF "\t\"${line}\\n\"\n";

}
print OTF <<EOF
  );
  exit(1);
}  
EOF
;

close(INF);
close(OTF);


sub trim($)
{
	my $string = shift;
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	return $string;
}

sub ltrim($)
{
	my $string = shift;
	$string =~ s/^\s+//;
	return $string;
}

sub rtrim($)
{
	my $string = shift;
	$string =~ s/\s+$//;
	return $string;
}
