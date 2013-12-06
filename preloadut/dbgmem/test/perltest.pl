use Cwd;


my $test_fail_count;
my $this_test_failed;
my $TEST_LOGFILE="test.log";

# --------------------------------------------------------
# Test loop 
# -------------------------------------------------------- 
sub run_tests
{
   my @tests = split(" ",shift);
   my $tst;

   Print_init();
   Print("\n");

   foreach $tst (@tests) {
     $tst_name = $tst." ";
     $tst_name =~ s/\_/ /g;
     
     test_announce( $tst_name );
      
     $this_test_failed = 0;
     
     if (!defined($tst)) {
	Print(" [FAIL]\n\tTest method $tst_name is not defined\n");
	next;
     }
     
     #print "$tst\n";
     $tst->();
    
     if ($this_test_failed) {
       Print(" [FAIL]\n");
     } else {
       Print(" [ok]\n"); 
     }
     Print_log("\n");
   }
   
   if ($test_fail_count != 0) { 
     Print("\n*** $test_fail_count tests failed (see $TEST_LOGFILE for test transcript) ***\n");
     exit(1);
   } else {
     Print("\n*** all tests passed (see $TEST_LOGFILE for test transcript) ***\n"); 
   }
}

# --------------------------------------------------------
# Test output 
# -------------------------------------------------------- 

sub test_fail
{
  Print_log( shift );

  $test_fail_count ++;
  $this_test_failed = 1;
}

sub test_exec
{
   my $args = join( " ", @_ );
   Print_log("\n\n[run command]\n$args\n[command output]\n");
   #die $TEST_LOGFILE;
   #my $ret = `$args 2>&1 | tee -a $TEST_LOGFILE`;

   #system("./printenv.pl");
   my $ret = `$args 2>&1`;

   open(TST,">>$TEST_LOGFILE");
   print TST $ret;
   close(TST);

   Print_log("\n[eof command output]\n");
   return $ret;
}

sub assert_regex
{
  my $hay  = shift;
  my $needle = shift;


  Print_log("\n\n[assert regex $needle]\n");
  if ( $hay =~ m/$needle/) {
    Print_log("[regex ok]\n");
  } else {
    #test_fail( "Assert failed: regex did not match\nneedle: $needle\nhay: $hay\n---eof hay---" );
    test_fail( "Assert failed: regex needle not found in string hay\nneedle: $needle\nhay: $hay\n---eof hay---\n" );
   }
}

sub count_string
{
  my $hay  = shift;
  my $needle = shift;
  my $ret = 0;
  my $idx = 0;

  while($idx != -1) {

    $idx = index($hay,$needle,$idx);

    if ($idx != -1) {
      $idx += length($needle);
      $ret += 1;
    }
  }
  return $ret;
}

sub assert_substr
{
  my $hay  = shift;
  my $needle = shift;


  Print_log("\n\n[assert substring]\n");
  if (index( $hay, $needle ) == -1) {
    test_fail( "Assert failed: substring needle not found in string hay\nneedle: $needle\nhay: $hay\n---eof hay---" );
  } else {
    Print_log("[substring ok]\n");
  }  
}

sub test_announce
{
  my $title  = shift;

  my $i;

  for($i = length($title);$i<60;$i++) {
    $title = $title.".";
  }

  Print($title);
}


# --------------------------------------------------------
# Logging stuff  
# -------------------------------------------------------- 

sub Print_init
{
   `rm -f $TEST_LOGFILE`;
   $TEST_LOGFILE = getcwd."//$TEST_LOGFILE";
   $TEST_LOGFILE =~ s#//#/#g; 
 
}

###
# Print message to log file
###
sub Print_log
{
  my $args = join( " ", @_ );
  
  open(OFILE, ">>", ${TEST_LOGFILE});
  syswrite(OFILE, $args);  
  close(OFILE);

}

###
# print message to stdout and log file
### 
sub Print
{
  my $args = join( " ", @_ );
  
  Print_log($args);

  print $args;
}


###
# read argument file and return its string contents
###
sub Readfile
{
  my $file = shift;
  my $ret;
  
  open(IF,$file) || die "Can't open $file";
  while(<IF>) {
    $ret = $ret.$_;
  }
  close(IF);
 
  Print_log("Reading file $file\n$ret");

  return $ret;
}


###
# set name of test log
###
sub set_test_log_file
{
  $TEST_LOGFILE=shift;
}


