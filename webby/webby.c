#include "webby.h"
#include "webbyimpl.h"
#include <hutils/http.h>
#include <string.h>
#include <stdio.h>
#include <cutils/properties.h>
#include <butils/logg.h>

// ====================================================================
#define RESERVED_FOR_CHUNK_HEADER_SIZE 15

#define LAST_CHUNK_NOT_FIRST "\r\n0\r\n"
#define LAST_CHUNK_NOT_FIRST_SIZE  5

#define LAST_CHUNK_FIRST  "0\r\n"
#define LAST_CHUNK_FIRST_SIZE  3

#define HTTP_100_CONTINUE_RESPONSE "HTTP/1.1 100 Continue\r\n\r\n"
#define HTTP_100_CONTINUE_RESPONSE_LEN 25


// ====================================================================
typedef struct tagDATA_SINK_FILTER {
  HTTP_FILTER base;
} DATA_SINK_FILTER;

typedef struct tagDATA_SINK_FILTER_CONNECTION_CONTEXT {
  BF *out_buf;
  HTTP_RESPONSE_WRITER writer;  
  void *impl_connection_ctx;

} DATA_SINK_FILTER_CONNECTION_CONTEXT; 


DATA_SINK_FILTER_CONNECTION_CONTEXT *DATA_SINK_FILTER_CONNECTION_CONTEXT_init(void *impl_connection_ctx, BF *out_buf)
{
  DATA_SINK_FILTER_CONNECTION_CONTEXT *ret; 

  ret = (DATA_SINK_FILTER_CONNECTION_CONTEXT *) malloc( sizeof( DATA_SINK_FILTER_CONNECTION_CONTEXT ) );
  if (!ret) {
    return 0;
  }
  ret->impl_connection_ctx = impl_connection_ctx; 
  ret->out_buf = out_buf;
  return ret;
}

void DATA_SINK_FILTER_CONNECTION_CONTEXT_free( DATA_SINK_FILTER_CONNECTION_CONTEXT *ctx)
{
  free(ctx);
}

int    sink_on_response_header       (HTTP_RESPONSE *response,  FILTER_CONTEXT *context )
{
   DATA_SINK_FILTER_CONNECTION_CONTEXT *sink = (DATA_SINK_FILTER_CONNECTION_CONTEXT *)  context->connection_ctx;
   BF *bf;
   PARSER_STATUS stat;
   bf = sink->out_buf;
   HTTP_RESPONSE_WRITER_init( &sink->writer, response );
   
   while(1) { 
    stat =  HTTP_RESPONSE_WRITER_write( &sink->writer, bf );
    switch(stat) {
      case PARSER_STATUS_ERROR:
        return -1;
      case PARSER_STATUS_COMPLETED:
        if (WEBBY_impl_send_data( sink->impl_connection_ctx, bf->get_pos, BF_get_size( bf ) ) ) {
	  return -1;
	}
	return 0;

      case PARSER_STATUS_NEED_MORE_DATA:
        if (WEBBY_impl_send_data( sink->impl_connection_ctx, bf->get_pos, BF_get_size( bf ) ) ) {
	  return -1;
	}
        break;
    }
  } while( 1 );

}

int    sink_on_response_data         (HTTP_RESPONSE *response, int is_chunk, RDATA rdata, FILTER_CONTEXT *context )
{
  DATA_SINK_FILTER_CONNECTION_CONTEXT *sink = (DATA_SINK_FILTER_CONNECTION_CONTEXT *)  context->connection_ctx;
  BF *bf;
  void *data;
  size_t size;
  int len;
  char chunk_header[ 20 ];

  M_UNUSED( response );

  if (!is_chunk) {
    return WEBBY_impl_send_data( sink->impl_connection_ctx, rdata.no_chunk.data, rdata.no_chunk.data_size );
  }
  
  bf = rdata.chunk.bf;
  if (bf) { 
    if (rdata.chunk.chunk_no) {
      len = snprintf( chunk_header, sizeof( chunk_header ),  "\r\n%x\r\n", BF_get_size(bf)  );
    } else {
      len = snprintf( chunk_header, sizeof( chunk_header ),  "%x\r\n", BF_get_size(bf) );
    }
    if ( (bf->start - bf->bf) < len) {
      return -1;
    }
    bf->start -= len;
    strncpy( (char *) bf->start, chunk_header, len );
 
    return WEBBY_impl_send_data( sink->impl_connection_ctx, bf->start, bf->put_pos - bf->start  );
 
  } else {
    
    if (rdata.chunk.chunk_no) {
      data = LAST_CHUNK_NOT_FIRST;
      size = LAST_CHUNK_NOT_FIRST_SIZE;
    } else {
      data = LAST_CHUNK_FIRST;
      size = LAST_CHUNK_FIRST_SIZE;
    }
    return WEBBY_impl_send_data( sink->impl_connection_ctx, data, size  );
  }
}

 
int    sink_on_response_complete     (HTTP_RESPONSE *response, FILTER_CONTEXT *context )
{
  DATA_SINK_FILTER_CONNECTION_CONTEXT *sink = (DATA_SINK_FILTER_CONNECTION_CONTEXT *)  context->connection_ctx;

  M_UNUSED( response );
 
  return WEBBY_impl_response_completed( sink->impl_connection_ctx, response->base.flags & HTTP_MESSAGE_FLAG_CONNECTION_CLOSE );
}

DATA_SINK_FILTER *DATA_SINK_FILTER_init()
{
  DATA_SINK_FILTER * ret;

  ret =  (DATA_SINK_FILTER *) malloc( sizeof( DATA_SINK_FILTER ) );
  if (!ret) {
    return 0;
  }
  ret->base.on_response_header =  sink_on_response_header; 
  ret->base.on_response_data = sink_on_response_data;   
  ret->base.on_response_completed = sink_on_response_complete;  

  return ret;
}




// ====================================================================

typedef struct tagSERVLET_RUNNER_FILTER
{
  HTTP_FILTER base;
  ARRAY servlets;
} SERVLET_RUNNER_FILTER;

typedef struct SERVLET_FILTER_CONNECTION_CONTEXT 
{
  HTTP_servlet_request request;
  HTTP_servlet_response response;
  DBUF request_data;
  DBUF *buff;

  SERVLET_CONTEXT *servlet_contexts;

} SERVLET_FILTER_CONNECTION_CONTEXT;


SERVLET_FILTER_CONNECTION_CONTEXT * SERVLET_FILTER_CONNECTION_CONTEXT_init(SERVLET_RUNNER_FILTER *rfilter)
{
  size_t nservlets;
  SERVLET_FILTER_CONNECTION_CONTEXT *ret;
  SERVLET_CONTEXT *cur;
  HTTP_SERVLET **tmp;
  size_t  i;


  nservlets = ARRAY_size( &rfilter->servlets );

  ret = malloc( sizeof( SERVLET_FILTER_CONNECTION_CONTEXT ) );
  if (!ret) {
    return 0;
  }

  ret->servlet_contexts =  (SERVLET_CONTEXT *) malloc( nservlets * sizeof( SERVLET_CONTEXT ) );
  if (!ret->servlet_contexts) {
    return 0;
  }

  for( i = 0, cur = ret->servlet_contexts; i < nservlets; ++i, ++cur) {
     cur->connection_ctx = 0;
   
     tmp = (HTTP_SERVLET **) ARRAY_at( &rfilter->servlets, i );
     cur->servlet =  *tmp;
  }
 
  DBUF_init( &ret->request_data, 0 );
  HTTP_RESPONSE_init( &ret->response.response, HTTP_VERSION_1_1, 501 );

  return ret;
}


static int servlets_req_header_parsed	  (HTTP_REQUEST *request, FILTER_CONTEXT *context ) 
{
  M_UNUSED( request );
  
  if (!context->connection_ctx) {
    context->connection_ctx = SERVLET_FILTER_CONNECTION_CONTEXT_init( (SERVLET_RUNNER_FILTER *) context->filter );
  }
  if (! context->connection_ctx) {
    return -1;
  }

  return 0;
}


static int servlets_req_on_message_data   (HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context )
{
  SERVLET_FILTER_CONNECTION_CONTEXT *fcontext = (SERVLET_FILTER_CONNECTION_CONTEXT * ) context->connection_ctx;

  M_UNUSED( request );
  return DBUF_add( &fcontext->request_data, data, data_size );
}


static int servlets_req_finished	 (HTTP_REQUEST *request, FILTER_CONTEXT *context )
{
  size_t i;
  RDATA rd;

  SERVLET_FILTER_CONNECTION_CONTEXT *fcontext = (SERVLET_FILTER_CONNECTION_CONTEXT * ) context->connection_ctx;
  SERVLET_RUNNER_FILTER *runner = (SERVLET_RUNNER_FILTER *) context->filter; 
  SERVLET_CONTEXT *scontext;
  SERVLET_STATUS status; 

  if ( request->expect_100_continue) {
     
    rd.no_chunk.data = HTTP_100_CONTINUE_RESPONSE;
    rd.no_chunk.data_size =  HTTP_100_CONTINUE_RESPONSE_LEN; 

    if (call_next_filter_response_data ( 0, 0, rd,  context  ) ) {
      return -1;
    }
  } 

  fcontext->request.request = request;
  fcontext->request.request_data = fcontext->buff;
 
  HTTP_RESPONSE_free( &fcontext->response.response );
  HTTP_RESPONSE_init( &fcontext->response.response, request->version, 501 );
  fcontext->response.request = request;
  fcontext->response.filter_context = context;
  fcontext->response.data_sent = 0;
  fcontext->response.state = 0;
  fcontext->response.chunk_buf = 0;
  fcontext->response.chunk_buf_size = 0;
  fcontext->response.chunk_no = 0;


 
  for(i = 0, scontext = (SERVLET_CONTEXT *) fcontext->servlet_contexts;  i < ARRAY_size( &runner->servlets ); ++i, ++scontext) {
     status = scontext->servlet->servlet_action( &fcontext->request, &fcontext->response, scontext );  
     if (status == SERVLET_REQUEST_HANDLED) {
       break;
     }
     if (status == SERVLET_REQUEST_ERROR) {
       return -1;
     }
  }
  return 0;
}

static int servlets_connection_close     (FILTER_CONTEXT *context)
{
  size_t i;
  SERVLET_FILTER_CONNECTION_CONTEXT *fcontext = (SERVLET_FILTER_CONNECTION_CONTEXT * ) context->connection_ctx;
  SERVLET_RUNNER_FILTER *runner = (SERVLET_RUNNER_FILTER *) context->filter; 
  HTTP_SERVLET *servlet;
  SERVLET_CONTEXT *scontext;

  for(i = 0, scontext = (SERVLET_CONTEXT *) fcontext->servlet_contexts;  i < ARRAY_size( &runner->servlets ); ++i, ++scontext) {
     servlet = (HTTP_SERVLET *) ARRAY_at( &runner->servlets, i );
     servlet->free_connection( scontext ); 
  }
  return 0;
}

SERVLET_RUNNER_FILTER *SERVLET_RUNNER_FILTER_init()
{
   SERVLET_RUNNER_FILTER *ret;

   ret = (SERVLET_RUNNER_FILTER *) malloc( sizeof( SERVLET_RUNNER_FILTER ) );
   if (!ret) {
     return 0;
   }

   memset( &ret->base, 0, sizeof( HTTP_FILTER ) );
   ret->base.on_request_header_parsed = servlets_req_header_parsed;
   ret->base.on_request_data = servlets_req_on_message_data;
   ret->base.on_request_completed = servlets_req_finished;	
   ret->base.on_connection_close = servlets_connection_close;

   if (ARRAY_init( &ret->servlets, sizeof( void * ), 10 ) ) {
     free( ret );
     return 0;
   }

   return ret;
}

void SERVLET_RUNNER_FILTER_free(SERVLET_RUNNER_FILTER *servlets)
{
  size_t i;
  HTTP_SERVLET *servlet;
 
  for(i = 0; i < ARRAY_size( &servlets->servlets ); i++) {
    servlet = (HTTP_SERVLET *) ARRAY_at( &servlets->servlets, i );
    if (servlet->free_servlet) {
      servlet->free_servlet( servlet );
    }
  }
  ARRAY_free( &servlets->servlets );
  free( servlets );
}

int SERVLET_RUNNER_FILTER_add_servlet( SERVLET_RUNNER_FILTER *servlets, HTTP_SERVLET *servlet )
{
  if (servlet->init_servlet) {
     if (servlet->init_servlet( servlet )) {
       return -1;
     }
  }
  return ARRAY_push_back( &servlets->servlets, &servlet, sizeof( void * ) );
}

// ====================================================================
int WEBBY_CONFIG_load( WEBBY_CONFIG *cfg, const char *file)
{
  PROPERTIES props;
  const char *tmp;
  IPADDRESS addr;
  uint16_t port;

  if (PROPERTIES_init( &props, 30 )) {
    return -1;
  }
  if (PROPERTIES_load( &props, file )) {
    return -1;
  }

  tmp = PROPERTIES_get( &props, "host" );
  if (!tmp) {
    MLOG_ERROR( "Missing configuration parameter: host" );
    return -1;
  }

  if ( IPADDRESS_parse_string( &addr, tmp ) ) {
     MLOG_ERROR( "invalid configuration parameter: host" );
     return -1;
  }

  if (PROPERTIES_dec_uint16_t( &props, "port", &port)) {
     MLOG_ERROR( "Missing/Invalid configuration parameter: port" );
     return -1;
  }

  if (SOCKADDR_init( &cfg->listening_address, &addr, (uint16_t) port ) ) {
     MLOG_ERROR( "Missing/Invalid host/port configuration" );
     return -1;
  }

  if (PROPERTIES_dec_int32_t( &props, "listen_backlog", &cfg->listen_backlog)) {
     MLOG_ERROR( "Missing/Invalid configuration parameter: listen_backlog" );
     return -1;
  }

  if (PROPERTIES_dec_int32_t( &props, "socket_buffer_size", &cfg->socket_buffer_size)) {
     MLOG_ERROR( "Missing/Invalid configuration parameter: listen_backlog" );
     return -1;
  }

  if (PROPERTIES_dec_int32_t( &props, "max_connections", &cfg->max_connections)) {
     MLOG_ERROR( "Missing/Invalid configuration parameter: max_connections" );
     return -1;
  }

  if (PROPERTIES_dec_int32_t( &props, "io_timeout", &cfg->io_timeout)) {
     MLOG_ERROR( "Missing/Invalid configuration parameter: io_timeout" );
     return -1;
  }

  if (PROPERTIES_dec_int32_t( &props, "idle_timeout", &cfg->idle_timeout)) {
     MLOG_ERROR( "Missing/Invalid configuration parameter: idle_timeout" );
     return -1;
  }

  if (PROPERTIES_dec_int32_t( &props, "stack_pages_per_thread", &cfg->stack_pages_per_thread)) {
     MLOG_ERROR( "Missing/Invalid configuration parameter: stack_pages_per_thread");
     return -1;
  }
  PROPERTIES_free( &props );
  return 0;
}

// ====================================================================

WEBBY *WEBBY_init( WEBBY_CONFIG * cfg )
{ 
  WEBBY *ret;
  DATA_SINK_FILTER *data_sink; 


  ret = (WEBBY *) malloc( sizeof( WEBBY ) );
  if (!ret) {
    return 0;
  }
  ret->cfg = cfg;
 
 
  if ( ARRAY_init( &ret->filters, sizeof( HTTP_FILTER * ), 10 ) ) {
    return 0;
  }
 
  data_sink = DATA_SINK_FILTER_init();
  ARRAY_push_back( &ret->filters, &data_sink, sizeof( void * ) );
   
  ret->servlet_runner_filter = SERVLET_RUNNER_FILTER_init();
  if (!ret->servlet_runner_filter) {
    return 0;
  }

   if (WEBBY_impl_new( ret, &ret->impl ) ) {
    return 0;
  }
  return ret;
}

int WEBBY_add_filter(  WEBBY *server, HTTP_FILTER *filter )
{

#if 0
  filter->filter_ctx = 0;
  if (filter->init_filter!=0) {
    if (filter->init_filter( &filter->filter_ctx ) ) {
      return -1;
    }
  }
#endif  

  return ARRAY_push_back( &server->filters, &filter, sizeof( void * ) );
}

int WEBBY_run( WEBBY *server )
{
  if (WEBBY_add_filter( server, &server->servlet_runner_filter->base ) ) {
    return -1;
  }
  return WEBBY_impl_run( server->impl );
}

int WEBBY_shutdown( WEBBY *server )
{
  return WEBBY_impl_shutdown( server->impl ); 
}

int WEBBY_add_servlet( WEBBY *server, HTTP_SERVLET * servlet )
{
  return SERVLET_RUNNER_FILTER_add_servlet( (SERVLET_RUNNER_FILTER *) server->servlet_runner_filter, servlet );
}

static int http_header_parsed	   (HTTP_REQUEST *request, void *ctx)
{
  FILTER_CONTEXT *filter_data = (FILTER_CONTEXT *) ctx;

  ++filter_data;

  return filter_data->filter->on_request_header_parsed(request, filter_data );
}

 
static int http_on_message_data       (HTTP_REQUEST *request, void *data, size_t data_size, void *ctx)
{
  FILTER_CONTEXT *filter_data = (FILTER_CONTEXT *) ctx;

  ++filter_data;

  return filter_data->filter->on_request_data( request, data, data_size, filter_data );
}

static int http_req_finished	   (HTTP_REQUEST *request, void *ctx)
{
  FILTER_CONTEXT *filter_data = (FILTER_CONTEXT *) ctx;

  ++filter_data;

  return filter_data->filter->on_request_completed( request, filter_data );
}
 

WEBBY_CONNECTION *WEBBY_new_connection( WEBBY *server, void *implconndata  )
{
  WEBBY_CONNECTION *ret = 0;
  FILTER_CONTEXT *filter_data = 0,*cur;
  HTTP_FILTER **tmp;
  void *buff = 0;
  DATA_SINK_FILTER_CONNECTION_CONTEXT *sink_conn_ctx = 0;
  size_t nfilters, i;

  ret = (WEBBY_CONNECTION *)  malloc( sizeof(WEBBY_CONNECTION) ); 
  if (!ret) {
    goto err;
  }

  buff = malloc( HTTP_PARSER_BUFFER_SIZE );
  if (!buff) {
    goto err;
  }
  BF_init( &ret->in_buf, buff, HTTP_PARSER_BUFFER_SIZE);
 
  nfilters = ARRAY_size( &server->filters );  
  filter_data = (FILTER_CONTEXT *) malloc( (nfilters + 1)  * sizeof(FILTER_CONTEXT) );
  if (!filter_data) {
    goto err;
  }
  ret->filter_data = filter_data; 

  for( i = 0, cur = filter_data; i < nfilters; ++i, ++cur) {
     cur->connection_ctx = 0;
   
     tmp = (HTTP_FILTER **) ARRAY_at( &server->filters, i );
     cur->filter =  *tmp;
  }
  cur->connection_ctx = 0;
  cur->filter = 0;

  sink_conn_ctx = DATA_SINK_FILTER_CONNECTION_CONTEXT_init( implconndata, &ret->in_buf ); 
  if (! sink_conn_ctx) {   
    goto err;
  }
  filter_data->connection_ctx = sink_conn_ctx; 
 
 
 
  HTTP_REQUEST_PARSER_init( &ret->request_parser,
			     http_header_parsed,
			     http_on_message_data,
			     http_req_finished,
			     filter_data);

 
  HTTP_REQUEST_init( &ret->request );
 
  return ret;

err:
  if (filter_data) {
    free(filter_data);
  }
  if (buff) {
    free(buff);
  }
  if (ret) {
    free(ret);
  }
  if (sink_conn_ctx) {
    DATA_SINK_FILTER_CONNECTION_CONTEXT_free( implconndata );  
  }
  return 0;
}



int WEBBY_connection_data_received( WEBBY_CONNECTION * connection  )
{
  PARSER_STATUS st;
  
  st = HTTP_REQUEST_PARSER_process( &connection->request_parser, &connection->request, &connection->in_buf );
  switch( st ) {
    case PARSER_STATUS_ERROR:
      return -1;
    case PARSER_STATUS_COMPLETED:
      HTTP_REQUEST_init( &connection->request );
      BF_put_mode( &connection->in_buf );
      break;
    case PARSER_STATUS_NEED_MORE_DATA:
      break;
  }
  return 0;
}

int WEBBY_connection_close( WEBBY_CONNECTION * connection  )
{
  FILTER_CONTEXT *context = connection->filter_data;
  return context->filter->on_connection_close( context );
}

// ====================================================================

int HTTP_response_start( HTTP_servlet_response *resp, int status, const char *mime_type, HTTP_servlet_response_type rtype, size_t length  )
{
  if (resp->state !=0) {
    return -1;
  }
  resp->response.status_code = status;
  if (mime_type) {
    HTTP_MESSAGE_add_header( &resp->response.base, "Content-Type", mime_type );
  }

  if ( ! HTTP_REQUEST_is_persistent( resp->request ) ) {
    rtype = RESPONSE_CONNECTION_CLOSE;
  }
 
  switch(rtype) {
   case RESPONSE_CONNECTION_CLOSE:
     resp->response.base.flags = HTTP_MESSAGE_FLAG_CONNECTION_CLOSE;  
     resp->data_sent = (size_t) -1;
     break;
   case RESPONSE_CONTENT_LENGTH:
     HTTP_MESSAGE_set_content_length( &resp->response.base, length );
     resp->data_sent = length;
     break;
   case RESPONSE_CHUNKED:
     resp->response.base.flags = HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED;  
#if 0
     if (HTTP_MESSAGE_add_header( &resp->response.base, "Transfer-Encoding" , "chunked" )) {
       return -1;
     }
#endif
     break;
  }

  resp->state = 1;
  return 0;
}

int HTTP_response_send( HTTP_servlet_response *resp, void *data, size_t size)
{
  RDATA rdata;

  if (resp->state < 1 || resp->state > 2) {
    return -1;
  }

  if (resp->state == 1) {
    if (call_next_filter_response_header( &resp->response,  resp->filter_context )) {
      return -1;
    }
    resp->state = 2;
  }

  if (resp->response.base.flags & HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED ) {
    return -1;
  }

  // check that not sending more data than allowed by set value of Content-Length header.
  if (resp->data_sent != (size_t) -1) {
    if (resp->data_sent < size ) {
      return -1;
    }
    resp->data_sent -= size;
  }

  rdata.no_chunk.data = data;
  rdata.no_chunk.data_size = size;

  return call_next_filter_response_data ( &resp->response, 0, rdata, resp->filter_context );
}




BF *HTTP_response_get_chunk_buffer( HTTP_servlet_response *resp, size_t chunk_size ) 
{
  size_t sz;
  void *tmp;

  if (resp->chunk_buf != 0) {
    if (resp->chunk_buf_size < chunk_size ) {
      sz = chunk_size + RESERVED_FOR_CHUNK_HEADER_SIZE; 
      tmp = realloc( resp->chunk_buf, sz );
      if (!tmp) {
        return 0;
      }
      resp->chunk_buf = tmp;
      resp->chunk_buf_size = chunk_size;
    }
  } else {
    sz = chunk_size + RESERVED_FOR_CHUNK_HEADER_SIZE; 
    resp->chunk_buf = malloc( sz ); 
    if (!resp->chunk_buf) {
      return 0;
    }
    BF_init( &resp->bf, resp->chunk_buf, sz );
    resp->chunk_buf_size = chunk_size;
  } 
  
  BF_put_mode( &resp->bf );
  BF_set_start( &resp->bf, RESERVED_FOR_CHUNK_HEADER_SIZE ); 
  resp->bf.end = resp->bf.put_pos + chunk_size;
  
  return &resp->bf;
}

int HTTP_response_write_chunk( HTTP_servlet_response *resp, BF *bf)
{
  RDATA rdata;

  if (resp->state < 1 || resp->state > 2) {
    return -1;
  }
  if ( (resp->response.base.flags & (HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED | HTTP_MESSAGE_FLAG_CONNECTION_CLOSE)  ) == 0) {
    return -1;
  }

  if (resp->state == 1) {
    if (call_next_filter_response_header( &resp->response,  resp->filter_context )) {
      return -1;
    }
    resp->state = 2;
  } else {
    ++ resp->chunk_no;
  }

  if ( resp->response.base.flags & HTTP_MESSAGE_FLAG_CONNECTION_CLOSE ) { 
    rdata.no_chunk.data = bf->get_pos;
    rdata.no_chunk.data_size = BF_get_size( bf );  

    return call_next_filter_response_data ( &resp->response, 0, rdata, resp->filter_context  );
  }

#if 0
  if (first_chunk) {
    len = snprintf( chunk_header, sizeof( chunk_header ),  "%x\r\n", BF_get_size(bf) );
  } else {
    len = snprintf( chunk_header, sizeof( chunk_header ),  "\r\n%x\r\n", BF_get_size(bf) );
  }

#endif

  rdata.chunk.bf = bf;
  rdata.chunk.chunk_no = resp->chunk_no;


  return call_next_filter_response_data ( &resp->response, 1, rdata, resp->filter_context  );
}

int HTTP_response_finish( HTTP_servlet_response *resp )
{
  RDATA rdata;

  if (resp->state < 1 || resp->state > 2) {
    return -1;
  }

  if (resp->state == 1) {
    if (call_next_filter_response_header( &resp->response,  resp->filter_context )) {
      return -1;
    }
  } 
 
 
  if (resp->response.base.flags & HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED) {
 
    rdata.chunk.bf = 0;
    rdata.chunk.chunk_no = ++resp->chunk_no;

    if ( call_next_filter_response_data ( &resp->response, 1, rdata, resp->filter_context  ) )  {
      return -1;
    }

#if 0 
    if (resp->state == 1) {
      if (call_next_filter_response_data ( &resp->response, LAST_CHUNK, LAST_CHUNK_SIZE, resp->filter_context  )) {
        return -1;
      }
    } else {
      if (call_next_filter_response_data ( &resp->response, LAST_CHUNK_NOT_FIRST, LAST_CHUNK_NOT_FIRST_SIZE, resp->filter_context  )) {
        return -1;
      }
    }
#endif
  }
  resp->state = 3;
  
  return call_next_filter_response_completed ( &resp->response, resp->filter_context );
}
 
