#include <stdio.h>
#include <stdlib.h>

void print_help() {
  fprintf(stderr,"%s",
	"\n"
	"./captest -i <interface ip> -w <output file> [-f <filter expression>]\n"
	"\n"
	"Captures packets with libpcap, and dumps them into output file.\n"
	"This program tests the functioning of libpcap.\n"
	"\n"
	"\n"
	"Options:\n"
	"	-i <interface ip>	ip address of interface\n"
	"	-w <output file>	output file (pcap format)\n"
	"	-f <filter expression>	(for syntax see man pcap-filter(7))\n"
  );
  exit(1);
}  
