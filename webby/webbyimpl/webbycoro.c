#include <webby/webbyimpl.h>
#include <webby/webby.h>
#include <evcorothread/evthread.h>
#include <butils/logg.h>


//------------------------------------------------------------
typedef struct tagWEBBY_IMPL {
  WEBBY *server;
  WEBBY_CONFIG *config;
  STACKS stacks;
  EVTCPACCEPTOR *acceptor;
  EVLOOP *loop;

} WEBBY_IMPL;

typedef struct tagWEBBY_CONNECTION_IMPL {
  EVSOCKET *socket;
  WEBBY_CONNECTION *connection;
  struct timeval io_timeout;

} WEBBY_CONNECTION_IMPL;

static void webby_connection_thread( EVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx)
{
  WEBBY_CONNECTION_IMPL *wimpl = (WEBBY_CONNECTION_IMPL *) user_ctx; 
  BF *bf;
  int rt;

  M_UNUSED( thread );

  wimpl->socket = socket;
  bf = &wimpl->connection->in_buf;

  while( 1 ) {

    rt = EVSOCKET_recv( wimpl->socket, bf->get_pos, BF_put_size( bf ), 0, wimpl->io_timeout );
    if (rt < 0) {
      goto ret;
    }

    if (rt == 0) {
      goto ret;
    }

    bf->put_pos = bf->get_pos + rt;

    if ( WEBBY_connection_data_received( wimpl->connection ) ) {
      goto ret;
    }
  }
ret:
  if (wimpl->socket) {
    EVSOCKET_close( wimpl->socket );
  }
  WEBBY_connection_close(wimpl->connection);
  free(wimpl);
}

static int webby_thread_factory (int fd, EVTHREAD_PROC *proc, void **ctx, void *factory_ctx )
{
  WEBBY_IMPL *wimpl = (WEBBY_IMPL *) factory_ctx;
  WEBBY_CONNECTION_IMPL *conn;
  M_UNUSED(fd);
 
  conn = ( WEBBY_CONNECTION_IMPL * ) malloc( sizeof( WEBBY_CONNECTION_IMPL ) );
  if (!conn) {
    return -1;
  }
  conn->socket = 0;
  conn->connection = WEBBY_new_connection( wimpl->server, conn );
  conn->io_timeout.tv_sec = wimpl->config->io_timeout;
  conn->io_timeout.tv_usec = 0;
  
  *ctx = (void *) conn;
  *proc = webby_connection_thread;
 
  return 0;
}

int WEBBY_impl_new( WEBBY *srv, WEBBY_CONFIG *cfg, void **ret )
{
  WEBBY_IMPL *impl;

  impl = (WEBBY_IMPL *)  malloc( sizeof( WEBBY_IMPL ) );
  if (! impl ) {
    return -1;
  }
  *ret = (void *) impl;
  
  if (CTHREAD_libinit() ) {
    return -1;
  }

  if (STACKS_init( &impl->stacks, cfg->max_connections , cfg->stack_pages_per_thread )) {
    return -1;
  }

  impl->loop = EVLOOP_init( &impl->stacks );
  if (!impl->loop) {
    return -1;
  }

  impl->acceptor = 
	EVTCPACCEPTOR_init_ex( impl->loop, &cfg->listening_address, cfg->listen_backlog, webby_thread_factory, cfg->socket_buffer_size, cfg->socket_buffer_size, impl );
  if (!impl->acceptor) {
    return -1;
  }
  impl->config = cfg;
  impl->server = srv;
  
  return 0;
}

int WEBBY_impl_run( void *impl )
{
  WEBBY_IMPL *wimpl = ( WEBBY_IMPL * ) impl;
  
  MLOG_INFO( "WEBBY the web server is running" );

  return EVLOOP_run( wimpl->loop );
}

int WEBBY_impl_shutdown( void *impl )
{
  WEBBY_IMPL *wimpl = ( WEBBY_IMPL * ) impl;
  return EVLOOP_break( wimpl->loop );
}

int WEBBY_impl_send_data( void *impl, void *data, size_t size )
{
  WEBBY_CONNECTION_IMPL *impl_conn = (WEBBY_CONNECTION_IMPL *) impl;
  int rt;
  
  rt = EVSOCKET_send( impl_conn->socket, data, size, 0, impl_conn->io_timeout );
  return rt != (int) size;
}


int WEBBY_impl_response_completed( void *impl, int connection_close )
{
  WEBBY_CONNECTION_IMPL *impl_conn = (WEBBY_CONNECTION_IMPL *) impl;

  if (connection_close) { 
    EVSOCKET_close( impl_conn->socket );
    impl_conn->socket  = 0; 
  }
  return 0;
}

//------------------------------------------------------------

