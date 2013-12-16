#!/usr/bin/perl
#
#
#
$rdir = $ARGV[0];
$exe  = $ARGV[1];
$bdir = $ENV{'BIN_ROOT_DIR'};

$cmd ="${rdir}/runinitmem ${bdir}/test/${exe}"; 

print <<EOF

***
Tst: $__FILE__
Cmd: $cmd
***

EOF
;

system($cmd);

$stat = $? >> 8;

if ($stat != 0) 
{
    print "*** test failed ;-( ***\n";
    exit(1);
}

print "*** test passed ;-) ***\n";

