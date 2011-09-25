#include "http.h"
#include <sutils.h> 
#include <charclass.h>
#include <stdio.h>

int init_parsers_request_header( HTTP_PARSER *parser );
int init_parsers_general_header( HTTP_PARSER *parser );
 
#define S_METHOD_GET		"GET"
#define S_METHOD_GET_LEN	3
#define S_METHOD_POST		"POST" 
#define S_METHOD_POST_LEN	4
#define S_METHOD_PUT		"PUT"
#define S_METHOD_PUT_LEN	3
#define S_METHOD_DELETE		"DELETE"
#define S_METHOD_DELETE_LEN	6
#define S_METHOD_HEAD		"HEAD"
#define S_METHOD_HEAD_LEN	4
#define S_METHOD_OPTIONS	"OPTIONS"
#define S_METHOD_OPTIONS_LEN	7
#define S_METHOD_TRACE		"TRACE"
#define S_METHOD_TRACE_LEN	5
#define S_METHOD_CONNECT	"CONNECT"
#define S_METHOD_CONNECT_LEN	7

#define HTTP_START_S "HTTP/1."
#define HTTP_START_S_LEN 7



#define HTTP_EOF_LINE  "\r\n"
#define HTTP_EOF_LINE_LEN 2



//==========================================================================

#define to_lower(pos) \
  if (is_upper_case( *pos )) { \
    *pos = *pos - 'A' + 'a'; \
  }


//CTL   = <any US-ASCII control character (octets 0 - 31) and DEL (127)>
M_INLINE int http_is_ctl( char ch )
{
    return (ch >=0 && ch <= 31) || ch == 127;
}

M_INLINE int http_is_ctl_or_space( char ch )
{
    return (ch >=0 && ch <= 32) || ch == 127;
}

M_INLINE int http_is_space( char ch )
{
  return ch == ' ' || ch == '\t';
}

// separators     = "(" | ")" | "<" | ">" | "@"
//                      | "," | ";" | ":" | "\" | <">
//                      | "/" | "[" | "]" | "?" | "="
//                      | "{" | "}" | SP | HT
M_INLINE int http_is_separator( char ch )
{
  return ch == '('  || ch ==  ')' || ch ==  '<' || ch ==  '>'  || ch ==  '@'
      || ch ==  ',' || ch ==  ';' || ch ==  ':' || ch ==  '\\' || ch ==  '"'
      || ch ==  '/' || ch ==  '[' || ch ==  ']' || ch ==  '?'  || ch ==  '='
      || ch ==  '{' || ch ==  '}' || ch ==  ' ' || ch ==  '\t';
}


typedef struct tagSTRINGPAIR {
  char *key;
  char *value;
} STRINGPAIR;

static void free_stringpair( DLISTUNR *list, void *entry, void *context)
{
  STRINGPAIR *pair = (STRINGPAIR *) entry;
 
  M_UNUSED(list);
  M_UNUSED(context);
 
  free( pair->key );
  free( pair->value );
}


//==========================================================================
int HTTP_MESSAGE_init( HTTP_MESSAGE *message )
{  
  message->flags = 0;
  message->content_length = -1;

  if (DLISTUNR_init( &message->header_values, sizeof(STRINGPAIR), 10) ) {
    return -1;
  }
  return 0;
}

void HTTP_MESSAGE_free( HTTP_MESSAGE *message )
{  
  message->flags = 0;
  message->content_length = -1;
  DLISTUNR_free( &message->header_values,  free_stringpair, 0); 

}

int HTTP_MESSAGE_add_header( HTTP_MESSAGE *message, const char *name , const char *value )
{
  STRINGPAIR entry;

  entry.key   = strdup(name);
  entry.value = strdup(value);

  return DLISTUNR_push_back( &message->header_values, &entry, sizeof(entry));
}

//==========================================================================

typedef struct tagHEADER_HASH_ACTION {
  HASH_Entry entry;  
  HEADER_ACTION action;
  char *key;
  size_t key_size;
} HEADER_HASH_ACTION;

#define MAX_HTTP_TOKEN_SIZE 120

static int hash_compare(HASH_Entry *entry, void * key, ssize_t key_length)
{
  HEADER_HASH_ACTION *lhs;

  lhs = (HEADER_HASH_ACTION *) entry;

  return strncmp(lhs->key, key, key_length); 
}

int  HTTP_PARSER_init(  HTTP_PARSER *parser )
{
  parser->state = HTTP_STATE_PARSING_REQUEST_LINE;
  parser->token = malloc( MAX_HTTP_TOKEN_SIZE );
  if (! parser->token ) {
    return -1;
  }
  parser->token_length = MAX_HTTP_TOKEN_SIZE;
  
  if (HASH_init( &parser->header_action, 32, 0, hash_compare, 0 ) ) {
    return -1;
  }
 
  return 0;
}

int  HTTP_PARSER_free(  HTTP_PARSER *parser )
{
  if (parser->token) {
    free( parser->token );
    parser->token = 0;
  }
  return 0;
}


int HTTP_add_header_parser( HTTP_PARSER *parser, const char *header_name, HEADER_ACTION action_func )
{
  HEADER_HASH_ACTION * action;
  
  action = ( HEADER_HASH_ACTION *) malloc( sizeof( HEADER_HASH_ACTION ) ) ;
  if (!action) {
    return -1;
  }
  action->key = strdup( header_name );
  action->key_size = strlen( header_name );
  action->action = action_func;


  return HASH_insert( &parser->header_action, &action->entry, (void *) header_name, strlen(header_name) );
}


PARSER_STATUS HTTP_get_line( BF *bf, char **start_line )
{
   uint8_t *pos;
#if 0   
   uint8_t *prev;
#endif   
   char *line;

   pos = bf->get_pos;
   line = BF_get_line_ext( bf, HTTP_EOF_LINE, HTTP_EOF_LINE_LEN );
   if (!line) {
      if (BF_is_full( bf ) ) {
        // whole buffer does not even contain a line. untennable.
        return PARSER_STATUS_ERROR;
      }
      BF_compact(bf);
      return PARSER_STATUS_NEED_MORE_DATA;
   }


#if 0
   // check if this line is continued.
   while ( 1 ) {
     prev = bf->get_pos;
     line = BF_get_line_ext( bf, HTTP_EOF_LINE, HTTP_EOF_LINE_LEN );
     if (!line) {
       // restore the line marker.
       *(bf->get_pos - 2)  = '\r';

	if (BF_is_full( bf ) ) {
          // a continuation line spans the whole input buffer,
	  // That's very bad, unfortunately we can't support this.
	  return  PARSER_STATUS_ERROR;
	}
	bf->get_pos = pos;
	return PARSER_STATUS_NEED_MORE_DATA;
     }

     if (!is_space( *line )) {
       *(bf->get_pos - 2)  = '\r';
       bf->get_pos = prev;
       break;
     }
     // adjoin the previous line to this line.
     *(bf->get_pos - 2)  = ' ';
     *(bf->get_pos - 1)  = ' ';
   }
#endif
   
   *start_line = (char *) pos;
   return PARSER_STATUS_COMPLETED;
}
 
/*
token          = 1*<any CHAR except CTLs or separators>
separators     = "(" | ")" | "<" | ">" | "@"
                      | "," | ";" | ":" | "\" | <">
                      | "/" | "[" | "]" | "?" | "="
                      | "{" | "}" | SP | HT


quoted-string  = ( <"> *(qdtext | quoted-pair ) <"> )
qdtext         = <any TEXT except <">>
quoted-pair    = "\" CHAR

*/

int HTTP_get_header_token( HTTP_PARSER *parser) 
{
   char ch;
   size_t token_pos;

   if (*parser->tokpos == '\0') {
     return  HTTP_TK_EOF;
   }
    

     // skip leading spaces
   for( ;http_is_space( *parser->tokpos ); ++parser->tokpos );

   ch = *parser->tokpos;

   if (http_is_separator(ch)) {
     parser->token[ 0 ] = ch;
     parser->token[ 1 ] = '\0';
     ++ parser->tokpos;
     return HTTP_TK_SEPARATOR;
   }

   
   if (ch == '"') {
      token_pos = 0; 
      do {
        ch = *parser->tokpos ++ ;
chk:
	if (http_is_ctl( ch )) {
	   return -1;
	}
	if (ch == '"') {
	  if (token_pos > parser->token_length) {
	    return -1;
	  }
	  parser->token[ token_pos++ ] = '\0';
	  return  HTTP_TK_QUOTED_STRING; 
	} else {
          if (ch == '\\') {
	    goto chk;
	  }
	}

	// add character.
	if (token_pos > parser->token_length) {
	   return -1;
	}
	parser->token[ token_pos++ ] = ch;

      } while (ch != '\0');
      
   }

   if ( !http_is_separator( ch ) ) {
     token_pos = 0; 
     do {

	if (token_pos >= parser->token_length) {
	   return -1;
	}
	parser->token[ token_pos++ ] = ch;

       ch = *( ++ parser->tokpos );
     } while( ch != 0 && ! http_is_ctl( ch ) &&  ! http_is_separator( ch ) );

     parser->token[ token_pos ] = '\0';
     return HTTP_TK_TEXT; 
   }

   return -1;
}

PARSER_STATUS HTTP_parse_header_line( HTTP_PARSER *parser, HTTP_MESSAGE *request, BF *bf, int *eof_header)
{
  PARSER_STATUS rt;
  HEADER_HASH_ACTION *action;
  char *line, *pos;
  
  *eof_header = 0;

  rt = HTTP_get_line( bf, &line );
  if (rt != PARSER_STATUS_COMPLETED) {
    return rt;
  }

  if (! *line ) {
    *eof_header = 1;
     return PARSER_STATUS_COMPLETED;
  }

  if (is_space(*line)) {
    return -1;
  }
  // good enough, do not test for all separators though.
  for( pos = line  ;!http_is_ctl_or_space( *pos ) && *pos != ':'; ++pos ) {
     to_lower( pos );
  } 
  if (*pos != ':') {
    return -1;
  }
  *pos = '\0';

  if (HTTP_MESSAGE_add_header( request, line, pos+1 )) {
     return -1;
  }


  // got the header line. find header processor.
  action = (HEADER_HASH_ACTION *) HASH_find( &parser->header_action, line, pos - line );
  if (action) {

    parser->tokpos = pos + 1;
    action->action( request, parser );
  }

  // recored header value in http request (optional)

  return 0;
}

int HTTP_PARSER_content_length_init( HTTP_PARSER *parser, HTTP_MESSAGE *msg )
{
    parser->state = HTTP_STATE_PARSING_BODY_CONTENT_LENGTH;
    parser->content_left = msg-> content_length;
    return 0;
}

int HTTP_PARSER_content_length_process( HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg,   void *ctx)
{
   size_t bytes_to_take;
   size_t bytes_available; 

   bytes_available = BF_get_size( bf );
   if (bytes_available == 0) {
       return PARSER_STATUS_NEED_MORE_DATA;
   }

   bytes_to_take =  parser->content_left; 
   if (bytes_to_take == 0) {
     return  PARSER_STATUS_ERROR;
   }

   if (bytes_available < bytes_to_take) {
     bytes_to_take = bytes_available;
   } 

   cb( msg, bf->get_pos, bytes_to_take, ctx );

   parser->content_left -= bytes_to_take;
   bf->get_pos += bytes_to_take;

   if (parser->content_left == 0) {
     parser->state =  HTTP_STATE_PARSING_REQUEST_LINE;
     return PARSER_STATUS_COMPLETED;
   }

   BF_compact(bf);
   return PARSER_STATUS_NEED_MORE_DATA;
}

int HTTP_PARSER_chunked_data_init( HTTP_PARSER *parser )
{
    parser->state = HTTP_STATE_PARSING_BODY_CHUNK_HEADER;
    return 0;
}



/*
 Chunked-Body  = *chunk
                  last-chunk
                  trailer
	          CRLF
 chunk          = chunk-size [ chunk-extension ] CRLF
		  chunk-data CRLF
 chunk-size     = 1*HEX 
 last-chunk     = 1*("0") [ chunk-extension ] CRLF
 chunk-extension= *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
 chunk-ext-val  = token | quoted-string
 chunk-data     = chunk-size(OCTET)
 trailer        = *(entity-header CRLF)
*/
PARSER_STATUS HTTP_PARSER_chunked_data_process( HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx)
{
    char *line,*pos,*cur, *endptr;
    int rt, eof_header;
    long val;

    switch( parser->state ) {
      case HTTP_STATE_PARSING_BODY_CHUNK_HEADER:
chunk_header:      
         line = BF_get_line_ext( bf, HTTP_EOF_LINE, HTTP_EOF_LINE_LEN );
         if (!line) {
	   BF_compact(bf);
	   return PARSER_STATUS_NEED_MORE_DATA;
	 }
	 for(pos = line; is_space(*pos); ++pos);
	 for(cur = pos; is_hex(*cur); ++cur);
	 *cur = '\0';

         val = strtol( pos, &endptr, 16 );
         if (val != 0) {
           parser->content_left = (size_t) val;
	   parser->state = HTTP_STATE_PARSING_BODY_CHUNK_DATA;
	   goto chunk_data; 
	 } else {
	   parser->state = HTTP_STATE_PARSING_BODY_CHUNK_TRAILER;
	   goto chunk_trailer;
	 }	
	 

      case HTTP_STATE_PARSING_BODY_CHUNK_DATA:
chunk_data:      
        rt = HTTP_PARSER_content_length_process( parser, bf, cb, msg, ctx );
	if (rt != 0) {
	  return rt;
	}

	parser->state = HTTP_STATE_PARSING_BODY_CHUNK_EOF_AFTER_DATA;
	goto chunk_eof_data;

     case HTTP_STATE_PARSING_BODY_CHUNK_EOF_AFTER_DATA:
chunk_eof_data:
        line = BF_get_line_ext( bf, HTTP_EOF_LINE, HTTP_EOF_LINE_LEN );
        if (!line) {
	  BF_compact(bf);
	  return PARSER_STATUS_NEED_MORE_DATA;
	}
	parser->state = HTTP_STATE_PARSING_BODY_CHUNK_HEADER;
        goto chunk_header;

      case HTTP_STATE_PARSING_BODY_CHUNK_TRAILER:
chunk_trailer:      
        rt = HTTP_parse_header_line( parser, msg , bf, &eof_header );
	if (rt != PARSER_STATUS_COMPLETED) {
	  return rt;
	}
	if (eof_header) {
	  BF_compact(bf);
	  return PARSER_STATUS_COMPLETED;
        }   
        return PARSER_STATUS_NEED_MORE_DATA;
      
      default:
	return -1;
    }
    return 0;
}


//==========================================================================

// Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
// Method         = "OPTIONS"                ; Section 9.2
//             | "GET"                    ; Section 9.3
//             | "HEAD"                   ; Section 9.4
//             | "POST"                   ; Section 9.5
//             | "PUT"                    ; Section 9.6
//             | "DELETE"                 ; Section 9.7
//             | "TRACE"                  ; Section 9.8
//             | "CONNECT"                ; Section 9.9
//             | extension-method
//
//     extension-method = token
//
//  Request-URI    = "*" | absoluteURI | abs_path | authority
//
static PARSER_STATUS parse_request_line( HTTP_REQUEST *request, BF *bf )
{
  Http_method_type method;
  char *eof,*tok, *line;
  PARSER_STATUS rt;  

  rt = HTTP_get_line( bf, &line );
  if (rt != PARSER_STATUS_COMPLETED) {
    return rt;
  }

  switch( *line ) {
    case 'G':
      if (strncmp( line, S_METHOD_GET, S_METHOD_GET_LEN ) != 0) {
	return PARSER_STATUS_ERROR;
      }
      method = HTTP_METHOD_GET; 
      line += S_METHOD_GET_LEN;
      break;
    case 'P':
      if (strncmp( line, S_METHOD_POST, S_METHOD_POST_LEN ) == 0) {
        method = HTTP_METHOD_POST;
        line += S_METHOD_POST_LEN;
      } else if (strncmp( line, S_METHOD_PUT, S_METHOD_PUT_LEN ) == 0) {
        method = HTTP_METHOD_PUT; 
        line += S_METHOD_PUT_LEN;
      } else {
        return PARSER_STATUS_ERROR;
      }
      break;
    case 'H':
      if (strncmp( line, S_METHOD_HEAD, S_METHOD_HEAD_LEN ) != 0) {
	return  PARSER_STATUS_ERROR;
      }
      method = HTTP_METHOD_HEAD;
      line += S_METHOD_HEAD_LEN ;
      break;
    case 'D':
      if (strncmp( line, S_METHOD_DELETE, S_METHOD_DELETE_LEN ) != 0) {
	return  PARSER_STATUS_ERROR;
      }
      method = HTTP_METHOD_DELETE;
      line += S_METHOD_DELETE_LEN ;
      break;
    case 'T':
      if (strncmp( line, S_METHOD_TRACE, S_METHOD_TRACE_LEN ) != 0) {
	return PARSER_STATUS_ERROR;
      }
      method = HTTP_METHOD_TRACE;
      line += S_METHOD_TRACE_LEN ;
      break;
    case 'C':
      if (strncmp( line, S_METHOD_CONNECT, S_METHOD_CONNECT_LEN ) != 0) {
	return PARSER_STATUS_ERROR;
      }
      method = HTTP_METHOD_CONNECT; 
      line += S_METHOD_CONNECT_LEN;
      break;
    default:
      return -1;
  }
  request->method = method;

  tok =  get_token( line, &eof );
  if (!tok) {
   return PARSER_STATUS_ERROR;
  }

  *eof = 0;
  request->raw_url = strdup( tok );

  if (method != HTTP_METHOD_CONNECT) {
    if (URI_parse( &request->url, tok )) {
      return PARSER_STATUS_ERROR;
    }
  } else {
    if (strcmp(tok, "*") != 0) {
      return PARSER_STATUS_ERROR;
    }
    URI_init( &request->url );
  }

  tok =  get_token( eof+1, &eof );
  if (strncmp( tok, HTTP_START_S, HTTP_START_S_LEN ) != 0) {
    return PARSER_STATUS_ERROR;
  }
  tok += HTTP_START_S_LEN;

  switch(tok[0]) {
     case '0':
        request->version = HTTP_VERSION_1_0;
	break;
     case '1':
        request->version =  HTTP_VERSION_1_1; 
	break;
     default:
	return PARSER_STATUS_ERROR;
  }
  return PARSER_STATUS_COMPLETED;
}


int HTTP_REQUEST_PARSER_init( HTTP_REQUEST_PARSER *parser, 
		              HTTP_REQ_HEADER_PARSED header_parsed,
			      HTTP_REQ_MESSAGE_BODY_DATA on_message_body_data,
			      HTTP_REQ_FINISHED on_request_finished,
			      void *ctx)
{
  parser->ctx = ctx;
  parser->ev_header  = header_parsed;
  parser->ev_body = on_message_body_data;
  parser->ev_finish = on_request_finished;
     
  if (HTTP_PARSER_init( &parser->base )) {
    return -1;
  }
 
  if (init_parsers_request_header( &parser->base ) ||
      init_parsers_general_header( &parser->base ) ) {
    return -1;
  }
 
  return 0;
}



PARSER_STATUS HTTP_REQUEST_PARSER_process( HTTP_REQUEST_PARSER *parser, HTTP_REQUEST *request, BF *bf )
{
  int rt, eof_header;


  while(1) {
   switch( parser->base.state ) {
    case HTTP_STATE_PARSING_REQUEST_LINE:
      rt = parse_request_line( request, bf );
      if (rt != 0) {
       goto req_do_return;
      }
      parser->base.state = HTTP_STATE_PARSING_HEADERS; // 
   //break; - fall through


    case HTTP_STATE_PARSING_HEADERS:
      rt = HTTP_parse_header_line( (HTTP_PARSER *) parser, (HTTP_MESSAGE *) request, bf, &eof_header );
      if (rt != 0) {
        goto req_do_return;
      }

      if (eof_header) {

	  // if http 1.1 then host header is a must. no host header is an error.
	  if (request->version ==  HTTP_VERSION_1_1 &&
	      !request->has_host_header) {
	    return PARSER_STATUS_ERROR;  
	  }

	  // eof parsing http header.
	  parser->ev_header( request, parser->ctx ); 
	  BF_compact(bf);

	  if (request->base.flags & HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED) {
	     HTTP_PARSER_chunked_data_init( &parser->base );
	     continue;  
	  }

 
	  if (request->base.flags & HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH) {
	     HTTP_PARSER_content_length_init( &parser->base, &request->base );
	     continue;  
	  }

next_request:
	  parser->ev_finish( request, parser->ctx ); 
	  parser->base.state = HTTP_STATE_PARSING_REQUEST_LINE; 
	  HTTP_REQUEST_free(request);  
          return PARSER_STATUS_COMPLETED;
      }
      break;

    case HTTP_STATE_PARSING_BODY_CONTENT_LENGTH:
      rt = HTTP_PARSER_content_length_process( 
		&parser->base, bf, 
		(HTTP_PROCESS_MSG_DATA) parser->ev_body, 
		&request->base, parser->ctx);
      if (rt != 0) {
        goto req_do_return;
      }
      goto next_request;

    default: // remaining states deal with chunks
      rt =  HTTP_PARSER_chunked_data_process( 
		&parser->base, bf, 
		(HTTP_PROCESS_MSG_DATA) parser->ev_body, 
		&request->base, parser->ctx);

      if (rt != 0) {
        goto req_do_return;
      }
      goto next_request;
    }
  }

req_do_return:
  if (rt ==  PARSER_STATUS_NEED_MORE_DATA) {
    BF_compact( bf );
  }
  return rt;
}


// =============================================================================
//Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
static PARSER_STATUS parse_response_line( HTTP_RESPONSE *response, BF *bf )
{
  PARSER_STATUS rt;  
  char *line, *tok, *eof;
  int st;

  rt = HTTP_get_line( bf, &line );
  if (rt != PARSER_STATUS_COMPLETED) {
    return rt;
  }

  tok = line;
  if (strncmp( tok, HTTP_START_S, HTTP_START_S_LEN ) != 0) {
    return PARSER_STATUS_ERROR;
  }
  tok += HTTP_START_S_LEN;

  switch(tok[0]) {
     case '0':
        response->version = HTTP_VERSION_1_0;
	break;
     case '1':
        response->version =  HTTP_VERSION_1_1; 
	break;
     default:
	return PARSER_STATUS_ERROR;
  }
 
  tok =  get_token( line, &eof );
  if (!tok) {
   return PARSER_STATUS_ERROR;
  }

  st = atoi( tok );
  if (st < 100 || st >= 600) {
    return PARSER_STATUS_ERROR;
  }
 
  response->status_code = st;

  return PARSER_STATUS_COMPLETED;
}

int HTTP_RESPONSE_PARSER_init( HTTP_RESPONSE_PARSER *parser,
		               HTTP_RESP_HEADER_PARSED header_parsed,
			       HTTP_RESP_MESSAGE_BODY_DATA on_message_body_data,
				   HTTP_RESP_FINISHED on_request_finished,
			       void *ctx)
{
  parser->ctx = ctx;
  parser->ev_header = header_parsed; 
  parser->ev_body   = on_message_body_data;
  parser->ev_finish = on_request_finished;
 
  if (! HTTP_PARSER_init( &parser->base )) {
    return -1;
  }

  if (init_parsers_general_header( &parser->base ) ) {
    return -1;
  }
  return 0;
}



PARSER_STATUS HTTP_RESPONSE_PARSER_process( HTTP_RESPONSE_PARSER *parser, HTTP_RESPONSE *response, BF *bf )
{
  int rt, eof_header;

  while(1) {
   switch( parser->base.state ) {
    case HTTP_STATE_PARSING_REQUEST_LINE:
      rt = parse_response_line( response, bf );
      if (rt != 0) {
        goto resp_do_return; 
      }
      parser->base.state = HTTP_STATE_PARSING_HEADERS; // 
   //break; - fall through


    case HTTP_STATE_PARSING_HEADERS:
      rt = HTTP_parse_header_line( (HTTP_PARSER *) parser, (HTTP_MESSAGE *) response, bf, &eof_header );
      if (rt != 0) {
       goto resp_do_return; 
      }

      if (eof_header) {
	  // eof parsing http header.
	  parser->ev_header( response, parser->ctx ); 
	
	  if (response->base.flags & HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED) {
	     parser->base.state = HTTP_STATE_PARSING_BODY_CHUNK_HEADER;
	     continue;  
	  }

 
	  if (response->base.flags & HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH) {
	     HTTP_PARSER_content_length_init( &parser->base, &response->base );
	     continue;  
	  }

next_response:
	  HTTP_RESPONSE_free( response );
	  parser->ev_finish( response, parser->ctx ); 
	  parser->base.state = HTTP_STATE_PARSING_REQUEST_LINE; 
	  BF_compact(bf);
          return PARSER_STATUS_COMPLETED;
      }
      break;

    case HTTP_STATE_PARSING_BODY_CONTENT_LENGTH:
      rt = HTTP_PARSER_content_length_process( 
		&parser->base, bf, 
		(HTTP_PROCESS_MSG_DATA) parser->ev_body, 
		&response->base, parser->ctx);
      if (rt != 0) {
        goto resp_do_return; 
      }
      goto next_response;

    default: // remaining states deal with chunks
      rt =  HTTP_PARSER_chunked_data_process( 
		&parser->base, bf, 
		(HTTP_PROCESS_MSG_DATA) parser->ev_body, 
		&response->base, parser->ctx);

      if (rt != 0) {
        goto resp_do_return; 
      }
      goto next_response;
    }
  }
resp_do_return:
  if (rt ==  PARSER_STATUS_NEED_MORE_DATA) {
    BF_compact( bf );
  }
  return rt;
}

// =============================================================================

static const char *http_version_to_str( Http_version_type ver )
{
  switch(ver) {
    case HTTP_VERSION_1_0:
      return "1.0";
    case HTTP_VERSION_1_1:
      return "1.1";
  }
  return "";
}

#define S_CONNECTION_CLOSE "Connection: close\r\n"
#define S_CONNECTION_CLOSE_LEN 19

#define S_TRANSFER_CHUNKED "Transfer-encoding: chunked\r\n"
#define S_TRANSFER_CHUNKED_LEN 28


#define RETURN_WRITER_MORE_DATA \
  if (bf->start == bf->put_pos) { \
    return PARSER_STATUS_ERROR;  /* the buffer can't hold a single value */ \
  } \
  return PARSER_STATUS_NEED_MORE_DATA; 

static const char *get_reason_phrase( int code )
{
    switch( code ) {
 	  case 100:         return "Continue";
          case 101:         return "Switching Protocols";
          case 200:         return "OK";
          case 201:         return "Created";
          case 202:         return "Accepted";
          case 203:         return "Non-Authoritative Information";
          case 204:         return "No Content";
          case 205:         return "Reset Content";
          case 206:         return "Partial Content";
          case 300:         return "Multiple Choices";
          case 301:         return "Moved Permanently";
          case 302:         return "Found";
          case 303:         return "See Other";
          case 304:         return "Not Modified";
          case 305:         return "Use Proxy";
          case 307:         return "Temporary Redirect";
          case 400:         return "Bad Request";
          case 401:         return "Unauthorized";
          case 402:         return "Payment Required";
          case 403:         return "Forbidden";
          case 404:         return "Not Found";
          case 405:         return "Method Not Allowed";
          case 406:         return "Not Acceptable";
          case 407:         return "Proxy Authentication Required";
          case 408:         return "Request Time-out";
          case 409:         return "Conflict";
          case 410:         return "Gone";
          case 411:         return "Length Required";
          case 412:         return "Precondition Failed";
          case 413:         return "Request Entity Too Large";
          case 414:         return "Request-URI Too Large";
          case 415:         return "Unsupported Media Type";
          case 416:         return "Requested range not satisfiable";
          case 417:         return "Expectation Failed";
          case 500:         return "Internal Server Error";
          case 501:         return "Not Implemented";
          case 502:         return "Bad Gateway";
          case 503:         return "Service Unavailable";
          case 504:         return "Gateway Time-out";
          case 505:         return "HTTP Version not supported";
   }
   return "";
}

PARSER_STATUS HTTP_RESPONSE_WRITER_write( HTTP_RESPONSE_WRITER *writer, BF *bf )
{ 
  HTTP_RESPONSE *resp = writer->response;
  char line_buf[40];
  int nlen;

  while(1) {
  switch( writer->state ) {
    case HTTP_RESPONSE_WR_STATUS_LINE:
      nlen = sprintf(line_buf,"HTTP/%s %d %s\r\n", http_version_to_str( resp->version ), resp->status_code, get_reason_phrase( resp->status_code ) );
      if (BF_putn( bf, line_buf, nlen) ) {
         RETURN_WRITER_MORE_DATA;
      }
      writer->state = HTTP_RESPONSE_WR_CONNECTION_CLOSE; 
    //break;
    case HTTP_RESPONSE_WR_CONNECTION_CLOSE:
      if (resp->base.flags & HTTP_MESSAGE_FLAG_CONNECTION_CLOSE ) {
        if (BF_putn( bf, S_CONNECTION_CLOSE, S_CONNECTION_CLOSE_LEN ) ) {
          RETURN_WRITER_MORE_DATA;
        } 
      }
      writer->state = HTTP_RESPONSE_WR_CHUNKED; 
    //break;
    case HTTP_RESPONSE_WR_CHUNKED:
      if (resp->base.flags & HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED ) {
        if (BF_putn( bf, S_TRANSFER_CHUNKED, S_TRANSFER_CHUNKED_LEN ) ) {
          RETURN_WRITER_MORE_DATA;
	}
      }
      writer->state = HTTP_RESPONSE_WR_CONTENT_LENGTH; 
     //break;
    case HTTP_RESPONSE_WR_CONTENT_LENGTH:
      if (resp->base.flags & HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH ) {
        nlen = sprintf(line_buf,"Content-Length: %d\r\n", resp->base.content_length );
        if (BF_putn( bf, line_buf, nlen) ) {
          RETURN_WRITER_MORE_DATA;
        }
      }
      writer->state = HTTP_RESPONSE_WR_HEADERS; 
      writer->header_position = DLISTUNR_get_first( &resp->base.header_values );
      writer->state_header = 0;
     //break;
    case HTTP_RESPONSE_WR_HEADERS:
      if ( ! DLISTUNR_is_eof( &resp->base.header_values, writer->header_position)) {
        STRINGPAIR *header = (STRINGPAIR *) DLISTUNR_at(&resp->base.header_values, writer->header_position);
        switch(writer->state_header) {
	    case 0:
		if (BF_putn( bf, header->key, strlen(header->key))) {
	          RETURN_WRITER_MORE_DATA;
		}
		writer->state_header = 1;
	      //break;
 		
	    case 1:
		if (BF_putn( bf, ": ", 2)) {
	          RETURN_WRITER_MORE_DATA;
		}
		writer->state_header = 2;
	      //break;

	    case 2:
		if (BF_putn( bf, header->value, strlen(header->value))) {
	          RETURN_WRITER_MORE_DATA;
		}
		writer->state_header = 3;
	      //break;

	    case 3:
		if (BF_putn( bf, "\r\n", 2)) {
	          RETURN_WRITER_MORE_DATA;
		}
                writer->header_position = DLISTUNR_next(writer->header_position);
		writer->state_header = 0;
		break;
	   }
      } else {
	 writer->state = HTTP_RESPONSE_WR_EOF;
	 goto wr_eof;
	
      }
      break;
    case HTTP_RESPONSE_WR_EOF:
wr_eof:    
      return PARSER_STATUS_COMPLETED; 
    }
  }

}


