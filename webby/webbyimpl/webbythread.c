#include <webby/webbyimpl.h>
#include <webby/webby.h>
#include <tutils/tpool.h>
#include <nutils/sock.h>
#include <nutils/ioutils.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <butils/logg.h>

//------------------------------------------------------------
typedef struct tagWEBBY_IMPL {
  WEBBY *server;
  WEBBY_CONFIG *config;
  THREADPOOL *thread_pool;
  int acceptor_socket;
  pthread_t acceptor_thread;
} WEBBY_IMPL;

typedef struct tagWEBBY_CONNECTION_IMPL { 
  RUNNABLE base;
  WEBBY_CONNECTION *connection;
  SOCKCTX socket;
  int  io_timeout;
  int  idle_timeout;
  WEBBY_IMPL *server_impl;

} WEBBY_CONNECTION_IMPL;

 
void run_request( WEBBY_CONNECTION_IMPL *wimpl ){
  BF *bf;
  int rt;

  bf = &wimpl->connection->in_buf;

  while( 1 ) {

    rt = SOCK_wait_for_read_event( &wimpl->socket, wimpl->idle_timeout );
    if (rt  != 0) {
      goto ret;
    }

    rt = SOCK_recv( &wimpl->socket, bf->get_pos, BF_put_size( bf ), wimpl->io_timeout );
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
  SOCK_close( &wimpl->socket );
  WEBBY_connection_close(wimpl->connection);
}

void free_request( WEBBY_CONNECTION_IMPL *conn )
{
  free( conn );
}

void  *acceptor_thread(void * arg)
{
  WEBBY_IMPL *wimpl = (WEBBY_IMPL *) arg;
  WEBBY_CONNECTION_IMPL *conn;
  int sock;

  while( 1 ) {
    sock = accept( wimpl->acceptor_socket, 0, 0 );
    
    conn = (WEBBY_CONNECTION_IMPL *) malloc( sizeof(WEBBY_CONNECTION_IMPL) );
    if (!conn) {
      close(sock);
      break;
    }
    conn->server_impl = wimpl;   

    if (SOCK_attach( &conn->socket, sock, 0,  0 )) {
      close(sock);
      continue;
    }

    conn->connection = WEBBY_new_connection( wimpl->server, conn );
    conn->io_timeout = wimpl->config->io_timeout;
    conn->idle_timeout = wimpl->config->idle_timeout;

    RUNNABLE_init( &conn->base, (RUNNABLE_HANDLER) run_request,  (RUNNABLE_HANDLER) free_request );

    if (THREADPOOL_send_fail_on_queue_full( wimpl->thread_pool, &conn->base ) ) {
      close( sock );
      continue;
    }
  }
  close( wimpl->acceptor_socket );
  return 0;
}

int WEBBY_impl_send_data( void *impl, void *data, size_t size )
{
  WEBBY_CONNECTION_IMPL *impl_conn = (WEBBY_CONNECTION_IMPL *) impl;
  return SOCK_send( &impl_conn->socket, data, size, impl_conn->io_timeout ) != (int) size;
}

int WEBBY_impl_response_completed( void *impl, int connection_close )
{
  WEBBY_CONNECTION_IMPL *impl_conn = (WEBBY_CONNECTION_IMPL *) impl;
  if (connection_close) { 
    SOCK_close( &impl_conn->socket );
  }
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
  
  impl->acceptor_socket = fd_make_tcp_listener( &cfg->listening_address, cfg->listen_backlog );
  if (impl->acceptor_socket == -1) {
    return -1;
  }

  impl->config = cfg;
  impl->server = srv;
  
  impl->thread_pool = 0;   

  return 0;
}

int WEBBY_impl_run( void *impl )
{
  pthread_attr_t attr;
  int page_size;
  WEBBY_IMPL *wimpl = ( WEBBY_IMPL * ) impl;
  
  page_size = sysconf( _SC_PAGE_SIZE );
  wimpl->thread_pool = THREADPOOL_init( 0, 10, wimpl->config->max_connections, wimpl->config->stack_pages_per_thread * page_size / 1024 );

  pthread_attr_init( &attr );
//pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  pthread_attr_setstacksize( &attr, 10 * 1024 );

  if (pthread_create( &wimpl->acceptor_thread, &attr, acceptor_thread, wimpl ) ) {
    return -1;
  }
  MLOG_INFO( "WEBBY the web server is running" );
  pthread_join( wimpl->acceptor_thread, 0 );
  return 0;
}

int WEBBY_impl_shutdown( void *impl )
{
  WEBBY_IMPL *wimpl = ( WEBBY_IMPL * ) impl;
  
  THREADPOOL_close( wimpl->thread_pool );

  return 0;
}

WEBBY *WEBBY_init( WEBBY_CONFIG * cfg )
{
  WEBBY_impl_interface  iface;

  iface.impl_new = WEBBY_impl_new;
  iface.impl_shutdown =  WEBBY_impl_shutdown;
  iface.impl_run = WEBBY_impl_run;
  iface.impl_send_data =  WEBBY_impl_send_data  ;
  iface.impl_response_completed  = WEBBY_impl_response_completed; 

  return WEBBY_init_internal( cfg, &iface );
}

//------------------------------------------------------------

