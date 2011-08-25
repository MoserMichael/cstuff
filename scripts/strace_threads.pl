#!/usr/bin/perl



$in_file=$ARGV[0];


printHelp() if (! -f $in_file);



$threads=`awk '{ print \$1 }' $in_file | sort | uniq`;


@th = split(" ",$threads);

for $t (@th) {
  system("grep \"\^$t\" ${in_file} >${in_file}_thread_${t}");
}




sub printHelp
{
  print <<EOF

./strace_threads.pl <file>

The input file is an log file created by strace with -f option.
This file is split up into multiple files

<file>_<processid>




EOF
;
exit(1);
}
