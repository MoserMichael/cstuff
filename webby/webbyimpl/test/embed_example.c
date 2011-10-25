#include <webby/webby.h>
#include <butils/logg.h>
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
  if (strcmp( uri->path, "/headerservlet" ) == 0) {
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
      return SERVLET_REQUEST_ERROR;
    }
  }

  if (HTTP_response_finish( response ) ) {
    return SERVLET_REQUEST_ERROR;    
  }
 
  return SERVLET_REQUEST_HANDLED; 
}

int main(int argc, char *argv[])
{
  WEBBY_CONFIG webby_cfg;
  WEBBY *webby;
  HTTP_SERVLET servlet;
 
  if (argc < 2 ) {
     fprintf(stderr,
       "%s <cfg_file>\nExample on embedding a web server with a hello world servlet.\nThe single argument is the wevb server configuration file.\n", argv[0] );
     exit(1);
  }

  // enable logging 
  //MLOG_init( MLOG_LEVEL_INFO, MLOG_ACTION_CONSOLE, 0);
  MLOG_init( MLOG_LEVEL_TRACE, MLOG_ACTION_CONSOLE, 0);

  // read configuration
  if (WEBBY_CONFIG_load( &webby_cfg, argv[1] )) {
    fprintf(stderr,"ERROR: invalid configuration file %s\n", argv[1] );
    exit(1);
  }
  // initialise the server
  webby = WEBBY_init( &webby_cfg );
  if (!webby) {
    fprintf(stderr,"ERROR: Can't initialise web server.\n");
    exit(1);
  }
  
  // initialise the hello world servlet and add it.
  HTTP_SERVLET_init( &servlet, 0, 0, header_echo_servlet_action, 0 );
  WEBBY_add_servlet( webby, &servlet );
 

  // run the server
  if (WEBBY_run( webby )) {
    fprintf(stderr,"ERROR: failed to start webby the web server\n" );
    exit(1);
  }
  return 0;
}
