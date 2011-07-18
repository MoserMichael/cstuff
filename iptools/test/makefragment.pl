#!/usr/bin/perl

use Socket;

$port="9000";
$host="10.63.3.249";
$dev="eth0";
$packet_size=200;


my ($p, $i, $pid);

for($i=0;$i<($packet_size/2);$i++){
  $p=$p . sprintf("%02x",$i % 255);
}

if (length($p) != $packet_size) {
  die "Ups wrong packet...\n$p\n" . length($p)  ;
}


$proto = getprotobyname('udp');
$iaddr   = inet_aton($host);
$paddr   = sockaddr_in($port, $iaddr);

system("tcpdump -vvv -i $dev -s 0 -w fragment${packet_size}.dump 'host $host' >fragment${packet_size}.text 2>&1 &");


$pid=`pidof tcpdump`;
chomp($pid);
sleep(1);

socket(SOCK, PF_INET, SOCK_DGRAM, $proto) || die "socket: $!";
connect(SOCK, $paddr) || die "connect: $!";
syswrite(SOCK,$p);
close(SOCK);

system("kill -2 $pid");



