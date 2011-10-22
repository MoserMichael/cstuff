#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <butils/errorp.h>
#include <butils/logg.h>

MLOG_LEVEL m_current_log_level = MLOG_LEVEL_ERROR;

static MLOG_ACTION log_action;
static MLOG_ALLOC alloc_strategy = MLOG_ALLOC_TLS_HEAP;
static MLOG_LEVEL stack_dump_level = MLOG_LEVEL_ERROR;
static size_t alloc_size = 1024;
static pthread_key_t log_tls_entry;
static int log_fd;
static int log_prefix_level = MLOG_PREFIX_LOG_LEVEL | MLOG_PREFIX_SOURCE_FILE | MLOG_PREFIX_TIME;

static char * log_level_names[] = {
  "TURN_OFF_LOGGING",
  "ERROR",
  "WARN",
  "INFO",
  "DEBUG",
  "TRACE",
};

void error_dump_string( const char *msg, char *buff, size_t buff_size);


static void tls_free(void *val)
{
  if (val) {
    free(val);
  }
}

int  MLOG_init( MLOG_LEVEL current, MLOG_ACTION action, void *arg)
{
  m_current_log_level = current;
 
  log_action |= action;

  if (action & MLOG_ACTION_TO_FILE) {
    log_fd = open( (const char *) arg , O_CREAT | O_RDWR, S_IWUSR  | S_IRUSR );
    return log_fd == -1;
  }
  return 0;
}


int  MLOG_alloc_option( MLOG_ALLOC option, size_t size)
{
  alloc_strategy = option;
  alloc_size = size;


  if (alloc_strategy == MLOG_ALLOC_TLS_HEAP) {
    if (log_tls_entry == 0) {
      if (pthread_key_create( &log_tls_entry, tls_free )) {
        return -1;
      }
      pthread_setspecific( log_tls_entry, 0 );
    }
  }
  return 0;
}

void MLOG_dump_stack_level( MLOG_LEVEL level )
{
  stack_dump_level = level;
}


int MLOG_printf( MLOG_LEVEL current,  const char *file, int line, const char *format , ... )
{
  char *sbuf,*pos;
  size_t buf_size, msg_len;
  va_list vlist;
  struct timeval now_time_val;
  struct tm now_time;
  int rt;

  switch(alloc_strategy) {
    case MLOG_ALLOC_STACK:
      sbuf = alloca( alloc_size );
      break;
    case MLOG_ALLOC_HEAP:
      sbuf = (char *) malloc( alloc_size );
      if (!sbuf) {
        return -1;
      }
      break;
    case MLOG_ALLOC_TLS_HEAP:
      sbuf = pthread_getspecific( log_tls_entry );
      if (!sbuf) {
        sbuf = malloc( alloc_size );
	if (!sbuf) {
	  return -1;
	}
	pthread_setspecific( log_tls_entry, sbuf );
      }
      break;
  }
  
  buf_size = alloc_size - 2;
  pos = sbuf;

  if (log_prefix_level & MLOG_PREFIX_LOG_LEVEL && buf_size > 0) {
    rt = snprintf( pos, buf_size, "%s : ", log_level_names[ current ] );
    if (rt < 0) {
      return -1;
    }
    pos += rt;
    buf_size -= rt;
  }
  
  if (log_prefix_level & MLOG_PREFIX_SOURCE_FILE && buf_size > 0) {
    rt = snprintf( pos, buf_size, "[%s:%d] ", file, line  );
    if (rt < 0) {
      return -1;
    }
    pos += rt;
    buf_size -= rt;
   }
  
  if (log_prefix_level & MLOG_PREFIX_TIME && buf_size > 0) {
    gettimeofday( &now_time_val, 0 );
    localtime_r(  &now_time_val.tv_sec, &now_time);
          
    rt = snprintf( pos, buf_size, "%02d:%02d:%02d:%06ld ",
					(int)  now_time.tm_hour,
					(int)  now_time.tm_min,
					(int)  now_time.tm_sec,
					now_time_val.tv_usec );
    if (rt < 0) {
      return -1;
    }
    pos += rt;
    buf_size -= rt;
  }

  if (buf_size > 0) {
    va_start( vlist, format );
    rt = vsnprintf( pos, buf_size, format, vlist );
    va_end( vlist );
    if (rt < 0) {
      return -1;
    }
    pos += rt;
    buf_size -= rt;
  }
  
  strcpy( pos, "\n");
  msg_len = alloc_size - buf_size - 1;
  
  if (log_action & MLOG_ACTION_CONSOLE) {
    write( 2, sbuf, msg_len );
  } 


  if (log_action & MLOG_ACTION_TO_FILE) {
    write( log_fd, sbuf, msg_len );
  }

  if (log_action & MLOG_ACTION_SYSLOG_TRACE) {
    syslog( LOG_USER, "%s", sbuf );
  }
  
  if (current <= stack_dump_level) {
    if (!errorp_is_file_open()) {
       errorp_open_file( "err.log" );
    }
    error_dump_string( sbuf, sbuf, alloc_size );
  }

  switch(alloc_strategy) {
    case MLOG_ALLOC_TLS_HEAP:
    case MLOG_ALLOC_STACK:
      break;
    case MLOG_ALLOC_HEAP:
      free(sbuf);
      break;
  }
  return 0;
} 


