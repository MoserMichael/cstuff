#include <evthread.h>

#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <nutils/ioutils.h>

// ---------------------------------------------------------------------------

EVLOOP * EVLOOP_init(STACKS *stacks )
{
  EVLOOP *loop;

  loop = (EVLOOP *)  malloc( sizeof( EVLOOP ) );
  if (!loop) {
    return 0;
  }

  if (!stacks) {
    return 0;
  }
  loop->stacks = stacks;
  
  loop->ev_base = event_init();
  if (!loop->ev_base) {
    return 0;
  }

  return loop;
}

int EVLOOP_run( EVLOOP *loop )
{
  return event_base_dispatch( loop->ev_base );
}

// ---------------------------------------------------------------------------

void evthread_proc( void *ctx )
{
  EVTHREAD *thread = (EVTHREAD *) ctx;
  thread->thread_proc( thread->socket, thread->user_context );

  // on exit - clean up all sockets and timers attached to this thread.

  // no way to get exit status of the EVTHREAD object.
  //EVTHREAD_free(thread);
}

EVTHREAD *EVTHREAD_init(EVLOOP *loop, EVTHREAD_PROC thread_proc,  void *user_ctx)
{
  EVTHREAD *thread;

  thread = (EVTHREAD *) malloc( sizeof( EVTHREAD ) );
  if (!thread) {
    return 0;
  }
  thread->loop = loop;
  thread->thread_proc = thread_proc;
  thread->user_context = user_ctx;

  thread->cthread = CTHREAD_init( loop->stacks, evthread_proc, user_ctx );
  if (!thread->cthread) {
    free(thread);
    return 0;
  }
  thread->socket = 0;

  return thread;
}

int EVTHREAD_start( EVTHREAD *thread, struct tagEVSOCKET *socket )
{
  thread->socket = socket;
  CTHREAD_start( thread->cthread );
  return 0;
}

// ---------------------------------------------------------------------------
EVTIMER *ev_mktimer( EVLOOP *loop, int timer_id, int is_recurrent, struct timeval tm, EVTIMER_PROC proc, void *user_data);


EVTIMER *EVTIMER_init_one_shot(EVLOOP *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)
{
  return ev_mktimer( loop, timer_id, 1, tm, proc, user_data );
}

EVTIMER *EVTIMER_init_recurrent(EVLOOP *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)
{
  return ev_mktimer( loop, timer_id, 0, tm, proc, user_data );
}

static void timer_cb( int fd, short event, void *ctx);

int EVTIMER_start( EVTIMER *ret)
{
  struct timeval now;

  if (ret->state != EVTIMER_STATE_INIT) {
    return -1;
  }

  ret->state = EVTIMER_STATE_SCHEDULED;
  event_set( &ret->timer_event, -1, EV_READ , timer_cb, (void *) ret );
  event_base_set( ret->loop->ev_base, &ret->timer_event );
  event_add( &ret->timer_event, &ret->tm );

  if (ret->is_recurrent) {
    gettimeofday( &now, 0 );
    timeradd( &now, &ret->tm, &ret->next_due_date );
  }
  return 0;
}

int  EVTIMER_cancel( EVTIMER *timer )
{
  if (timer->state == EVTIMER_STATE_INIT) {
    return -1;
  }
  event_del( &timer->timer_event );
   
  timer->state = EVTIMER_STATE_INIT;
  return 0;
}

void  EVTIMER_free( EVTIMER *timer )
{
  EVTIMER_cancel(timer);
  free(timer);
}

EVTIMER *ev_mktimer( EVLOOP *loop, int timer_id, int is_recurrent, struct timeval tm, EVTIMER_PROC proc, void *user_data)
{
  EVTIMER *ret;
 
  ret = (EVTIMER *) malloc( sizeof( EVTIMER ) );
  if (!ret) {
    return 0;
  }

  ret->loop = loop;
  ret->is_recurrent = is_recurrent;
  ret->proc = proc;
  ret->user_data = user_data;
  ret->timer_id = timer_id;
  ret->tm = tm;
  ret->state = EVTIMER_STATE_INIT;
  ret->thread = 0;

  EVTIMER_start( ret ); 
  return ret;
}

static void  timer_thread_proc(void *arg)
{
  EVTIMER *timer;

  timer = (EVTIMER *) arg;
  timer->proc( timer, timer->user_data );
//EVTHREAD_free( timer->thread );
}


static void timer_cb( int fd, short event, void *ctx)
{
   struct timeval now;
   EVTIMER *timer;
   CTHREAD *thread;

   M_UNUSED(fd);
   M_UNUSED(event);

   timer = (EVTIMER *) ctx;

 
   event_del( &timer->timer_event );
   
   thread = CTHREAD_init( timer->loop->stacks , timer_thread_proc , timer );
   if (thread) {
     CTHREAD_start( thread );
   }
   timer->thread = thread;
   
   if (timer->state == EVTIMER_STATE_INIT) {
     return;
   }

   if (timer->is_recurrent) {
     
     gettimeofday( &now, 0 );
     while( timercmp( &now, &timer->next_due_date, >  ) ) {
       timeradd( &timer->next_due_date, &timer->tm, &timer->next_due_date );
     }
     timersub( &timer->next_due_date, &now, &timer->tm ); 
     event_add( &timer->timer_event, &timer->tm );

     timer->state = EVTIMER_STATE_SCHEDULED;
   } else {
     timer->state = EVTIMER_STATE_INIT;
   }
}

// ---------------------------------------------------------------------------
static void socket_cb( int fd, short event, void *ctx);

EVSOCKET *EVSOCKET_init(EVTHREAD *thread, int fd, int is_connected)
{
  EVSOCKET *socket;

  socket = (EVSOCKET *) malloc( sizeof( EVSOCKET ) );
  if (!socket) {
    return 0;
  }

  socket->fd = fd;
  socket->timer_idle_timeout = 0;
  socket->timer_io_timeout = 0;
  socket->state = is_connected ? EVSOCKET_STATE_CONNECTED : EVSOCKET_STATE_INIT; 
  
  socket->thread = thread;
  socket->loop = thread->loop;
  
  fd_set_blocking( fd, 0 );

  event_set( &socket->socket_event, fd, 0 , socket_cb, (void *) socket );
  event_base_set( socket->loop->ev_base, &socket->socket_event );

  return socket;
}   


int EVSOCKET_close(EVSOCKET *socket)
{
  int rt;

  if (socket->timer_idle_timeout) {
    EVTIMER_free( socket->timer_idle_timeout );
  }
  if (socket->timer_io_timeout) {
    EVTIMER_free( socket->timer_io_timeout);  
  }

  if (socket->state == EVSOCKET_STATE_READING ||
      socket->state == EVSOCKET_STATE_CONNECTING || 
      socket->state == EVSOCKET_STATE_WRITING) {
    event_del( &socket->socket_event );   
  }
  do {
    rt = close(socket->fd);
  } while(rt == -1 && errno == EINTR);
  return rt;
}

static void socket_cb( int fd, short event, void *ctx)
{
   EVSOCKET *socket; 

   M_UNUSED( fd );
   
   socket = (EVSOCKET *) ctx;

   if (event & EV_READ && event & EV_WRITE) {
     socket->state = EVSOCKET_STATE_ERROR;
     CTHREAD_resume( socket->thread->cthread );
     return;
   }

   if (event & EV_READ) {
     if (socket->state == EVSOCKET_STATE_READING) {
        event_del( &socket->socket_event );
	socket->state = EVSOCKET_STATE_CONNECTED;
	CTHREAD_resume( socket->thread->cthread );
     }
   }
  
   if (event & EV_WRITE) {
     if (socket->state == EVSOCKET_STATE_CONNECTING || socket->state == EVSOCKET_STATE_WRITING) {
        event_del( &socket->socket_event );
	socket->state = EVSOCKET_STATE_CONNECTED;
	CTHREAD_resume( socket->thread->cthread );
     }
   }
    
}

// called when either connect/read/write has timed out.

void io_timeout_proc(EVTIMER *timer, void *user_data)
{
  EVSOCKET *socket;
 
  M_UNUSED(timer);

  socket = (EVSOCKET *) user_data;

  socket->state = EVSOCKET_STATE_ERROR;
  EVSOCKET_close( socket );
}

int EVSOCKET_connect( EVSOCKET *socket, struct sockaddr *address, socklen_t socklen, struct timeval timeout)
{
  int rt;

  if (socket->state != EVSOCKET_STATE_INIT) {
    return -1;
  }

  do {
    rt = connect( socket->fd, address, socklen );
  } while (rt == -1 && errno == EINTR);
 
  if (rt == 0) {
     socket->state = EVSOCKET_STATE_CONNECTED;
     return 0;
  }
  if (rt == -1 && errno == EINPROGRESS) {
 
     socket->state = EVSOCKET_STATE_CONNECTING;

     socket->timer_io_timeout = EVTIMER_init_one_shot( socket->loop, 1, timeout, io_timeout_proc, socket);

     socket->socket_event.ev_flags = EV_WRITE;
     event_add( &socket->socket_event, 0 );
     
     CTHREAD_yield();

     if (socket->timer_io_timeout) {
       EVTIMER_free( socket->timer_io_timeout ); 
       socket->timer_io_timeout = 0; 
     }

     if (socket->state == EVSOCKET_STATE_CONNECTED) {
       return 0;
     }
  } 
  return -1;
}
void EVSOCKET_set_idle_timeout(EVSOCKET *socket, struct timeval timeout )
{ 
  if (socket->timer_io_timeout != 0) {  
    EVTIMER_free( socket->timer_io_timeout ); 
  }
  socket->timer_io_timeout = EVTIMER_init_one_shot( socket->loop, 1, timeout, io_timeout_proc, socket);
}

int EVSOCKET_read( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout )
{
   int rt;

   if ( socket->state != EVSOCKET_STATE_CONNECTED) {
     return -1;
   }
   
r_again:   
   do {
    rt = recv( socket->fd, buf, buf_size, flags);
   } while(rt == -1 && errno == EINTR);

   if (rt == -1) {
     if (errno == EAGAIN) {

       socket->state = EVSOCKET_STATE_READING;

       socket->timer_io_timeout = EVTIMER_init_one_shot( socket->loop, 1, timeout, io_timeout_proc, socket);

       socket->socket_event.ev_flags = EV_READ;
       event_add( &socket->socket_event, 0 );
       
       CTHREAD_yield();
       
       if (socket->state == EVSOCKET_STATE_READING) {
         goto r_again;
       }
     }
   }

   if (socket->timer_io_timeout) {
     EVTIMER_free( socket->timer_io_timeout ); 
     socket->timer_io_timeout = 0; 
   }
   if  (socket->timer_io_timeout) {
     EVTIMER_free( socket->timer_io_timeout);
     socket->timer_io_timeout = 0;
   }
   return rt;
} 

int EVSOCKET_read_all( EVSOCKET *socket, void *buf, size_t buf_size, struct timeval read_timeout );

int EVSOCKET_write( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout )
{
   int rt;

   if ( socket->state != EVSOCKET_STATE_CONNECTED) {
     return -1;
   }
   
w_again:   
   do {
    rt = send( socket->fd, buf, buf_size, flags);
   } while(rt == -1 && errno == EINTR);

   if (rt == -1) {
     if (errno == EAGAIN) {

       socket->state = EVSOCKET_STATE_READING;

       socket->timer_io_timeout = EVTIMER_init_one_shot( socket->loop, 1, timeout, io_timeout_proc, socket);

       socket->socket_event.ev_flags = EV_READ;
       event_add( &socket->socket_event, 0 );
       
       CTHREAD_yield();
       
       if (socket->state == EVSOCKET_STATE_READING) {
         goto w_again;
       }
     }
   }

   if (socket->timer_io_timeout) {
     EVTIMER_free( socket->timer_io_timeout ); 
     socket->timer_io_timeout = 0; 
   }

   if (socket->timer_io_timeout) {
     EVTIMER_free( socket->timer_io_timeout ); 
     socket->timer_io_timeout = 0; 
   }
    
   return rt;
} 

// ---------------------------------------------------------------------------

static void socket_listener_cb( int fd, short event, void *ctx);

EVTCPACCEPTOR * EVTCPACCEPTOR_init( EVLOOP *loop, int fd, EVTHREAD_FACTORY factory )
{
  EVTCPACCEPTOR *acceptor;

  acceptor = (EVTCPACCEPTOR *) malloc( sizeof( EVTCPACCEPTOR ) );
  if (!acceptor) {
    return 0;
  }

  acceptor->loop = loop;
  acceptor->factory = factory;

  event_set( &acceptor->read_event, fd, EV_READ | EV_PERSIST, socket_listener_cb, (void *) acceptor );
  event_base_set( loop->ev_base, &acceptor->read_event );
  event_add( &acceptor->read_event, 0 );

  return acceptor;
}

static void socket_listener_cb( int fd, short event, void *ctx)
{
   EVTCPACCEPTOR *acceptor;
   EVTHREAD_PROC thread_proc;
   void *thread_ctx;
   EVTHREAD *thread; 
   EVSOCKET *socket;

   if (event & EV_READ && event && EV_WRITE) {
      return;
   }

   acceptor = (EVTCPACCEPTOR *) ctx;
 
   do {
     fd = accept( acceptor->fd, 0, 0 );
   } while( fd == -1 && errno == EINTR);

   if (fd == -1) {
     return;
   }
  
   // get thread procedure and thread argument data.
   acceptor = (EVTCPACCEPTOR *) ctx;
   acceptor->factory( fd, &thread_proc, &thread_ctx );


   // create the user thread.
   thread =EVTHREAD_init( acceptor->loop, thread_proc, thread_ctx);
   if (!thread) {
     close(fd);
     return;
   }

   socket = EVSOCKET_init(thread, fd, 1);
   if (!socket) {
     //close thread)
     close(fd);
     return;
   }

   EVTHREAD_start( thread, socket );

}

void custom_timeout_handling( struct event_base *base, struct timeval *tv )
{
   M_UNUSED(base);
   M_UNUSED(tv);
}
