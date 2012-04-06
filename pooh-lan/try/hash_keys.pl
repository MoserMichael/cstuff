#!/usr/bin/perl

use strict;

my (@key_a, @key_b, %hash );

@key_a = ( 1, 2, 3 );

$hash{ @key_a } = 1;

print "Hash entry for @key_a " . $hash{ @key_b } . "\n";

print "Alter the key_a \n";
push( @key_a, 2 );

@key_b = (1, 2, 3 );
print "Find entry with copy of original key_a " . $hash{ @key_b } . "\n";

print "When value is inserted into hash, a deep copy of the key value is made; ";
