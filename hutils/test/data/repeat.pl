#!/usr/bin/perl


if (!defined($ARGV[0])) {
  print <<EOF
Usage:
    ./repeat.pl <times> [<char>]
Description
    repeats the input character <char> for a number of <times>
EOF
;

}

$repeat = scalar($ARGV[0]);

$nchar = '1';
if (defined scalar($ARGV[1])) {
  $nchar = substr($ARGV[1],0,1);
}

for($i=0; $i<$repeat; $i++) {
  print $nchar;
}
