
/* 
 * DBGMEM - memory allocation leak tracker and debugging tool.
 *
 *  (c) 2008 by Michael Moser
 * 
 * 3-clause BSD License applies  
 *
 * See LICENSE file for the full copy of the license
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include "memalloc.h"
#include <errno.h>

#ifdef WIN32						
#include <io.h>
#define PATH_MAX 1024
#else
#include <unistd.h>
#endif

static int log_file = -1, safe_fd;
static char log_dir[ PATH_MAX + 1 ];
#ifdef WIN32	

#define STDERR_FILENO 2
#define FILE_PERM 
#define vsnprintf _vsnprintf
#define snprintf _snprintf

#else

#define FILE_PERM ,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH 

#endif

STATIC_C void log_set_dir(const char *fname)
{
  strncpy(log_dir, fname, sizeof(log_dir)-1 );
}

STATIC_C void log_get_path(const char *fname, char *buf, int buf_size)
{
   
  if (log_dir[0] != '\0') {
 
    snprintf(buf,buf_size,"%s/%s",log_dir, fname); 
  } else {
    strncpy(buf, fname, buf_size);
  }
}

STATIC_C void log_close()
{
  if (log_file != -1) {
    close( log_file);
  }
  log_file = -1;
}

static void l_init(const char *fname, int append)
{
  log_close();

  //fprintf(stderr,"log file name: %s\n",fname);
  if (!append) {
    log_file = open( fname, O_WRONLY | O_CREAT | O_TRUNC 
						 FILE_PERM
						); 
  } else {
    log_file = open( fname, O_WRONLY| O_CREAT | O_APPEND  
						 FILE_PERM
						); 
  }
  if (log_file == -1) {
     char err[100];

     snprintf(err, 100, "DBGMEM ERROR: can't open output file. error %d\n", errno);  
     write(2, err, strlen(err));
  }
}

STATIC_C void log_open(const char *fname, int append)
{
  char name[ PATH_MAX ];
  
  log_get_path( fname, name, sizeof(name));
  
  l_init(name, append);

//log("hello world\n");
}

STATIC_C void log_init(const char *fname)
{
  log_open(fname, 0);
}

STATIC_C void log_push_current_file()
{
  safe_fd = log_file;
  log_file = -1;
}

STATIC_C void log_pop_current_file()
{
  if (safe_fd != log_file) {  
    log_close(); 
  }
  log_file = safe_fd;
}

static void do_log(int is_err, const char *format, va_list *args)
{
  char line[500];
  int nline;

  nline = vsnprintf(line, sizeof(line) - 1, format, *args);
  va_end(*args);
 
  if (log_file != -1) {
    write( log_file, line, strlen( line ) );
  }
  if (is_err) {
    write( STDERR_FILENO, line, strlen( line ) );
  }
}

STATIC_C void dbgmem_log(const char *format, ... )
{
  va_list args;

  va_start(args, format);

  do_log( 0, format, &args );
}

STATIC_C void  dbgmem_log_err(const char *format, ... )
{
  va_list args;

  va_start(args, format);

  do_log( 1, format, &args );
}

STATIC_C void  dbgmem_log_ext(int is_err, const char *format, ... )
{
  va_list args;

  va_start(args, format);

  do_log( is_err, format, &args );
}

typedef void (*FILTER_DATA) (unsigned char *buf, int size);

STATIC_C int copy_file_fd_to_fd(int fdin, int fdout,FILTER_DATA  filter)
{
  unsigned char buf[ 500 ];
  int  inCount, outCount;  
  int  ret = 0;
  
  while( (inCount = read( fdin, buf, sizeof(buf))) > 0)  {

	 if (filter) {
	   filter(buf,inCount);
	 }
	 
	 outCount = write( fdout, buf, inCount );
 
	 if (outCount != inCount) {
	    ret = -1;
	    break;
	 }
         //fprintf(stderr,"> inCount %d outCount %d\n", inCount, outCount );
   }
   return ret;
}

STATIC_C int copy_file_to_log(const char *in_file,FILTER_DATA  filter)
{
  int fdin;
  int ret = 0;
  
  fdin  = open( in_file, O_RDONLY ); 
  if (fdin != -1) {
    ret = copy_file_fd_to_fd(fdin, log_file, filter );
    close(fdin);
  } else {
    ret = -1;
  }
  return ret;
}

STATIC_C int copy_file(const char *in_file,const char *out_file,FILTER_DATA  filter)
{
  int fdin,fdout;
  int ret = 0;
  

  fdin  = open( in_file, O_RDONLY ); 
  if (fdin != -1) {

    fdout = open( out_file, O_WRONLY | O_CREAT | O_TRUNC 
			 FILE_PERM
		);
    if (fdout != -1) {
      
      ret = copy_file_fd_to_fd(fdin, fdout, filter);
      close( fdout );  
      
    } else {
      ret = -1;
    }
    close( fdin ); 
  } else {
    ret = -1;
  }
  return ret;
}

#define DBG_TRACE_HERE(offset_frames) \
    dbgmem_log_err("-current stack while calling this function\n"); \
    { \
      void *sframes[ STACK_FRAMES + (offset_frames) + 1 ]; \
      int nframes; \
      int j; \
	\
      nframes = backtrace( sframes, STACK_FRAMES + (offset_frames) );  \
	\
      for(j=(offset_frames); j < nframes && sframes[j]!=0;  j++) { \
        dbgmem_log_err( "\tframe %d : %p\n", j-(offset_frames), sframes[j] ); \
      } \
    } \
 


