#!/usr/bin/perl

#---
# Get the latest version of openssl from the WEB
# Will break if they change the page (did not happen for years now ;-)
# Build & install locally.
#---

use strict;


my $BASE_URL="http://www.openssl.org/";	    # base url
my $INDEX_URL=$BASE_URL."source/";	    # url of page that lists tar ball url's.

my ($tarball_url,$md5_url);

if (-d "include" && -d "lib") {
    exit(0);
}

#--- is there an archive here? ---
my $tarfile = (split(' ', `ls openssl*.tar.gz`)) [0];

if ($tarfile eq "") {
    download();
} else {
    print( "Openssl source tar $tarfile\n" );
}

$tarfile = (split(' ', `ls openssl*.tar.gz`)) [0];

my $dirname = $tarfile;
$dirname =~ s/\.tar\.gz$//g;


if (! -d $dirname) {
    print(">extract $tarfile to $dirname\n");
    system("/bin/sh -c \"tar xfz $tarfile\" >/dev/null 2>&1");
    if ($? != 0) {
	error("Can't extract archive file");
    }
}

print( ">configure\n" );

my $curdir=`pwd`;
chomp( $curdir );

chdir( $dirname );

my $dirname = `pwd`;
chomp($dirname);

system("/bin/sh -c \"./config no-shared --prefix=$curdir\">config.log 2>&1");
if ($? != 0) {
    print("Error: Make failed. see ${dirname}config.log\n");
    exit(1);
}

print( ">make\n" );
system("/bin/sh -c \"make\" >make.log 2>&1");
if ($? != 0) {
    print("Error: Make failed. see ${dirname}make.log\n");
    exit(1);
}

print( ">install\n" );
system("/bin/sh -c \"make install\" >install.log 2>&1");
if ($? != 0) {
    print("Error: Install failed. see ${dirname}make.log\n");
    exit(1);
}

print( "*** finished ***\n" );


sub error
{
    unlink($tarball_url) if ($tarball_url ne "");
    unlink($md5_url) if ($md5_url ne "");

    print "Error: $_\n";
    print("Put openssl tarball into directory ".`pwd`." and run again\n");

    exit(1);
}

sub download
{
    my ($pos,$next,$url,);

    print(">Downloading index page\n");
    system("wget $INDEX_URL -O index -t 3");
    if ($? != 0) {
      error("Error: can't download page ${INDEX_URL} - does not work if not connected to the WEB");
    }
    print(">index page downloaded\n");

    system("grep -F '[LATEST]' index >index.txt");

    open(IN,"index.txt");
    my $latest = <IN>;
    close(IN);
 
    unlink("index.txt");
    unlink("index");

    $pos = 0;
    while(1) {
      $pos = index($latest, "href=\"", $pos );
      last if ($pos == -1);

      $pos += 6;
      $next = index($latest, "\"", $pos);

      $url = substr($latest, $pos, $next - $pos );

      if ($url =~ /tar\.gz$/) {
	 $tarball_url=$url;
      } elsif ($url =~ /gz\.md5$/) {
	 $md5_url = $url;
      }
      $pos = $next;
    }

    if ($tarball_url eq "") {
	error("Failed to extract tarball url");
    }

    if ($md5_url eq "") {
	error("Failed to extract md5 checksum url");
    }

    print(">Downloading openssl TAR archive\n");

    system("wget ${INDEX_URL}${tarball_url} -O ${tarball_url} -t 3");

    if ($? != 0) {
      error("Can't download openssl tar archive ${BASE_URL}${tarball_url}");
    }

    print(">Downloading MD5 checksum of openssl archive\n");

    system("wget ${INDEX_URL}${md5_url} -O ${md5_url} -t 3");
    if ($? != 0) {
      error("Can't download openssl tar archive ${BASE_URL}${tarball_url}");
    }

    print(">Verifying MD5 checksum\n");
    my $chksum = `md5sum ${tarball_url}`;
    if ($? != 0) {
      error("failed to get checksum of downloaded file");
    }

    my $chksum2 = `cat $md5_url`;

    if ( (split( ' ', $chksum )) [ 0 ] ne (split(' ', $chksum2)) [ 0 ]) {
      error("Downloaded file and md5 don't match");
    }

    print(">Download ok\n");
}



