#ifndef __WEBBY_API_H__
#define __WEBBY_API_H__


#include <cutils/array.h>
#include <cutils/dbuf.h>
#include <nutils/bf.h>
#include <nutils/addrutil.h>
#include <hutils/http.h>

struct tagHTTP_FILTER; 
struct tagHTTP_SERVLET;

// =============================================================================================

/**
 * @defgroup HTTP_FILTER
 * @brief HTTP_FILTER - a filter implements a stage of processing the HTTP request / response.
 *
 * A filter can modify either one of the following aspects, as they are processed.
 *   - http request header
 *   - http request data
 *   - http response header
 *   - http response data
 *
 * @{
 */


/**
 * @brief structure is passed to filter function as argument. 
 */
typedef struct tagFILTER_CONTEXT {
  void *connection_ctx;               /** per connection data data maintained for particular filter */
  struct tagHTTP_FILTER *filter;      /** pointer to filter instance */
#if 0
  struct tagHTTP_FILTER *next_filter;      /** pointer to filter instance */
#endif

} FILTER_CONTEXT;
 

typedef int    (*HTTP_FILTER_INIT) (struct tagHTTP_FILTER *filter);


typedef int    (*HTTP_FILTER_FREE) (struct tagHTTP_FILTER *filter);

/**
 * @brief filter callback: a http request header has been parsed, now the request header is passed through the filter chain.
 *
 * @param request - the http request header
 * @param context - filter context, we need it to call the next filter.
 *
 */
typedef int    (*HTTP_FILTER_REQUEST_HEADER_PARSED) (HTTP_REQUEST *request,  FILTER_CONTEXT *context );

/**
 * @brief filter callback: a buffer that is part of the request data has been received.
 *
 * @param request - the http request header
 * @param data - request data, if request data is chunked then this is after parsing of chunk (no chunk headers included)
 * @param data_size - size of request data
 * @param context - filter context, we need it to call the next filter.
 */
typedef int    (*HTTP_FILTER_REQUEST_DATA)          (HTTP_REQUEST *request, void *data, size_t data_size,  FILTER_CONTEXT *context );

/**
 * @brief filter callback: the http request has been parsed completely, and all request data has already been processed.
 *
 * @brief request - the http request header.
 * @param context - filter context, we need it to call the next filter.
 */
    typedef int    (*HTTP_FILTER_REQUEST_COMPLETED)     (HTTP_REQUEST *request, FILTER_CONTEXT *context );

/**
 * @brief filter callback: a response header has now passed through the filter.
 *
 * @brief response - the http response header.
 * @param context - filter context, we need it to call the next filter.
 */
typedef int    (*HTTP_FILTER_RESPONSE_HEADER) (HTTP_RESPONSE *response,  FILTER_CONTEXT *context );


typedef union {
  struct {
    void *data;
    size_t data_size;
  } no_chunk;

  struct {
    BF *bf;
    int chunk_no;
   } chunk;
} RDATA;

/**
 * @brief filter callback: a buffer that is part of the response data  has now passed through the filter
 *
 * There are two situations - the response uses 'chunks', the response does not use chunks.
 *
 * If the response is chunked then a buffer object (BF) is used, which reserves space for chunk header before the start of the buffer.
 
 * The case of chunks: is_chunk != 0
 * rdata.chunk.bf - the chunk buffer (zero for the last chunk)
 * rdata.chunk.chunk_no - the number of the chunk (starts with zero).
 *
 * The case of no chunks:
 * rdata.no_chunk.data - response data
 * rdata.no_chunk.data_size - size of response data.
 *
 *
 * @brief response - the http response header.
 * @brief is_chunk - not zero if response is chunked
 * @brief rdata - union that holds data for both chunked and not chunked case;
 * @param context - filter context, we need it to call the next filter.
 */
typedef int    (*HTTP_FILTER_RESPONSE_DATA) (HTTP_RESPONSE *response, int is_chunk, RDATA rdata, FILTER_CONTEXT *context );

/**
 * @brief filter callback: a http response has now passed completely through the filter, all response data has already been sent.
 *
 * @brief response - the http response header.
 * @param context - filter context, we need it to call the next filter.
 */
typedef int    (*HTTP_FILTER_RESPONSE_COMPLETED)     (HTTP_RESPONSE *response, FILTER_CONTEXT *context );  

/**
 * @brief filter callback: called when a connection has been closed, the filter should free the connection context data, if any of it has been allocated.
 *
 * @param context - filter context, we need it to call the next filter.
 */
typedef int    (*HTTP_FILTER_CONNECTION_CLOSE)	     (FILTER_CONTEXT *context );


/**
 * @brief http filter definition
 */
typedef struct tagHTTP_FILTER {
  
  HTTP_FILTER_INIT		     filter_free;
  HTTP_FILTER_FREE		     filter_init; 
  
  HTTP_FILTER_REQUEST_HEADER_PARSED  on_request_header_parsed;
  HTTP_FILTER_REQUEST_DATA           on_request_data;
  HTTP_FILTER_REQUEST_COMPLETED      on_request_completed;

  HTTP_FILTER_RESPONSE_HEADER        on_response_header;
  HTTP_FILTER_RESPONSE_DATA          on_response_data;
  HTTP_FILTER_RESPONSE_COMPLETED     on_response_completed;

  HTTP_FILTER_CONNECTION_CLOSE       on_connection_close;

} HTTP_FILTER;


/**
 * @brief called by implementaiton of HTTP_FILTER_REQUEST_HEADER_PARSED filter callback, calls the next filter in the chain
 */
M_INLINE int call_next_filter_request_header_parsed( HTTP_REQUEST *request, FILTER_CONTEXT *context )
{
  FILTER_CONTEXT *next = context + 1;
  if (next->filter == 0) {
    return 0;
  }
  return next->filter->on_request_header_parsed( request, next );
}

/**
 * @brief called by implementaiton of HTTP_FILTER_REQUEST_DATA filter callback, calls the next filter in the chain
 */
M_INLINE int call_next_filter_request_data( HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context )
{
  FILTER_CONTEXT *next = context + 1;
  if (next->filter == 0) {
    return 0;
  }
  return next->filter->on_request_data( request, data, data_size, next );
}

/**
 * @brief called by implementaiton of HTTP_FILTER_REQUEST_COMPLETED filter callback, calls the next filter in the chain
 */
M_INLINE int call_next_filter_request_completed( HTTP_REQUEST *request,  FILTER_CONTEXT *context )
{
  FILTER_CONTEXT *next = context + 1;
  if (next->filter == 0) {
    return 0;
  }
  return next->filter->on_request_completed( request,  next );
}

/**
 * @brief called by implementaiton of HTTP_FILTER_RESPONSE_HEADER filter callback, calls the next filter in the chain
 */
M_INLINE int call_next_filter_response_header (HTTP_RESPONSE *response,  FILTER_CONTEXT *context )
{
  FILTER_CONTEXT *next = context - 1;
  if (next->filter == 0) {
    return 0;
  }
  return next->filter->on_response_header( response, next );
}

/**
 * @brief called by implementaiton of HTTP_FILTER_RESPONSE_DATA filter callback, calls the next filter in the chain
 */
M_INLINE int call_next_filter_response_data (HTTP_RESPONSE *response,  int is_chunk, RDATA rdata, FILTER_CONTEXT *context )
{
  FILTER_CONTEXT *next = context - 1;
  if (next->filter == 0) {
    return 0;
  }
  return next->filter->on_response_data( response, is_chunk, rdata, next );
}

/**
 * @brief called by implementaiton of HTTP_FILTER_RESPONSE_COMPLETED filter callback, calls the next filter in the chain
 */
M_INLINE int call_next_filter_response_completed (HTTP_RESPONSE *response, FILTER_CONTEXT *context )
{
  FILTER_CONTEXT *next = context - 1;
  if (next->filter == 0) {
    return 0;
  }
  return next->filter->on_response_completed( response, next );
}

/**
 * @brief called by implementaiton of HTTP_FILTER_CONNECTION_CLOSE filter callback, calls the next filter in the chain
 */
M_INLINE int call_next_filter_connection_close( FILTER_CONTEXT *context )
{
  FILTER_CONTEXT *next = context + 1;
  if (next->filter == 0) {
    return 0;
  }
  return next->filter->on_connection_close( next );
} 


/**
 * @}
 */

/**
 * @defgroup HTTP_SERVLET
 * @brief Servlet - the logic of processing the http request and producer of http response
 *
 * @{
 */

// =============================================================================================

typedef struct tagHTTP_servlet_request
{
  HTTP_REQUEST *request;
  DBUF *request_data;
} HTTP_servlet_request;


M_INLINE Http_version_type HTTP_servlet_protocol(HTTP_servlet_request *req)
{ 
  return req->request->version; 
}

M_INLINE Http_method_type HTTP_servlet_method(HTTP_servlet_request *req)
{
  return req->request->method;
}

M_INLINE URI *HTTP_servlet_uri( HTTP_servlet_request *req )
{
  return &req->request->url;
}

M_INLINE const char * HTTP_servlet_find_header( HTTP_servlet_request *req, const char *header_name ) {
  return HTTP_MESSAGE_find_header( &req->request->base, header_name );
}

M_INLINE STRINGPAIR * HTTP_servlet_first_header( HTTP_servlet_request *req, DLISTUNR_position *pos ) { 
  return HTTP_MESSAGE_first_header(  &req->request->base, pos );
}

M_INLINE STRINGPAIR * HTTP_servlet_next_header( HTTP_servlet_request *req, DLISTUNR_position *pos ) { 
  return HTTP_MESSAGE_next_header(  &req->request->base, pos );
}

M_INLINE DBUF * HTTP_servlet_data( HTTP_servlet_request * req ) { 
  return req->request_data;
}


// =============================================================================================

typedef struct tagHTTP_servlet_response {
  HTTP_RESPONSE response;
  HTTP_REQUEST *request;
  FILTER_CONTEXT *filter_context; 
  size_t data_sent;
  int state;
  BF bf;
  void *chunk_buf;
  size_t chunk_buf_size;
  int    chunk_no;
} HTTP_servlet_response;

typedef enum {
  RESPONSE_CONNECTION_CLOSE,
  RESPONSE_CONTENT_LENGTH,
  RESPONSE_CHUNKED,
} HTTP_servlet_response_type;

/**
 * @brief initiate an HTTP response
 */
int HTTP_response_start( HTTP_servlet_response *resp, int status, const char *mime_type, HTTP_servlet_response_type rtype, size_t length  );

/**
 * @brief send response data when sending RESPONSE_CONTENT_LENGTH or RESPONSE_CONNECTION_CLOSE responses
 *
 * Precondition: HTTP_response_start has been called. and response type is either RESPONSE_CONTENT_LENGTH or RESPONSE_CONNECTION_CLOSE  
 */
int HTTP_response_send( HTTP_servlet_response *resp, void *data, size_t size);

/**
 * @brief finish sending of response
 *
 */
int HTTP_response_finish( HTTP_servlet_response *resp );

/**
 * @brief returns buffer for sending chunks
 * The buffer reserves enough space before start of buffer, in order to allow addition of chunk header before the sent daa
 */
BF *HTTP_response_get_chunk_buffer( HTTP_servlet_response *resp, size_t chunk_size );

/**
 * @brief send chunk from buffer
 * The buffer must reserve enough space for the chunk header, before start of buffer
 *
 * Precondition: HTTP_response_start has been called, and response type is RESPONSE_CHUNKED 
 *
 */
int HTTP_response_write_chunk( HTTP_servlet_response *resp, BF *bf);



// =============================================================================================

struct tagHTTP_SERVLET;

typedef enum {
  SERVLET_REQUEST_IGNORED,  /** the request has not been handled by this servlet */
  SERVLET_REQUEST_HANDLED,  /** the request has been handled by this servlet */
  SERVLET_REQUEST_ERROR,    /** error occured while servlet instance has handled a request  */


} SERVLET_STATUS;

typedef struct tagSERVLET_CONTEXT {
  struct tagHTTP_SERVLET *servlet; /** pointer to servlet */
  void *connection_ctx;    /** per connection data of servlet */
} SERVLET_CONTEXT;

/**
 * @brief initialise the servlet instance (optional)
 */
typedef int  (* HTTP_SERVLET_INIT) (struct tagHTTP_SERVLET *servlet_ctx);

/**
 * @brief free  the servlet instance (optional)
 */
typedef void * (* HTTP_SERVLET_FREE) (struct tagHTTP_SERVLET *servlet_ctx);



/**
 * @brief called to handle an HTTP request
 */
typedef SERVLET_STATUS ( *HTTP_SERVLET_ACTION) ( HTTP_servlet_request * request, HTTP_servlet_response *response, SERVLET_CONTEXT *context );

/**
 * @brief called when socket connection is closed.
 */
typedef void   ( *HTTP_SERVLET_FREE_CONNECTION) ( SERVLET_CONTEXT *contextx ); 

/**
 * @brief HTTP_SERVLET definition
 */
typedef struct tagHTTP_SERVLET {
  HTTP_SERVLET_INIT             init_servlet;
  HTTP_SERVLET_FREE             free_servlet;
  HTTP_SERVLET_ACTION           servlet_action;
  HTTP_SERVLET_FREE_CONNECTION  free_connection;
 
} HTTP_SERVLET;


M_INLINE void HTTP_SERVLET_init( HTTP_SERVLET *servlet, HTTP_SERVLET_INIT init_servlet, HTTP_SERVLET_FREE free_servlet, HTTP_SERVLET_ACTION  servlet_action, HTTP_SERVLET_FREE_CONNECTION free_connection )
{
  servlet->init_servlet = init_servlet;
  servlet->free_servlet = free_servlet;
  servlet->servlet_action = servlet_action;
  servlet->free_connection = free_connection;
}

/**
 * @}
 */


// =============================================================================================

/**
 * @defgroup  WEBBYCFG   
 * @brief embedded web server configuration
 *
 * @{
 */

typedef struct tagWEBBY_CONFIG {
  SOCKADDR listening_address;	   /** listening address */
  int listen_backlog;		   /** listen(2) backlog parameter */
  int socket_buffer_size;          /** socket buffer size, in bytes, for each client connection (both receive and send buffer sizes) */
  int max_connections;		   /** maximum number of connections supported at the same time */
  int io_timeout;                  /** read / write timeout in seconds. */
  int idle_timeout;		   /** idle timeout in seconds - if connection that inactive for more than this value between requests then connection will be close */
  int stack_pages_per_thread;	  /** pages per thread */
 
} WEBBY_CONFIG;

int WEBBY_CONFIG_load( WEBBY_CONFIG *cfg, const char *file);

/**
 * @}
 */


// =============================================================================================

struct tagSERVLET_RUNNER_FILTER;

/** 
 * @defgroup WEBBY 
 * @brief embedded web server facade class.
 * 
 * @{
 */
typedef struct tagWEBBY {
  WEBBY_CONFIG *cfg;
  ARRAY filters;
  struct tagSERVLET_RUNNER_FILTER * servlet_runner_filter;
  void *impl;
} WEBBY;


WEBBY *WEBBY_init( WEBBY_CONFIG * );

int WEBBY_add_filter(  WEBBY *, HTTP_FILTER * );
int WEBBY_add_servlet( WEBBY *, HTTP_SERVLET * );	

int WEBBY_run( WEBBY * );
int WEBBY_shutdown( WEBBY * );


#define HTTP_PARSER_BUFFER_SIZE 4096

typedef struct tagWEBBY_CONNECTION {
  void *in_buf_data;
  BF in_buf;
  HTTP_REQUEST_PARSER request_parser;
  HTTP_REQUEST request;
  FILTER_CONTEXT *filter_data;  

} WEBBY_CONNECTION;

WEBBY_CONNECTION *WEBBY_new_connection( WEBBY *server, void *implconndata );
int WEBBY_connection_data_received( WEBBY_CONNECTION * connection );

/**
 * @}
 */

#endif


