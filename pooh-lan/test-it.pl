#!/usr/bin/perl


use strict;


my ($f, @tests, $TEST_TOOL, $LIBDIR, @TEST_ERRORS, $failed, $total );

$failed = $total = 0;

#$ENV{"TEST_VERBOSE"}=1;  #enable dumping of ast.

$TEST_TOOL = $ARGV[0];

$LIBDIR=`dirname $TEST_TOOL`;
chomp($LIBDIR);
$LIBDIR=`dirname $LIBDIR`;
chomp($LIBDIR);
$LIBDIR="$LIBDIR/lib";

$ENV{ 'TEST_TOOL' };
if (defined $ENV{ 'TEST_TOOL' }) {
  $TEST_TOOL = $ENV{ 'TEST_TOOL' } . " " . $TEST_TOOL; 
}


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

    $total = $total - $failed;
    print <<EOF

    Number of tests that failed $failed 
    Number of tests that passed $total
EOF
;
  }
}

sub addFailed
{ 
  my $test = shift;
  push(@TEST_ERRORS,$test);
  $failed ++;
}

sub clean_test_results
{
  my ($dir);

  $dir = shift;

  system("cd ${dir}; rm -f *.ast.1 *.ast.2 *.sout *.serr");
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
  test_it($dir);
}

sub show_result
{
  my $test = shift;
  
  print "\n---Test: ${test} (status: $?)---\n"; 
  #system("cat ${test}.serr");
  print "---Eof Test---\n"; 
}

sub WSIGNAL
{
   my $stat = shift;

   return ($stat & 0x7E00) / 256;
}


sub test_it
{
  my ( $test,  $skipline,$diff); 
 

  for $test (@tests) {
    print "Testing $test ... ";
    
    $total += 1;
   #system("$ttol $test >${test}.out 2>&1");
    print "\n>>$TEST_TOOL $test<<\n";
    system("$TEST_TOOL -I $LIBDIR -I lib -x $test >${test}.sout 2>${test}.serr");

    my $res = $?;


    if (WSIGNAL( $res ) != 0) {
       print "Test Failed !\n\ttest crashed. Signal  " . WSIGNAL( $res ) . " { $res }\n";
       addFailed( "${test} - crashed. Signal " . WSIGNAL( $res )  );
       next;
    }

    show_result( $test );

    my @tspec = get_test_spec( $test );
        	
    if ($tspec[0] eq "ok") {
	if (scalar($res) != 0) {
	   print "Test Failed !\n\tactual exit status $res expected to be zero\n";
	   addFailed( "${test} - test expected to pass, but failed" );
	   next;
	}
    } 
    if ($tspec[0] eq "fail") {
	if (scalar($res) == 0) {
	   print "Test Failed !\n\texit status is 0 expected to be not zero.\n";
	   addFailed( "${test} - test expected to fail, but passed" );
	   next;
	}
    }
    print "Test ok\n";

    if ($tspec[1] ne "ignore" && $tspec[1] ne "ignore-stdout") {

      
      if (-f "${test}.sout.expected") {
      
        $diff=`diff ${test}.sout ${test}.sout.expected`;
 
        if ($? != 0) {
	  print "Test FAILED\n\tActual STDOUT output differs from expected\n";
	  addFailed( "${test} - Actual STDOUT output differs from expected");
          next;
        }
      }
    }  

    if ($tspec[1] ne "ignore" && $tspec[1] ne "ignore-stderr") {

     if (-f "${test}.serr.expected") {
        $diff=`diff ${test}.serr ${test}.serr.expected`;
        if ($? != 0) {
	  print "Test FAILED\n\tActual STDERR output differs from expected\n";
       	  addFailed( "${test} - Actual STDERR output differs from expected"  );
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
     return split(" ","ok ignore-stderr");
#    return split(" ","ok compare");
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


