/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "errorp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef __linux__
#include <execinfo.h>
#endif

#define ERROR_TOKEN "ERROR: "

#define STACK_FRAMES 30

#define STACK_START "*** start stack ***\n"
#define STACK_EOF   "\n*** eof stack ***\n\n"


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

void errorp(int rval, const char *fmt, ... ) 
{
  char buff[ 512 ];
  char *p, *eof;
  va_list ap;
  int len, n;
#if __linux__
  void *sframes[ STACK_FRAMES + 1 ];
  int nframes, i;
#endif

 
  p = buff;
  eof = p + sizeof( buff );

  strcpy( p, ERROR_TOKEN );
  p += strlen( ERROR_TOKEN );

  va_start( ap, fmt );
  len =  vsnprintf( p, eof - p - 1, fmt, ap );
  va_end( ap );
  p += len;

  n = snprintf(p, eof - p - 1, ". returns %d errno %d\n", rval, errno );
  p += n;

#if __linux__
  nframes = backtrace( sframes, STACK_FRAMES + 1); \
  write( FD_OUT, STACK_START, strlen( STACK_START ) );
  
  nframes =  backtrace( sframes, STACK_FRAMES );
  for (i=0; i<nframes; i++) {
    snprintf( buff, sizeof(buff),  "frame %d ip: %p\n", i, sframes[ i ]);
    write( FD_OUT , buff, strlen( buff ) );
  }

  dump_modules( buff, sizeof(buff) );
  write( FD_OUT, STACK_EOF, strlen( STACK_EOF ) );
#endif
}

void error_dump_string( const char *msg, char *buff, size_t buff_size)
{
#if __linux__
  void *sframes[ STACK_FRAMES + 1 ];
  int nframes, i;
#else 
  (void) buff;
  (void) buff_size;
#endif



  write( FD_OUT , msg , strlen( msg ) );

#if __linux__
  nframes = backtrace( sframes, STACK_FRAMES + 1); \
  write( FD_OUT, STACK_START, strlen( STACK_START ) );
  
  nframes =  backtrace( sframes, STACK_FRAMES );
  for (i=0; i<nframes; i++) {
    snprintf( buff, buff_size,  "frame %d ip: %p\n", i, sframes[ i ]);
    write( FD_OUT , buff, strlen( buff ) );
  }

  dump_modules( buff, buff_size );
  write( FD_OUT, STACK_EOF, strlen( STACK_EOF ) );
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
  char line[30];
  char *pos, *eof_line, *tokens[ TOKENS_COUNT ];

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
	  write( FD_OUT, line, strlen(line) );
	  write( FD_OUT, tokens[ TOKENS_PATH ], strlen( tokens[ TOKENS_PATH ] ) );
       
       }
      
       pos = eof_line + 1;
    }

    buf_start = 0;
    if (pos < (buf + buff_size - 1)) {
      buf_start = buf + buff_size - pos;
      memmove( buf, pos, buf_start ); 
    }
  }
  close(fd);

}
#endif
