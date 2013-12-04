
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

#include <stdio.h>
#include <stdlib.h>

#include "simple_list.c"

//#pragma pack(1)
typedef struct tagMEM_ENTRY {
  MDLIST entry;
  unsigned int generation;
  size_t size;
  void *frames[0];
} MEM_ENTRY;
//#pragma pack()
static size_t     ARENA_SIZE;
static MEM_ENTRY  root;

#define GET_USER_DATA(ptr) (void *) (BPTR(ptr) + ARENA_SIZE)
#define GET_USER_DATA(ptr) (void *) (BPTR(ptr) + ARENA_SIZE)

#define GET_USER_DATA_LOW_GUARD(ptr)  (void **) (BPTR(ptr) + ARENA_SIZE - sizeof(void*))

#define GET_USER_DATA_HIGH_GUARD(ptr) (void **) (BPTR(ptr) + ARENA_SIZE + ((MEM_ENTRY*)ptr)->size)

#define GET_MENTRY_FROM_UDATA(ptr) (MEM_ENTRY *) (BPTR(ptr) - ARENA_SIZE)


#define DBG_ERRROR_REPORT(mentry,offset_frames) \
   DBG_TRACE_HERE(offset_frames); \
   if (ptr) { \
      int j; \
      dbgmem_log_err("-allocated block stack:\n"); \
       \
      for(j=0; j < STACK_FRAMES && mentry->frames[j]!=0;  j++) { \
        dbgmem_log_err( "\tframe %d : %p\n", j, mentry->frames[j] ); \
      }\
    } \
    attach_debugger();



static void init_alloc()
{   
  ARENA_SIZE = sizeof(MEM_ENTRY) + ((STACK_FRAMES + 1) * sizeof(void *));

  // memory returned by malloc has to be aligned to 8 bytes (has to be aligned to sizeof(double)) 
  ARENA_SIZE = DO_ALIGN( ARENA_SIZE, 8 );

  MDLIST_init(&root.entry);
}  

static int get_arena_size()
{
  return ARENA_SIZE;
}


#include "valloc.c"

static void * alloc_mem(size_t sz, size_t align, void ***frames, unsigned int allocation_type)
{
  MEM_ENTRY *mem;

  (void) allocation_type;

  mem = (MEM_ENTRY *) common_alloc_mem( sz, align, ARENA_SIZE, sizeof(void *) );

#if 0
   dbgmem_log( "real malloc returned %p sz %d ARENA_SIZE %d\n", mem, sz, ARENA_SIZE );
#endif

  if (mem) {
    mem->size = sz; 
#ifndef  __cplusplus
    mem->generation = alloc_generation;
#else 
    mem->generation = MAKE_GENERATION( alloc_generation, allocation_type ); 
#endif

    MT_LOCK( &memlock );
    
    MDLIST_add( &root.entry, &mem->entry );
    
    * (GET_USER_DATA_LOW_GUARD(mem)) = !align ? SENTINEL : SENTINEL_VALIGN;
    * (GET_USER_DATA_HIGH_GUARD(mem)) = SENTINEL;


    MT_UNLOCK( &memlock );

    *frames = (void **) mem->frames;

    return GET_USER_DATA( mem );
 
  } else {
  
    return 0;
  }
}

static int is_entry_ok(MEM_ENTRY *mem)
{
  int ret = 0;
  void *low_value; 

  low_value = * GET_USER_DATA_LOW_GUARD(mem);

  if ( low_value != SENTINEL && low_value != SENTINEL_VALIGN) {
    if (low_value != SENTINEL_FREED) {
      ret = 1;
    } else {
      ret = 4;
    }
  } else 
  if ( * GET_USER_DATA_HIGH_GUARD(mem) != SENTINEL) {
    ret = 2;
  }

  return ret;
}

static void show_status(int status, char *buf,size_t buf_len, MEM_ENTRY *ptr)
{
  if (status == 0) {
    strcpy(buf,"OK");
  } else {
    strcpy(buf,"");
    if (status & 4) { 
         snprintf(buf, buf_len, "[memory block already freed. ptr=%p size=%zd] ", GET_USER_DATA(ptr), ptr->size);
    
    }
    
    if (status & 1) {
         snprintf(buf, buf_len, "[memory low mark overwritten ptr=%p size=%zd] ", GET_USER_DATA(ptr), ptr->size);
    } else if (status & 2) {
         snprintf(buf, buf_len, "[memory upper mark overwritten ptr=%p size=%zd]", GET_USER_DATA(ptr), ptr->size);
    } 
  } 
}



static size_t mem_size(void *ptr)
{
  MEM_ENTRY *mentry = GET_MENTRY_FROM_UDATA(ptr); 
  return mentry->size;
}

static int check_block(MDLIST *entry )
{
  int status;
  char buf[100];
  int i;
  MEM_ENTRY *mentry = (MEM_ENTRY *) entry;    

  status = is_entry_ok( mentry);
  if (status) {
    dbgmem_log_ext(status, "ERROR Invalid memory block. Entry: %p size: %zd generation: %d\n",
            GET_USER_DATA(mentry),
            mentry->size,
            GET_GENERATION(mentry->generation)
	   
	   );
    show_status( status, buf, sizeof(buf), mentry );
    dbgmem_log_ext(status,"status %s\n", buf );
      
    for(i=0; i < STACK_FRAMES && mentry->frames[i]!=0;  i++) {
      dbgmem_log_ext(status, "\tframe %d : %p\n", i, mentry->frames[i] );
    }
    attach_debugger();
    return 1;
  }
  return 0;
}

#define check_adjacent_node_list(mentry) \
  if (check_adjacent_nodes) { \
    if (MDLIST_walk_nodes( &root.entry, (MDLIST *) mentry, check_adjacent_nodes, 1, check_block) || \
        MDLIST_walk_nodes( &root.entry, (MDLIST *) mentry, check_adjacent_nodes, 0, check_block)) { \
       DBG_ERRROR_REPORT(mentry,2); \
    } \
  }

static void free_mem(void *ptr , unsigned int allocation_type )
{
  int status;
  int is_valign;
  MEM_ENTRY *mentry;

  (void) allocation_type;

  // dbgmem_log("free %p\n",ptr);
  
 
  mentry = GET_MENTRY_FROM_UDATA(ptr); 
  // dbgmem_log("free entry %p\n",mentry);
  
  status = is_entry_ok(mentry);
  if (status != 0) {
     char buf[100];

     show_status(status, buf, sizeof(buf), mentry);
     
     dbgmem_log_err( "ERROR Freeing invalid memory block. Entry: %p size: %zd generation: %d\n",
	       GET_USER_DATA(mentry),
	       mentry->size,
	       GET_GENERATION(mentry->generation)
     );

     dbgmem_log_err( "status %s\n", buf );
     
     DBG_ERRROR_REPORT(mentry,2);

  }  else {
     check_adjacent_node_list( mentry );	  
  } 
#ifdef  __cplusplus
   if (cpp_warnings && allocation_type !=  GET_ALLOC_TYPE(mentry->generation)) {
       
     dbgmem_log_err( "ERROR Memory allocated with %s but freed with %s. Entry: %p size: %zd generation: %d\n",
	       alloc_name(  GET_ALLOC_TYPE(mentry->generation) ),
	       free_name( allocation_type ),
	       GET_USER_DATA(mentry),
	       mentry->size,
	       GET_GENERATION(mentry->generation)
     );
     dbgmem_log_err( "status OK\n" );
     DBG_ERRROR_REPORT(mentry,2);
   }
#endif

  
  is_valign = * GET_USER_DATA_LOW_GUARD(mentry) == SENTINEL_VALIGN;
  MT_LOCK( &memlock );
  MDLIST_remove( &mentry->entry );
  * (GET_USER_DATA_LOW_GUARD(mentry)) = SENTINEL_FREED;
  MT_UNLOCK( &memlock );
  
#if 0  
  // TEMPORARY
  unsigned char *break_ptr =  (unsigned char *) 0x84f52c0;
  if ( (unsigned char*) ptr < break_ptr  &&  ((unsigned char *) ptr + mentry->size) >  break_ptr) {
    attach_dbg();
  }
#endif

  // freed memory is initialised with some value, so that access to free memory will cause some error
  if (fill_byte)  {
     memset(ptr,FILL_BYTE_FREED_MEMORY,mentry->size);
  }  
  
  common_free(mentry, is_valign);
}



static int do_check_memory()
{
  int num_errors;

  MT_LOCK( &memlock );
  num_errors = MDLIST_walk_block_list( &root.entry, check_block ); 
  MT_UNLOCK( &memlock );
  
  return num_errors;
}

static int print_block( MDLIST *entry)
{
  int status = 0;
  char buf[100];
  int i;
  MEM_ENTRY *mentry = (MEM_ENTRY *) entry;    


  status = is_entry_ok( mentry);
  show_status( status, buf, sizeof(buf), mentry );
 
  dbgmem_log_ext(status, "Entry: %p size: %zd generation: %d\n",
	       GET_USER_DATA(mentry),
	       mentry->size,
	       GET_GENERATION(mentry->generation) );
  dbgmem_log_ext(status,  "status %s\n", buf );
      
  for(i=0; i < STACK_FRAMES && mentry->frames[i]!=0;  i++) {
     dbgmem_log_ext(status, "\tframe %d : %p\n", i, mentry->frames[i] );
  }
  if (status) {
    attach_debugger();
  }
  return 0;
}

static void do_print_leaks(void)
{
   dbgmem_log( "MEMALLOC: SIMPLE_ALLOC\n");

  MT_LOCK( &memlock );
  MDLIST_walk_block_list( &root.entry,  print_block );
  MT_UNLOCK( &memlock );
  
   dbgmem_log( "MEMALLOC: EOF\n");
}



    
