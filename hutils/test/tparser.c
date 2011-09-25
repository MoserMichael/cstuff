#include "vtest/vtest.h"
#include <hutils/http.h>
#include <cutils/dbuf.h>
#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct tagREQUEST_DATA
{
  int flags;
  int content_length;

  Http_method_type method;  
  Http_version_type version;

  int has_finished;
  DBUF rdata;
} REQUEST_DATA;

#define MAX_REQUEST 10

typedef struct tagRECEIVED_REQUESTS
{
  REQUEST_DATA rdata[ MAX_REQUEST ] ;
  size_t idx;

} RECEIVED_REQUESTS;

static int h_header_parsed	   (HTTP_REQUEST *request, void *ctx)
{ 
  RECEIVED_REQUESTS *rq; 
  REQUEST_DATA *rd;

  rq = ( RECEIVED_REQUESTS * ) ctx; 
  rq->idx++;
  rd = &rq->rdata[ rq->idx ];

  DBUF_init( &rd->rdata, 100 );

  rd->flags = request->base.flags;
  rd->content_length = request->base.content_length;
  rd->method = request->method;
  rd->version = request->version;
  return 0;
}

static int h_on_message_data       (HTTP_REQUEST *request, void *data, size_t data_size, void *ctx)
{ 
  RECEIVED_REQUESTS *rq; 
  REQUEST_DATA *rd;

  M_UNUSED( request );

  rq = ( RECEIVED_REQUESTS * ) ctx; 
  rd = &rq->rdata[ rq->idx ];

  DBUF_add( &rd->rdata, data, data_size );

  return 0;
}

static int h_req_finished	   (HTTP_REQUEST *request, void *ctx)
{
  RECEIVED_REQUESTS *rq; 
  REQUEST_DATA *rd;

  M_UNUSED( request );

  rq = ( RECEIVED_REQUESTS * ) ctx; 
  rd = &rq->rdata[ rq->idx ];

  rd->has_finished = 1;

  return 0;
}

// must be at least big enough to hold a full http header line, 
#define BUFFER_SIZE 200

typedef struct tagREQUEST_DATA_PROPS
{
  int flags;
  int content_length;

  Http_method_type method;  
  Http_version_type version;

} REQUEST_DATA_PROPS;

typedef struct tagREQUEST_PARSRE_TEST_DATA {
  const char *file_name;
  int  nrequests_ok;
  int  nrequests_failed;

  REQUEST_DATA_PROPS *rdata;
} REQUEST_PARSER_TEST_DATA;


void do_test(REQUEST_PARSER_TEST_DATA *rdata)
{
  HTTP_REQUEST_PARSER rparser; 
  HTTP_REQUEST request;
  char *buffer;
  BF bf;
  FILE *fp;
  PARSER_STATUS st;
  int status = 0, ch, n_ok = 0, n_failed = 0;
  RECEIVED_REQUESTS received;
  int i;
  size_t j;
  struct stat stb;
  char fname[ 100 ];
	
  received.idx = -1;

  buffer = malloc(BUFFER_SIZE);
  BF_init( &bf, buffer, BUFFER_SIZE );
  
  VASSERT( HTTP_REQUEST_PARSER_init( &rparser,
			      h_header_parsed,
			      h_on_message_data,
			      h_req_finished,
			      &received) == 0 );



  fprintf(stderr,"\t\t%s\n", rdata->file_name );

  fp = fopen( rdata->file_name, "r" );
  VASSERT(fp );
  
  HTTP_REQUEST_init( &request );

  do {
    st = HTTP_REQUEST_PARSER_process( &rparser, &request, &bf );
    switch(st) {
      case PARSER_STATUS_ERROR:
        status = -1;
	++n_failed;
        break;
      case PARSER_STATUS_COMPLETED:
        ++n_ok;
        break;
      case PARSER_STATUS_NEED_MORE_DATA:
        if (BF_check( &bf )) {
	  VASSERT(0);
	}
	VASSERT( bf.get_pos == bf.start ); // check if always ready to receive as much data as possible

        ch = getc(fp);

        if (ch == -1) {
	   // parser did consume the remaining data.
	   VASSERT(bf.get_pos == bf.put_pos);
	   status = 1;

        } else {
          VASSERT( BF_put_size( &bf ) > 0 );  
          BF_put_uint8_t( &bf, (uint8_t) ch );
	}
        break;
    }
  } while(status == 0);

  VASSERT( rdata->nrequests_ok == n_ok );
  VASSERT( rdata->nrequests_failed == n_failed );

  for(i = 0; i < n_ok; i++ ) {
     VASSERT( received . rdata[ i ] . flags == rdata -> rdata [ i ] . flags );
     VASSERT( received . rdata[ i ] . content_length == rdata -> rdata [ i ] . content_length );
     VASSERT( received . rdata[ i ] . method  == rdata -> rdata [ i ] .  method );  
     VASSERT( received . rdata[ i ] . version == rdata -> rdata [ i ] . version );

     if ( DBUF_size( &received . rdata[ i ] . rdata ) != 0) {


        // have received response data, compare it with stored response

	sprintf(fname, "%s_%d", rdata->file_name, (i+1) );

        VASSERT( stat(fname, &stb ) == 0);
	VASSERT( (size_t) stb.st_size  ==  DBUF_size( &received . rdata[ i ] . rdata ) );  
	
	fp = fopen(fname,"r");
	VASSERT(fp != 0);
	for (j  = 0; j < (size_t) stb.st_size; j++ ) {
	   ch = getc( fp );
	   VASSERT( ch != -1) 
	   VASSERT((uint8_t) ch == received . rdata[ i ] . rdata . buf[ j ] ); 
	}
        fclose(fp);

     }
  }

}


static REQUEST_DATA_PROPS props1_r1[] = {
 {  0 , -1 , HTTP_METHOD_GET, HTTP_VERSION_1_1 },
 {  HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH , 192 ,HTTP_METHOD_POST, HTTP_VERSION_1_1 },
 {  HTTP_MESSAGE_FLAG_CONNECTION_CLOSE , -1,HTTP_METHOD_GET, HTTP_VERSION_1_1 } 
};

static REQUEST_DATA_PROPS props1_r2[] = {
 {  0 , -1 , HTTP_METHOD_GET, HTTP_VERSION_1_1 },
 {  HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED, -1,HTTP_METHOD_POST, HTTP_VERSION_1_1 },
 {  HTTP_MESSAGE_FLAG_CONNECTION_CLOSE , -1,HTTP_METHOD_GET, HTTP_VERSION_1_1 } 
};

#if 0
static REQUEST_DATA_PROPS props1_r3[] = {
 {  HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH , 192 ,HTTP_METHOD_POST, HTTP_VERSION_1_1 },
};
#endif

static REQUEST_PARSER_TEST_DATA rdata [] = {
    { "data/rq", 3, 0, props1_r1 },
    { "data/rqs", 3, 0, props1_r2 },
#if 0    
    { "data/fh", 3, 0, props1_r3 },
#endif
    { 0 , 0 , 0, 0 }
    
};

void HTTP_request_ok()
{
  int i;

  for( i = 0; rdata[i].file_name != 0; ++i) {
    do_test( &rdata[i] );    
  }
}

static REQUEST_PARSER_TEST_DATA fdata [] = {
    { "data/bad_no_host", 0, 1, 0 },
    { 0 , 0 , 0, 0 }
    
};


void HTTP_request_fail()
{
  int i;

  for( i = 0; fdata[i].file_name != 0; ++i) {
    do_test( &fdata[i] );    
  }
}

