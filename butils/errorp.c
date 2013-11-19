/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "errorp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#ifdef __linux__
#include <execinfo.h>
#endif

#define ERROR_TOKEN "ERROR: "

#define STACK_FRAMES 30

#define STACK_START "*** start stack ***\n"
#define STACK_EOF   "\n*** eof stack ***\n\n"


static pthread_mutex_t counter_mutex  = PTHREAD_MUTEX_INITIALIZER;


static int FD_OUT  = 2;
static int FD_ASSIGNED = 0;

#ifdef __linux__
static void dump_modules(char *buff, size_t buff_size);
#endif

int errorp_is_file_open()
{  
  return FD_ASSIGNED;
}

int errorp_open_file(const char *file)
{
  int fd;

  fd = open( file , O_CREAT | O_APPEND | O_RDWR, S_IWUSR  | S_IRUSR );
  if (fd != -1) {
    FD_OUT = fd;
    FD_ASSIGNED = 1;
  }
  return fd;
}

void errorp_close_file()
{
  if (FD_ASSIGNED != 1) {
    close( FD_OUT );
    FD_OUT = 2;
    FD_ASSIGNED = 0;
  }
}

static char buff[ 8192 ];


static int print_stack_trace()
{
#if __linux__
  int rt = 0;
  void *sframes[ STACK_FRAMES + 1 ];
  int nframes, i;

  nframes = backtrace( sframes, STACK_FRAMES + 1); \
  rt = write( FD_OUT, STACK_START, strlen( STACK_START ) );
  if (rt == -1)
    return -1;
  nframes =  backtrace( sframes, STACK_FRAMES );
  for (i=0; i<nframes; i++) {
    snprintf( buff, sizeof(buff) - 1,  "frame %d ip: %p\n", i, sframes[ i ]);
    rt += write( FD_OUT , buff, strlen( buff ) );
  }

  dump_modules( buff, sizeof(buff) );
  rt += write( FD_OUT, STACK_EOF, strlen( STACK_EOF ) );
  return rt;
#else
  return -1;
#endif
}


void errorp(int rval, const char *fmt, ... ) 
{
  char *p, *eof;
  va_list ap;
  int len, n, rt;
  pthread_mutex_lock( &counter_mutex );
  
  p = buff;
  eof = p + sizeof( buff );

  strcpy( p, ERROR_TOKEN );
  p += strlen( ERROR_TOKEN );

  va_start( ap, fmt );
  len =  vsnprintf( p, eof - p - 1, fmt, ap );
  va_end( ap );
  p += len;

  if (p < eof) {
    n = snprintf(p, eof - p - 1, ". returns %d errno %d\n", rval, errno );
    p += n;
  }    

  rt = write( FD_OUT , buff, strlen( buff ) );
  if (rt == -1)
    goto err;
 
  print_stack_trace();

err:
   pthread_mutex_unlock( &counter_mutex );
  
}

void error_dump_string( const char *msg, char *buff, size_t buff_size)
{
  int rt;
#if __linux__
  void *sframes[ STACK_FRAMES + 1 ];
  int nframes, i;
#else 
  (void) buff;
  (void) buff_size;
#endif


  rt = write( FD_OUT , msg , strlen( msg ) );
  if (rt == -1)
    return;
#if __linux__
  nframes = backtrace( sframes, STACK_FRAMES + 1); \
  rt = write( FD_OUT, STACK_START, strlen( STACK_START ) );
  if (rt == -1)
    return;
  
  nframes =  backtrace( sframes, STACK_FRAMES );
  for (i=0; i<nframes; i++) {
    snprintf( buff, buff_size,  "frame %d ip: %p\n", i, sframes[ i ]);
    rt = write( FD_OUT , buff, strlen( buff ) );
    if (rt == -1)
      return;
 }

  dump_modules( buff, buff_size );
  rt = write( FD_OUT, STACK_EOF, strlen( STACK_EOF ) );
  if (rt == -1)
     return;
#endif
}

#define TOKENS_COUNT	    6
#define TOKENS_ADDRESS	    0
#define TOKENS_PERMISSIONS  1 
#define TOKENS_FILEOFFSET   2
#define TOKENS_PATH	    5

#ifdef __linux__
static void dump_modules(char *buf, size_t buff_size)
{
  int fd, n, i, buf_start;
  char line[40];
  char *pos, *eof_line, *tokens[ TOKENS_COUNT ];
  int rt;

  sprintf(buf,"/proc/%d/maps", getpid() );


  fd = open( buf, O_RDONLY );
  if (fd == -1) {
    return;
  }
  
  buf_start = 0;
  while(1) {
    n = read(fd, buf + buf_start, buff_size - buf_start );
    if (n <= 0) {
      break;
    }

    pos = buf;
    while(1) {

       eof_line = memchr( pos, '\n', buf + buff_size - pos );
       if (eof_line == 0) {
         break;
       }
       
       *eof_line = '\0';
       for(i=0; i< TOKENS_COUNT; i++) {
         tokens[ i ] = strtok( !i ? pos : 0," ");
	 if (! tokens[ i ]) {
	   break;
	 }
	 if (i == TOKENS_PERMISSIONS) {
	   if (strchr( tokens[i], 'x') == 0) {
	     break;
	   }
	 }
       }
       if (i == TOKENS_COUNT) {
          snprintf(line, sizeof(line) - 1, "\n%s %s ", tokens[ TOKENS_ADDRESS ] , tokens[ TOKENS_FILEOFFSET ]);
	  rt = write( FD_OUT, line, strlen(line) );
	  if (rt == -1)
	    return;
	  rt = write( FD_OUT, tokens[ TOKENS_PATH ], strlen( tokens[ TOKENS_PATH ] ) );
      	  if (rt == -1)
	    return;
       }
      
       pos = eof_line + 1;
    }

    buf_start = 0;
    if (pos > buf && pos < (buf + buff_size - 1)) {
      buf_start = buf + buff_size - pos;
      memmove( buf, pos, buf_start ); 
    } else {
      break;
    }
  }
  close(fd);

}
#endif


#define DO_SIGACTTION

#ifndef WIN32

#ifdef DO_SIGACTTION
static void crash_handler( int signal, siginfo_t *siginfo, void *context )
{
    char msg[50];
    int rt;
    
    (void) siginfo;
    (void) context;
#else
static void crash_handler( int signal )
{
    char msg[50];
    rt;
#endif

    switch( signal ) {
     case SIGSEGV:
	sprintf(msg, "ERROR : Crash! SIGSEGV occured\n" );
	break;
     case SIGILL:
	sprintf(msg, "ERROR : Crash! SIGILL occured\n" );
	break;
     case SIGBUS:
	sprintf(msg, "ERROR : Crash! SIGBUS occured\n" );
	break;
    default:
	sprintf(msg, "ERROR : Crash! signal %d\n", signal);
    }

    rt = write( FD_OUT , msg , strlen( msg ) );
    print_stack_trace();
    exit(1);
}
#endif

void install_crash_handler()
{
#ifndef WIN32
#ifdef DO_SIGACTTION
    struct sigaction act;
     
    memset (&act, '\0', sizeof(act));
	 
    act.sa_sigaction = &crash_handler;
    act.sa_flags = SA_SIGINFO;
			 
    sigaction( SIGSEGV, &act, NULL );
    sigaction( SIGILL,  &act, NULL );
    sigaction( SIGBUS,  &act, NULL );
#else
    signal( SIGSEGV, crash_handler );
    signal( SIGILL,  crash_handler );
    signal( SIGBUS, crash_handler );
#endif
#endif
}
