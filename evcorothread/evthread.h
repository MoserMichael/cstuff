#ifndef __EVTHREAD_H_
#define __EVTHREAD_H_

#include <corothread/cthread.h>
#include <event.h> // libevent
#include <sys/socket.h>
#include <sys/types.h>

// ---------------------------------------------------------------------------

/**
 * @brief event loop object.
 * The event loop wraps a small user mode threading library where a user mode thread is created per connection.
 */
typedef struct tagEVLOOP {
  struct event_base *ev_base;
  STACKS *stacks;
} EVLOOP;

EVLOOP * EVLOOP_init(STACKS *stacks );

int EVLOOP_run( EVLOOP *loop );

// ---------------------------------------------------------------------------
struct tagEVSOCKET;

typedef void (*EVTHREAD_PROC) ( struct tagEVSOCKET *socket, void *user_ctx);

/**
 * @brief user mode thread attached to an event loop
 */
typedef struct tagEVTHREAD {
  EVLOOP *loop;
  EVTHREAD_PROC thread_proc;
  CTHREAD *cthread;
  void *user_context;
  struct tagEVSOCKET *socket; 
  DLIST   timer_list;
  DLIST   socket_list;
} EVTHREAD;

EVTHREAD *EVTHREAD_init(EVLOOP *loop, EVTHREAD_PROC thread_proc,  void *user_ctx);

int EVTHREAD_start( EVTHREAD *thread, struct tagEVSOCKET *socket );

// ---------------------------------------------------------------------------
struct tagEVTIMER;

typedef void (*EVTIMER_PROC) (struct tagEVTIMER *timer, void *user_data);

typedef  enum {
  EVTIMER_STATE_INIT,
  EVTIMER_STATE_SCHEDULED,
  EVTIMER_STATE_HANDLER_RUNNING,
} EVTIMERSTATE;

/**
 * @brief timer object atached to event loop, a user mode thread is created per timer invocation.
 */
typedef struct tagEVTIMER {
  EVLOOP *loop;

  struct event timer_event;
  struct timeval timer_period;
  int    is_recurrent;
 
  EVTIMER_PROC proc;
  void *user_data;

  int timer_id;
  struct timeval tm;
  struct timeval next_due_date; 
  CTHREAD *thread;

  EVTIMERSTATE state;
} EVTIMER;

EVTIMER *EVTIMER_init_one_shot(EVLOOP *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data);

EVTIMER *EVTIMER_init_recurrent(EVLOOP *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data);

int EVTIMER_start( EVTIMER *ret);

int  EVTIMER_cancel( EVTIMER *timer );

// ---------------------------------------------------------------------------
typedef enum {
  EVSOCKET_STATE_INIT,
  EVSOCKET_STATE_CONNECTING,
  EVSOCKET_STATE_CONNECTED,
  EVSOCKET_STATE_READING,
  EVSOCKET_STATE_WRITING,
  EVSOCKET_STATE_CLOSED,
  EVSOCKET_STATE_ERROR,
} EVSOCKET_STATE;

/**
 * @brief a socket attached to user mode thread.
 * Here a thread can either read from or write to a socket, not both at the same time.
 */
typedef struct tagEVSOCKET {
  int fd;
  struct event socket_event;
  
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
int EVSOCKET_read( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout );
int EVSOCKET_write( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout );

// ---------------------------------------------------------------------------

typedef int (*EVTHREAD_FACTORY) (int fd, EVTHREAD_PROC *proc, void **ctx );

/**
 * @brief tcp listener
 */
typedef struct tagTCPACCEPTOR {
  EVLOOP *loop; 
  int fd;
  struct event read_event;
  EVTHREAD_FACTORY factory;

} EVTCPACCEPTOR;

EVTCPACCEPTOR * EVTCPACCEPTOR_init( EVLOOP *loop, int fd, EVTHREAD_FACTORY factory );

#endif


