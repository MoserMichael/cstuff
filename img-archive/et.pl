#!/usr/bin/perl

#use lib abs_path("./exif/lib");

use Cwd 'abs_path';

if (-d "./exif")
{
	use lib abs_path("./exif/lib");
}
else
{
	use lib;
}
use Image::ExifTool  qw(:Public);


$info = ImageInfo( $ARGV[0] );

foreach (keys %$info) {
   print "$_ => $$info{$	_}\n";
}
