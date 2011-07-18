#!/usr/bin/perl

$infile=shift;

$outfile=$infile;
$pos=rindex($infile,".");
if ($pos != -1) {
  $outfile = substr($outfile, 0, $pos );
}
$outfile="$outfile.c";


if ($infile eq "") {
  print "Missing input file argument <infile>\n";
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
