#include <evthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <nutils/ioutils.h>

// ---------------------------------------------------------------------------

#define EVENT_ID_HAS_IO_EVENT  0 
#define EVENT_ID_HAS_IO_ERROR  1
#define TIMER_ID_COMM_TIMEOUT  2
#define TIMER_ID_IDLE_TIMEOUT  3

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
 
  disable_sigpipe();

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

int EVLOOP_break( EVLOOP *loop )
{
  return  event_base_loopbreak( loop->ev_base ); 
}

// ---------------------------------------------------------------------------
int EVTHREAD_free( EVTHREAD *thread )
{
   DLIST_entry *entry, *next;
   EVTHREAD_OBJECT *obj;

   DLIST_FOREACH_SAVE( entry, next,  &thread->object_list ) {
     obj = (EVTHREAD_OBJECT *) entry;
     switch( obj->object_type ) {
       case EVTHREAD_OBJECT_SOCKET:
	 EVSOCKET_close( (EVSOCKET *) obj );    
	 break;
       case EVTHREAD_OBJECT_TIMER:
	 EVTIMER_free( (EVTIMER *) obj );    
         break;
     }
   }

   CTHREAD_free(thread->cthread);
   free(thread);
   return 0;
}

static void evthread_proc( VALUES *ctx );
static void thread_timer_cb( int fd, short event, void *ctx);

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

  thread->cthread = CTHREAD_init( loop->stacks, evthread_proc );
  if (!thread->cthread) {
    free(thread);
    return 0;
  }
  thread->socket = 0;

  DLIST_init( &thread->object_list );

  return thread;
}

int EVTHREAD_start( EVTHREAD *thread, struct tagEVSOCKET *socket )
{
  thread->socket = socket;
  CTHREAD_start( thread->cthread, 0, "%p", thread );
  return 0;
}

int EVTHREAD_delay( EVTHREAD *thread, struct timeval delay )
{
  event_set( &thread->timer_event, -1, 0, thread_timer_cb, (void *) thread );
  event_base_set( thread->loop->ev_base, &thread->timer_event );
  event_add( &thread->timer_event, &delay );
   
  CTHREAD_yield( 0, 0);
   
  event_del( &thread->timer_event );

 
  return 0;
}

static void evthread_proc( VALUES *values )
{
  EVTHREAD *thread;

  VALUES_scan( values, "%p", &thread );
  thread->thread_proc( thread, thread->socket, thread->user_context );

  // no way to get exit status of the EVTHREAD object. everything is hereby closed.
  EVTHREAD_free(thread);
}

static void thread_timer_cb( int fd, short event, void *ctx)
{
  EVTHREAD *thread = (EVTHREAD *) ctx;
 
  M_UNUSED(fd);
  M_UNUSED(event);

  CTHREAD_resume( thread->cthread, 0, 0 );
}


// ---------------------------------------------------------------------------
void EVTHREAD_OBJECT_init(EVTHREAD_OBJECT *obj, int type, EVTHREAD *owner)
{
  DLIST_push_back( &owner->object_list, &obj->entry );
  obj->object_type = type;
  obj->owner = owner;
}

void EVTHREAD_OBJECT_free(EVTHREAD_OBJECT *obj)
{
  DLIST_unlink( &obj->owner->object_list, &obj->entry );
  free(obj);
}

// ---------------------------------------------------------------------------

static void timer_cb( int fd, short event, void *ctx);


EVTIMER *EVTIMER_init(EVTHREAD *thread, int timer_id, struct timeval tm )
{
  EVTIMER *ret;
 
  ret = (EVTIMER *) malloc( sizeof( EVTIMER ) );
  if (!ret) {
    return 0;
  }

  ret->loop = thread->loop;
  ret->timer_id = timer_id;
  ret->tm = tm;
  ret->state = EVTIMER_STATE_INIT;
 
  EVTHREAD_OBJECT_init( &ret->object_base, EVTHREAD_OBJECT_TIMER , thread );
 
  return ret;
}


int EVTIMER_start( EVTIMER *ret)
{
  if (ret->state != EVTIMER_STATE_INIT) {
    return -1;
  }

  ret->state = EVTIMER_STATE_SCHEDULED;
  
  event_set( &ret->timer_event, -1, 0, timer_cb, (void *) ret );
  event_base_set( ret->loop->ev_base, &ret->timer_event );
  event_add( &ret->timer_event, &ret->tm );

  return 0;
}

int  EVTIMER_cancel( EVTIMER *timer )
{
  if (timer->state != EVTIMER_STATE_SCHEDULED) {
    return -1;
  }
  event_del( &timer->timer_event );
  timer->state = EVTIMER_STATE_INIT;
  return 0;
}

int  EVTIMER_free( EVTIMER *timer )
{
  EVTIMER_cancel(timer);
  EVTHREAD_OBJECT_free( &timer->object_base );
  return 0;
}



static void timer_cb( int fd, short event, void *ctx)
{
   EVTIMER *timer;

   M_UNUSED(fd);
   M_UNUSED(event);

   timer = (EVTIMER *) ctx;

   event_del( &timer->timer_event );
   timer->state = EVTIMER_STATE_INIT;
   
   CTHREAD_resume( timer->object_base.owner->cthread , 0, "%d", timer->timer_id );
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
  memset( &socket->idle_timeout, 0 , sizeof(struct timeval));

  
  if (fd_set_blocking( fd, 0 )) {
    free(socket);
    return  0;
  }
  
  EVTHREAD_OBJECT_init( &socket->object_base, EVTHREAD_OBJECT_SOCKET , thread );
 
  event_set( &socket->read_event, fd, EV_READ , socket_cb, (void *) socket );
  event_base_set( socket->loop->ev_base, &socket->read_event );

  event_set( &socket->write_event, fd, EV_WRITE , socket_cb, (void *) socket );
  event_base_set( socket->loop->ev_base, &socket->write_event );

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

  event_del( &socket->read_event );   
  event_del( &socket->write_event );   
  
  do {
    rt = close(socket->fd);
  } while(rt == -1 && errno == EINTR);
  

  EVTHREAD_OBJECT_free( &socket->object_base );
  return rt;
}

static void socket_cb( int fd, short event, void *ctx)
{
   EVSOCKET *socket; 

   M_UNUSED( fd );
   
   socket = (EVSOCKET *) ctx;

   if (event & EV_READ && event & EV_WRITE) {
     socket->state = EVSOCKET_STATE_ERROR;
     CTHREAD_resume( socket->thread->cthread, 0, 0 );
     return;
   }

   if (event & EV_READ) {
     if (socket->state == EVSOCKET_STATE_READING) {
	CTHREAD_resume( socket->thread->cthread, 0, 0 );
     }
   }
  
   if (event & EV_WRITE) {
     if (socket->state == EVSOCKET_STATE_CONNECTING || socket->state == EVSOCKET_STATE_WRITING) {
	CTHREAD_resume( socket->thread->cthread, 0, 0 );
     }
   }
}

void EVSOCKET_set_idle_timeout(EVSOCKET *socket, struct timeval timeout )
{ 
  M_UNUSED( socket );
  M_UNUSED( timeout );

  socket->idle_timeout = timeout;
  if (socket->state == EVSOCKET_STATE_CONNECTED) {
     socket->timer_idle_timeout = EVTIMER_init( socket->thread, TIMER_ID_IDLE_TIMEOUT, timeout );
     if (socket->timer_idle_timeout) { 
       EVTIMER_start(socket->timer_idle_timeout); 	
     }
  }
}

int EVSOCKET_connect( EVSOCKET *socket, struct sockaddr *address, socklen_t socklen, struct timeval timeout)
{
  int rt;
  VALUES *rvalues;
  int event_id;

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

     socket->timer_io_timeout = EVTIMER_init( socket->thread, TIMER_ID_COMM_TIMEOUT, timeout );
     if (socket->timer_io_timeout) { 
       EVTIMER_start(socket->timer_io_timeout); 	
     }

     event_add( &socket->write_event, 0 );
     
     CTHREAD_yield( &rvalues, 0 );
     VALUES_scan( rvalues, "%d", &event_id );

     event_del( &socket->write_event );
     
     if (socket->timer_io_timeout) {
       EVTIMER_free( socket->timer_io_timeout ); 
       socket->timer_io_timeout = 0; 
     }

     switch(event_id) {
       case EVENT_ID_HAS_IO_EVENT:
	 socket->state = EVSOCKET_STATE_CONNECTED;
	 rt = 0;
	 break;
       default: 
         socket->state = EVSOCKET_STATE_ERROR;
	 close( socket->fd );
	 socket->fd = -1;
	 rt = -1;
	 break;
      }
  } 
  return rt;
}

static int EVSOCKET_recv_internal( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout )
{
   int rt;
   int has_event = 0;
   VALUES *rvalues;
   int event_id;

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
       
       socket->timer_io_timeout = EVTIMER_init( socket->thread, TIMER_ID_COMM_TIMEOUT, timeout );
       if (socket->timer_io_timeout) {  
         EVTIMER_start(socket->timer_io_timeout); 	
       }

       if (!has_event) {
         event_add( &socket->read_event, 0 );
         has_event = 1;
       }

       CTHREAD_yield( &rvalues, 0 );
       VALUES_scan( rvalues, "%d", &event_id );
 
       if (socket->timer_io_timeout) {  
         EVTIMER_free(socket->timer_io_timeout); 	
	 socket->timer_io_timeout = 0; 
       }
      
       switch(event_id) {
         case EVENT_ID_HAS_IO_EVENT:
	   goto r_again;
         default: 
	   socket->state = EVSOCKET_STATE_ERROR;
	   close( socket->fd );
	   socket->fd = -1;
	   rt = -1;
	   break;
       }
     }
   }

   if (has_event) {
     event_del( &socket->read_event );
   }
   
   if (rt != -1) {
     socket->state = EVSOCKET_STATE_CONNECTED;
   } 
   
   if (rt >= 0) {
     if (socket->timer_idle_timeout) { 
       EVTIMER_cancel(socket->timer_idle_timeout);
     }
   }


   return rt;
} 

static int EVSOCKET_send_internal( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout )
{
   int rt;
   int has_event = 0;
   VALUES *rvalues;
   int event_id;


   if ( socket->state != EVSOCKET_STATE_CONNECTED) {
     return -1;
   }
   
w_again:   
   do {
    rt = send( socket->fd, buf, buf_size, flags);
   } while(rt == -1 && errno == EINTR);

   if (rt == -1) {
     if (errno == EAGAIN) {

       socket->state = EVSOCKET_STATE_WRITING;
       
       socket->timer_io_timeout = EVTIMER_init( socket->thread, TIMER_ID_COMM_TIMEOUT, timeout );
       if (socket->timer_io_timeout) {
         EVTIMER_start(socket->timer_io_timeout); 	
       }

       if (!has_event) {
         event_add( &socket->write_event, 0 );
         has_event = 1;
       }

       CTHREAD_yield( &rvalues, 0 );
       VALUES_scan( rvalues, "%d", &event_id );
 
       if (socket->timer_io_timeout) {  
         EVTIMER_free(socket->timer_io_timeout); 	
	 socket->timer_io_timeout = 0; 
       }
      
       switch(event_id) {
         case EVENT_ID_HAS_IO_EVENT:
	   goto w_again;
         default: 
	   socket->state = EVSOCKET_STATE_ERROR;
	   close( socket->fd );
	   socket->fd = -1;
	   rt = -1;
	   break;
       }
     }
   }
  
   if (has_event) {
     event_del( &socket->write_event );
   }  

   if (rt != -1) {
     socket->state = EVSOCKET_STATE_CONNECTED;
   } 


   return rt;
} 

int EVSOCKET_recv( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout )
{
  int rt;

  rt = EVSOCKET_recv_internal( socket, buf, buf_size, flags, timeout );
  if (rt >= 0) {
     if (socket->timer_idle_timeout) { 
       EVTIMER_start(socket->timer_idle_timeout);
     }
  }
  return rt;
}

int EVSOCKET_recv_all( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout )
{
  uint8_t *cur = (uint8_t *) buf;
  int pos, rt;

  for(pos = 0 ; buf_size != 0 ; pos += rt ) {
    rt = EVSOCKET_recv_internal( socket, cur, buf_size, flags, timeout );
    if (rt <= 0) {
      return rt;
    }
    cur += rt;
    buf_size -= rt;
  }

  if (rt >= 0) {
     if (socket->timer_idle_timeout) { 
       EVTIMER_start(socket->timer_idle_timeout);
     }
  }
  
  return pos;
}

int EVSOCKET_send( EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout )
{
  uint8_t *cur = (uint8_t *) buf;
  int pos, rt;

  for(pos = 0 ; buf_size != 0 ; pos += rt ) {

    rt = EVSOCKET_send_internal( socket, cur, buf_size, flags, timeout );
    if (rt < 0) {
      return -1;
    }
    cur += rt;
    buf_size -= rt;
  } 
  return pos;
}

// ---------------------------------------------------------------------------

static void socket_listener_cb( int fd, short event, void *ctx);

EVTCPACCEPTOR * EVTCPACCEPTOR_init( EVLOOP *loop, int fd, EVTHREAD_FACTORY factory, int read_buffer_size, int send_buffer_size )
{
  EVTCPACCEPTOR *acceptor;

  acceptor = (EVTCPACCEPTOR *) malloc( sizeof( EVTCPACCEPTOR ) );
  if (!acceptor) {
    return 0;
  }
  
  if (fd_set_blocking( fd, 0 )) {
    free(acceptor);
    return  0;
  }

  acceptor->loop = loop;
  acceptor->factory = factory;
  acceptor->fd = fd;

  acceptor->read_buffer_size = read_buffer_size;
  acceptor->send_buffer_size = send_buffer_size;

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
   int sock;

   M_UNUSED(fd);

   if (event & EV_READ && event & EV_WRITE) {
      return;
   }

   acceptor = (EVTCPACCEPTOR *) ctx;
 
   do {
     sock = accept( acceptor->fd, 0, 0 );
   } while( sock == -1 && errno == EINTR);

   if (sock == -1) {
     return;
   }
   
   if (acceptor->read_buffer_size != -1) {
     fd_set_buf_size( sock, Receive_buffer, acceptor->read_buffer_size );
   }

   if (acceptor->send_buffer_size != -1) {
     fd_set_buf_size( sock, Send_buffer, acceptor->send_buffer_size);
   }
  
   // get thread procedure and thread argument data.
   if ( acceptor->factory( sock, &thread_proc, &thread_ctx ) ) {
     close(sock);
     return;
   }


   // create the user thread.
   thread =EVTHREAD_init( acceptor->loop, thread_proc, thread_ctx);
   if (!thread) {
     close(sock);
     return;
   }

   socket = EVSOCKET_init(thread, sock, 1);
   if (!socket) {
     //close thread)
     close(sock);
     return;
   }

   EVTHREAD_start( thread, socket );

}

void custom_timeout_handling( struct event_base *base, struct timeval *tv )
{
   M_UNUSED(base);
   M_UNUSED(tv);
}
