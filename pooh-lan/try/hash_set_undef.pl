#!/usr/bin/perl


$hash{ "a" } = 1;
$hash{ "b" } = 2;

print "keys;\n";
for $k ( keys %hash ) {
  print "$k\n";
}

print "That means that assigning a undef value to hash does not delete the key !!!\n";
