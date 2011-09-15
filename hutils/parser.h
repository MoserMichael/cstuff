#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#include <stdlib.h>
#include <nutils/bf.h>
#include <hutils/uri.h>
#include <cutils/bhash.h>


// ===============================================================

#define HTTP_MESSAGE_FLAG_CONNECTION_CLOSE   1
#define HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED   2
#define HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH 4

/**
 * @brief Holds common data of both HTTP request and response objects.
 */
typedef struct tagHTTP_MESSAGE {
  int flags;
  int content_length;
} HTTP_MESSAGE;

M_INLINE void HTTP_MESSAGE_init( HTTP_MESSAGE *message )
{  
  message->flags = 0;
  message->content_length = -1;
}

// ===============================================================

struct tagHTTP_PARSER;
struct tagHTTP_REQUEST_PARSER; 

/**
 * @brief callback that is invoked in order to parse contents of a specific http header 
 */
typedef int (*HEADER_ACTION) ( struct tagHTTP_MESSAGE *request, struct tagHTTP_PARSER *parser ); 

// ===============================================================

typedef enum tagHttp_method_type {
  HTTP_METHOD_GET,           
  HTTP_METHOD_POST,   
  HTTP_METHOD_PUT,  
  HTTP_METHOD_HEAD,
  HTTP_METHOD_OPTIONS,
  HTTP_METHOD_DELETE,  
  HTTP_METHOD_TRACE,                
  HTTP_METHOD_CONNECT
 
} Http_method_type;

typedef enum tagHttp_version_type {
  HTTP_VERSION_1_0,          
  HTTP_VERSION_1_1,

} Http_version_type;


typedef struct tagHTTP_REQUEST {
  HTTP_MESSAGE base;
  
  // request line
  Http_method_type method;
  URI url; 
  char *raw_url;
  Http_version_type version; 

  // does the request expect 100 continue response?
  int expect_100_continue;

  // host headera
  int has_host_header;
  char *host_header;
  int host_header_port;

} HTTP_REQUEST;


M_INLINE void HTTP_REQUEST_init( HTTP_REQUEST *message )
{
  HTTP_MESSAGE_init( &message->base );

  URI_init( & message->url );

  message->expect_100_continue = message->has_host_header = 0;
  message->raw_url = message->host_header = 0;
  message->has_host_header = 0;
  message->host_header_port = -1;
}

M_INLINE void HTTP_REQUEST_free( HTTP_REQUEST *message )
{
  URI_free( &message->url );
  if (message->host_header) {
    free( message->host_header);
  }
  if (message->raw_url) {
    free( message->raw_url );
  }
  HTTP_REQUEST_init( message );  
}
 
typedef int  (*HTTP_REQ_HEADER_PARSED)	   (HTTP_REQUEST *request, void *ctx);
typedef int  (*HTTP_REQ_MESSAGE_BODY_DATA) (HTTP_REQUEST *request, void *data, size_t data_size, void *ctx);
typedef int  (*HTTP_REQ_FINISHED)	   (HTTP_REQUEST *request, void *ctx);  


// ===============================================================
typedef struct tagHTTP_RESPONSE {
  HTTP_MESSAGE base;
 
  int status_code;
  Http_version_type version; 


} HTTP_RESPONSE;

M_INLINE void HTTP_RESPONSE_init( HTTP_RESPONSE *message )
{
  HTTP_MESSAGE_init( &message->base );
  message->status_code = -1;
}

typedef int (*HTTP_RESP_HEADER_PARSED)	   (HTTP_RESPONSE *request, void *ctx);
typedef int (*HTTP_RESP_MESSAGE_BODY_DATA) (HTTP_RESPONSE *request, void *data, size_t data_size, void *ctx);
typedef int (*HTTP_RESP_FINISHED)	   (HTTP_RESPONSE *request, void *ctx);  


// ===============================================================

typedef enum tagHTTP_STATE_PARSING {

  HTTP_STATE_PARSING_REQUEST_LINE,
  HTTP_STATE_PARSING_HEADERS,
  HTTP_STATE_PARSING_BODY_CONTENT_LENGTH,
  HTTP_STATE_PARSING_BODY_CHUNK_HEADER,
  HTTP_STATE_PARSING_BODY_CHUNK_DATA,
  HTTP_STATE_PARSING_BODY_CHUNK_EOF_AFTER_DATA,
  HTTP_STATE_PARSING_BODY_CHUNK_TRAILER,


} HTTP_STATE_PARSING;


/** 
 * @brief base class of both http request and http response parsers
 */
typedef struct tagHTTP_PARSER {
  HTTP_STATE_PARSING  state;
  HASH header_action;  
  char *token;  // the current parsed header token.
  size_t token_length; // length of token buffer

  char *tokpos; // current position in http header data - during parsing of header.
 
  size_t  content_left; // how many bytes will make up the following chunk or Content-length data.

} HTTP_PARSER;

typedef enum {
  PARSER_STATUS_ERROR = -1,
  PARSER_STATUS_COMPLETED = 0,
  PARSER_STATUS_NEED_MORE_DATA = 1,
}
  PARSER_STATUS;

typedef enum {
  HTTP_TK_QUOTED_STRING,
  HTTP_TK_TEXT,
  HTTP_TK_SEPARATOR,
  HTTP_TK_EOF,
} HTTP_TK_TYPE;

typedef int (*HTTP_PROCESS_MSG_DATA) ( HTTP_MESSAGE *msg, void *data, size_t data_size, void *ctx);

int  HTTP_PARSER_init(  HTTP_PARSER *parser );
int  HTTP_PARSER_free(  HTTP_PARSER *parser );


/**
 * @brief add a parser for a specific http header type
 */
int HTTP_add_header_parser( HTTP_PARSER *parser, const char *header_name, HEADER_ACTION action );

/**
 * @brief returns next raw line from http header, adjoins line continuations.
 */
PARSER_STATUS HTTP_get_line( BF *bf, char **start_line );

/**
 * @brief return next token while parsing http header value.
 * @return -1 on error, otherwise type of token returns (value out of HTTP_TK_TYPE)
 */
int HTTP_get_header_token( HTTP_PARSER *parser );

/**
 * @brief dispatch parsing of one http header
 * @return -1 error, 0 - http header finished, 1 - need more data, 2 - header line parsed ok.
 */
PARSER_STATUS HTTP_parse_header_line( HTTP_PARSER *parser, HTTP_MESSAGE *request, BF *bf, int *eof_header );

/**
 * @brief start parsing content length message body 
 */
int HTTP_PARSER_content_length_init( HTTP_PARSER *parser, HTTP_MESSAGE *msg );

/**
 * @brief consume content length message body.
 */
PARSER_STATUS HTTP_PARSER_content_length_process( HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx);

/**
 * @brief start parsing chunked data 
 */
int HTTP_PARSER_chunked_data_init( HTTP_PARSER *parser );

/**
 * @brief finish parsind chunked content data.
 */
PARSER_STATUS HTTP_PARSER_chunked_data_process( HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx);


// ===============================================================




typedef struct tagHTTP_REQUEST_PARSER {

  HTTP_PARSER  base;
  void *ctx;

  HTTP_REQ_HEADER_PARSED     ev_header;
  HTTP_REQ_MESSAGE_BODY_DATA ev_body  ;
  HTTP_REQ_FINISHED	     ev_finish;  

} HTTP_REQUEST_PARSER;



/**
 * @brief initialise request parser object
 */
int HTTP_REQUEST_PARSER_init( HTTP_REQUEST_PARSER *parser, 
		              HTTP_REQ_HEADER_PARSED header_parsed,
			      HTTP_REQ_MESSAGE_BODY_DATA on_message_body_data,
			      HTTP_REQ_FINISHED on_request_finished,
			      void *ctx);


/**
 * @brief parse a http request 
 * @return  0 - done parsing, 1 need more data, -1 error occured.
 */
PARSER_STATUS HTTP_REQUEST_PARSER_process( HTTP_REQUEST_PARSER *parser, HTTP_REQUEST *request, BF *data);

// ===============================================================



typedef struct tagHTTP_RESPONSE_PARSER {

  HTTP_PARSER  base;

  HTTP_RESP_HEADER_PARSED	ev_header;
  HTTP_RESP_MESSAGE_BODY_DATA   ev_body  ;
  HTTP_RESP_FINISHED	        ev_finish;  

  HTTP_STATE_PARSING  state;
  void *ctx;

} HTTP_RESPONSE_PARSER;


/**
 * @brief initialise request parser object
 */
int HTTP_RESPONSE_PARSER_init( HTTP_RESPONSE_PARSER *parser,
		               HTTP_RESP_HEADER_PARSED header_parsed,
			       HTTP_RESP_MESSAGE_BODY_DATA on_message_body_data,
			       HTTP_RESP_FINISHED on_request_finished,
			       void *ctx);
	

	
/**
 * @brief parse a http response
 * @return  0 - done parsing, 1 need more data, -1 error occured.
 */
PARSER_STATUS HTTP_RESPONSE_PARSER_process( HTTP_RESPONSE_PARSER *parser, HTTP_RESPONSE *response, BF *data );

#endif

