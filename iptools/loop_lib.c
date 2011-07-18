#include "loop_lib.h"
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <malloc.h>
#include <errno.h>
#include <ifaddrs.h>

#include <cutils/bhash.h>
#include <cutils/dring.h>

#define ETHERNET_L2_HEADER_SIZE 14


struct cb_data {
  HASH    fragments;
  int64_t cnt;
  void * cb;
  pcap_t *pcap;
  int datalink_type;
  IP_FRAGMENT_ACTION fragment_action;
};

static int is_datalink_type_supported(int datalink_type)
{
  switch(datalink_type) {
    case DLT_EN10MB:
  //case DLT_IEEE802:
	return 1;
  }

  return 0;
}

// I have no way to know if that works, If you want to add one just try to do the same as what TCPDUMP is doing, when it prints a packet.
int skip_datalink_header(int datalink_type, const uint8_t **frame, size_t *size)
{
   switch(datalink_type) {
     case DLT_EN10MB:
	*frame += ETHERNET_L2_HEADER_SIZE;
	*size  -= ETHERNET_L2_HEADER_SIZE;
	return 1;

     //case DLT_IEEE802:
     //	return 1;	
   }
   return 0;
}



typedef struct tagIP_FRAGMENT_INFO {
  DRING link;

  uint16_t offset;
  uint16_t length;

} IP_FRAGMENT_INFO;


typedef struct tagIP_PACKET_INFO {
  HASH_Entry link;

  char  *fragment_key;
  DRING	 fragments;
  int    has_last_fragment;

} IP_PACKET_INFO;

static int hash_compare(HASH_Entry *entry, void * key, ssize_t key_length)
{
  (void) key_length;
  IP_PACKET_INFO *info = (IP_PACKET_INFO *) entry;
  return strcmp( info->fragment_key, key );
}

static void make_fragment_key( char *key_string, struct ip * ip )
{
   sprintf( key_string, "%s-%s-%d", 
	inet_ntoa( ip->ip_src ), 
	inet_ntoa( ip->ip_src ),
	ip->ip_id );
}



static IP_PACKET_INFO * find_fragment_info( HASH *all_fragments, char *skey, struct ip *ip) 
{
   IP_PACKET_INFO * info;

	
   make_fragment_key( skey, ip );

   info = (IP_PACKET_INFO *) HASH_find( all_fragments, skey, 0 );
   if (info == 0) {

      info = (IP_PACKET_INFO *) malloc( sizeof( IP_PACKET_INFO ) ); 
      if (! info) {
        return 0;
      }
      info->fragment_key = strdup( skey );
      DRING_init( &info->fragments );
      info->has_last_fragment = 0;      		
      HASH_insert( all_fragments, &info->link, info->fragment_key, 0 );  
   }
   return info;
}

static void clear_fragment_info( HASH *all_fragments, char *skey )
{
   IP_PACKET_INFO * info;
   DRING *cur, *next;
	
   info = (IP_PACKET_INFO *) HASH_unlink( all_fragments, skey, 0 );
   if (info) {
     free(info->fragment_key);	
     DRING_FOREACH_SAVE( cur, next, &info->fragments) {
       free(cur); 	
     }
     free(info);	
   }
}

static int has_all_fragments(IP_PACKET_INFO *info)
{
   IP_FRAGMENT_INFO *frag;
   DRING *cur;
   uint16_t cur_pos = 0;

   DRING_FOREACH( cur, &info->fragments ) {
      frag = (IP_FRAGMENT_INFO *) cur;  
      if (cur_pos != frag->offset) {
	 return 0; 
      }
      cur_pos += frag->length;
   }
   return 1;
}

static void *combine_fragments( IP_PACKET_INFO *info, size_t *rlength )
{
   size_t length = 0;
   int is_first = 1;
   int header_len;
   uint8_t *ret,*pos,*data;
   IP_FRAGMENT_INFO *frag;
   DRING *cur;

   DRING_FOREACH( cur, &info->fragments ) {
      frag = (IP_FRAGMENT_INFO *) cur;  
      data = (uint8_t *) (frag + 1);

      length += frag->length;

      if (is_first) {
	length += IP_HDR_LEN( (struct ip *) data );
      }
      is_first = 0;
   }

   if (length > 0xFFFF){
     return 0;
   }

   pos = ret = (uint8_t *) malloc(length);
   if (ret == 0){
     return 0;
   }

   is_first = 1;
   DRING_FOREACH( cur, &info->fragments ) {
      frag = (IP_FRAGMENT_INFO *) cur;  
      header_len = IP_HDR_LEN( (struct ip *) data );
      data = (uint8_t *) (frag + 1);

      if (is_first) {
        memcpy(pos, data, frag->length + header_len);
        pos += frag->length + header_len;
      } else {
        memcpy(pos, data + header_len, frag->length);
        pos += frag->length;
      }
   		
      is_first = 0;
   }

   ((struct ip *) ret)->ip_len = htons( (uint16_t)length );
   ((struct ip *) ret)->ip_off = 0;

   //(struct ip *) ret)->ip_sum //update.
   //update udp checksum too.

   *rlength = length;

   return ret;    	
} 


static int add_fragment( IP_PACKET_INFO *info, PACKET_DATA *data  )
{
   void    *cmb;
   size_t  cmb_size;
   IP_FRAGMENT_INFO *fragment;
   IP_FRAGMENT_INFO *frag;
   DRING *cur;

   fragment = (IP_FRAGMENT_INFO * ) malloc(sizeof (IP_FRAGMENT_INFO) + data->fragment_length);
   if (!fragment) {
     return -1;
   }
   fragment->offset = data->fragment_offset;
   fragment->length = data->fragment_length - IP_HDR_LEN(data->ip);
   memcpy( fragment + 1, data->ip, data->fragment_length );


   // insert into fragment list.
   DRING_FOREACH( cur, &info->fragments ) {
      frag = (IP_FRAGMENT_INFO *) cur;  

      if (frag->offset < fragment->offset && (frag->offset + frag->length) > fragment->offset) {
         // overlapping fragments.
	 free(fragment);
	 return -1;
      }

      if (frag->offset >  fragment->offset) {
	break;
      }
   }
   DRING_insert_before( cur, &fragment->link );

   if (data->fstate == FRAGMENTS_LAST) {
     info->has_last_fragment = 1;
   }
   if (info->has_last_fragment) {
     if (has_all_fragments( info )) {
	cmb = combine_fragments( info, &cmb_size );
	if (!cmb) {
	  return -1;
	} 
	data->frame_data= cmb;
	data->frame_size= cmb_size;
	data->ip = cmb;
	data->fstate = FRAGMENTS_NONE;

        return 1;
     }
  }
  return 0;
}




static int process_fragments( PACKET_DATA *data, HASH *fragments, IP_FRAGMENT_ACTION fragment_action )
{

   int fragoff,offval;
   IP_PACKET_INFO *hdr;
   int rt; 
   char skey[50];  

   offval = ntohs( data->ip->ip_off );
   fragoff = offval & 0x1FFF;


   if ((offval & IP_MF) != 0) {
     data->fstate = fragoff == 0 ? FRAGMENTS_FIRST : FRAGMENTS_MIDDLE;
   } else if (fragoff != 0) {
     data->fstate = FRAGMENTS_LAST;
   } else {
     data->fstate = FRAGMENTS_NONE;
   }
   data->fragment_offset = fragoff << 3; //fragoff * 8;
   data->fragment_length = ntohs( data->ip->ip_len );

   if (data->fstate != FRAGMENTS_NONE) {
     switch( fragment_action ) {
       case IP_FRAMGENT_PASS_AS_IS:
	 return 0;	

       case IP_FRAGMENT_TREAT_AS_ERROR:
	 return -1;	


       case IP_FRAGMENT_REASSEMBLE:
	 hdr = find_fragment_info( fragments, skey, data->ip );
	 if (!hdr) {
	   return -1;
	 }
	 rt = add_fragment( hdr, data );    
	 if (rt == 1) {
	    clear_fragment_info( fragments, skey );		    
	 }
	 return rt;

     }

   }
   return 0;
}
  

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
   skip_datalink_header( cbdata->datalink_type, &packet, &size);


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

  if (!is_datalink_type_supported( datalink_type) ) {
     fprintf(stderr, "ERROR: data link layer type not supported for file %s link-type %u link-type-name %s\n",
		file, datalink_type, datalink_type_name ? datalink_type_name : "<unknown>" );
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

  if (HASH_init( &cb.fragments, 100, 0, hash_compare, 0) ) {
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


