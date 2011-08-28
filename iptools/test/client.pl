#!/usr/bin/perl

use strict;
use Socket;

my $srv_ip='10.63.3.249';
my $srv_port='11112';
my $clnt_port='11111';


my $tcpdump_pid;

sub run_capture
{
   my $capture_file = shift;
   my $pid;
    
   
   #return;
   $tcpdump_pid = fork();
   if ($tcpdump_pid == 0) {
     exec( "tcpdump -s 0 -i any -w $capture_file host $srv_ip");
   } else {
      sleep(1); 
   }
}

sub stop_capture
{
   #return;
   sleep(1);
   system("kill $tcpdump_pid");
}


sub make_message
{
   my $msg_length = shift;
   my (@msg, $i);

   for($i = 0; $i < $msg_length; $i++ ) {
     push(@msg, $i % 255 );
   }

   return pack("C*", @msg );
}


sub do_connect
{
  socket(SOCKET,PF_INET,SOCK_DGRAM,(getprotobyname('udp'))[2]) || 
     die "Can't create a socket $!\n";
  
  bind( SOCKET, pack( 'Sn4x8', AF_INET, $clnt_port, "\0\0\0\0" )) ||
     die "Can't bind to port $clnt_port \n";

  
  connect( SOCKET, sockaddr_in( $srv_port, inet_aton($srv_ip) ) )  ||
     die "Can't connect to port $srv_ip : $srv_port \n";
  

}

sub send_message
{
  my $msg = shift;

  send( SOCKET, make_message( $msg ), 0 ) || die "Can't send message";

#  send( SOCKET, make_message( $msg ), 0, pack( 'Sn4x8', AF_INET, $srv_port, inet_aton($srv_ip) )  ) || die "Can't send message";
}



do_connect();
run_capture( "fragment3000.cap" );
send_message(3000);             
stop_capture();


run_capture( "fragment9000.cap" );
send_message(9000);
stop_capture();


run_capture( "fragment_mixed.cap" );
send_message(10);
send_message(100);
send_message(3000);
send_message(20);
send_message(500);
send_message(6000);
send_message(300);
send_message(10);
stop_capture();
