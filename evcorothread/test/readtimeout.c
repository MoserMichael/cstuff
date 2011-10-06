#include <evcorothread/evthread.h>
#include <corothread/stacks.h>
#include <nutils/ioutils.h>

#include <sys/time.h>
#include <unistd.h>

#include <nutils/sock.h>
#include <pthread.h>
#include <string.h>

#include "vtest/vtest.h"



#define BF_SIZE 1024
#define MSG_SIZE 1024
#define TEST_NUM 30
#define TEST_PORT 8101

static struct timeval TIME_OUT = { 1 , 0 };

static int status;
static  EVLOOP *loop;  
 

// ------------------------------------------------------------------------------------
static void echo_thread( EVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx)
{
  char *buffer;

  M_UNUSED(thread);
  M_UNUSED(user_ctx);

  buffer = malloc( BF_SIZE);
  if (!buffer) {
    // don't worry, the socket will be closed by the library here.
    return;
  }

  status = EVSOCKET_recv( socket, buffer, BF_SIZE, 0, TIME_OUT );
  EVLOOP_break( loop );
  
  
  free(buffer);
}

static int echo_thread_factory (int fd, EVTHREAD_PROC *proc, void **ctx )
{
  M_UNUSED(fd);

  *proc = echo_thread;
  *ctx = 0;

  return 0;
}

// ------------------------------------------------------------------------------------
static void *test_thread(void * arg)
{
  SOCKCTX ctx;
  IPADDRESS addr;
  SOCKADDR saddr;

  M_UNUSED( arg );
  
  IPADDRESS_loopback( &addr, AF_INET );
  SOCKADDR_init( &saddr, &addr, TEST_PORT );

  if ( SOCK_init( &ctx, 0, 0 ) ) {
    goto err;
  }
  if (SOCK_connect( &ctx, SOCKADDR_saddr( &saddr ), SOCKADDR_length( &saddr ) , 3 )) {
    goto err;
  }
  sleep(10000);
  SOCK_close( &ctx );
   
  return 0;
 
err:
  SOCK_close( &ctx );
  return 0;
}

static void start_test_client()
{
  pthread_t pth;
  pthread_attr_t attr;
   
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  
  pthread_create(&pth, &attr, test_thread, 0);
}

// ------------------------------------------------------------------------------------
void EVTHREAD_read_timeout_test()
{
  STACKS stacks;
  EVTCPACCEPTOR *acceptor;
  IPADDRESS addr;
  SOCKADDR saddr;
  int listener;
 
  // ** init thread library and init the loop
  VASSERT( ! CTHREAD_libinit() );
  
  VASSERT( ! STACKS_init( &stacks, 100 , 5 ) );

  VASSERT( loop = EVLOOP_init( &stacks ) );

  // ** init the listener 
  VASSERT( ! IPADDRESS_loopback( &addr, AF_INET ) );

  VASSERT( ! SOCKADDR_init( &saddr, &addr, TEST_PORT  ) );

  VASSERT( (listener = fd_make_tcp_listener( &saddr, 30) ) != -1 );

  acceptor = EVTCPACCEPTOR_init( loop, listener, echo_thread_factory );

  start_test_client();

  // ** run event loop
  EVLOOP_run( loop );

  close( listener );

  // ** cleanup stacks.
  VASSERT( ! STACKS_destroy( &stacks ) ); 

  VASSERT( status == -1 );
}
