#!/usr/bin/perl

# script for migrating google+ comments to ... anything else;
# In this instance conversion is made to github blog (textile based).
# The command line arguments:
#
# ./google+-migration.pl <source-dir> <target-dir>
#
#   <source-dir> - directory that contains files created gy google takeout (if google+ stream is selected)
#   <target-dir> - directory where converted posts are written.
#
# Customization:
#   The function write_post is called for each post; it is supposed to write the output post into <target-dir>
#   Rewrite the function for your own purposes.
#   The function write_post receives the following arguments:
#
#   write_post($title,$post,$year,$month,$day,$post_id);
#

use HTML::DOM;

$mnames = ( 'January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December' );

$dir = $ARGV[0];
$odir = $ARGV[1];
$files = `ls $dir`;

#system("rm -f $odir/*");
$post_id = 1;

for $a (split(' ',$files)) {

  print "$a\n";

  $obj = new HTML::DOM || die "failed to parse $a error $!" ;
  $obj->parse_file("$dir/$a");

  @items =   $obj->getElementsByClassName(  "published" );
  for $n (@items) {
   if ($n->title =~ /(\d+)-(\d+)-(\d+)T/) {
     $year = $1;
     $month = $2;
     $day = $3;
   }
  } 
  
  
  print("# $year - $month - $day#\n" );

  $post = "";
  @items =   $obj->getElementsByClassName(  "entry-content" );
  for $n (@items) {
   $post = $n->innerHTML;
   last;
  } 

  if ($post ne "") {
    print $post;

    $idx = index($post,"<br>");
    if ($idx != 0) {
      $title = substr( $post, 0, $idx );
      $post = substr( $post, $idx );
    } else {
      $title = "";
    }

    write_post($title,$post,$year,$month,$day,$post_id);
    $post_id ++;
  }
 
  print "\n-----------------\n";
}

sub write_post
{
    my ($title,$post,$year,$month,$day,$post_id) = @_;

    open(OF,">${odir}/${year}-${month}-${day}-post-${post_id}.textile");
    $mn = $mnames[ $month - 1 ];   
print OF <<EOF
---
layout: post
title: <notextile>$title</notextile>
---
h2. {{ page.title }}
<p class="publish_date">$day $mn $year</p>

<notextile>
$post
</notextile>
EOF
;
}
