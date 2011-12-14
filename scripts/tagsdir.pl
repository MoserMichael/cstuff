#!/usr/bin/perl

#
# This script is run from vim when it starts up:
# finds the value of the tags directory - tag files are
# searched in the parent directory, then the parent of the parent, etc.
# 
# The invoking vim script is:
#
#command! -nargs=* GotoTag call s:RunGotoTagFile()
#
#function! s:RunGotoTagFile()
#  let tagdir = system("~/bin/tagsdir.pl")
#  if tagdir != ""
#   execute "set tags=". tagdir 
#  endif 
#endfunction
#
#:call s:RunGotoTagFile()
#


use strict;
use Cwd;


my $tags;
my $dir = ".";

while(1) {

  if (! -d $dir) {
    last;
  }

  if (-f "${dir}/tags") {
    if ($tags ne "") {
      $tags = "$tags,";
    }
    $tags = "${tags}${dir}/tags";
  }

  $dir = "$dir/..";

} 

print $tags;


