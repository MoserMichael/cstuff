#include <stdio.h>
#include <stdlib.h>

void print_help() {
  fprintf(stderr,"%s",
	"loop_udp -i <infile> [-s] [-f <filter>] [-n]\n"
	"\n"
	"Print specific information on all udp packets in input capture file.\n"
	"It shows the following information\n"
	"  seq(#) 	  - sequence number\n"
	"  time(sec.usec)  - time of packet\n"
	"  ipv4_src        - source ip\n"
	"  udp_port 	  - udp port\n"
	"  ipv4_dst 	  - destination ip\n"
	"  udp_dst 	  - destination port\n"
	"  data_len 	  - length of UDP payload\n"
	"  data		  - hexadecimal dump of UDP payload.\n"
	"\n"
	"By default, IP fragments are reassembled into packets, this requires that fragments have been\n"
	"captured and are present in the capture file.\n"
	"\n"
	"For whatever reason I could not bring tshark to show packet data, so i did my own, by using libpcap;\n"
	"\n"
	" -i <infile>	- input capture file\n"
	" -s		- don't show header line\n"
	" -f <filter>	- filter expression (for syntax see man pcap-filter(7))\n"
	" -n		- don't reassemble IP fragments, (default is to reassemble them)\n"
  );
  exit(1);
}  
