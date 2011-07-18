#ifndef __LOOP_UDP_H__
#define __LOOP_UDP_H__


#include <stdint.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <pcap.h>

//-----------------------------------------------------
// Stuff for working with IP/UDP headers
//-----------------------------------------------------


#ifdef __linux__

#define UDP_SPORT(x) (x)->source
#define UDP_DPORT(x) (x)->dest
#define UDP_LEN(x) (x)->len
#define UDP_CKSM(x) (x)->check
	
#else

#define UDP_SPORT(x) (x)->uh_sport
#define UDP_DPORT(x) (x)->uh_dport
#define UDP_LEN(x) (x)->uh_ulen
#define UDP_CKSM(x) (x)->uh_sum

#endif

#define IP_HDR_LEN(ip) ((int) (ip)->ip_hl << 2)


//
// derived from here http://web.eecs.utk.edu/~cs594np/unp/checksum.html
//
uint16_t ip_cksum(void *ip_, int len);

//
// derived from here http://minirighi.sourceforge.net/html/udp_8c-source.html
//
uint16_t udp_cksum( struct ip *ip_, struct udphdr *udp_  );


//-----------------------------------------------------
// Stuff for working with capture files
//-----------------------------------------------------

typedef enum {
  FRAGMENTS_NONE,
  FRAGMENTS_FIRST,
  FRAGMENTS_MIDDLE,
  FRAGMENTS_LAST,
} 
  FRAGMENTS_STATE;

typedef struct tagPACKET_DATA {


  pcap_t *capture;

  int64_t count;
  const struct pcap_pkthdr *pkthdr;
  
  const uint8_t *frame_data;
  size_t frame_size;

  struct ip *ip;

  struct udphdr *udp;
  const uint8_t *udp_data;
  size_t udp_data_size;

  FRAGMENTS_STATE fstate;
  uint16_t fragment_offset;
  uint16_t fragment_length;

} PACKET_DATA;

typedef enum {

  IP_FRAGMENT_REASSEMBLE,
  IP_FRAMGENT_PASS_AS_IS,
  IP_FRAGMENT_TREAT_AS_ERROR,

} IP_FRAGMENT_ACTION;

typedef void (*INSPECT_UDP_ON_IPV4) (PACKET_DATA *data);

int inspect_udp_on_ipv4(const char *file,const char *filter_expr, IP_FRAGMENT_ACTION fragment_action, INSPECT_UDP_ON_IPV4 inspect_udp );

void show_packet(FILE *out, int64_t count, const struct timeval *tval, struct ip *ip, struct udphdr *udp, const void *data, size_t size);

const char * is_interface_address( struct in_addr *addr );


//-----------------------------------------------------
// Stuff for doing live captures.
//-----------------------------------------------------

typedef struct tagCAPTURECTX
{
  pcap_t *dev;
  pcap_dumper_t *dumpfile; 
  uint64_t packets_captured;
} CAPTURECTX;

CAPTURECTX * live_capture_init( const char *device,  const char *filter, const char *file_name, int mode );

void live_capture_start(CAPTURECTX *ctx);

void live_capture_stop(CAPTURECTX *ctx);

void live_capture_cleanup(CAPTURECTX *ctx);

#endif

