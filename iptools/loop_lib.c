#include "loop_lib.h"
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <malloc.h>
#include <errno.h>
#include <ifaddrs.h>
#include <unistd.h>

#include <cutils/bhash.h>
#include <cutils/dring.h>

#include "datalink.h"
#include "fragments.h"

struct cb_data {
  HASH    fragments;
  int64_t cnt;
  void * cb;
  pcap_t *pcap;
  int datalink_type;
  int datalink_processor_type;
  IP_FRAGMENT_ACTION fragment_action;
};


 

static void callback_udp_on_ipv4(
	uint8_t * cb,
	const struct pcap_pkthdr *pkthdr,
	const uint8_t * packet)
{

   PACKET_DATA data;


   size_t size; 
   struct cb_data *cbdata;
   int rt;

   cbdata = (struct cb_data *) cb;   
   size = pkthdr->caplen;

   data.count = cbdata->cnt;
   cbdata->cnt ++;

   data.frame_data = packet;
   data.frame_size = size = pkthdr->caplen;
   data.capture = cbdata->pcap;
   data.pkthdr = pkthdr;

   // skip ethernet header. ( rely on type stored in capture file format)
   datalink_skip_layer_data( cbdata->datalink_processor_type, &packet, &size );

   // ip header
   data.ip = (struct ip *) packet;

   if (data.ip->ip_v != 4) {
      return;  // not ipv4
   }

   if (data.ip->ip_p != 0x11) {
      return;  // encapsulate protocol is not udp. 
   }   

   rt = process_fragments( &data, &cbdata->fragments , cbdata->fragment_action );  
   if (rt < 0) {
     // fragment not reassembled, or error processing it.
     return;
   }

   if (rt == 1) {
     size = data.frame_size;
     packet = data.frame_data;
   }

   // skip ip header   
   packet += IP_HDR_LEN( data.ip );
   size -= IP_HDR_LEN( data.ip );


   if (data.fstate != FRAGMENTS_MIDDLE  && data.fstate != FRAGMENTS_LAST) {
     data.udp = (struct udphdr *) packet;

     // skip udp header
     packet += sizeof(struct udphdr);
     size -= sizeof(struct udphdr);
   } else {
     data.udp = 0;
   }

   // store payload data.
   data.udp_data = packet;
   data.udp_data_size = size;


   ((INSPECT_UDP_ON_IPV4) cbdata->cb)( &data ); //cbdata->cnt, pkthdr, hdr, pudpdhr, packet, size);
}



int inspect_udp_on_ipv4(const char *file,const char *filter_expr, IP_FRAGMENT_ACTION fragment_action, INSPECT_UDP_ON_IPV4 inspect_udp)
{
  char errbuf[512];
  pcap_t *pcap;
  struct cb_data cb;
  struct bpf_program fp;
  int datalink_type;
  const char *datalink_type_name;
 
  if ((pcap = pcap_open_offline(file, errbuf)) == NULL) {
    fprintf(stderr, "Can't open capture %s\n",file);
    return -1;
  }


  datalink_type = pcap_datalink( pcap );
  datalink_type_name = pcap_datalink_val_to_name( datalink_type );

  cb.datalink_processor_type = datalink_is_type_supported( datalink_type );
  if (cb.datalink_processor_type == -1)  {
     fprintf(stderr, "ERROR: data link layer type not supported for file %s link-type %u link-type-name %s\n",
	file, 
	datalink_type, datalink_type_name ? datalink_type_name : "<unknown>" );

     fprintf(stderr,"Supported link layer types are:\n");
     datalink_print_supported_types();
     return -1;	
  }


  if (datalink_type_name == NULL) {
     fprintf(stderr, "reading from file %s, link-type %u\n",
			    file, datalink_type);
  } else {
     fprintf(stderr,
            "reading from file %s, link-type %s (%s)\n",
			    file, datalink_type_name,
			    pcap_datalink_val_to_description( datalink_type ));
  }
  
  if (filter_expr != 0) {
    if (pcap_compile(pcap, &fp, filter_expr, 0, 0) == -1) {
      fprintf(stderr,"Illegal filter expression %s : %s\n", filter_expr, pcap_geterr(pcap) );
      return -1;
    }
    if (pcap_setfilter(pcap, &fp) == -1) {
      fprintf(stderr, "Couldn't install filter %s: %s\n", filter_expr, pcap_geterr(pcap));
      return -1;
    }
  }

  cb.cb = inspect_udp;
  cb.cnt = 0;
  cb.pcap = pcap;
  cb.datalink_type = datalink_type;
  cb.fragment_action = fragment_action;

  if (init_fragments(  &cb.fragments )) {
    return -1;
  }

  pcap_loop(pcap, -1, callback_udp_on_ipv4, (uint8_t *) &cb);
	
  pcap_close(pcap);
  return 0;
}

void show_packet(FILE *out, int64_t count, const struct timeval *tval, struct ip *ip, struct udphdr *udp, const void *data, size_t size)
{
  char ipname_src[30], ipname_dst[30];
  size_t i;
  uint8_t *pos;

  strcpy( ipname_src, inet_ntoa( ip->ip_src ) );  	   
  strcpy( ipname_dst, inet_ntoa( ip->ip_dst ) );


  if (udp != 0) {
     fprintf(out, 
  
#if __WORDSIZE == 64
  "%ld %ld.%06ld %s %u %s %u %u ",
#else
  "%lld %ld.%06ld %s %u %s %u %u ",
#endif
	  count,
	  tval->tv_sec, 
	  tval->tv_usec,
	  ipname_src,
	  (uint32_t) ntohs( UDP_SPORT( udp ) ),
          ipname_dst,
	  (uint32_t) ntohs( UDP_DPORT( udp ) ),
          (unsigned int) size);
  } else {

     fprintf(out, 
  
#if __WORDSIZE == 64
  "%ld %ld.%06ld %s ? %s ? %u ",
#else
  "%lld %ld.%06ld %s ? %s ? %u ",
#endif
	  count,
	  tval->tv_sec, 
	  tval->tv_usec,
	  ipname_src,
          ipname_dst,
          (unsigned int) size);

  }

  for(i = 0, pos = (uint8_t *) data; i < size; i++ ) {
    fprintf(out, "%02x", (unsigned int) pos[ i ] );
  }
  fprintf(out,"\n");

}


static void dispatcher_handler(u_char *ctx,
                        const struct pcap_pkthdr *header, const u_char *pkt_data)
{

    CAPTURECTX *pctx = (CAPTURECTX *) ctx;

#if 0
    size_t i;
    uint8_t *pos;

    fprintf(stderr,
#if __WORDSIZE == 64
      "%ld %ld %u %u\n",
#else
      "%ld %ld %u %u\n",
#endif
	header->ts.tv_sec,
	header->ts.tv_usec,
	header->caplen,
	header->len);

    for(i = 0, pos = (uint8_t *) pkt_data; i < header->caplen; i++ ) {
      fprintf(stderr, "%02x", (unsigned int) pos[ i ] );
    }
    fprintf(stderr,"\n");
#endif

    pcap_dump( (uint8_t * ) pctx->dumpfile,header,pkt_data);
    pctx->packets_captured++;
}


CAPTURECTX * live_capture_init( const char *device,  const char *filter, const char *file_name, int mode )
{
  CAPTURECTX *ret;

  char error[ PCAP_ERRBUF_SIZE ];
  bpf_u_int32 SubNet,NetMask;
  struct bpf_program filter_code;

  ret = (CAPTURECTX *) malloc( sizeof( CAPTURECTX ) );
  if (!ret) {
    return 0;
  }
  ret->packets_captured = 0;

  strcpy(error,"");
  ret->dev = pcap_open_live( device, 65535, mode /* promiscuous mode */, 1000 , error);
  if (!ret->dev) {
    fprintf(stderr,"Can't open capture device %s, error %s errno %d\n", device, error, errno );
    return 0;
  }

  if(pcap_lookupnet(device, &SubNet, &NetMask, error)<0){
    fprintf(stderr,"\nUnable to obtain the netmask. error %s errno %d\n", error, errno);
    return 0;
  }

  if(pcap_compile(ret->dev, &filter_code, filter, 1, NetMask)<0){
    fprintf(stderr,"\nError compiling filter: wrong syntax. Filter %s. error %s\n", filter, pcap_geterr( ret->dev ));
    return 0;
  }
      
  if(pcap_setfilter(ret->dev, &filter_code)<0){
    fprintf(stderr,"Error setting the filter\n");
    return 0;
  }
  
  ret->dumpfile=pcap_dump_open(ret->dev, file_name);
  if(ret->dumpfile==NULL){
    fprintf(stderr,"Error opening output file %s. errno=%d\n", file_name, errno);
    return 0;
  }
  return ret;
}

void live_capture_start(CAPTURECTX *ctx)
{
  pcap_loop(ctx->dev, 0, dispatcher_handler, (unsigned char *) ctx);
}

void live_capture_stop(CAPTURECTX *ctx)
{
  pcap_breakloop( ctx->dev );
}

void live_capture_cleanup(CAPTURECTX *ctx)
{
  pcap_dump_close( ctx->dumpfile );
  pcap_close( ctx->dev );
  free(ctx);
}


#define DEV_PREFIX  "rpcap://\\Device\\NPF_"
    
const char * is_interface_address( struct in_addr *addr )
{

#if 0
  struct ifaddrs *ifaddr, *ifa;
  int family;

  if (getifaddrs(&ifaddr) == -1) {
     //error_msg( "Can't get list of network interfaces on this machine. (getifaddrs failed)" );
     return 0;
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;

      family = ifa->ifa_addr->sa_family;

      if (family == AF_INET) {
	if ( ((struct sockaddr_in *) ifa->ifa_addr)->sin_addr.s_addr ==  addr->s_addr)
#ifdef __CYGWIN__
          {
            char *tmp2;

            tmp2 = malloc( strlen(ifa->ifa_name) + strlen(DEV_PREFIX) + 1 );
            strcpy(tmp2, DEV_PREFIX);   
            strcat(tmp2, ifa->ifa_name);   
	    return tmp2;
          }
#else
	  return strdup( ifa->ifa_name );
#endif
      } 
    } 

#else 
  pcap_if_t *devpointer;
  char ebuf[PCAP_ERRBUF_SIZE];
  pcap_addr_t *paddr;

  if (pcap_findalldevs(&devpointer, ebuf) < 0) {
    fprintf( stderr,"ERROR: %s\n", ebuf);
    return 0;
  }

  if (devpointer == 0) {
    fprintf( stderr,"ERROR: Can't capture traffic from no devices\n");
    return 0;
  }

  while(devpointer != 0) {
    paddr = devpointer->addresses;
    while( paddr != 0 ) {
      struct sockaddr_in *addrp = (struct sockaddr_in *) paddr->addr;
      if (addrp->sin_family == AF_INET) {

#if 0
        fprintf(stderr,"%s - %s\n", 
		    devpointer->name, 
		    inet_ntoa( addrp->sin_addr ) );
#endif

        if (addrp->sin_addr.s_addr == addr->s_addr) {
           return devpointer->name;
	}
      }
      paddr = paddr->next;
    }
    devpointer = devpointer->next;
  }
  return 0;

#endif
}


//
// derived from here http://web.eecs.utk.edu/~cs594np/unp/checksum.html
//
uint16_t ip_cksum(void *ip_, int len)
{
  int32_t sum = 0;  /* assume 32 bit long, 16 bit short */
  uint16_t *ip = (uint16_t *) ip_;

  ((struct ip *) ip)->ip_sum = 0;

  while(len > 1){
      sum += *(ip)++;
      if(sum & 0x80000000)   /* if high order bit set, fold */
        sum = (sum & 0xFFFF) + (sum >> 16);
      len -= 2;
  }

  if(len)       /* take care of left over byte */
    sum += (unsigned short) *(unsigned char *)ip;
          
  while(sum>>16)
    sum = (sum & 0xFFFF) + (sum >> 16);

  return ~sum;
}

//
// derived from here http://minirighi.sourceforge.net/html/udp_8c-source.html
//
uint16_t udp_cksum( struct ip *ip_, struct udphdr *udp_  )

{
  const uint16_t *buf=(uint16_t *) udp_;
  uint16_t *ip_src=(uint16_t *) &ip_->ip_src.s_addr;
  uint16_t *ip_dst=(uint16_t *) &ip_->ip_dst.s_addr;
  uint32_t sum;
  size_t length= ntohs( UDP_LEN(udp_) );
  size_t len = length;

  UDP_CKSM(udp_) = 0;

  // Calculate the sum                                            //
  sum = 0;
  while (len > 1)
  {
    sum += *buf++;
    if (sum & 0x80000000)
      sum = (sum & 0xFFFF) + (sum >> 16);
     len -= 2;
  }

  // Add the padding if the packet lenght is odd          //
  if ( len & 1 )
    sum += *((uint8_t *)buf);

  // Add the pseudo-header                                        //
  sum += *(ip_src++);
  sum += *ip_src;

  sum += *(ip_dst++);
  sum += *ip_dst;

  sum += IPPROTO_UDP;
  sum += htons(length);

  // Add the carries                                              //
  while (sum >> 16)
    sum = (sum & 0xFFFF) + (sum >> 16);

  // Return the one's complement of sum                           //
  return ( (uint16_t)(~sum)  );
}

int get_interface_mtu(struct in_addr *interface_addr  )
{
   int s;
   struct sockaddr_in saddr;
   int rt, port = 10000;
   int opt_val;
   socklen_t opt_len;

   s = socket( AF_INET, SOCK_DGRAM, 0);
   if (s == -1) {
     return -1;
   }

   saddr.sin_family = AF_INET;
   saddr.sin_addr = *interface_addr;

   while(1) {
       saddr.sin_port = ntohs( port );
       rt = bind(s, (struct sockaddr *) &saddr, sizeof(saddr));
       if (rt == -1 && errno == EADDRINUSE) {
	 continue;
       }
       port ++;
   }

   if (rt == -1) {
     goto err;
   }

   opt_len = sizeof( opt_val );
   if (getsockopt( s, IPPROTO_IP, IP_MTU, (void *) &opt_val, &opt_len ) ) {
     goto err;
   }

   close(s);
   return opt_val;

err:
   close(s);
   return -1;
}
