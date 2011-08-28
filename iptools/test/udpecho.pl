#!/usr/bin/perl

use Socket;
use strict;

#*** udp echo server ***

my $port = shift || 11112;
my $msg;
my $cnt = 0;

socket(SOCKET,PF_INET,SOCK_DGRAM,(getprotobyname('udp'))[2]) || 
   die "Can't create a socket $!\n";


bind( SOCKET, pack( 'Sn4x8', AF_INET, $port, "\0\0\0\0" )) ||
        die "Can't bind to port $port! \n";

while (1) {

  my $src_addr = recv( SOCKET, $msg, 65535, 0 );
  
  print "message $cnt\n";

  $cnt ++;

  if (defined $src_addr) {
     send( SOCKET, $msg, 0, $src_addr ) || die "Can't send message";
  }
}




