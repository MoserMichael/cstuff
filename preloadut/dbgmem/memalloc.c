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

#include "config.h"
#include <execinfo.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <wchar.h>
#include <string.h>
#include <dlfcn.h>

#include "../common.h"
#include "memalloc.h"
#include "load.h"

#define REPORT_FILE_NAME "DBGMEM_report_%d%s.log"
#define REPORT_FILE_NAME_MMAP "DBGMEM_report_mmap_%d%s.log"

#include "version.h"


STATIC_C const char *VERSION = DBG_VERSION;

/* *** thunks *** */
static int	dlsym_nesting = 0;

MAKE_FUNC( malloc );
MAKE_FUNC( calloc );
MAKE_FUNC( realloc );
MAKE_FUNC( free );
MAKE_FUNC( getcwd );
MAKE_FUNC( __getcwd_chk );
MAKE_FUNC( mallopt );
MAKE_FUNC( strdup );
MAKE_FUNC( __strdup );
MAKE_FUNC( wcsdup );
MAKE_FUNC( strndup );
MAKE_FUNC( __strndup );

/* *** parameters *** */
static int	STACK_FRAMES = 3;
static int	ignore_this_process = 0;
static int	attach_gdb_on_errors = 0;  
static int 	check_adjacent_nodes = 0;
static int 	fill_byte = 0;
static int      set_segv_handler = 0;
static int	verbose_mode = 0;
static int      set_next_generation_signal = 0; 
static int      set_dump_blocks_signal = 0;
static int      follow_forks = 0; 
static int      cpp_warnings = 0;

/* *** global state *** */
static unsigned int alloc_generation = 0;
static int	dump_id = 0;
static int	record_trace = 1;

#if 0
static int      init_pthread_stack = 0;
static void    *pthread_low_address;
static void    *pthread_high_address;
#endif

static void dump_maps();


#include "../log.c"

#include "fixed_size_alloc.c"

#include "dlist.c"

#include "proc_mmap.c"

#include "attach_dbg.c"

#ifdef  _SIMPLE_TOOL_

/*
 * memory tracking allocator for finding leaks.
 */
#include "simple_alloc.c"

#else

/*
 * memory allocator that tracks memory to find leaks, and check memory access in common library functions.
 */
#include "radix_tree.c"

#include "mem_range.c"

#include "mem_stack.c"

#include "mem_arena.c"

//#include "mem_check.c"
//
STATIC_C void init_alloc();

#include "area_alloc.c"

#endif


//------------------------------------------------
// globals
//------------------------------------------------

typedef struct tagMMAP_MEM_ENTRY {
  DLIST entry;
  void *ptr;
  unsigned int generation;
  size_t udata_size;
  void  *frames[0];
} 
  MMAP_MEM_ENTRY;




//------------------------------------------------
// globals
//------------------------------------------------

static char memblock_nonreclaimable[ 32 * 1024 ]; // 4096 ]; 
static size_t non_reclaimable_ptr = 0;

static void *alloc_nonreclaimable(size_t sz)
{
  void *ret;

  if ( (non_reclaimable_ptr + sz) > sizeof(memblock_nonreclaimable) ) {
    dbgmem_log_err("DBGMEM: eof non reclaimable memory;\n");
    return 0;
  }
  
  ret = memblock_nonreclaimable + non_reclaimable_ptr;
  non_reclaimable_ptr += sz; 

  return ret;
  
}

static int is_init = 0;

#ifdef __RECURSIVE_MALLOC

static __thread int is_in_malloc = 0;

#define ENTER_MALLOC  is_in_malloc = 1;
#define EXIT_MALLOC   is_in_malloc = 0;
#define TEST_IN_MALLOC  is_in_malloc

#else

#define ENTER_MALLOC
#define EXIT_MALLOC
#define TEST_IN_MALLOC  0

#endif
  

static void mmap_init();


static void init_libs()
{
  get_malloc();
  get_calloc();
  get_free();
  get_realloc();
  get_mallopt();
  
  get_getcwd();
  get___getcwd_chk();

  get_strdup();
  get___strdup();
  
  get_wcsdup();
  
  get_strndup();
  get___strndup();
 
  if (!ignore_this_process) {
     mmap_init();
  }

#if 0
  get_lib_address("/lib/tls/libpthread", &pthread_low_address, &pthread_high_address);
#endif

}

static int not_empty_str(const char *opt) 
{
  const char *cur;
  
  for(cur = opt; *cur != '\0';++cur) {
    if (*cur != ' ' || *cur != '\t') {
      return 1;
    }
  }
  return 0;
}

static void dump_command_line(const char *event);

static int  get_params(const char *event)
{
  // environment does not always work here, for whatever reasons.
  // therefore we get parameters from a parameter file.
  char *opt;
  char buf[ PATH_MAX + 100 ], param_file[100], out_file[100];
  int  fd,i,n;
  
  snprintf(param_file,sizeof(param_file), "/tmp/dbgmem_param_%d",getppid());
  snprintf(out_file,sizeof(out_file), "/tmp/dbgmem_param_%d",getpid());

  fd = open(param_file,O_RDONLY);
  if (fd == -1) {
    snprintf(param_file,sizeof(param_file), "/var/tmp/dbgmem_param_%d",getppid());
    snprintf(out_file,sizeof(out_file), "/var/tmp/dbgmem_param_%d",getpid());
    
    fd = open(param_file,O_RDONLY);
    if (fd == -1) {
      snprintf(param_file, sizeof(param_file), "~/dbgmem_param_%d",getppid());
      snprintf(out_file, sizeof(out_file), "~/dbgmem_param_%d",getpid());
      
      fd = open(param_file,O_RDONLY);
    }
  }  
 
  if (fd != -1) {  
    n = read(fd,buf,sizeof(buf)-1);
    if (n > 0) {
      buf[n] = '\0';

      opt = strtok(buf,",");
      for(i=0; opt; ++i,opt=strtok(0,",")) {
        switch(i) {
	  case 0: 
            if (opt != 0) {
             pid_t pid = atoi( opt );
             if (getppid() == pid) {
               ignore_this_process = 1;
             }
           }
	   break;
	  case 1:
	   STACK_FRAMES = atoi( opt );
	   break;
	  case 2:
           attach_gdb_on_errors = atoi( opt );
           break;
	  case 3: 
	   check_adjacent_nodes = atoi( opt );
           break;
	  case 4: 
	   fill_byte = atoi( opt );
	   break;
	  case 5:
	   set_segv_handler = atoi(opt);
	   break;
	  case 6:
	   verbose_mode = atoi(opt);
	   break;
	  case 7:
	   set_next_generation_signal = atoi(opt);
	   break;
	  case 8:
	   set_dump_blocks_signal = atoi(opt);
	   break;
	  case 9:
	   follow_forks = atoi(opt);
	   break;
	  case 10:
	   cpp_warnings = atoi(opt);
	   break;
	  case 11:
	   if (not_empty_str(opt)) {
  	     log_set_dir(opt);
	   }
	   break;
	} // eof switch  
      } // eof for 
     
      close(fd);

      if (follow_forks) {
	copy_file( param_file, out_file, 0 );
      } else {
	unlink(param_file); 
      }	

      dump_command_line( event );
  
      return 0;
    }       
  } else {
#if 0  
    fprintf(stderr,"DBGMEM: no parameter file for process %d, ignoring process\n", getpid());
#endif    
    ignore_this_process = 1;
  }
  
  return -1;
}


static void next_generation_signal(int sig)
{
  (void) sig;

  mallopt(CHECK_MEMORY,0);
  mallopt(INC_GENERATION_VAL,0);
 
  // those apps that block SIGINT - now is the evil undone.
  //signal(SIGINT,SIG_DFL);
}

static void dump_blocks_signal(int sig)
{
  (void) sig;

  mallopt(DUMP_MEMORY,0); 
  
  // those apps that block SIGINT - now is the evil undone.
  // signal(SIGINT,SIG_DFL);
  
}

static void fatal_error(int sig)
{
  dbgmem_log_err("ERROR: FATAL SIGNAL %d\n", sig);
  
  attach_gdb_on_errors=0; 
  DBG_TRACE_HERE(0);
 
  dump_maps();
  do_check_memory();
  
  signal(sig, SIG_DFL);
  kill(getpid(), sig);

}

static void int_handler(int sig)
{
  (void) sig;
  exit(0);
}

static const char *get_allocator_mode_name()
{
#ifdef _SIMPLE_TOOL_
  return "simple";
#else
#ifdef _ADVANCED_TOOL_ 
  return "checking tool";
#else
  return "???";
#endif
#endif
}

static void init_memdbg(const char *event_name)
{
  get_params( event_name );
    
  init_libs();  

  if (verbose_mode) {
      const char *tool_name = 
#ifdef _SIMPLE_TOOL_
  	"simple";
#else
#ifdef _ADVANCED_TOOL_
	"checking";
#else
	0;
#error "undefined tool name"
#endif
#endif

    fprintf(stderr,"\nDBGMEM Options:\n\t"\
		   "Tool name: %s\n\t"\
       		   "process id: %d\n\t"\
		   "Follow forks: %s\n\t"\
		   "ignore this process: %s\n\t"\
		   "allocator: %s\n\t"\
		   "stack frames: %d\n\t"\
		   "action on memory errors: %s\n\t"\
		   "check adjacent nodes: %s\n\t"\
		   "init new & freed memory: %s\n\t"\
		   "SIGSEGV/SIGBUS error handler: %s\n\t"\
		   "Install signal for leak detection: %s\n\t\t"\
		   "signal to increment generation tag: %d\n\t\t"\
		   "signal to dump heap contents: %d\n\t"
		   "attached on: %s\n\t"
#ifdef __cplusplus
		   "check for c++ errors : %s\n\t"
#endif
		   "log directory: %s\n\n",

	  tool_name,
       	  getpid(),
	  follow_forks ? "on" : "off", 
	  ignore_this_process ? "on" : "off", 
	  get_allocator_mode_name(), 
	  STACK_FRAMES, 
	  get_attach_debugger_mode(),
	  check_adjacent_nodes ? "on" : "off", 
	  fill_byte ? "on" : "off", 
	  set_segv_handler ? "on" : "off", 
          set_next_generation_signal || set_dump_blocks_signal ? "on" : "off",
	  set_next_generation_signal,
	  set_dump_blocks_signal,
	  event_name,
#ifdef __cplusplus
	  cpp_warnings ? "on" : "off",
#endif
	  log_dir);
  }
   
  if (!ignore_this_process) {  
    char file_name[ sizeof(REPORT_FILE_NAME) + 20 ];

    snprintf(file_name, sizeof(file_name), REPORT_FILE_NAME, getpid(), "" );
    
    log_open( file_name, 1);

    init_alloc();

    set_signal( SIGINT, int_handler);
    set_signal( SIGHUP, int_handler);
    set_signal( SIGPIPE, int_handler);
    set_signal( SIGALRM, int_handler);
    set_signal( SIGTERM, int_handler);
 
    if (set_segv_handler) {
      //signal(SIGSEGV, fatal_error);
      //signal(SIGBUS, fatal_error);

      set_signal( SIGSEGV, fatal_error);
      set_signal( SIGBUS, fatal_error);
    }

    if (set_next_generation_signal) {
      set_signal( set_next_generation_signal, next_generation_signal);
    }
    if (set_dump_blocks_signal) {
      set_signal( set_dump_blocks_signal, dump_blocks_signal);
    }


  }
}


static void do_init()
{
  if (is_init) {
    return;
  }
  is_init ++; 
  set_core_unlimited();
  init_memdbg( "execve" );
#if 0
  fprintf(stderr,"do_init finished %d\n", ignore_this_process);
#endif

}


void filter_cmd(unsigned char *buf, int len)
{
  int i;

  for(i=0;i<len;i++) {
    if (buf[i] == '\0') {
      buf[i] = ' ';
    }
  }

}

static void dump_command_line(const char *event)
{
    char cmd[40];
    
    dbgmem_log("LOGGING START\n");

    if (event) {
       dbgmem_log("Initiated by: %s\n", event );
    }
    
    dbgmem_log("\tcommand line: ");
    snprintf(cmd, sizeof(cmd), "/proc/%d/cmdline", getpid());
    copy_file_to_log(cmd, filter_cmd);
    dbgmem_log("\n");

    dbgmem_log("Process id: %d\n", getpid());
    dbgmem_log("Parent process id: %d\n", getppid());

    dbgmem_log("LOGGING EOF\n");
    
    
}

static void print_meminfo()
{
   struct mallinfo info = mallinfo();

    dbgmem_log("MALLINFO START\n"),
    
    dbgmem_log("\tnon-mmaped space allocated from system: %d\n", info.arena);
    dbgmem_log("\tnumber of free chunks: %d\n", info.ordblks);
    dbgmem_log("\tnumber of fastbin blocks: %d\n",info.smblks);
    dbgmem_log("\tnumber of mmapped regions: %d\n", info.hblks); 
    dbgmem_log("\tspace in mmapped regions: %d\n", info.hblkhd);   
    dbgmem_log("\tmaximum total allocated space: %d\n", info.usmblks);  
    dbgmem_log("\tspace available in freed fastbin blocks: %d\n", info.fsmblks);  
    dbgmem_log("\ttotal allocated space: %d\n", info.uordblks);  
    dbgmem_log("\ttotal free space: %d\n", info.fordblks); ; 
    dbgmem_log("\ttop-most, releasable (via malloc_trim) space: %d\n", info.keepcost);  
    dbgmem_log("\tDBGMEM arena size: %d\n", get_arena_size());
    
    dbgmem_log("MALLINFO EOF\n");

}

static void print_leaks(int on_exit)
{
  if (!ignore_this_process) {  
    if (verbose_mode) {
      if (on_exit) {
        LOAD_TRACE("DBGMEM: on_exit\n");
      } 
      LOAD_TRACE("DBGMEM: logging all heap blocks... \n");
    }

    print_meminfo();
    do_print_leaks( );
    log_close();

    // copy /proc/self/mmap to target directory.
    dump_maps();

    if (on_exit) {     
#ifdef  _TRACE_MMAP_
      char file_name[ sizeof(REPORT_FILE_NAME) + 20 ];

      snprintf( file_name, sizeof(file_name), REPORT_FILE_NAME, getpid(), "" );
      log_init( file_name ); 
      do_print_mmaps();
      log_close();
#endif
    }
  }  
}


//------------------------------------------------
// internal allocation helper functions
//------------------------------------------------


//------------------------------------------------
// weird logic
// 1 - on some machines dlsym is allocating memory internally, when we call dlsym there is still no glibc, we come first;
//     therefore we got alloc_nonreclaimable
// 2 - on glibc version > 2.3 the backtrace function is allocating memory internally (but only once)
//     therefore test if we are in malloc - with thread local variable - (see TEST_IN_MALLOC, ENTER_MALLOC macros)
//------------------------------------------------
#define CUSTOM_MALLOC_ALIGN(sz,alignment,fill,allocator_type) \
({ \
  void **pframes; \
  int nframes; \
  int i; \
  void *ret = 0; \
   \
  if (!real_malloc) { \
    ret = alloc_nonreclaimable(sz); \
  } else if (TEST_IN_MALLOC) { \
    ret = get_malloc()(sz); \
  } else { \
    ENTER_MALLOC \
     \
    ret  = alloc_mem( (sz), (alignment), &pframes, allocator_type); \
    if (ret) { \
      void *sframes[ STACK_FRAMES + 1 ];\
      \
      if (record_trace) { \
        nframes = backtrace( sframes, STACK_FRAMES + 1); \
        for(i=1;i < nframes; i++) { pframes[i-1] = sframes[i]; } \
        if (nframes < STACK_FRAMES) pframes[nframes-1] = 0;\
      } else{ \
        pframes[0] = 0;\
      } \
	  \
    } else {\
       dbgmem_log_err("DBGMEM: [allocation failed. very interesting. size=%d]\n",sz);\
    }\
    EXIT_MALLOC; \
    if (fill && fill_byte && ret) { \
      memset(ret,FILL_BYTE_MALLOCED_MEMORY,sz); \
    } \
  }\
  ret; \
})

#define CUSTOM_MALLOC(sz,fill,allocator_type) CUSTOM_MALLOC_ALIGN(sz,0,fill,allocator_type) 

static inline void CUSTOM_FREE(void *ptr,unsigned int allocator_type )
{
  if (!ptr) {
    return; 
  }
  if  ((char *) ptr > &memblock_nonreclaimable[0] && 
        (char *) ptr < ( &memblock_nonreclaimable[0] + sizeof( memblock_nonreclaimable)) ) {
    return ;
  }
  free_mem(ptr, allocator_type);
}

//------------------------------------------------
// wrappers of C allocation functions
//------------------------------------------------

EXPORT_C void __attribute__((constructor)) mdbg_init_mem_alloc(void)
{
  do_init();
}

EXPORT_C void __attribute((destructor)) mdbg_cleanup_mem_alloc(void)
{
  print_leaks(1);
}

EXPORT_C void *malloc(size_t sz)
{
  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    return CUSTOM_MALLOC(sz,1, ALLOC_MALLOC);
  } else {
    return get_malloc() (sz);
  }
}

EXPORT_C void *calloc( size_t count, size_t size )
{
  size_t sz;   
  void *ret;

  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    sz = count * size;
    ret = CUSTOM_MALLOC(sz,0, ALLOC_MALLOC);
    if (ret) {
      memset( ret, 0, sz );
    }
  } else {
    ret = get_calloc() (count,size); 
  }
  return ret;
}

EXPORT_C void *realloc( void *ptr, size_t size )
{
  size_t sz;
  void *new_ptr;
  
  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    if (ptr == 0) {
      return CUSTOM_MALLOC(size,1, ALLOC_MALLOC);
    }

    if (size == 0) {
      CUSTOM_FREE(ptr, ALLOC_MALLOC);
      return 0;
    }    

    sz = mem_size( ptr );
    if (sz == (size_t) -1) {
      return 0;
    }  
    
    if (size <= sz) {
      return ptr;
    }

    new_ptr = CUSTOM_MALLOC(size,0, ALLOC_MALLOC);
    if (new_ptr) {
      memcpy( new_ptr, ptr, sz );
      CUSTOM_FREE(ptr, ALLOC_MALLOC);

      if (fill_byte && size > sz) {
        memset( BPTR_AT( new_ptr, sz), FILL_BYTE_MALLOCED_MEMORY , size - sz);
      }
      
      return new_ptr;
    } else {
      return 0;
    }
  
  } else {
    return get_realloc() (ptr,size);  
  }
}

EXPORT_C size_t malloc_usable_size(void *ptr)
{
  size_t sz;
  
  if (dlsym_nesting) {
    return 0;
  }
  sz = mem_size( ptr );
  if (sz == (size_t) -1) {
    return 0;
  }
  return sz;
}

EXPORT_C void free(void *ptr)
{
  if (dlsym_nesting) {
    return;
  }

  if (!ignore_this_process) {
    CUSTOM_FREE(ptr, ALLOC_MALLOC);
  } else {
    get_free() (ptr);
  }
} 

#ifdef strdup
#define save_strdup strdup
#undef strdup
#endif

EXPORT_C char * strdup(const char *arg) 
{
  char *cpy;
  size_t len;

  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    len = strlen(arg);
    cpy = (char *) CUSTOM_MALLOC(len+1,1, ALLOC_MALLOC);
    if (!cpy) {
      return 0;
    } 
    strcpy( cpy, arg );
    return cpy; 
  
  } else {
    return get_strdup()(arg); 
  }
  
}

#ifdef save_strdup 
#define strdup save_strdup
#endif


#ifdef __strdup
#define save_strdup2 __strdup
#undef __strdup
#endif


EXPORT_C char * __strdup(const char *arg) 
{
  char *cpy;
  size_t len;

  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    len = strlen(arg);
    cpy = (char *) CUSTOM_MALLOC(len+1,1, ALLOC_MALLOC);
    if (!cpy) {
      return 0;
    } 
    strcpy( cpy, arg );
    return cpy; 
  
  } else {
    return get___strdup()(arg); 
  }
}

#ifdef save_strdup2
#define __strdup save_strdup2
#endif


#ifdef strndup
#define save_strndup strndup
#undef strndup
#endif

EXPORT_C char *strndup( const char *arg, size_t nlen)
{
  char *cpy;
  size_t len;

  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {

    len = strnlen( arg, nlen );
    cpy = (char *) CUSTOM_MALLOC( len + 1 , 1 , ALLOC_MALLOC);
    if (!cpy) {
      return 0;
    }
    strncpy(cpy,arg,len);
    cpy[ len ] = '\0';
  } else {
    return get_strndup()(arg,nlen);
  }
  return cpy;
}

#ifdef save_strndup
#define strndup save_strndup
#endif


#ifdef __strndup
#define save_strndup2 __strndup
#undef __strndup
#endif


EXPORT_C char * __strndup(const char *arg, size_t nlen) 
{
  char *cpy;
  size_t len;

  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {

    len = strnlen( arg, nlen );
    cpy = (char *) CUSTOM_MALLOC( len + 1 , 1 , ALLOC_MALLOC);
    if (!cpy) {
      return 0;
    }
    strncpy(cpy,arg,len);
    cpy[ len ] = '\0';
  } else {
    return get___strndup()(arg,nlen);
  }
  return cpy;
}

#ifdef save_strndup2
#define __strndup save_strndup2
#endif



#ifdef wcsdup
#define save_wcsdup wcsdup
#undef wcsdup
#endif


EXPORT_C wchar_t *wcsdup( const wchar_t *arg )
{
  wchar_t *cpy;
  size_t len;

  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {

    len = wcslen( arg );
    cpy = (wchar_t *) CUSTOM_MALLOC( sizeof(wchar_t) * (len + 1) , 1 , ALLOC_MALLOC);
    if (!cpy) {
      return 0;
    }
    wcscpy(cpy,arg);
  } else {
    return get_wcsdup()(arg);
  }
  return cpy;
}

#ifdef save_wcsdup
#define wcsdup save_wcsdup
#endif



EXPORT_C char * getcwd( char *buf, size_t nlen )
{
  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    if (!buf) {
      buf = (char *) CUSTOM_MALLOC( PATH_MAX, 1, ALLOC_MALLOC);
    }
  }
  return get_getcwd() ( buf, nlen);
}

#ifdef __getcwd_chk
#define save_getcwd __getcwd_chk
#undef __getcwd_chk
#endif


EXPORT_C char * __getcwd_chk( char *buf, size_t nlen, size_t nlen2 )
{
  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    if (!buf) {
      buf = (char *) CUSTOM_MALLOC( PATH_MAX, 1, ALLOC_MALLOC);
    }
  }
  return get___getcwd_chk() ( buf, nlen, nlen2 );
}

#ifdef save_getcwd
#define __getcwd_chk save_getcwd
#endif


 
EXPORT_C void *memalign(size_t boundary, size_t size)
{
  if (dlsym_nesting) {
    return 0;
  }

  if (!ignore_this_process) {
    return CUSTOM_MALLOC_ALIGN(size, boundary, 1, ALLOC_MALLOC);
  } else {
    return get_malloc() (size);
  }
}

EXPORT_C void *valloc(size_t size)
{
  return memalign( getpagesize(), size);
}

EXPORT_C int posix_memalign(void **memptr, size_t boundary, size_t size)
{
  if (!memptr) {
     return EINVAL;
  } 
  *memptr = memalign( boundary, size );
  if (*memptr) {
    return 0;
  }
  return errno;
}

EXPORT_C int mallopt(int arg, int value )
{
    if (dlsym_nesting) {
      return 0;
    }

    if (!ignore_this_process) {
	switch(arg) {
		case IS_INSTALLED: {
			return 1;
		}

		case GET_GENERATION_VAL: {
			return alloc_generation;
		}
		
		case INC_GENERATION_VAL: {
			if (verbose_mode) {
			  LOAD_TRACE("DBGMEM: increasing generation value\n"); 
			}
			alloc_generation ++; 
			//record_trace = 1; 
			return alloc_generation;  
		}
		
		case DUMP_MEMORY: {
			char suffix[30];
		        char file_name[ sizeof(REPORT_FILE_NAME) + 20 + sizeof(suffix)];
		
			if (verbose_mode) {
			  LOAD_TRACE("DBGMEM: dumping of heap memory blocks starts\n"); 
			}

			log_push_current_file();
		
			snprintf(suffix, sizeof(suffix), "_snapshot%d", ++ dump_id );
			snprintf(file_name, sizeof(file_name), REPORT_FILE_NAME, getpid(), suffix );
        		log_init( file_name);
        
			print_leaks(0);
			
			log_pop_current_file();
			
			if (verbose_mode) {
			  LOAD_TRACE("DBGMEM: dumping of heap memory blocks finished\n"); 
			}
		}
		break;
		
		case CHECK_MEMORY: {
			int rs;
			
			if (verbose_mode) {
			  LOAD_TRACE("DBGMEM: check heap memory blocks starts\n"); 
			}
			rs = do_check_memory();
			if (verbose_mode) {
			  LOAD_TRACE("DBGMEM: check heap memory finished\n"); 
			}
			return rs;
		}
		
		case SET_STRACE_ON_OFF: {
			record_trace = value;
		}
		break;
	}
    }
    return get_mallopt() (arg, value );
}


//------------------------------------------------
// wrappers of C++ allocation functions
//------------------------------------------------

#ifdef  __cplusplus
void* operator new(size_t sz) 
{
  if (!ignore_this_process) {
    return CUSTOM_MALLOC(sz, 1, ALLOC_NEW);
  } else {
    return get_malloc() (sz);
  }
}


void* operator new[] (size_t sz) 
{

  if (!ignore_this_process) {
    return CUSTOM_MALLOC(sz, 1, ALLOC_NEW_ARRAY);
  } else {
    return get_malloc() (sz);
  }
}


void operator delete (void *ptr) 
{
  if (dlsym_nesting) {
    return;
  }

  if (!ignore_this_process) {
    CUSTOM_FREE(ptr, ALLOC_NEW);
  } else {
    get_free() (ptr);
  }
}


void operator delete[] (void *ptr) 
{
  if (dlsym_nesting) {
    return;
  }

  if (!ignore_this_process) {
    CUSTOM_FREE(ptr, ALLOC_NEW_ARRAY);
  } else {
    get_free() (ptr);
  }
}

#endif


//--------------------------------------------------------------------------------
// wrappers of memory map allocation functions
//
// if you want to see who allocated all those memory mapped files that are left on 
// process termination, then you can enable this feature with -D_TRACE_MMAP_
//
// it is assumed that three are relatively few allocations with mmap (big blocks)
// so tracking can be relatively slow - we put all blocks into linked list.
//--------------------------------------------------------------------------------

#ifdef _TRACE_MMAP_

static pthread_mutex_t mmap_memlock = PTHREAD_MUTEX_INITIALIZER;
static FIXED_SIZE_ALLOC_HDR * g_mmap_entry_alloc;
static int MMAP_MENTRY_SIZE;



static void mmap_init()
{
  MMAP_MENTRY_SIZE = sizeof(MMAP_MEM_ENTRY) + sizeof(void *) * STACK_FRAMES;  
 
  g_mmap_entry_alloc =  FIXED_SIZE_ALLOC_init(1000,MMAP_MENTRY_SIZE);
    if (!g_mmap_entry_alloc) {
      dbgmem_log("DBGMEM: Can't allocate from shared memory (shared memory tracker)\n");
      return;
    }
  DLIST_init( & g_mmap_entry_alloc );
    
 }

static MMAP_MEM_ENTRY *mmap_add(void *ret, size_t len)
{
  size_t elm = FIXED_SIZE_ALLOC_malloc(&g_mmap_entry_alloc);
  if (elm != FIXED_SIZE_EOF) {
    MMAP_MEM_ENTRY *entry = (MMAP_MEM_ENTRY *) FIXED_SIZE_GET_PTR( g_mmap_entry_alloc, elm);

    entry->ptr = ret;
    entry->generation = alloc_generation;  
    entry->udata_size = len;
  
    DLIST_add(g_mmap_entry_alloc, elm );
    return entry;
  }
  return 0;
}

static void mmap_remove(void *ptr)
{
  DLIST *entry;
  MMAP_MEM_ENTRY *mentry;
  size_t mentry_pos;
   

  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( g_mmap_entry_alloc, 0 );
  if (entry->next == 0) {
    return;
  }
  mentry_pos = entry->next;
  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( g_mmap_entry_alloc, mentry_pos );

  while(entry != 0) {

    mentry = (MMAP_MEM_ENTRY *) entry;
    
    if (mentry->ptr == ptr) {
       DLIST_remove( g_mmap_entry_alloc, mentry_pos );
       FIXED_SIZE_ALLOC_free( g_mmap_entry_alloc, mentry_pos ); 
       break;
    }

    mentry_pos = entry->next;
    if (mentry_pos == 0) {
      break;
    }
    
    entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( g_mmap_entry_alloc, mentry_pos );
  }
 
}

static void do_print_mmaps()
{
  DLIST *entry;
  MMAP_MEM_ENTRY *mentry;
  size_t mentry_pos;
  int i; 

  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( g_mmap_entry_alloc, 0 );
  if (entry->next == 0) {
    return;
  }
  mentry_pos = entry->next;
  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( g_mmap_entry_alloc, mentry_pos );

  while(entry != 0) {

    mentry = (MMAP_MEM_ENTRY *) entry;
    
    dbgmem_log( "Entry: %p size: %zd generation: %d\n",
	       mentry->ptr,
	       mentry->udata_size,
	       GET_GENERATION(mentry->generation)
	   );
    
     dbgmem_log(  "status OK\n" );
      
    for(i=0; i < STACK_FRAMES && mentry->frames[i]!=0;  i++) {
        dbgmem_log( "\tframe %d : %p\n", i, mentry->frames[i] );
    }
    
    mentry_pos = entry->next;
    if (mentry_pos == 0) {
      break;
    }
    
    entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( g_mmap_entry_alloc, mentry_pos );
  }
}


#define MMAP_BACKTRACE(map) \
      void *sframes[ STACK_FRAMES + 1 ];\
      void **pframes;\
      int i, nframes; \
       \
      nframes = backtrace( sframes, STACK_FRAMES + 1); \
      for(pframes=(void **) &map->frames,i=1;i < nframes; i++) { pframes[i-1] = sframes[i]; }\
      if (nframes < STACK_FRAMES) pframes[nframes-1] = 0;
 

EXPORT_C void *mmap (void *__addr, size_t __len, int __prot,
		     int __flags, int __fd, __off_t __offset)
{
  void *ret;
  MMAP_MEM_ENTRY *map;  

  ret = get_mmap() ( __addr, __len, __prot, __flags, __fd, __offset);
  
  if (ret != (void *) -1) {
    MT_LOCK( &mmap_memlock );
    map = mmap_add( ret, __len);
    MT_UNLOCK( &mmap_memlock );
    
    if (map) {
      MMAP_BACKTRACE(map)
    }
  }
#if 0  
  if (init_pthread_stack) { 
    void *ret_addr = __builtin_return_address(0);  
    if ( ret_addr > pthread_low_address && ret_addr < pthread_high_address) {
      memset( ret, 0, __len) ;//FILL_BYTE_MALLOCED_MEMORY, __len ); 
    }
  }
#endif  

  return ret;
  
}

EXPORT_C int  munmap (void *__addr, size_t __len)
{
  MT_LOCK( &mmap_memlock );
  mmap_remove( __addr );
  MT_UNLOCK( &mmap_memlock );

  return get_munmap() ( __addr, __len);
}

EXPORT_C void *mremap (void *__addr, size_t __old_len, size_t __new_len, int __may_move)
{
  void *ret;

  ret = get_mremap() ( __addr, __old_len, __new_len, __may_move);
  if (ret != (void *) -1) {
    MMAP_MEM_ENTRY *map;
    
    MT_LOCK( &mmap_memlock );
    mmap_remove(__addr); 
    map = mmap_add( ret, __new_len);
    MT_UNLOCK( &mmap_memlock );
 
    if (map) {
      MMAP_BACKTRACE(map)
    }
  } 
  return ret;
}
#else





static void mmap_init()
{
}

#endif



MAKE_FUNC( fork );

pid_t fork(void)
{
  pid_t ret;
  
  LOAD_TRACE("fork intercepted\n");
  ret =  get_fork() ();
  if (!ret) {

    // new child init.
    if (follow_forks) {
      init_memdbg( "fork" );
    }
    else {
     ignore_this_process = 1; 
    }
  }
  return ret;
}


// intercept dlclose - unloading of shared libraries; don't want to have a situation where stack trace instruction pointer was pointing to an address in library A and later the same adddress would refer to another library.
// disable dlclose, so that shared libraries always stay in memory, this may have adverse effects on memory footprint.
int dlclose(void *arg)
{
    (void) arg;
    return 1;
}


#ifndef  _SIMPLE_TOOL_

#include "mem_check.c"

#endif


