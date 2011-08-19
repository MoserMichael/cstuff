#!/usr/bin/perl                                    	


use strict;

my ($file, $generated_parser_file, @lines, $l, @errors, $state, $state_text, %states, $e, $has_errors);


sub print_usage
{

print <<EOF
Usage:
 ./parse_bison_report.pl <bison-error-report> <generated-parser-file>

 The script parses a verbose bison report, so that all
 shift/reduce reduce/reduce conflicts will become obvious at once.
 As a bonus it deletes the generated parser file if the report contains
 shift/reduce or reduce/reduce conflicts.

 The arguments:
   <the file name of bison report>
   <the file name of the generated parser>

 The bison report is generated as follows:
    bison --report=all --report-file=pars.report     
EOF
;
  exit(1);
}

$file = $ARGV[0];
$generated_parser_file = $ARGV[1];

if (! -f $file || ! -f $generated_parser_file) {
  print_usage();
}

open(INFILE,$file);
@lines=<INFILE>;
close(INFILE);


for $l (@lines) {
   if ($l =~ /State (\d+) conflicts:/) {
      push(@errors, [ $1, $l ]);
   }
}


$state = -1;
for $l (@lines) {

   if ($state == -1) {
	   if ($l =~/^state (\d+)/) {
	   	$state = $1;
		$state_text = "";
	   }
   } else {
 	if ($l =~/^state (\d+)/) {
		$states{ $state } = $state_text;
	   	$state = $1;
		$state_text = "";
	} else {
		$state_text = "$state_text$l";
	}
   }
}
$states{ $state } = $state_text;



$has_errors = 0;
for $e (@errors) {

    my $error_msg = @{ $e } [ 1 ];
    my $error_state = @{ $e } [ 0 ];
    my $error_state_msg = $states{ $error_state };

    print <<EOF
--------------------------------
$error_msg

$error_state_msg

EOF
;

    $has_errors = 1;
}


if ($has_errors != 0) {
  print "*** the grammar has errors. very bad ***\n";
  unlink( $generated_parser_file );
  exit(1);
}
