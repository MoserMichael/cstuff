#!/usr/bin/perl


use strict;


my (@tests, $test, $test_tool, $tnames );

@tests = list_parts( $ARGV[0] );
$test_tool = $ARGV[1];


$tnames = join(" ",@tests);

print <<EOF

==================
Running tests
  - Using: $test_tool
  - Tests: $tnames 
==================

EOF
;
test_it( $ARGV[1] );


sub show_result
{
  my $test = shift;
  
  print "\n---Test: ${test} (status: $?)---\n"; 
  system("cat ${test}.out");
  print "---Eof Test---\n"; 
}


sub test_it
{
  my $ttol = shift;
  my $failed = 0;

  for $test (@tests) {
    print "Testing $test ... ";
    
    system("$ttol $test >${test}.out 2>&1");

    my $res = $?;

    show_result( $test );

    my @tspec = get_test_spec( $test );
        	
    if ($tspec[0] eq "ok") {
	if (scalar($res) != 0) {
	   print "Failed !\n\tactual exit status $res expected to be zero\n";
           $failed ++;
 	   next;
	}
    } 
    if ($tspec[0] eq "fail") {
	if (scalar($res) == 0) {
	   print "Failed !\n\texit status is 0 expected to be not zero.\n";
           $failed ++;
	   next;
	}
    }
    print "ok\n";

    if ($tspec[1] ne "ignore") {

      open(OUTFILE,"${test}.out");
      my @lines=<OUTFILE>;
      my $text = join("",@lines);
      close(OUTFILE);

      my @slines = <SPECFILE>;
      my $stext = join("",@slines);
      close(SPECFILE);
	
      if ($stext ne $text) {
	print "FAILED\n\tActual output differs from expected\n";
        $failed ++;
      }
    }
  }
}


# 
#
#

sub get_test_spec
{
   my $tfile = shift;
   my ($idx, $tspecfile, $first_line, $tok );

   $idx = rindex($tfile,'.');

   $tspecfile =  substr($tfile, 0, $idx ) . ".tspec";

   if (! -f $tspecfile) {
      return split(" ","ok ignore");
   }
   
   open(SPECFILE,$tspecfile) || die "Can'topen $tspecfile";
   $first_line=<SPECFILE>;
   

   return split(" ",$first_line);
}

sub list_parts
{
   my $test_dir = shift;
   my ($dh, @result, @entries);

   opendir($dh,$test_dir);
   @entries = grep { /(\d*)-(.*)\.p$/ } readdir($dh);
   closedir($dh);


   @result = sort { 
	my ( $a_num, $a_part, $b_num, $b_part, $rt ); 

	$a =~ /(\d+)-([^\.]+)/;  

	$a_num = $1;
	$a_part = $2;

	$b =~ /(\d+)-([^\.]+)/;

	$b_num = $1;
	$b_part = $2;


	$rt = $a_num <=> $b_num;
	if ($rt == 0) {
	  return $a_part cmp $b_part;
	} 
 	return $rt;

   } @entries;

   @result = map { "$test_dir/$_" } @result;

   return @result;
}


