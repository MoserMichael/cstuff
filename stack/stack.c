/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SUM_BYTES 64
#define DEFAULT_STACK_DEPTH 1024

#define FATAL_ERROR_S     "FATAL ERROR:"
#define FATAL_ERROR_S_LEN 12

#define STACK_START "ERROR: Stack corruption detected\n**** start stack ***\n"
#define STACK_EOF   "\n*** eof stack ***\n\n"
#define FD_OUT 2

typedef struct tagFRAME_ENTRY {
  void *ip;
  void *cs;
  void *bp;
  ptrdiff_t chksum;
} FRAME_ENTRY;


typedef struct tagTHREAD_ENTRY {
  FRAME_ENTRY *entries;
  int cur_idx;
  int num_entries;

} THREAD_ENTRY;



static __thread THREAD_ENTRY  * thread_entry;

static void dump_modules();

static int fatal_error( const char *fmt, ... )
{
  char msg[120];
  va_list ap;
  int len;
  int rt;

  strcpy(msg, FATAL_ERROR_S );

  va_start( ap, fmt );
  len =  vsnprintf( msg + FATAL_ERROR_S_LEN, sizeof(msg) - 1 - FATAL_ERROR_S_LEN, fmt, ap );
  va_end( ap );

  rt = write(1, msg, len  + FATAL_ERROR_S_LEN );
  exit(1);

  return rt;
}

#if 0
static void show_msg( const char *fmt, ... )
{
  char msg[120];
  va_list ap;
  int len;


  va_start( ap, fmt );
  len =  vsnprintf( msg,  sizeof(msg) - 1,   fmt, ap );
  va_end( ap );

  write(1, msg, len );
  exit(1);
}
#endif


#if 0
static int dir_stack_growth( void *parent )
{
  int cur = 0;

  if ( (ptrdiff_t) parent > (ptrdiff_t) &cur) {
    return 1;
  } else {
    return -1;
  }
}
#endif


static THREAD_ENTRY *init_thread_entry()
{
  THREAD_ENTRY *r = (THREAD_ENTRY *) malloc( sizeof( THREAD_ENTRY ) );
  if (!r)  {
    fatal_error("Can't initialise thread entry ");
    return 0;
  }
  r->entries = (FRAME_ENTRY *) malloc(  sizeof( FRAME_ENTRY )  * DEFAULT_STACK_DEPTH );
  if (! r->entries) {
    fatal_error("Can't allocate frame entries");
    return 0;
  }

  r->num_entries = DEFAULT_STACK_DEPTH;
  r->cur_idx = -1;
  
  //pthread_cleanup_push( :
  return r;
}

static void free_thread_entry( THREAD_ENTRY *entry )
{
  free(entry->entries);
  free(entry);

}

static FRAME_ENTRY * next_frame( THREAD_ENTRY *entry )
{
  FRAME_ENTRY *tentry;
  int new_num_entries;

  entry->cur_idx ++;
  if (entry->cur_idx >= entry->num_entries) {
     // resize the entry array.

     new_num_entries = entry->num_entries * 2;
     tentry =  (FRAME_ENTRY *) realloc( entry->entries, new_num_entries  * sizeof(FRAME_ENTRY) );
     if (tentry == 0) {
        fatal_error("Can't grow the shadow stack to %d ", new_num_entries );
     }
     entry->entries = tentry;
     entry->num_entries = new_num_entries;
  }
 
  return entry->entries + entry->cur_idx ;
}

ptrdiff_t make_checksum( void *ptr, size_t size )
{
#if 0
   uint8_t *cptr = (uint8_t *) ptr;
   uint32_t res = 0;
   size_t i;

   for(i=0; i < size; i++) {

      res = (res << 8) + *cptr ++;
      if  (res & 0x80000000) {
	 res = (res & 0xFFFF) + (res >> 16);
      }
   }

   return res;
#else 
  (void) size;

  return *((ptrdiff_t *) ptr);
#endif
}

static void make_frame( void *func_addr, void *call_site, void *frame_addr )
{
  FRAME_ENTRY *frame;

  if (thread_entry == 0) {
    thread_entry = init_thread_entry();
  }

  frame = next_frame( thread_entry );
  if (frame != 0) {
    frame->ip = func_addr;
    frame->bp = frame_addr;
    frame->cs = call_site;
    frame->chksum = make_checksum( frame_addr, SUM_BYTES );
  }
}

static void pop_frame( void *func_addr, void *frame_addr )
{
  ptrdiff_t chksum;
  FRAME_ENTRY *frame;
  THREAD_ENTRY *entry;
  int i,j;
  char buff[64];
  int stat = 0;

  entry = thread_entry;
  if (entry == 0) {
    return; //??? WTF ???
  }


  frame = entry->entries +  entry->cur_idx;

#ifndef DONT_FIX_SETJMP
  if (frame->ip != func_addr) {
     // it might be that the stack was altered with setjmp/longjmp or othr
     // co procedure like stuff.
     // also: search for a frame with function address and frame pointer.
     for(i=entry->cur_idx; i>=0; i--) {
	if (entry->entries[i].ip == func_addr &&
	    entry->entries[i].bp == frame_addr) {
	  // have found our match.
	  entry->cur_idx = i;
	  break;
	}
	if (i < 0) {
	  // very sorry, we lost the stack.
          return;
	}
     }
  }
#endif

  chksum = make_checksum( frame_addr, SUM_BYTES );
  if (frame->chksum != chksum) {
    
    if (write( FD_OUT, STACK_START, strlen(STACK_START) ) != -1) {
      for (j = 0, i = entry->cur_idx; i >= 0; i--, j++) {
        snprintf( buff, sizeof(buff) - 1,  "frame %d ip: %p\n", j , entry->entries[ i ].ip );
        if (write( FD_OUT , buff, strlen( buff ) ) == -1) {
	  break;
	}

        snprintf( buff, sizeof(buff) - 1,  "frame %d ip: %p\n", j , entry->entries[ i ].cs );
        if (write( FD_OUT , buff, strlen( buff ) ) == -1) {
	  break;
	}
      }
    }
    dump_modules();
    if (write( FD_OUT, STACK_EOF, strlen(STACK_EOF) ) == -1) {
      stat = -1;
    }

#ifdef FIX_STACK_AND_CORE_DUMP
    // here we put back recorded values into stack, so that core dump will
    // have a usable stack trace (hopefully).
    // I do not enable this by default though.

    for ( i = entry->cur_idx; i >= 0; i-- ) {
       * ((ptrdiff_t *) entry->entries[i].bp) = entry->entries[i].chksum;
    }
    do { int *core_now = 0; * core_now = 12; } while(0);
#endif
  }
  
  -- entry->cur_idx;
 
  if (entry->cur_idx < 0) {
    free_thread_entry( thread_entry );
    thread_entry = 0;  
  }


   
}

#define TOKENS_COUNT	    6
#define TOKENS_ADDRESS	    0
#define TOKENS_PERMISSIONS  1 
#define TOKENS_FILEOFFSET   2
#define TOKENS_PATH	    5

static void dump_modules()
{
  int fd, n, i, buf_start;
  char buf[ 1024 ], line[30];
  char *pos, *eof_line, *tokens[ TOKENS_COUNT ];

  sprintf(buf,"/proc/%d/maps", getpid() );


  fd = open( buf, O_RDONLY );
  if (fd == -1) {
    return;
  }
  
  buf_start = 0;
  while(1) {
    n = read(fd, buf + buf_start, sizeof(buf) );
    if (n <= 0) {
      break;
    }

    pos = buf;
    while(1) {

       eof_line = memchr( pos, '\n', buf + sizeof(buf) - pos );
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
	  if (write( FD_OUT, line, strlen(line) ) == -1 ||
  	      write( FD_OUT, tokens[ TOKENS_PATH ], strlen( tokens[ TOKENS_PATH ] ) ) == -1) {
	    break; 
	  }
       
       }
      
       pos = eof_line + 1;
    }

    buf_start = 0;
    if (pos < (buf + sizeof(buf) - 1)) {
      buf_start = buf + sizeof(buf) - pos;
      memmove( buf, pos, buf_start ); 
    }
  }
  close(fd);

}

void __cyg_profile_func_enter( void *func_addr, void *call_site ) 
{ 
   void *ptr;
   void *ret[1];
   
   ptr =  __builtin_frame_address(1);
   if (ptr == 0) {
     ptr = ret[2]; // used to be true, now guess if it is always true ...
   }
 
#if 0
   fprintf(stderr, "enter ip: %p bp: %p [bp] %p\n", func_addr, ptr,  * ((void **) ptr) );
#endif
   make_frame( func_addr, call_site, ptr );
}

void __cyg_profile_func_exit(  void *func_addr,  void *call_site )
{
  void *ptr;    
  void *ret[1];

  (void) call_site;

  ptr =  __builtin_frame_address(1);
  if (ptr == 0) {
    ptr = ret[2];
  }

#if 0
  fprintf(stderr, "exit ip: %p bp: %p [bp] %p\n", func_addr, ptr,  * ((void **) ptr) );
#endif  
  pop_frame (func_addr, ptr );
}
