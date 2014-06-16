#!/usr/bin/perl

use strict;

use File::Find;
use File::Basename;
use File::Spec;
use File::Copy;
use File::Path;
use Cwd 'abs_path';
use Getopt::Long;


eval "use lib abs_path(\"./exif/lib\");use Image::ExifTool  qw(:Public);";

#use Image::ExifTool  qw(:Public);

our ($opt_h, $opt_d, @opt_s, $opt_c, $opt_i, $opt_m, $DEBUG, $NOPROGRESS );
our ($the_index, $tf, $has_jpeginfo);
our ($copied_files, $repeated_files, @file_list);
our ($INDEX_FILE_NAME);

$copied_files = $repeated_files = 0;
$NOPROGRESS = 0;
$INDEX_FILE_NAME = "hash2file.lst";

GetOptions(  
	     "source=s" => \@opt_s,
	     "dest=s" => \$opt_d,
  	     "noprogress" => \$NOPROGRESS,

	     "check" => \$opt_c,
	     "import" => \$opt_i,
	     "merge" =>  \$opt_m,

	     "verbose" => \$DEBUG,
             "help" => \$opt_h
	  );

if ($opt_h)
{
	print_msg();
}

if ( ($opt_c + $opt_i + $opt_m) != 1 ) 
{
	print_msg( "Need either one of the following options -i -m -c (--import --merge --check)");
}

if (scalar( @opt_s ) == 0)
{
   print_msg("no source directory; -s option missing");
}

for $tf ( @opt_s )
{
   if (! -d $tf) 
   {
	print_msg("source directory $tf does not exist");
   }
}	

# check if all spawned programs are present.
check_requirements();

if ( $opt_i )
{
	if ($opt_d eq "")
	{
   		print_msg("no destination directory; -d option missing");
	}

	if (! -d $opt_d)
	{
   		print_msg("destination directory $opt_d does not exist");
	}
	import_to_archive()
}

if ( $opt_c )
{
	# --- check archives  ---
	foreach $tf (@opt_s) {
	    verify_archive( $tf );
	}	
}

if ( $opt_m )
{
	merge_archives();
}

print "*** eof ***\n" if ($DEBUG);


# ====================================================================================

sub print_msg
{
  my $msg = shift;

  print "Error: $msg\n\n" if ($msg ne "");
print <<EOF

$0 -i -s <dir> [ -s <dir2> ] -d <dir>   [ --noprogress ]
$0 --import --source <dir> [ --source <dir2> ] --dest <dir>

	import images from source directory (specified by -s <dir>) to archive (specified  by -d <dir>)
	Multiple source directories can be specified by repeating option -s

	Images are ordered into a directory hierarchy

	 <year the picture was taken>/<month picture was taken>/<day picture was taken>/<file name>

	The date of picture taken is extracted from the image file (where possible) - 
	this is enabled by the ExifTool, written by Phil Harvey.

	The script checks if the picture already exists in the destination directory;

	The option --noprogress supresses display of progress indicator during copying.

	Script is part of my simple picture archive system.

$0 -m -s <dir> [ -s <dir2> ]    [ --noprogress ]
$0 --merge --source <dir> [ --source <dir2> ] 

	Merge a number of archives; after merging every archive should have the same set of data;
	However the file names may differ.

$0 -c -s <dir> [ -s <dir2> ] [ --noprogress ]
$0 --check --source <dir> [ --source <dir2> ]  [ --noprogress ]
	
	checks an existing archive for consistency; 

$0 -h

	prints this help message

EOF
;

exit(1);
}

sub check_requirements
{
  `openssl version`;
  if ($? != 0)
  {
	die "Error: openssl is not installed\n";
  }


  $has_jpeginfo = "";
  `./jpeginfo_prog/jpeginfo --help 2>\&1`;
  if ($? == 0)
  {
     $has_jpeginfo="./jpeginfo_prog/jpeginfo";
  } else
  {
     `jpeginfo --help 2>\&1`;
     if ( $? == 0)
     {
	$has_jpeginfo = "jpeginfo";
     }
  }
}

sub process_files
{
   my ($f, $func, $percent, $old_percent, $total, $cur );
  
   $func = shift;
   $total = scalar( @file_list );

   print("       \r");

   if ($total == 0)
   {
	print "100 %\r";
	return;
   }

   $cur = 0; 
   $old_percent = 0;

   print "0 %\r";
	
   for $f (@file_list) {

        print "procesing - $f\n" if ($DEBUG);    
	$func->( $f );
	$percent = int( ($cur / $total) * 100 );
	if ($percent != $old_percent)
	{
		print "$percent %\r";
	 	$old_percent = $percent;
	}
 	$cur ++;
   }
   #@#file_list = -1;
   @file_list = ();
}  

sub add_file
{
  return if ($_ eq $INDEX_FILE_NAME);
  return if (! -f $_);

  my $fullp = abs_path($_);
  print " -> $fullp\n" if ($DEBUG);
  push(@file_list, $fullp ); 
}

sub list_files
{
  my $dir  = shift; 
  " List dir $dir\n" if ($DEBUG);
  find( \&add_file, $dir );
}


#
# ==================================================
#

sub import_to_archive()
{
	# --- import images into archive ---

	$opt_d = abs_path( $opt_d );

	print "Prepare import...\n" if (!$NOPROGRESS);
	print "read index file\n" if ($DEBUG);

	$the_index = new Index( $opt_d ); 
	$the_index->read_index_file( );

	print "List files in destination directory ${opt_d}\n" if ($DEBUG);
	list_files( $opt_d );

	print "Checking destination files...\n" if (!$NOPROGRESS);
	process_files( \&get_chksum_if_needed );

	print "Prepare copying...\n" if (!$NOPROGRESS);
	foreach $tf (@opt_s) {
	  print "Copy files from source directory $tf\n" if ($DEBUG);
	  list_files( $tf );
	} 

	print "Copying source files...\n" if (!$NOPROGRESS);
	process_files( \&copy_source_file );

	print "save index file\n" if ($DEBUG);
	$the_index->save_index_file();

	if (!$NOPROGRESS)
	{
 	print <<EOF

----
Copied files:	$copied_files
Repeated files:	$repeated_files
EOF
;
	}
}



our( @invalid_files, @changed_files );

sub verify_archive
{
   my ($hash, $file, $ref);

   $opt_d = shift;

   print "Prepare checking $opt_d ...\n" if (!$NOPROGRESS);

   $the_index = new Index( $opt_d );
   $the_index->read_index_file();

   list_files( $opt_d );
   
   print "Check archive $opt_d ...\n" if (!$NOPROGRESS);
   process_files( \&get_chksum_if_needed );

   $ref = $the_index->{ 'digest_to_file' }; 
   while (($hash, $file) = each (%{ $ref } )) {
	push( @file_list, [ $hash, $file ] );
   }

   process_files( \&verify_file );

   if (scalar( @invalid_files ) != 0)
   {
	print "Invalid files\n";	              
   	for $hash (@invalid_files) {
	    print "\t " . $the_index->hash_to_file( $hash ) . "\n";
	    $the_index->delete_entry( $hash );
	    unlink( File::Spec->catfile( $opt_d , $the_index->hashtofile( $hash ) ) );	 
	}	    
   }
   
   if (scalar( @changed_files ) != 0)
   {
	my ($r, $old_file);

	print "Changed files\n";	              
	for $r (@changed_files) {

		$old_file = $the_index->hash_to_file( $$r[ 0 ] );
		print "\t " . $old_file  . "\n";
		
		$the_index->delete_entry( $$r[ 0 ] );
		$the_index->add( $$r[ 1 ] , $old_file);		 	
	}	   
   }

   #print "Verifying archive in $opt_d) if (!$NOPROGRESS || $DEBUG);
   #process_files( \&get_chksum_if_needed );
	   
   $the_index->save_index_file();			
}

our (@indexes);


sub merge_archives
{
    if (scalar( @opt_s ) < 2)
    {
	print_msg("More then one source archive needed, specify multiple -s (or --source) options");
    }

    my ($i,$j,$n,$idx_obj,$idx_obj_c,$hash,$file,$copied);

    foreach $tf (@opt_s) {
	my $idx = new Index( $tf );
	if ($idx->read_index_file() == -1)
	{
	    print_msg("Directory $tf is not an imported archive / has no index file\n");
	}

	push( @indexes, \$idx );
    }	
 
    $n = scalar(@indexes);
    for($i = 0; $i < $n; $i++)
    {
	print " Merge from " . $opt_s[ $i ] . "\n" if (!$NOPROGRESS);
	$idx_obj = $indexes[ $i ];

	while (($hash, $file) = each (%{ $$idx_obj->{ 'digest_to_file' } } ) ) 
	{
	    $copied = 0;

	    print "-> $hash $file\n" if ($DEBUG); 

	    for($j = 0; $j <$n; $j++)
	    {
		if ($j != $i)
		{
		    $idx_obj_c = $indexes[ $j ];

		    if (!defined( $$idx_obj_c->hash_to_file( $hash ) ) )
		    {

			print "Must copy $file from $opt_s[$i] to $opt_s[$j]\n" if ($DEBUG);
			push( @file_list, [ $hash, $file, $i, $j ] );
		    }
		}
	    }
	}
	
	process_files( \&merge_copy_file );

	if (!$NOPROGRESS)
	{
	    print "Copied files:	$copied_files\n";
	}

    }

    foreach $tf (@indexes) {
	$$tf->save_index_file();
    }	
}



#
#======================================================================================
#

sub merge_copy_file
{
    my ($r, $hash, $file, $copy_from, $copy_to, $index_from, $index_to, $nfile );

    $r = shift;
    
    $hash = $$r[0];
    $file = $$r[1];
    $copy_from = $$r[2];
    $copy_to = $$r[3];

    $index_from = $indexes[ $copy_from ];
    $index_to   = $indexes[ $copy_to ];

    $nfile = copy_f( File::Spec->catfile( $$index_from->{ 'path' },  $file ), $$index_to->{ 'path' } ); 
    $$index_to->add( $hash, $nfile );
}

sub verify_file
{
   my ($r, $hash, $file, $fullp, $name, $path, $suffix, $is_valid, $new_hash );

   $r = shift;

   $hash = $$r [ 0 ];
   $file = $$r [ 1 ];

   # compute checksum of file;
   $fullp = File::Spec->catfile( $opt_d, $file );
   $new_hash = get_chksum( $fullp );
   if ($new_hash eq "") {  
     # file could not be read; very bad		    
     push( @invalid_files, $hash );
		
   } elsif ($new_hash ne $hash) {
     # hash has changed ;

     $is_valid = 1;
	    
     ( $name, $path, $suffix) = fileparse( $file );  

     # check if this is still a valid file. can check for jpg files .
     $suffix = lc($suffix);
     if ($has_jpeginfo ne "" && ($suffix eq ".jpeg" || $suffix eq ".jpg"))
     {
	`$has_jpeginfo "$file"`;
	$is_valid = ($? == 0);
     }
       	
     if ($is_valid)
     {
	# assume the file is valid; record change so that to update the index
	push( @changed_files, [ $hash, $new_hash ] );
     }
     else
     {
        push( @invalid_files, $hash);
     }
   }  
}

sub get_chksum_if_needed
{
  my ($fullp,$relp,$res);
 
  $fullp = shift;

  $relp = File::Spec->abs2rel( $fullp, $opt_d );    
  
  print "${fullp}\n" if ($DEBUG);    

  if (! $the_index->has_file( $relp ) ) {
    $res = get_chksum( $fullp ); 

    $the_index->add( $res, $relp );

    print "-> no chksum $res -> $relp\n" if ($DEBUG);    
  }

}

sub copy_source_file
{
    my ($fullp, $nfile,$chksum);

    $fullp = shift;
	

    print "${fullp}\n" if ($DEBUG);    

    $chksum = get_chksum( $fullp );
    if (!check_repeated_file( $fullp, $chksum )) {
       $nfile = copy_f( $fullp, $opt_d );
       $the_index->add( $chksum, $nfile );
    }
    else {
       print "$fullp has been REPEATED\n" if ($DEBUG);
       ++ $repeated_files;
    }
}

sub copy_f
{
  my ($f, $dest_dir, $of, $info, $cdate, $fdir, $name, $path, $suffix, $nsuffix, $reldir);
  
  $f = shift;
  $dest_dir = shift;

  # extract file creation date from image metadata

  $info = ImageInfo($f);

  #foreach (keys %$info) {
  #  print "$_ => $$info{$_}\n";
  #}

  if (defined($$info{ "CreateDate" }))
  {
     my @tok = split( " ", $$info{ "CreateDate" } );
     $cdate =  $tok[ 0 ];
  } 
  elsif (defined($$info{ "DateTimeOriginal" }))
  {
     my @tok = split( " ", $$info{ "DateTimeOriginal" } );
     $cdate =  $tok[ 0 ];
  } 
  elsif (defined($$info{ "ModifyDate" }))
  {
     my @tok = split( " ", $$info{ "ModifyDate" } );
     $cdate =  $tok[ 0 ];
  }
  # if no exim info is availabe then grab the OS date ; don't know why Picassa sometimes strips this info when importing files ;-(
  elsif (defined($$info{ "FileCreateDate" }))
  {
     my @tok = split( " ", $$info{ "FileCreateDate" } );
     $cdate =  $tok[ 0 ];
  }
  elsif (defined($$info{ "FileModifyDate" }))
  {
     my @tok = split( " ", $$info{ "FileModifyDate" } );
     $cdate =  $tok[ 0 ];
  }
  elsif (defined($$info{ "FileInodeChangeDate" }))
  {
     my @tok = split( " ", $$info{ "FileModifyDate" } );
     $cdate =  $tok[ 0 ];
  }
  else 
  {
     # if all fails take the file date

     my @tok = stat( $f );	
     my $utime = $tok[ 10 ];
     my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime( $utime );     
     $year += 1900;
     
     $cdate = "$year:$mon:$mday";
  }

  if ($cdate eq "") {
     die "no file date has been established for $f.\n";
  }

  my @date_comp = split(":", $cdate);

  $reldir =  File::Spec->catdir( $date_comp[0], $date_comp[1], $date_comp[2] );

  $fdir = File::Spec->catdir( $dest_dir, $reldir );
  

  if (! -d $fdir) 
  {
   	File::Path->make_path( $fdir );
	if ($? != 0)
	{
	   die "Failed to create directory $fdir error=$?\n";
	}
  }

  ($name, $path, $suffix) = fileparse( $f );  
  
  $nsuffix = $suffix;
  if (-f "$fdir$name$nsuffix" )
  {
   	# target file exists, choose different one.
        my $i = 1;
	for(;;) {
	  if (! -f "${fdir}${name}${nsuffix}_${i}" )
          {
	     $nsuffix = "${nsuffix}_${i}";
	     last;
	  }
	  ++ $i;
	}
  }

  print "copy ${f} to dir ${fdir}\n" if ($DEBUG);
  $of = File::Spec->catfile( $fdir,  "$name$nsuffix" ); 
  copy("$path$name$suffix", $of );
  
  if ($? != 0)
  {
     my $retry;

     for ($retry = 0; $retry < 3; ++$retry)
     {
	copy( "$path$name$suffix", $of );		
	last if ($? != 0);
     }
     if ($? != 0)
     {
	die "Failed to copy file $f to $of error=$?\n";
     }	
  } 
  else
  {
     $copied_files ++;
  }
  
  return File::Spec->catfile( $reldir,  "$name$nsuffix" );
}

sub get_chksum
{
        my ($f, $res, $idx);

	$f = shift;
    	
	$res = `openssl dgst -sha1 "$f"`;
 	if ($? != 0) {
 	    print("Can't read file $f\n");
	    return "";
	}

	$idx = index( $res, ")=");
	if ($idx != -1) {
	   $res = substr($res, $idx + 3);
	}	
 	chomp($res);

	print "$f -> $res\n" if ($DEBUG);

	return $res;

}

sub check_repeated_file
{
        my ($f, $chksum);
	
        $f = shift;
	$chksum =  shift; 

	if (defined( $the_index->hash_to_file( $chksum ) )) { # found repetition of the same hash

          
	  my ($isequal, $cmd, $second);

	  #$second =  $digest_to_file{ $chksum };
	  #$cmd = "diff $f $opt_d/$second"; # double check that files are really different (no hash collision)
	  #$isequal=`$cmd`;
	  #
 	  #if ($isequal != 0) {
 	  # 	print "Bingo! the digest hash is of the same value but files differ: $f != $second sha1 $res\n";
	  #	return 0;
	  #} else {
	  #	return 1;	
	  #}
	  return 1;

        }
	return 0;
}

#--------------------------------------------------
#

package Index;

sub new
{
  my $class = shift;
  my $p = shift;

  my $self = {

     'digest_to_file' => {},
     'has_file' => {},
     'path' => $p,
     'index_file_name' => File::Spec->catfile( $p, $INDEX_FILE_NAME ),
     'is_modified' => 0
  };

  bless $self, $class;

  return $self;
}

sub read_index_file
{
   my ($self, $l, $fullp, $hash, $file, $rt, $changed, @tmp);

   $self = shift;

   $rt = 0;
   $changed = 0;

   if (open(IN, $self->{ 'index_file_name'} )) {
     while(<IN>) {
       chomp($_);
       
       @tmp = split( " ", $_);
       $hash = shift(@tmp);
       $file = join(" ", @tmp);

       $fullp = File::Spec->catfile( $self->{'path'} , $file);

       if (-f $fullp)
       {
	 $self->add( $hash, $file );
       }
       else
       {
         print "${fullp} has been deleted !\n" if ($DEBUG || !$NOPROGRESS);           
	 $changed = 1;
       }
     }
     close(IN);

     $self->{'is_modified'} = $changed;

     #use Data::Dumper;
     #print Dumper( $self );
   }
   else
   {
     $rt = -1;
   }

   return $rt;
}

sub save_index_file
{
   my ($self, $hash, $file);
   
   $self = shift;

   if ($self->{'is_modified'})
   {
     open(OF, ">", $self->{'index_file_name' } ) || die "Can't write index file " . $self->{'index_file_name'} . "\n";

     while (($hash, $file) = each ( %{ $self->{ 'digest_to_file' } } )) {
       print OF "$hash $file\n";
     }
     close(OF);
   }     
}

sub add
{
    my ($r);
    my ($self, $hash, $path ) = @_;

    $r =  $self->{ 'digest_to_file' }; 
    ${ $r } { $hash } = $path;
    
    $r = $self->{ 'has_file' };  
    ${ $r } { $path } = 1;

    $self->{'is_modified'} = 1;
}

sub delete_entry
{
    my ($f, $r);
    my ($self, $hash) = @_;

    $r =   $self->{ 'digest_to_file' };
    $f =   ${ $r } { $hash };
    delete ${ $r } { $hash };
    delete ${ $self->{ 'has_file' } } { $f };

    $self->{'is_modified'} = 1;
}

sub has_file
{
    my ($self, $path) = @_;

    return ${ $self->{ 'has_file' } } { $path } == 1;
}

sub hash_to_file
{
    my ($self, $hash) = @_;
    return  ${ $self->{ 'digest_to_file' } } { $hash };
}


