#!/usr/bin/perl

use strict;

my %result;

sgetopt( [ 'h', 'e:', 'c:', 'o:', 'n' ], \%result);

print_usage() if (exists $result{'h'});
print_usage("Missing -c <core file> option") if (! exists $result{'c'});
print_usage("Missing -e <exe file> option") if (! exists $result{'e'});
print_usage("Missing -o <out file> option") if (! exists $result{'o'});

my $exe_file = $result{'e'};
my $core_file = $result{'c'};
my $tar_file = $result{'o'};
my $copy_exe = ! defined $result{'n'};

if (! -f $exe_file ) {
  print_usage("<executable file path> file does not exist");
}

if (defined $core_file && ! -f $core_file) {
  print_usage("<core-file> file does not exist");
}

my $VERBOSE = 0;

my @libs = get_dependent_libs(  $exe_file );
make_tar();

sub make_tar
{
  my ($basedir,$tmp_dir,@dep_files,$f,$dir);
 
  my $tmp_dir = "/tmp/tmp-dir-$$";

  @dep_files =get_dependent_libs($exe_file);

  run("mkdir -p $tmp_dir");
  
  foreach $f (@dep_files) {
     $dir = "$tmp_dir/".get_base_dir($f);
     run("mkdir -p $dir");
     run("cp -Lf $f $dir");
  }
  
  copy_core($tmp_dir);
 
  run("tar cvfj $tar_file -C $tmp_dir .");
  run("rm -rf $tmp_dir");
}


sub copy_core
{
  my $tmp_dir = shift;
  my ($cfile,$efile);

  if (defined $core_file) {
    run("cp -f $core_file $tmp_dir");
    $cfile = get_base_name($core_file);
  }

  if ($copy_exe) {
    run("cp -f $exe_file $tmp_dir");
    $efile = get_base_name($exe_file);
  }
  

  my $sf = "$tmp_dir/run.sh";
  open(OF,">$sf") || die "Can't create $sf";
  print OF <<EOFS
#!/bin/bash

EOFS
;

  print OF "CFILE=$cfile\n" if ($cfile ne "");
  print OF "EXE=$efile\n" if ($efile ne "");

  print OF <<EOFS

function usage()
{
  local \$optline

  optline = "\$0 "
  if ["x\$CFILE" != "x"]; then
    optline = "\$optline [-c <core-file> ] "
  else
    optline = "\$optline -c <core_file> "
  fi

  if ["x\$EXE" != "x"]; then
    optline = "\$optline [ -e <exe_file> ] "
  else 
    optline = "\$optline -e <exe_file> "
  fi

cat <<EOF
\$optline

Invokes gdb while it is looking at dependent shared libraries
of ${exe_file} copied from a remote machine.

  -c core file.
  -e exe file.
EOF

  if [ "x\$EXE" != "x" ]; then
     echo " default value of -e option is \$EXE"
  fi
  if [ "x\$CFILE" != "x" ]; then
     echo " default value of -c option is \$CFILE"
  fi

  exit 1
}

while getopts 'hc:e:' OPT
do
  case \$OPT in
    h) 
	usage
	;;
    e)
	EXE=\$OPTARG
	;;
    c)
	CFILE=\$OPTARG
	;;
   esac
done

if [ ! -f \$CFILE ]; then
  echo "Please provide core file as argument"
  exit 1
fi

if [ ! -f \$EXE ]; then
  echo "Please provide executable file as argument"
  exit 1
fi

has_dbg_info=`objdump --section-headers \$EXE | grep -F '.debug' | wc -l`
if [ "x\$has_dbg_info" = "x0" ]; then
  cat <<EOF
The executable \$EXE has no debug information.
Please replace the executable with the same version that includes debug info
EOF
  exit 1
fi

cat >gdb_script <<EOF
set solib-absolute-prefix .
core-file \$CFILE
EOF

gdb -x gdb_script \$EXE
  
EOFS
;


  close(OF);
  run("chmod +x $sf");
}

sub get_dependent_libs
{
  my $exe_file = shift;
  my (@ret,$line,$txt);
   
  $txt = `ldd $exe_file`;
   
  foreach $line (split('\n',$txt)) {
  
    my $file;
    my @tokens = split(' ',$line);
    if (index($line,"=>") != -1) {
      $file = $tokens[2];
    } else {
      $file = $tokens[1];
    }
    if (-f $file) {
      push(@ret,$file);
    }
  }
  return @ret;
}

sub run
{
  my $args = shift;

  print "Running: $args\n" if ($VERBOSE);

  my $f = `$args`;

  if ($? != 0) {
    print "Error: running $args failed.";
    exit(1);
  }

  return $f;

}
sub get_base_dir
{
  my $f = shift;
  my $idx = rindex($f,"/");
  return $idx != -1 ? substr($f,0,$idx) : $f;
}

sub get_base_name
{
  my $f = shift;
  my $idx = rindex($f,"/");
  return $idx != -1 ? substr($f,$idx+1) : $f;
}


sub print_usage
{
  my $msg = shift;

  if ($msg ne "") {
    print "Error: $msg\n\n";
  }
  print <<EOF
gather-libs -e <exe-path> [-c <core-file>] [-n] -o <output-tar-file>

Parameters
 -c <core-file>		- core file (optional)
 -e <exe-path>		- path of executable that produced core file
 -o <output-tar-file>	- output archive file.
 -n			- don't copy executable, only shared libs (optional)

the <output-tar-file> will contain following data
 - copy of <exe-path> (unless -n is specified)
 - copy of all dependent shared libraries of <exe-path>
 - copy of <core-file> (if specified by -c option)
 - script that runs gdb so that gdb is looking at 
   copied shared libraries + using the copied cores/executables.


The tar file is compressed with bz2 compression, so in order to extract the archive run

tar xvfj <output-tar-file>

This program is useful if we want to examine a core file of an executable on a remote machine.
It is assumed that the core file has been caused by running <executable-file-path>


EOF
;
exit(1);

}


# ----------------------------------------------------------------------------
sub sgetopt
{
   my $opts = shift;
   my $resref = shift;
   my ($i,$argval,$optval,$has_value);

   for($i=0; $i < scalar(@ARGV) ;  ) {
      $argval = $ARGV[ $i++ ];
      next if (! substr($argval,0,1) eq "-" && length($argval) == 2);
      $argval = substr($argval,1);
      $has_value = 0;
      foreach $optval (@{ $opts }) {
	   if ($argval eq substr($optval,0,1)) {
	      $has_value = 1;
	      if (index($optval,':') != -1) {
		 if ($i >=  scalar(@ARGV)) {
	           print "ERROR: option -$argval must have a value\n";
		   exit(1);
		 }
		 $has_value = 1;
	         ${ $resref } { $argval } = $ARGV[ $i ++ ];
	      } else {
                 ${ $resref } { $argval } = 1;
	      }
	   }
       }
       if (! $has_value) {
	 print "ERROR: Command line option -$argval is not defined\n";
	 exit(1);
       }
   }
}


