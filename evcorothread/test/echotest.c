#include <evcorothread/evthread.h>
#include <corothread/stacks.h>
#include <nutils/ioutils.h>

#include <sys/time.h>

#include <nutils/sock.h>
#include <pthread.h>
#include <string.h>

#include "vtest/vtest.h"



#define BF_SIZE 1024
#define MSG_SIZE 1024
#define TEST_NUM 30
#define TEST_PORT 8101

static struct timeval TIME_OUT = { 3 , 0 };

// ------------------------------------------------------------------------------------
void echo_thread( EVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx)
{
  char *buffer;
  int rt;

  M_UNUSED(thread);
  M_UNUSED(user_ctx);

  buffer = malloc( BF_SIZE);
  if (!buffer) {
    // don't worry, the socket will be closed by the library here.
    return;
  }

  while ( (rt = EVSOCKET_recv( socket, buffer, BF_SIZE, 0, TIME_OUT)) > 0) {
    if ( EVSOCKET_send( socket, buffer, rt, 0, TIME_OUT ) != rt ) {
      break;
    }
  }

  free(buffer);
}

int echo_thread_factory (int fd, EVTHREAD_PROC *proc, void **ctx )
{
  M_UNUSED(fd);

  *proc = echo_thread;
  *ctx = 0;

  return 0;
}

static  EVLOOP *loop;  
 
// ------------------------------------------------------------------------------------
static void *test_thread(void * arg)
{
  int *status = (int *) arg;
  SOCKCTX ctx;
  char msg[ MSG_SIZE ],msg_in[ MSG_SIZE ];
  int i;
  IPADDRESS addr;
  SOCKADDR saddr;
  
  for(i = 0; i< MSG_SIZE;i++)  {
    msg[i] = 'a' + i % 24; 
  }
  IPADDRESS_loopback( &addr, AF_INET );

  SOCKADDR_init( &saddr, &addr, TEST_PORT );

  for(i =0; i<TEST_NUM;i++) {
    if ( SOCK_init( &ctx, 0, 0 ) ) {
      goto err;
    }
    if (SOCK_connect( &ctx, SOCKADDR_saddr( &saddr ), SOCKADDR_length( &saddr ) , 3 )) {
      goto err;
    }
    if (SOCK_send( &ctx, msg, sizeof(msg ), 3 ) != sizeof( msg ) ) {
      goto err;
    }
    if (SOCK_recv_all( &ctx, msg_in, sizeof(msg), 3 ) != sizeof( msg ) ) {
      goto err;
    }
    if (memcmp( msg, msg_in, sizeof(msg) ) != 0) {
      goto err;
    }
    SOCK_close( &ctx );
  } 
  EVLOOP_break( loop );
  return 0;
 
err:
  SOCK_close( &ctx );
  EVLOOP_break( loop );
  *status = -1; 
  return 0;
}

void start_test_client(int *status)
{
  pthread_t pth;
  pthread_attr_t attr;
   
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  
  pthread_create(&pth, &attr, test_thread, status);
}

// ------------------------------------------------------------------------------------
void EVTHREAD_echo_server_test()
{
  STACKS stacks;
  EVTCPACCEPTOR *acceptor;
  IPADDRESS addr;
  SOCKADDR saddr;
  int listener;
  int status = 0;
 
  // ** init thread library and init the loop
  VASSERT( ! CTHREAD_libinit() );
  
  VASSERT( ! STACKS_init( &stacks, 100 , 5 ) );

  VASSERT( loop = EVLOOP_init( &stacks ) );

  // ** init the listener 
  VASSERT( ! IPADDRESS_loopback( &addr, AF_INET ) );

  VASSERT( ! SOCKADDR_init( &saddr, &addr, TEST_PORT  ) );

  VASSERT( (listener = fd_make_tcp_listener( &saddr, 30) ) != -1 );

  acceptor = EVTCPACCEPTOR_init( loop, listener, echo_thread_factory );

  start_test_client(&status);

  // ** run event loop
  EVLOOP_run( loop );

  // ** cleanup stacks.
  VASSERT( ! STACKS_destroy( &stacks ) ); 

  VASSERT( status == 0 );
}
