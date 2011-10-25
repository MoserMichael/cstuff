#include <webby/webby.h>
#include <stdio.h>

static SERVLET_STATUS hello_world_servlet_action( HTTP_servlet_request * request, HTTP_servlet_response *response, SERVLET_CONTEXT *context )
{ 
   URI *uri;
   
   M_UNUSED(request);
   M_UNUSED(context);
 
   uri = HTTP_servlet_uri( request );
   if (strcmp( uri->path, "/helloworldservlet" ) != 0 ) {
     return SERVLET_REQUEST_IGNORED;
   }


   HTTP_response_start( response, 200, "text/plain", RESPONSE_CONTENT_LENGTH, 10 );
   if (HTTP_response_send( response, "Hello World", 11 ) ) {
     return SERVLET_REQUEST_ERROR;    
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

  HTTP_SERVLET_init( ret, 0,  0, hello_world_servlet_action , 0 );
  return ret;
}
