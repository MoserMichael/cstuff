#include <evcorothread/evthread.h>
#include <corothread/stacks.h>
#include <nutils/ioutils.h>

#include <sys/time.h>
#include <unistd.h>

#include <nutils/sock.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <butils/logg.h>

#include "vtest/vtest.h"



#define BF_SIZE 1024
#define MSG_SIZE 1024
#define TEST_NUM 30
#define TEST_PORT 8101

static struct timeval TIME_OUT = { 3 , 0 };

// ------------------------------------------------------------------------------------
static void echo_thread( EVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx)
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

static int echo_thread_factory (int fd, EVTHREAD_PROC *proc, void **ctx, void *factory_ctx )
{
  M_UNUSED(fd);
  M_UNUSED(factory_ctx);

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
   
 
  sleep(1);

  for(i = 0; i< MSG_SIZE;i++)  {
    msg[i] = 'a' + i % 24; 
  }
  IPADDRESS_loopback( &addr, AF_INET );

  SOCKADDR_init( &saddr, &addr, TEST_PORT );

  for(i =0; i<TEST_NUM;i++) {
    printf(" %d ", i );
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
  *status = 0;
 
  return 0;
 
err:
  SOCK_close( &ctx );
  *status = -1; 
  EVLOOP_break( loop );
  return 0;
}

static int start_test_client(int *status)
{
  pthread_t pth;
  pthread_attr_t attr;
   
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  
  return pthread_create(&pth, &attr, test_thread, status);
}

#if 0 
void save_mmaps()
{
  char cmd[100];

  sprintf(cmd,"cat /proc/%d/maps >maps", getpid());
  system(cmd);
}
#endif

// ------------------------------------------------------------------------------------
void EVTHREAD_echo_server_test()
    {
  STACKS stacks;
  EVTCPACCEPTOR *acceptor;
  IPADDRESS addr;
  SOCKADDR saddr;
  int status;

  MLOG_init( MLOG_LEVEL_TRACE, MLOG_ACTION_CONSOLE, 0);

  // ** init thread library and init the loop
  VASSERT( ! CTHREAD_libinit() );
  
  VASSERT( ! STACKS_init( &stacks, 100 , 5 ) );

#if 0
  save_mmaps();
#endif

  VASSERT( loop = EVLOOP_init( &stacks ) );

  // ** init the listener 
  VASSERT( ! IPADDRESS_loopback( &addr, AF_INET ) );

  VASSERT( ! SOCKADDR_init( &saddr, &addr, TEST_PORT  ) );

  VASSERT( acceptor = EVTCPACCEPTOR_init_ex( loop, &saddr, 30, echo_thread_factory, -1, -1, 0 ) );

  VASSERT( start_test_client(&status) == 0 );

  // ** run event loop
  EVLOOP_run( loop );

  EVTCPACCEPTOR_close( acceptor );
  
  // ** cleanup stacks.
  VASSERT( ! STACKS_destroy( &stacks ) ); 

  VASSERT( status == 0 );
}
