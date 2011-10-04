#ifndef __EVTHREAD_H_
#define __EVTHREAD_H_

#include <corothread/cthread.h>
#include <event.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <cutils/dlist.h>

// ---------------------------------------------------------------------------

/**
 * @defgroup EVLOOP
 * @brief event loop object.
 * The event loop  wraps a small user mode threading library where a user mode thread is created per connection.
 *
 * When a new connection is received via accept(2), then an EVSOCKET object is created that encapsulates this socket, 
 * and the socket is set to non blocking mode, and a user mode thread EVTHREAD is created that owns the new EVSOCKET object.
 * TCPACCEPTOR is the class that does all that.
 *
 * A user mode thread can own one or more EVSOCKET objects, it can read or write to an EVSOCKET object which has the API of a blocking socket.
 * when the socket blocks, (i.e. recv or send returns -1 and errno == EWOULDBLOCK) then the attached user mode thread is suspended, and control
 * returns to the event loop, the event loop schedules a different user mode thread, when an IO event occurs on a socket owned by that thread.
 *
 * A user mode thread (EVTHREAD) can create other sockets (EVSOCKET) that are then attached to the current thread.
 *
 * A use mode thread also owns a set of timer objects (EVTIMER), when the thread exits, all the timer objects are cleaned up.
 *
 * The event loop is implemented by libevent.
 *
 * Unlike other packages, where one can create several user mode thread and then has to cope with the problem of their synchronization, we keep things simple here.
 * There is one thread per connection, that is supposed to do protocol handling and processing, all on condition that processing does not involve heavy CPU intensive tasks.
 * If your problem does have such task, then there is also support for the Half-sync/Half-async pattern.
 * 
 * @{
 */
typedef struct tagEVLOOP {
  struct event_base *ev_base;
  STACKS *stacks;
} EVLOOP;

EVLOOP * EVLOOP_init(STACKS *stacks );

int EVLOOP_run( EVLOOP *loop );

int EVLOOP_break( EVLOOP *loop );

/**
 * @}
 */
 
// ---------------------------------------------------------------------------
struct tagEVSOCKET;
struct tagEVTHREAD;

/**
 * @defgroup EVTHREAD
 * @brief user mode thread attached to an event loop
 * The thread owns one or more EVSOCKETS, it initially activated when the thread is created, and is futher activated when an IO event occurs on a EVSOCKET object 
 * owned by this thread.
 *
 * @{
 */

typedef void (*EVTHREAD_PROC) ( struct tagEVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx);

typedef struct tagEVTHREAD {
  EVLOOP *loop;
  EVTHREAD_PROC thread_proc;
  void *user_context;
  CTHREAD *cthread;
  struct tagEVSOCKET *socket; 
  struct event timer_event;
  DLIST   object_list;
} EVTHREAD;

EVTHREAD *EVTHREAD_init(EVLOOP *loop, EVTHREAD_PROC thread_proc,  void *user_ctx);


int EVTHREAD_start( EVTHREAD *thread, struct tagEVSOCKET *socket );

int EVTHREAD_delay( EVTHREAD *thread, struct timeval delay );

#if 0
int EVTHREAD_dns_lookup ( EVTHREAD *thread, int  addr_family, const char *dns_name );

int EVTHREAD_dns_lookup ( EVTHREAD *thread, int  addr_family, const char *dns_name )
{
  CTHREAD *thread;

  switch( addr_family ) {
    case AF_INET:
      evdns_resolve_ipv6( dns_name, 0, evdns_callback_type callback, void *ptr);
      break;
    case AF_INET6:
      evdns_resolve_ipv6( dns_name, 0, evdns_callback_type callback, void *ptr);
      break;
  }

  CHREAD_yield();
}
#endif


/**
 * @}
 */

// ---------------------------------------------------------------------------

typedef enum { 
  EVTHREAD_OBJECT_SOCKET,
  EVTHREAD_OBJECT_TIMER,
} EVTHREAD_OBJECT_TYPE;

/**
 * @brief base class of sockets and timers. API of this class is not not called directly by the user of this library.
 */
typedef struct tagEVTHREAD_OBJECT {
  DLIST_entry entry;
  int object_type;
  EVTHREAD *owner;
} EVTHREAD_OBJECT;

// ---------------------------------------------------------------------------

/**
 * @defgroup EVTIMER
 * @brief timer object atached to event loop, a user mode thread is created when the event loop dispatches a timer event.
 * @{
 */
struct tagEVTIMER;

typedef void (*EVTIMER_PROC) (struct tagEVTIMER *timer, void *user_data);

typedef  enum {
  EVTIMER_STATE_INIT,
  EVTIMER_STATE_SCHEDULED,
  EVTIMER_STATE_HANDLER_RUNNING,
} EVTIMERSTATE;

typedef struct tagEVTIMER {
  EVTHREAD_OBJECT object_base;
  
  EVLOOP *loop;
 
  EVTIMER_PROC proc;
  void  *user_data;
  int    timer_id;

  struct event timer_event;
  struct timeval timer_period;
  int    is_recurrent;
 
  struct timeval tm;
  struct timeval next_due_date; 

  EVTIMERSTATE state;
} EVTIMER;

EVTIMER *EVTIMER_init_one_shot(EVTHREAD *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data);

EVTIMER *EVTIMER_init_recurrent(EVTHREAD *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data);

int EVTIMER_start( EVTIMER *ret);

int  EVTIMER_cancel( EVTIMER *timer );

int  EVTIMER_free( EVTIMER *timer );

/**
 * @}
 */

// ---------------------------------------------------------------------------
/**
 * @defgroup EVSOCKET
 * @brief a socket attached to user mode thread.
 *
 *  - a thread can either read from or write to a socket, not both at the same time.
 *  - a thread can read or write from one socket at a time.
 *
 * @{
 */

typedef enum {
  EVSOCKET_STATE_INIT,
  EVSOCKET_STATE_CONNECTING,
  EVSOCKET_STATE_CONNECTED,
  EVSOCKET_STATE_READING,
  EVSOCKET_STATE_WRITING,
  EVSOCKET_STATE_CLOSED,
  EVSOCKET_STATE_ERROR,
} EVSOCKET_STATE;


typedef struct tagEVSOCKET {
  EVTHREAD_OBJECT object_base;
  
  int fd;
  struct event read_event;
  struct event write_event;
  
  EVSOCKET_STATE state;
  EVTIMER  *timer_idle_timeout; // idle timeout.
  EVTIMER  *timer_io_timeout; // timeout of current operation (read or write).
  
  EVLOOP *loop;
  EVTHREAD *thread;
} 
  EVSOCKET;

EVSOCKET *EVSOCKET_init(EVTHREAD *thread, int fd, int is_connected);

int EVSOCKET_close(EVSOCKET *socket);

int EVSOCKET_connect( EVSOCKET *socket, struct sockaddr *address, socklen_t socklen, struct timeval timeout);

void EVSOCKET_set_idle_timeout(EVSOCKET *socket, struct timeval timeout );

int EVSOCKET_recv( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout );

int EVSOCKET_recv_all( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout );

int EVSOCKET_send( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout );

/**
 * @}
 */

// ---------------------------------------------------------------------------

/**
 * @defgroup TCPACCEPTOR
 * @brief listener for tcp connections, a usermode thread is created for each new connection.
 *
 * @{
 */

typedef int (*EVTHREAD_FACTORY) (int fd, EVTHREAD_PROC *proc, void **ctx );


typedef struct tagTCPACCEPTOR {
  EVLOOP *loop; 
  int fd;
  struct event read_event;
  EVTHREAD_FACTORY factory;

} EVTCPACCEPTOR;

EVTCPACCEPTOR * EVTCPACCEPTOR_init( EVLOOP *loop, int fd, EVTHREAD_FACTORY factory );

/**
 * @}
 */

#endif


