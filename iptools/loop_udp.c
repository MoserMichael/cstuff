#include "loop_lib.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


static const char *infile;
static const char *filter;
static int show_hdr = 1;
static struct timeval first_time;
static int has_first_time;
static IP_FRAGMENT_ACTION fragment_action = IP_FRAGMENT_REASSEMBLE; 

void print_help();


void dump_udp_on_ipv4 (PACKET_DATA *data) //int64_t count, const struct pcap_pkthdr *pkthdr, struct ip *ip, struct udphdr *udp, const uint8_t *data, size_t size)
{
#if 0
  int offval = ntohs(data->ip->ip_off);
  int fragoff = offval & 0x3FFF;
  int flags = offval & 0xC000;
#endif  
  struct timeval diftime;

   

  if (!has_first_time) {
     first_time = data->pkthdr->ts; 
     has_first_time = 1;
  }

#if 0
  if( (flags & IP_MF) != 0) { 

    fprintf(stderr,
#if __WORDSIZE == 64
    "More fragments bit on packet %ld\n",
#else
    "More fragments bit on packet %lld\n",
#endif
    data->count);
    exit(1);
  }
  if ( fragoff != 0) {
    fprintf(stderr,
#if __WORDSIZE == 64
    "fragment offset %d on packet %ld\n",
#else
    "fragment offset %d on packet %lld\n",
#endif
    fragoff, data->count);
    exit(1);
  }
#endif

  timersub( &data->pkthdr->ts, &first_time, &diftime);


  show_packet( stdout, data->count, &diftime, data->ip, data->udp, data->udp_data, data->udp_data_size );

}

void print_header()
{
  fprintf(stdout,"seq(#) time(sec.usec) ipv4_src udp_port ipv4_dst udp_dst data_len data\n");
}






void parse_cmd_line(int argc, char *argv[]) {

  int c;

  while ( (c = getopt(argc, argv, "hnsi:f:")) != -1) {
     switch(c) {
	case 'h':
	   print_help();
	   break;
	case 'i':
	   infile = strdup( optarg );
	   break;
	case 'f':
           filter = strdup( optarg );
	   break;
	case 'b':
	   show_hdr = 0;
	   break;
	case 'n':
	   fragment_action = IP_FRAMGENT_PASS_AS_IS;
	   break;
      }
  }
  if (!infile) {
    print_help();
  }	
}

int main(int argc, char *argv[])
{
  parse_cmd_line(argc, argv);


  if (show_hdr) {
    print_header();
  }
  inspect_udp_on_ipv4(infile, filter, fragment_action, dump_udp_on_ipv4 );
  return 0;
}


