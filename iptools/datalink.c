#include "datalink.h"
#include <pcap.h>



#define ETHERNET_L2_HEADER_SIZE 14
#define FDDI_HDR_LEN 13
#define CHDLC_HDRLEN 4

typedef int (*SKIP_DATALINK_LAYER) ( const uint8_t **frame, size_t *size );

typedef struct tagDATALINK_INFO {
  int datalink_type;
  SKIP_DATALINK_LAYER skip_datalink;
} 
  DATALINK_INFO;


static int skip_eth_datalink_header( const uint8_t **frame, size_t *size);
static int skip_ffdi_header( const uint8_t **frame, size_t *size);
static int skip_hdlc_header( const uint8_t **frame, size_t *size);
 
static DATALINK_INFO datalink_info [] = {
  { DLT_EN10MB, skip_eth_datalink_header },  
  { DLT_FDDI, skip_ffdi_header },
  { DLT_C_HDLC, skip_hdlc_header },
  { -1, 0 }
};


int datalink_is_type_supported(int datalink_type)
{
  int i;

  for( i = 0; datalink_info[ i ].datalink_type != -1; i++ ) {
    if (datalink_info[ i ].datalink_type == datalink_type ) {
	return i;
    }
  }
  return -1;
}

void datalink_print_supported_types()
{
  int i;
  const char *name;
  int ty;

  for( i = 0; datalink_info[ i ].datalink_type != -1; i++ ) {
     ty = datalink_info[i].datalink_type;  
     name = pcap_datalink_val_to_name( ty );  
     if (name != 0) {
	fprintf(stderr, "\t%s\n", name );
     } else {
	fprintf(stderr,"\tdatalink type %d\n", ty);
     }
  }

}

int datalink_skip_layer_data(  int idx,  const uint8_t **frame, size_t *size )
{
  return datalink_info[ idx ].skip_datalink ( frame, size );
}


static int skip_eth_datalink_header( const uint8_t **frame, size_t *size)
{
   *frame += ETHERNET_L2_HEADER_SIZE;
   *size  -= ETHERNET_L2_HEADER_SIZE;
   return 1;
}

static int skip_ffdi_header( const uint8_t **frame, size_t *size)
{
   *frame += FDDI_HDR_LEN;
   *size  -= FDDI_HDR_LEN;
   return 1;
}
 
static int skip_hdlc_header( const uint8_t **frame, size_t *size)
{
   *frame += CHDLC_HDRLEN;
   *size  -= CHDLC_HDRLEN;
   return 1;
}
 


