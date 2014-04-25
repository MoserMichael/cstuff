
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


#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "../common.h"
#include "load.h"
#include "sbuf.c"
#include "../log.c"

#define SET_STRACE_ON_OFF	1004

#define REPORT_FILE_NAME "TRACEMEM_report_%d.log"

#define NEWPTR "|newptr "  
#define FREEPTR "|freeptr "

static int dlsym_nesting;

//
// size of string for string format of message;  ups, what happens if the STACK_FRAME count does not fit within this buffer ????
// 
#define MSG_LEN  512	

static int TRACE_MEM = 1;

// number of stack frames written into each report
static int STACK_FRAMES = 3;

// if enabled: byte for newly allocated memory (with disclaimers)
#define FILL_BYTE_MALLOCED_MEMORY 0xDD

// if enabled: byte for newsly freed memory (with disclaimers0
#define FILL_BYTE_FREED_MEMORY    0xEE

typedef int    (*PFN_posix_memalign) (void **m, size_t boundary, size_t size);

typedef void * (*PFN_memalign) (size_t boundary, size_t size);

typedef void * (*PFN_valloc) (size_t size);


MAKE_FUNC( malloc );
MAKE_FUNC( realloc );
MAKE_FUNC( free );
MAKE_FUNC( posix_memalign );
MAKE_FUNC( memalign );
MAKE_FUNC( valloc );
MAKE_FUNC( mallopt );


#if 1
static __thread int is_in_malloc = 0;

#define ENTER_MALLOC  is_in_malloc = 1;
#define EXIT_MALLOC   is_in_malloc = 0;
#define TEST_IN_MALLOC  is_in_malloc
#endif

#define TRACE_STACK\
  {\
  ENTER_MALLOC;\
  void *sframes[ STACK_FRAMES + 1 ];\
  int nframes, i;\
\
  nframes = backtrace( sframes, STACK_FRAMES + 1); \
  for(i = 1; i < nframes; i++) { \
    SBUF_add_s(&sbuf,"Frame: "); \
    SBUF_fmt_size_t(&sbuf, i ); \
    SBUF_add_s(&sbuf, " " ); \
    SBUF_fmt_ptr(&sbuf, sframes[ i ]); \
    SBUF_add_s(&sbuf, "\n" ); \
  } \
  EXIT_MALLOC; \
  }

static void switch_trace_handler(int sig)
{
  (void) sig;
  TRACE_MEM = ! TRACE_MEM;
}


static int  get_params()
{
  // environment does not always work here, for whatever reasons.
  // therefore we get parameters from a parameter file.
  char *opt;
  char buf[ PATH_MAX + 100 ], param_file[100];
  int  fd,i,n;

  pid_t npid = getppid(); // getpid();
  
  snprintf(param_file,sizeof(param_file), "/tmp/tracemem_param_%d", npid);

  fd = open(param_file,O_RDONLY);
  if (fd == -1) {
    snprintf(param_file,sizeof(param_file), "/var/tmp/tracemem_param_%d", npid);
    
    fd = open(param_file,O_RDONLY);
    if (fd == -1) {
      snprintf(param_file, sizeof(param_file), "~/tracemem_param_%d", npid);
      
      fd = open(param_file,O_RDONLY);
    }
  }  
 
//fprintf(stderr,"cfg-param-file %s cfg-file %d\n",param_file, fd); 
  if (fd != -1) {  
    n = read(fd,buf,sizeof(buf)-1);
    if (n > 0) {
      buf[n] = '\0';
//fprintf(stderr,"cfg-data %s\n", buf);

      opt = strtok(buf,",");
      for(i=0; opt; ++i,opt=strtok(0,",")) {
        switch(i) {
	  case 0:
	   TRACE_MEM = atoi( opt ); 
	  case 1:
	   STACK_FRAMES = atoi( opt );
	   break;
	  case 2:
	   {
	     int signame = atoi( opt );
	     if (signame != 0)
	     {
		set_signal( signame, switch_trace_handler );
	     }
	   }
	   break;
	}
      } // eof for 
     
      close(fd);
      unlink(param_file);
      
      return 0;
    }       
  } else {
#if 0  
    fprintf(stderr,"DBGMEM: no parameter file for process %d, ignoring process\n", getpid());
#endif    
  }
  
  return -1;
}



EXPORT_C void __attribute__((constructor)) tracemem_init_mem_alloc(void)
{
  int read_cfg;

  get_malloc();
  get_realloc();
  get_free();
  get_posix_memalign();
  get_memalign();
  get_valloc();
  get_mallopt();

  set_core_unlimited();


  read_cfg = get_params();
  if (!read_cfg)
  {
    char file_name[ sizeof(REPORT_FILE_NAME) + 20 ];

    snprintf(file_name, sizeof(file_name), REPORT_FILE_NAME, getpid() );
 
    log_set_dir(".");
    log_open( file_name, 1);
  }

#if 0
   fprintf(stderr,"MEMTRACE init completed.\n"
		  "\tTRACE_MEM %d\n"
		  "\tSTACK_FRAMES %d\n"
		  "\tread_cfg %d\n"
		  "\tlog_file %d\n",
		    TRACE_MEM,
		    STACK_FRAMES,
		    read_cfg,
		    log_file);
#endif

}







EXPORT_C void __attribute((destructor)) tracemem_cleanup_mem_alloc(void)
{
    dump_maps();
}

static void init( void *ptr )
{
#ifdef ZERO_MEM
    if (ptr)
    {
	size_t asz =  malloc_usable_size( ptr );
	memset( ptr, FILL_BYTE_MALLOCED_MEMORY, asz );
    }
#else
    (void) ptr;
#endif    
}


static void deinit( void *ptr )
{
#ifdef ZERO_MEM
    if (ptr)
    {
	size_t asz =  malloc_usable_size( ptr );
	memset( ptr, FILL_BYTE_FREED_MEMORY,Y, asz );
    }
#else
    (void) ptr;
#endif    
}



EXPORT_C void *malloc(size_t sz)
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];


  if (TEST_IN_MALLOC)
  {
    return get_malloc() (sz);
  }
  ret = get_malloc() (sz);

  init( ret );

  if (TRACE_MEM)
  {

    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, NEWPTR "malloc size=");
    SBUF_fmt_size_t(&sbuf, sz );
    SBUF_add_s(&sbuf, " ptr=0x");
    SBUF_fmt_ptr(&sbuf, ret );
    SBUF_add_s(&sbuf, "\n" );

    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }

  return ret;
}

EXPORT_C void *valloc(size_t sz)
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];

  ret = get_valloc() (sz);

  init( ret );

  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, NEWPTR "valloc size=");
    SBUF_fmt_size_t(&sbuf, sz );
    SBUF_add_s(&sbuf, " ptr=0x");
    SBUF_fmt_ptr(&sbuf, ret );
    SBUF_add_s(&sbuf, "\n" );

    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }    


  return ret;
}



EXPORT_C void *realloc( void *ptr, size_t sz )
{
  size_t old_size = 0;
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 

  if (ptr) {
    old_size = malloc_usable_size(ptr);
  }

  // memory is freed if size is 0
  if (!sz && ptr) {
#ifdef ZERO_MEM
    memset(ptr, FILL_BYTE_FREED_MEMORY, old_size);
#endif  

    if (TRACE_MEM)
    {
      SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
      SBUF_add_s(&sbuf, FREEPTR "free by realloc ptr=0x");
      SBUF_fmt_ptr(&sbuf, ptr );
      SBUF_add_s(&sbuf, "\n" );

      TRACE_STACK

      write(log_file, sbuf.buf, sbuf.pos );
    }  
    return get_realloc() ( ptr, 0 );
  }

  ret = get_realloc() (ptr, sz);
 
  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, NEWPTR "realloc size=");
    SBUF_fmt_size_t(&sbuf, sz );
    SBUF_add_s(&sbuf, " inptr=0x");
    SBUF_fmt_ptr(&sbuf, ptr );
    SBUF_add_s(&sbuf, " ptr=0x");
    SBUF_fmt_ptr(&sbuf, ret );
    SBUF_add_s(&sbuf, "\n" );

    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }

#ifdef ZERO_MEM
  // same memory pointer
  if (ret) {
     size_t nsz = malloc_usable_size(ret);
     
     // if size increased - alloc new memory. (probably incorrect, since old_size is larger than requested memory.
     if (nsz > old_size && ret) {
       memset( BPTR_AT(ret, old_size), FILL_BYTE_MALLOCED_MEMORY, nsz - old_size);
     }
  } 
#endif

  return ret;

}

EXPORT_C void free(void *ptr)
{
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  
  deinit( ptr );
  
  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, FREEPTR "free ptr=0x");
    SBUF_fmt_ptr(&sbuf, ptr );
    SBUF_add_s(&sbuf, "\n" );

    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }

  get_free() (ptr);
} 

EXPORT_C int posix_memalign(void **memptr, size_t boundary, size_t size)
{
  int ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 

  if (! *memptr ) {
     return EINVAL;
  } 

  ret = get_posix_memalign() (memptr, boundary, size);
  if (!ret) {
    init( *memptr );
  }

  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, NEWPTR "posix_memalign size=");
    SBUF_fmt_size_t(&sbuf, size );
    SBUF_add_s(&sbuf, " boundary=");
    SBUF_fmt_size_t(&sbuf, boundary);
    SBUF_add_s(&sbuf, " ptr=0x");
    SBUF_fmt_ptr(&sbuf, *memptr );
    SBUF_add_s(&sbuf, "\n" );

    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }

  return 0;  
}

EXPORT_C void *memalign(size_t boundary, size_t size)
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 

  ret = get_memalign() (boundary, size);
  
  if (ret) {
    init( ret );
  }
  
  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, NEWPTR "posix_memalign size=");
    SBUF_fmt_size_t(&sbuf, size );
    SBUF_add_s(&sbuf, " boundary=");
    SBUF_fmt_size_t(&sbuf, boundary);
    SBUF_add_s(&sbuf, " ptr=0x");
    SBUF_fmt_ptr(&sbuf, ret );
    SBUF_add_s(&sbuf, "\n" );

    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }

  return ret;  
}

EXPORT_C int mallopt(int arg, int value )
{
    if (dlsym_nesting) {
      return 0;
    }

    switch(arg) {
	case SET_STRACE_ON_OFF: {
	    TRACE_MEM = value;
	}
	break;
    }
    return get_mallopt() (arg, value );
}


#ifdef  __cplusplus

void* operator new(size_t sz) 
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  ret = get_malloc() (sz);

  init( ret );

  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, NEWPTR "new size=");
    SBUF_fmt_size_t(&sbuf, sz );
    SBUF_add_s(&sbuf, " ptr=0x");
    SBUF_fmt_ptr(&sbuf, ret );
    SBUF_add_s(&sbuf, "\n" );

    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }

  return ret;
}


void* operator new[] (size_t sz) 
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  ret = get_malloc() (sz);

  init( ret );

  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, NEWPTR "new[] size=");
    SBUF_fmt_size_t(&sbuf, sz );
    SBUF_add_s(&sbuf, " ptr=0x");
    SBUF_fmt_ptr(&sbuf, ret );
    SBUF_add_s(&sbuf, "\n" );
  
    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }

  return ret;
}


void operator delete (void *ptr) 
{
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  deinit( ptr );
  
  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, FREEPTR "delete ptr=0x");
    SBUF_fmt_ptr(&sbuf, ptr );
    SBUF_add_s(&sbuf, "\n" );
 
    TRACE_STACK
 
    write(log_file, sbuf.buf, sbuf.pos );
  }

  get_free() (ptr);
}


void operator delete[] (void *ptr) 
{
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  deinit( ptr );
  
  if (TRACE_MEM)
  {
    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, FREEPTR "delete[] ptr=0x");
    SBUF_fmt_ptr(&sbuf, ptr );
    SBUF_add_s(&sbuf, "\n" );
 
    TRACE_STACK

    write(log_file, sbuf.buf, sbuf.pos );
  }
  get_free() (ptr); 
}

#endif


