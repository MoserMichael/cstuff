#include "vtest/vtest.h"
#include <hutils/http.h>
#include <cutils/dbuf.h>
#include <stdio.h>



#define BUFER_SIZE 100

static int  compare_text(FILE *fp, BF *bf) 
{
  int ch;
  uint8_t tch;

  while( BF_can_get( bf ) != 0 ) {
     ch = fgetc( fp );

     if ( BF_get_uint8_t( bf, &tch ) ) {
       return -1;
     }

     if ( (uint8_t) ch != tch ) {
       return -1;
     }
     //fprintf(stdout,"%c",(char) ch);
  }
  return 0;
}

#define BUFFER_SIZE  80


static void test_write_header( HTTP_RESPONSE *resp, const char *file)
{
  char *buffer;
  BF bf;
  HTTP_RESPONSE_WRITER writer;  
  PARSER_STATUS stat;
  FILE *fp;

  fp = fopen(file,"r");
  VASSERT(fp != 0 );

  buffer = malloc(BUFFER_SIZE);
  BF_init( &bf, buffer, BUFFER_SIZE );

  HTTP_RESPONSE_WRITER_init( &writer, resp );

  while(1) {
    stat =  HTTP_RESPONSE_WRITER_write( &writer, &bf );
    switch(stat) {
      case PARSER_STATUS_ERROR:
        VASSERT(0);
        break;
    
      case PARSER_STATUS_COMPLETED:
      //fprintf(stdout,"\n\n");
        VASSERT( compare_text( fp, &bf ) == 0 );
	VASSERT( bf.get_pos == bf.put_pos );
	fclose(fp);
	return;

      case PARSER_STATUS_NEED_MORE_DATA:
        VASSERT( compare_text( fp, &bf ) == 0 );
	BF_put_mode( &bf );
        break;
    }
  }
}


void HTTP_writer_ok()
{
  HTTP_RESPONSE resp;

  HTTP_RESPONSE_init( &resp, HTTP_VERSION_1_1, 200 );

  HTTP_MESSAGE_set_content_length(  &resp.base, 1339 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "Date", "Mon, 12 Sep 2011 11:10:16 GMT" ) == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "Server", "Apache/2.2.9 (Debian) mod_ssl/2.2.9 OpenSSL/0.9.8g" ) == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "Last-Modified", "Tue, 30 Nov 2010 06:58:48 GMT" ) == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "ETag", "\"1f6e0-53b-4963fb86b7a00\"" ) == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "Accept-Ranges", "bytes" ) == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "P3P", "CP=\"NOI DSP COR NID CUR ADM DEV OUR BUS\"") == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "Content-Type", "text/css" ) == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "Age", "0" ) == 0 );
  VASSERT( HTTP_MESSAGE_add_header( &resp.base, "Via", "1.1 localhost.localdomain" ) == 0 );

  test_write_header( &resp, "data/resp" ); //HTTP_RESPONE *resp, const char *file)
}


