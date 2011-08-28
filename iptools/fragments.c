/* Copyright NDS Group - 2011 */
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

#include "fragments.h"

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




int process_fragments( PACKET_DATA *data, HASH *fragments, IP_FRAGMENT_ACTION fragment_action )
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
	    return 1;
	 }
   	 return -1; // if fragment not assembled - do not pass higher up

     }

   }
   return 0;
}

int init_fragments( HASH *fragments )
{
  if (HASH_init( fragments, 100, 0, hash_compare, 0) ) {
    return -1;
  }
  return 0;
}
 
