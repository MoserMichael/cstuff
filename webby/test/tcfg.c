#include "vtest/vtest.h"
#include <webby/webby.h>

void CFG_test()
{
  WEBBY_CONFIG cfg;
  char *tmp;

  VASSERT( WEBBY_CONFIG_load( &cfg, "data/tconfig.props" ) == 0 );
  
  tmp = SOCKADDR_to_string(  & cfg.listening_address ); 
  VASSERT( strcmp( tmp, "0.0.0.0:12321" ) == 0 );
  VASSERT( cfg.listen_backlog == 30 );
  VASSERT( cfg.socket_buffer_size == 8192 );
  VASSERT( cfg.max_connections == 300 );
  VASSERT( cfg.io_timeout == 30 );
  VASSERT( cfg.idle_timeout == 300 );
  VASSERT( cfg.stack_pages_per_thread == 8 );
 
}
