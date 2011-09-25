#include "http.h"


// Content-Length    = "Content-Length" ":" 1*DIGIT
static int parser_content_length_header( struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser )
{
  int rt;
  
  rt = HTTP_get_header_token( parser );
  if (rt != HTTP_TK_TEXT) {
    return -1;
  }

  rt = atoi( parser->token );
  if (rt <= 0) {
    return -1;
  }

  message->content_length = rt;  
  message->flags |=  HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH;  
  return 0;
}


//Connection = "Connection" ":" 1#(connection-token)
//       connection-token  = token
// ? close seems to be the only value

static int parser_connection_header(  struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser )
{
  int rt;
  
  rt = HTTP_get_header_token( parser );
  if (rt != HTTP_TK_TEXT) {
    return -1;
  }

  if ( strcmp( parser->token,"close") == 0) {
    message->flags |= HTTP_MESSAGE_FLAG_CONNECTION_CLOSE;
    return 0;
  }

  return -1;
}


       
//Transfer-Encoding       = "Transfer-Encoding" ":" 1#transfer-coding
//

static int parser_transfer_encoding_header(  struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser )
{
  int rt;
  
  rt = HTTP_get_header_token( parser );
  if (rt != HTTP_TK_TEXT) {
    return -1;
  }

  if ( strcmp( parser->token,"chunked") == 0) {
    message->flags |= HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED;
    return 0;
  }

  return -1;
}

static int parser_expect_header(  struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser )
{
  HTTP_REQUEST *req;
  int rt;
  
  rt = HTTP_get_header_token( parser );
  if (rt != HTTP_TK_TEXT) {
	return -1;
  }

  if ( strcmp( parser->token,"100-continue") == 0) {
    req =  (HTTP_REQUEST *)  message;
    req->expect_100_continue = 1;
  }
  return 0;
}

// Host = "Host" ":" host [ ":" port ] ; Section 3.2.2
static int parser_host_header(  struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser )
{
  int rt;
  HTTP_REQUEST *req = (HTTP_REQUEST *) message;
 
  
  req->has_host_header = 1;
  req->host_header_port = -1;
  
  rt = HTTP_get_header_token( parser );
  if (rt == HTTP_TK_TEXT) {
    req->host_header = strdup( parser->token );
    rt = HTTP_get_header_token( parser );
    if (rt == HTTP_TK_SEPARATOR && parser->token[0] == ':') {
      rt = HTTP_get_header_token( parser );
      if (rt == HTTP_TK_TEXT) {
	req->host_header_port = atoi( parser->token );
      }
    }
  }
 
  return 0;
}

int init_parsers_general_header( HTTP_PARSER *parser )
{
  if (HTTP_add_header_parser( parser, "content-length",  parser_content_length_header) ||
      HTTP_add_header_parser( parser, "connection",  parser_connection_header) || 
       HTTP_add_header_parser( parser, "transfer-encoding",  parser_transfer_encoding_header) ) {
    return -1;
  }
  return 0;
}

int init_parsers_request_header( HTTP_PARSER *parser )
{
  if (HTTP_add_header_parser( parser, "expect",  parser_expect_header) || 
      HTTP_add_header_parser( parser, "host",  parser_host_header) ) {
    return -1;
  }
  return 0;
}
