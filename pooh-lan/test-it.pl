#!/usr/bin/perl


use strict;


my ($f, @tests, $TEST_TOOL,@TEST_ERRORS);

$ENV{"TEST_VERBOSE"}=1;  #enable dumping of ast.

$TEST_TOOL = $ENV{ 'TEST_TOOL' };
if ($TEST_TOOL ne "") {
  $TEST_TOOL = "$TEST_TOOL ";
}
$TEST_TOOL = $ARGV[0];

shift(@ARGV);


for $f (@ARGV) {
  last if ($f eq ".");
  run_test_in_dir( $f );
}
show_test_errors();


sub show_test_errors
{
  my $f;
  if (scalar(@TEST_ERRORS) != 0) {
print <<EOF
--------------------
ERRORS ERRORS ERRORS
--------------------
EOF
;
    for $f (@TEST_ERRORS) {
	print "\t$f\n";
    }
  }
}

sub addFailed
{ 
  my $test = shift;
  push(@TEST_ERRORS,$test);
}

sub clean_test_results
{
  my ($dir);

  $dir = shift;

  system("rm -f *.ast *.ast1 *.sout *.serr");
}
 


sub run_test_in_dir
{ 
  my ($dir, $test, $tnames  );

  $dir = shift;
  
  clean_test_results( $dir );
  @tests = list_parts( $dir );

  $tnames = join(" ",@tests);


  print <<EOF

==================
Running tests
  - Directory: $dir
  - Tests: $tnames 
==================

EOF
;
  test_it();
}

sub show_result
{
  my $test = shift;
  
  print "\n---Test: ${test} (status: $?)---\n"; 
  system("cat ${test}.serr");
  print "---Eof Test---\n"; 
}

sub WSIGNAL
{
   my $stat = shift;

   return ($stat & 0x7E00) / 256;
}


sub test_it
{
  my ($failed, $test, $skipline,$diff); 
  
  $failed = 0;

  for $test (@tests) {
    print "Testing $test ... ";
    
   #system("$ttol $test >${test}.out 2>&1");
    print "\n>>$TEST_TOOL $test<<\n";
    system("$TEST_TOOL $test >${test}.sout 2>${test}.serr");


    my $res = $?;


    if (WSIGNAL( $res ) != 0) {
       print "Test Failed !\n\ttest crashed. Signal  " . WSIGNAL( $res ) . "\n";
       addFailed( "${test} - crashed. Signal " . WSIGNAL( $res )  );
       next;
    }

    show_result( $test );

    my @tspec = get_test_spec( $test );
        	
    if ($tspec[0] eq "ok") {
	if (scalar($res) != 0) {
	   print "Test Failed !\n\tactual exit status $res expected to be zero\n";
           $failed ++;
	   addFailed( "${test} - test expected to pass, but failed" );
	   next;
	}
    } 
    if ($tspec[0] eq "fail") {
	if (scalar($res) == 0) {
	   print "Test Failed !\n\texit status is 0 expected to be not zero.\n";
           $failed ++;
	   addFailed( "${test} - test expected to fail, but passed" );
	   next;
	}
    }
    print "Test ok\n";

    if ($tspec[1] ne "ignore") {

      if (-f "${test}.sout.expected") {
        $diff=`diff ${test}.sout ${test}.sout.expected`;
 
        if ($? != 0) {
	  print "Test FAILED\n\tActual STDOUT output differs from expected\n";
          $failed ++;
	  addFailed( ${test} );
          next;
        }
      }

      if (-f "${test}.serr.expected") {
        $diff=`diff ${test}.serr ${test}.serr.expected`;
        if ($? != 0) {
	  print "Test FAILED\n\tActual STDERR output differs from expected\n";
          $failed ++;
       	  addFailed( ${test} );
	}
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


