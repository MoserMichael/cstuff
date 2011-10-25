#include <webby/webby.h>
#include <stdio.h>

static SERVLET_STATUS header_echo_servlet_action( HTTP_servlet_request * request, HTTP_servlet_response *response, SERVLET_CONTEXT *context )
{ 
  DLISTUNR_position idx;
  STRINGPAIR *cur;
  BF *bf;
  char buf[512];
  int nprint;
  URI *uri;
  
  M_UNUSED( context );

  uri = HTTP_servlet_uri( request );
  if (strcmp( uri->path, "/headerservlet" ) != 0 ) {
    return SERVLET_REQUEST_IGNORED;
  }

  HTTP_response_start( response, 200, "text/plain", RESPONSE_CHUNKED,  0 );

  bf = HTTP_response_get_chunk_buffer( response, 1024 );

  DLISTUNR_FOREACH( idx, &request->request->base.header_values ) {
     cur = (STRINGPAIR *) DLISTUNR_at( &response->response.base.header_values, idx );
     nprint = snprintf(buf, sizeof(buf), "%s = %s\n", cur->key, cur->value);
     if (BF_put_size(bf) > (size_t) nprint) {
       BF_putn( bf, buf, nprint);
     } else {
        if (HTTP_response_write_chunk( response, bf)) {
	  return -1;
	}
	bf = HTTP_response_get_chunk_buffer( response, 1024 );
     }
  }
  if (BF_get_size(bf) > 0) {
    if (HTTP_response_write_chunk( response, bf)) {
      return -1;
    }
  }

  if (HTTP_response_finish( response ) ) {
    return SERVLET_REQUEST_ERROR;    
  }
 
  return SERVLET_REQUEST_HANDLED; 
}

HTTP_SERVLET *load_servlet()
{ 
  HTTP_SERVLET *ret;

  ret = malloc( sizeof( HTTP_SERVLET *) );
  if (!ret) {
    return 0;
  }

  HTTP_SERVLET_init( ret, 0,  0, header_echo_servlet_action , 0 );
  return ret;
}
