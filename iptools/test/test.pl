#!/usr/bin/perl

#
# Makes the capture file used by test.pl
#

if (  ! defined  $ENV{ 'BIN_ROOT_DIR'  }  ) {
  print  <<EOF
BIN_ROOT_DIR environment not defined.
Run make from current directory.
EOF
;
  exit( 1 );
}

# ---------------- test loop_udp ----------------------------------

sub test_loop_udp
{
  my $LOOP_UDP=$ENV{'BIN_ROOT_DIR'}."/bin/loop_udp";

  print "Check fragment3000 - defragentation\n";
  `$LOOP_UDP -s -i fragment3000.cap >t1`;  
  check_defragmented_content("t1",3000);
  `rm -f t1`;

  print "Check fragment9000 - defragentation\n";
  `$LOOP_UDP -s -i fragment9000.cap >t1`;  
  check_defragmented_content("t1",9000);
  `rm -f t1`;


  print "Check fragment9000 - no defrgmentation\n";
   `$LOOP_UDP -s -n -i fragment9000.cap >t1`;
  $tmp = `wc -l t1`;
  if (int($tmp) != 14) {
     die "14 packets expected in capture fragment9000.cap";
  }
  `rm -f t1`;
}

sub check_data
{
  my $line=shift;
  my $length=shift;

  if ($length == -1) {
    $length = length( $line ) /2; 
  }
  
  if (length($line) != ($length * 2)) {
    die "dump expected to have $length bytes, but has " . (length($line) / 2);
  }

  for($i = 0 ;$length>0; $length --, $i+=2 ) {
    $num = hex( substr($line, $i, 2) );
    if ($num != (($i/2) % 255) ) {
       die "Expected ". ($i % 255) . " at byte offset $i. instead have $num";
    }	
  }
}

sub check_defragmented_content
{
  my $fname = shift;
  my $length = shift;
  my $line;

  `awk '{ print \$8 }' t1 >t2`;
  open(INF, "t2") || die "can't extract";  
  $line = <INF>;
  chomp($line);
  check_data($line,$length);
  close(INF);
  `rm -f $fname`;
}

# ---------------- test udpsim  ----------------------------------

sub test_udpsim
{
  $CAPTURE_FILE="fragment_mixed.cap";

  $CAPTURE_CLIENT="192.168.8.154";
  $CAPTURE_SERVER="10.63.3.249";
  $CAPTURE_SERVER_PORT="11112";

  #$REPLAY_CLIENT=find_iface_with_mtu( 1500 );
  #$REPLAY_SERVER=$REPLAY_CLIENT;
  $REPLAY_CLIENT="127.0.0.1";
  $REPLAY_SERVER="127.0.0.1";
  $REPLAY_SERVER_PORT="12321";

  $OUT_DIR="out";
  
  check_root();
  start_udp_echo($REPLAY_SERVER_PORT);

  $UDPSIM=$ENV{'BIN_ROOT_DIR'}."/bin/udpsim";
 
  system("mkdir out");

  print("$UDPSIM -v -x $CAPTURE_FILE -c $CAPTURE_CLIENT -i $REPLAY_CLIENT -f \"dst $CAPTURE_SERVER\" -r dst-port,$CAPTURE_SERVER_PORT,$REPLAY_SERVER_PORT -r dst-ip,$CAPTURE_SERVER,$REPLAY_SERVER -o $OUT_DIR/out.cap -y \"host $REPLAY_CLIENT\"");
  #exit(1);

  system("$UDPSIM -v -x $CAPTURE_FILE -c $CAPTURE_CLIENT -i $REPLAY_CLIENT -f \"dst $CAPTURE_SERVER\" -r dst-port,$CAPTURE_SERVER_PORT,$REPLAY_SERVER_PORT -r dst-ip,$CAPTURE_SERVER,$REPLAY_SERVER -o $OUT_DIR/out.cap -y \"host $REPLAY_CLIENT\"");

  stop_udp_echo();

  test_cap_result();
  `rm -f t1`; 
}

sub test_cap_result
{
  my $LOOP_UDP=$ENV{'BIN_ROOT_DIR'}."/bin/loop_udp";

  `$LOOP_UDP -s -i out/out.cap >t1`;  
  my $count = `grep -F " $REPLAY_SERVER_PORT " t1 | wc -l`;

  if ($count != 16) {
    die "Expected 16 packets, but where $count";
  }

  `grep -F " $REPLAY_SERVER_PORT " t1 >t2`;
  check_defragmented_content("t2",-1);


}
 


sub find_iface_with_mtu
{
  my $mtu_size = shift;

  system("ifconfig >x");
  open(TMPF,"x");
  while(<TMPF>) {
     $line = $_;
     if ($line =~ /inet addr:(\S+)/) {
       $addr = $1;
     }
     if ($line =~ /MTU:(\d+)/) {
       $mtu = $1;
       if ($mtu == $mtu_size) {
	 return $addr;
       }
     }
  }
  die;
  return "";
}
sub check_root
{
  if (int($<) != 0) {
    die "ERROR: udpsim test must be run as root. sorry.\n";
  }
}

sub start_udp_echo
{
  my $port = shift;

  $udp_echo_pid = fork();
  if (! defined $udp_echo_pid) {
    die "can't fork";
  }	
  if ($udp_echo_pid == 0) {
     exec("./udpecho.pl $port");
     exit(1);
  } 
}

sub stop_udp_echo
{
  kill(15,$udp_echo_pid);
}

# --------------------------------------------

test_loop_udp();
test_udpsim();


print " *** all tests passed *** \n";

