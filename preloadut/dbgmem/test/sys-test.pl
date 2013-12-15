#!/usr/bin/perl

# * 
# * DBGMEM - memory allocation leak tracker and debugging tool.
# *
# *  (c) 2008 by Michael Moser
# * 
# * This code is available under the GNU Lesser Public License version 2.1
# *
# * See LICENSE file for the full copy of the license
# *
# * 


use strict 'vars';
use Cwd;
use Cwd "realpath";

require "perltest.pl";

#my $plat = `uname -r`;
#chomp($plat);
#my $LIB_PATH="../lib/$plat";

my $LDIR=realpath($ARGV[0]);
my $EDIR=realpath($ARGV[1]);
my $ENGINE=$ARGV[2];
my $TEST_LOG_FILE="test-engine-${ENGINE}.log";
print <<EOF

***
Engine: $ENGINE
ExeDir: $EDIR
LibDir: $LDIR
TstLog: $TEST_LOG_FILE
***
EOF
;

set_test_log_file($TEST_LOG_FILE);

$ENV{'LD_LIBRARY_PATH'}=$ENV{'LD_LIBRARY_PATH'}.":".$LDIR;
$ENV{'DBGMEM_ENGINE'}=${ENGINE};
$ENV{'DBGMEM_STACK_FRAMES'} = 5;

my $all_tests;


if ($ENGINE eq "s") {
    $all_tests="cpp_test uninitialized_memory uninitialized_free_memory generation_test leak_test sigsegv_test follow_fork_test2 " ;
} else {
    $all_tests="cpp_test uninitialized_memory uninitialized_free_memory mem_underwrite mem_overwrite overlap_copy string_check stack_smash illegal_free free_twice api_test generation_test leak_test sigsegv_test follow_fork_test2 " ;

    #$all_tests="mem_underwrite mem_overwrite" ;

}    


run_tests($all_tests);

clean_files();


 

sub set_opts
{
  my $frames = shift;
  my $check_adj = shift;
  my $attach_dbg = 0;
  my $fill_bytes = shift;
  my $fatal_err = shift;
  my $follow_forks = shift;
  my $dir = shift;
  
  my $fname = "/tmp/dbgmem_param_$$";
  if (! open(OF,">$fname")) {
     $fname = "/var/dbgmem_param_$$";
     if (! open(OF,">$fname")) {
        $fname = "~/dbgmem_param_$$";
	open( OF, ">$fname") || die "Can't write parameter file, very bad";
     }
  }
  
  print OF "0,$frames,$attach_dbg,$check_adj,$fill_bytes,$fatal_err,1,0,0,$follow_forks,1,$dir";
  close(OF);

}

sub follow_fork_test
{
  set_opts(5,0,0,0,1,"");
  my $out = run_test("dbgtest", "10 3"); 

  if (count_string($out,"Follow forks: on") != 7) {
    test_fail("expected 7 runs of DBGMEM; one for /bin/sh run by system(3), the other for process created by system(3)");
  }
}

sub follow_fork_test2
{
  set_opts(5,0,0,0,1,"");
  my $out = run_test("mdbgtest", "14"); 

  if (count_string($out,"attached on: fork") != 3) {
    test_fail("expected 3 invocations of fork");
  }
 
}

sub generation_test
{
  set_opts(5,0,0,0,0,"");
  my $out = run_test("mdbgtest", "15"); 

  my $f = <DBGMEM_report*.log>;
  my $d = Readfile($f);

  assert_regex($d, "Entry: 0x(\\S+) size: 8 generation: 0");
  assert_regex($d, "Entry: 0x(\\S+) size: 10 generation: 1");
  assert_regex($d, "Entry: 0x(\\S+) size: 10 generation: 2");
  assert_regex($d, "Entry: 0x(\\S+) size: 10 generation: 3");
}

sub leak_test
{
   set_opts(5,0,0,0,0,"");
  
   my $out = run_test("mdbgtest", "8");

   my $f = <DBGMEM_report*.log>;
   my $d = Readfile($f);

   assert_regex($d, "Entry: 0x(\\S+) size: 8 generation: 0");
   assert_regex($d, "Entry: 0x(\\S+) size: 8 generation: 1");
}


sub sigsegv_test
{
  set_opts(5,0,0,1,0,"");
  
  my $out = run_test("mdbgtest", "12"); 

  assert_substr($out, "ERROR: FATAL SIGNAL 11");
  assert_regex($out, "ERROR Invalid memory block. Entry: 0x(\\S+) size: 8 generation: 0");

} 
  
sub cpp_test
{
  set_opts(5,0,0,0,0,"");
  
  my $out = run_test("mdbgtest2", "11"); 
  assert_regex($out, "ERROR Memory allocated with malloc but freed with delete. Entry: 0x(\\S+) size: 11 generation: 0");
  assert_regex($out, "ERROR Memory allocated with new but freed with delete\\[\\]\\. Entry: 0x(\\S+) size: 4 generation: 0");
  assert_regex($out, "ERROR Memory allocated with new\\[\\] but freed with delete. Entry: 0x(\\S+) size: 21 generation: 0");
    
}

sub uninitialized_memory
{
  set_opts(5,0,1,0,0,"");
  
  $ENV{'DBGMEM_FILL_BYTE'} = "1";    
  my $out = run_test("mdbgtest", "0"); 
  $ENV{'DBGMEM_FILL_BYTE'} = ""; 
  assert_substr($out, "0xdddddddd" );
 
}

sub uninitialized_free_memory
{
  set_opts(5,0,1,0,"");
  
  $ENV{'DBGMEM_FILL_BYTE'} = "1";   
  my $out = run_test("mdbgtest", "1"); 
  $ENV{'DBGMEM_FILL_BYTE'} = ""; 
  assert_substr($out, "0xeeeeeeee" );
}
sub mem_underwrite
{
  set_opts(5,0,0,0,0,"");
  
  my $out = run_test("mdbgtest", "2");
  assert_substr($out, "Writing 5 bytes before start of allocated block.");  
}

sub mem_overwrite
{
   set_opts(5,0,0,0,0,"");
  
   my $out = run_test("mdbgtest", "3");
  assert_substr($out, "Error in memset: Writing access skips 7 bytes past end of allocated block.");
}

sub overlap_copy
{
  set_opts(5,0,0,0,0,"");
  
  my $out = run_test("mdbgtest", "4");
  assert_substr($out, "Warning in in memcpy: overlap of source range");  
  
}

sub stack_smash
{
  set_opts(5,0,0,0,0,"");
  
  my $out = run_test("mdbgtest", "5");
  assert_substr($out, "Error in memset: overwriting stack frame"); 
}

sub string_check
{
  set_opts(5,0,0,0,0,"");
  
  my $out = run_test("mdbgtest", "13");

  assert_regex($out, "DBGMEM: Error in strcpy: will overwrite target, writing 4 bytes, target buffer is 3 bytes long at 0x(\\S+) \\[allocated block 0x(\\S+) length 3\\]" );

  #assert_regex($out, "DBGMEM: Error in strcat: will now overwrite target, writing 5 bytes, target buffer is 3 bytes long at 0x(\\S+) \\[allocated block 0x(\\S+) length 3\\]" );

  #assert_regex($out, "DBGMEM: Error in strncat: will now overwrite target, writing 5 bytes, target buffer is 3 bytes long at 0x(\\S+) \\[allocated block 0x(\\S+) length 3\\]" );

}

sub illegal_free
{
  set_opts(5,0,0,0,0,"");
  
 
  my $out = run_test("mdbgtest", "6");
  assert_substr($out, "Error in free_mem: passing pointer");   
  assert_substr($out, "from within content of allocated range.");
}
 
sub free_twice
{
  set_opts(5,0,0,0,0,"");
  
  my $out = run_test("mdbgtest", "7");
  assert_substr($out, "Error in free_mem: pointer");  
  assert_substr($out, "is not from any allocated range.");
}

sub api_test
{
   set_opts(5,0,0,0,0,"");
  
   my $out = run_test("mdbgtest", "10");

  assert_substr($out, "DBGMEM_is_installed() returns 2");
  assert_regex ($out, "The size is 8");
  assert_regex ($out, "DBGMEM: Error in test1: Writing access skips 1 bytes past end of allocated block. \\[allocated block 0x(\\S+) length 8 \\]");
  assert_regex ($out, "DBGMEM: Error in test2: lower bound of memory block is overwritten. \\[allocated block 0x(\\S+) length 8\\]");
  assert_regex ($out, "DBGMEM: Error in test3: argument string 0x(\\S+) is not null terminated\\. will read past heap block\\! \\[allocated block 0x(\\S+) length 10\\]");
-
  assert_regex ($out, "DBGMEM: Error in testy: upper bound of memory block is overwritten. \\[allocated block 0x(\\S+) length 8\\]");
}



sub clean_files
{
 `rm -f DBGMEM_report_*.log maps*`; 
}


# --------------------------------------------------------
# Test support 
# -------------------------------------------------------- 
sub run_test
{
  my $pname=shift;
  my $exe_path="${EDIR}/${pname}";
  my $args = shift;
 
  clean_files();

  my $is_cpp=`ldd $exe_path | grep stdc++ | wc -l`;
  chomp($is_cpp);

  my $prev = $ENV{'LD_PRELOAD'};

  if ($is_cpp ne "0") {
    $ENV{'LD_PRELOAD'}=$LDIR."/libdmem${ENGINE}cpp.so:$prev";
  } else {
    $ENV{'LD_PRELOAD'}=$LDIR."/libdmem${ENGINE}.so:$prev";
  }    


  my $ret = &test_exec($exe_path, $args);
  
  Print_log("is_cpp: ${is_cpp}\n");
  
  $ENV{'LD_PRELOAD'}=$prev;

 
  return $ret;
}



