#include <webby/webby.h>
#include <webby/webbyimpl.h>
#include <stdio.h>
#include "vtest/vtest.h"

// --------------------------------------------------------



int WEBBY_impl_new( WEBBY *srv, void **ret )
{
  M_UNUSED(srv);
  *ret = 0;
  return 0;
}

int WEBBY_impl_run( void *impl )
{
  M_UNUSED(impl);
  return 0;
}

int WEBBY_impl_shutdown( void *impl )
{
  M_UNUSED(impl);
  return 0;
}

int WEBBY_impl_send_data( void *impl, void *data, size_t size )
{
  FILE *fp = (FILE *) impl;
  size_t wr;

  wr = fwrite( data, 1, size, fp ); 
  return wr != size;
}

int WEBBY_impl_response_completed( void *impl, int connection_close )
{
  FILE *fp = (FILE *) impl;
  if (connection_close) {
    fclose(fp);
  } else {
    fflush(fp);
  }
  return 0;
}

// --------------------------------------------------------

SERVLET_STATUS header_echo_servlet_action( HTTP_servlet_request * request, HTTP_servlet_response *response, SERVLET_CONTEXT *context )
{ 
  DLISTUNR_position idx;
  STRINGPAIR *cur;
  BF *bf;
  char buf[512];
  int nprint;

  M_UNUSED( request );
  M_UNUSED( context );

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

// --------------------------------------------------------

SERVLET_STATUS fixed_response_servlet_action( HTTP_servlet_request * request, HTTP_servlet_response *response, SERVLET_CONTEXT *context )
{ 
   M_UNUSED(request);
   M_UNUSED(context);

   HTTP_response_start( response, 200, "text/plain", RESPONSE_CONTENT_LENGTH, 10 );
   if (HTTP_response_send( response, "0123456789A", 10 ) ) {
     return SERVLET_REQUEST_ERROR;    
   }
      
   if (HTTP_response_finish( response ) ) {
     return SERVLET_REQUEST_ERROR;    
   }
 
   return SERVLET_REQUEST_HANDLED; 
}
// --------------------------------------------------------
void WEBBY_one_test(const char *in_file_name, const char *out_file_name, int connection_close, HTTP_SERVLET_ACTION servlet_action   )
{
  WEBBY *server;
  HTTP_SERVLET counter_servlet;
  WEBBY_CONFIG cfg;
  WEBBY_CONNECTION *conn;
  FILE *fp_in,*fp_out;
  int ch,rt;
  char tmp_out[ 512 ], cmd[ 512 ];

  fprintf(stderr,"\tChecking %s\n", in_file_name );
  
  fp_in = fopen(in_file_name,"r");
  VASSERT(fp_in != 0 );

  VASSERT( WEBBY_CONFIG_load( &cfg, "data/tconfig.props" ) == 0 );

  server = WEBBY_init( &cfg );
  VASSERT( server != 0 );

  HTTP_SERVLET_init( &counter_servlet, 0, 0, servlet_action, 0 );
  VASSERT( WEBBY_add_servlet( server, &counter_servlet ) == 0 );	 
  
  rt = WEBBY_run( server );  
  VASSERT( rt == 0 );

  strcpy( tmp_out, in_file_name );
  strcat( tmp_out, ".tresult" );

  fp_out= fopen( tmp_out, "w");
  VASSERT( fp_out != 0 );

  conn = WEBBY_new_connection( server, fp_out );

  while( 1 ) {
    ch = fgetc( fp_in );
    if (ch == -1) {
      break;
    }
    VASSERT( BF_put_uint8_t( &conn->in_buf, (uint8_t) ch ) == 0 );
    VASSERT( ! WEBBY_connection_data_received( conn ) );
  }
  
  fclose(fp_in);
  if (! connection_close ) {
    fclose(fp_out);
  }
  VASSERT( WEBBY_shutdown( server ) == 0 );

  sprintf(cmd,"diff %s %s", tmp_out, out_file_name);
  rt = system(cmd);
  VASSERT( WEXITSTATUS( rt ) ==  0 );

}

void WEBBY_test()
{
  WEBBY_one_test("data/chunk.test", "data/chunk.result", 1, header_echo_servlet_action);

  WEBBY_one_test("data/contentlen.test", "data/contentlen.result", 1, fixed_response_servlet_action);

  WEBBY_one_test("data/contentlen-100.test", "data/contentlen-100.result", 1, fixed_response_servlet_action);


}

