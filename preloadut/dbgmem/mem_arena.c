
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

#include "memalloc.h"

//---------------------------------------------------------------------------
// arena entry lib 
//---------------------------------------------------------------------------
typedef struct tagARENA_ENTRY {
//size_t mem_entry_offset;
#if __WORDSIZE == 32
  void   *stam_alignment; // so that resulting struct will be 8 bytes long.
#endif
  void   *guard_ptr;
}   
  ARENA_ENTRY;

#define GET_USER_DATA_LOW_GUARD(mem)  (((ARENA_ENTRY*) (mem)->ptr ) - 1)->guard_ptr      
#define SET_USER_DATA_LOW_GUARD(mem,value)  GET_USER_DATA_LOW_GUARD(mem) = (value);      


#define GET_USER_DATA_HIGH_GUARD(mem) (* VVPTR_AT( (mem)->ptr, (mem)->udata_size ) )
#define SET_USER_DATA_HIGH_GUARD(mem,value) GET_USER_DATA_HIGH_GUARD(mem) = (value);

static int is_entry_ok(MEM_ENTRY *mem)
{
  int ret = 0;
  void *low;

  low = GET_USER_DATA_LOW_GUARD( mem ) ; 
  if ( low != SENTINEL && low != SENTINEL_VALIGN) {
    ret = 1;
  } else if ( GET_USER_DATA_HIGH_GUARD( mem ) != SENTINEL) {
    ret = 2;
  }

  return ret;
}

static void show_status(int status, char *buf, size_t buf_len, MEM_ENTRY *ptr)
{

  if (status == 0) {
    strcpy(buf,"OK");
  } else {
    if (status == 1) {
       snprintf(buf,buf_len,"[memory low mark overwritten ptr=%p size=%zd]", ptr->ptr, ptr->udata_size);
    } else 
    if (status == 2) {
       snprintf(buf,buf_len,"[memory upper mark overwritten ptr=%p size=%zd]", ptr->ptr, ptr->udata_size);
    } else {
       snprintf(buf,buf_len,"[unknown status ptr=%p]",ptr->ptr);
    }
  } 
}

static void do_print_frames(MEM_ENTRY *mentry,int is_err)
{
  int i;
     
   for(i=0; i < STACK_FRAMES && mentry->frames[i]!=0;  i++) {
     dbgmem_log_ext ( is_err, "\tframe %d : %p\n", i, mentry->frames[i] );
   }
}

#define DBG_ERRROR_REPORT(ptr,offset_frames) \
    DBG_TRACE_HERE(offset_frames); \
    if (ptr) { \
      dbgmem_log_err("-allocated block stack:\n"); \
      do_print_frames( ptr, 1 ); \
    } \
    attach_debugger();



STATIC_C int check_block( DLIST *entry)
{
  int status = 0;
  char buf[100];
  MEM_ENTRY *mentry;
  
  mentry = (MEM_ENTRY *) entry;    
   
  status = is_entry_ok( mentry);
  if (status) {
    dbgmem_log_err( "ERROR Invalid memory block. Entry: %p size: %zd generation: %d\n",
	       mentry->ptr,
	       mentry->udata_size,
	       GET_GENERATION(mentry->generation)
	   );
    show_status( status, buf, sizeof(buf), mentry );
    dbgmem_log_err( "status %s\n", buf );
    do_print_frames( mentry, 1);
    attach_debugger();
    return 1;
  }
  return 0;
}

STATIC_C int print_block( DLIST *entry)
{
  int status = 0;
  char buf[100];
  MEM_ENTRY *mentry;
     
  mentry = (MEM_ENTRY *) entry;
 
  status = is_entry_ok( mentry);
  show_status( status, buf, sizeof(buf), mentry );
     
  dbgmem_log_ext(status, "Entry: %p size: %zd generation: %d\n",
	       mentry->ptr,
	       mentry->udata_size,
	       GET_GENERATION(mentry->generation)
    	   );
  dbgmem_log_ext(status, "status %s\n", buf );
  
  do_print_frames( mentry, status );
  if (status) {
    attach_debugger();
  }
  return 0;
}

#define check_adjacent_node_list(mentry) \
  if (check_adjacent_nodes) { \
    if (DLIST_walk_nodes( g_mem_entry_alloc, (DLIST *) mentry, check_adjacent_nodes, 1, check_block) || \
        DLIST_walk_nodes( g_mem_entry_alloc, (DLIST *) mentry, check_adjacent_nodes, 0, check_block)) { \
       DBG_ERRROR_REPORT(mentry,2); \
    } \
  }



STATIC_C int MEM_ENTRY_check_exact_ptr( const void *udata, MEM_ENTRY **ret, const char *function_name )
{ 
  MEM_ENTRY *entry = 0;
  int status;
  
  if (udata == 0) {
     // pointe must may not be null pointer.
     dbgmem_log_err("MEMCHECK Error in %s: null pointer argument \n", 
		 function_name );
     goto eof_error;
  }

  entry = MEM_ENTRY_find( udata, udata );
  if (!entry) {
     dbgmem_log_err("MEMCHECK Error in %s: pointer %p is not from any allocated range.\n",
	         function_name,
	         udata );
     
     goto eof_error;
  }

  if (ret) {
    *ret = entry;
  } 

  if (entry->ptr != udata) {

    dbgmem_log_err("MEMCHECK Error in %s: passing pointer %p from within content of allocated range.[allocated block %p length %zd]\n",
	         function_name,
	         udata,
  	         entry->ptr,
	         entry->udata_size);
   
    goto eof_error;
   
  } 

  status =  is_entry_ok(entry);
  if (status == 1) {

    dbgmem_log_err("MEMCHECK Error in %s: lower bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         entry->ptr,
	         entry->udata_size);
 
    goto eof_error;

  }

  if (status == 2 )  {
    dbgmem_log_err("MEMCHECK Error in %s: lower bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         entry->ptr,
	         entry->udata_size);
 
    goto eof_error;
  }

  check_adjacent_node_list(entry);

  return 0;

eof_error:
  DBG_ERRROR_REPORT(entry,3); 
  return 1;
  
}

EXPORT_C int DBGMEM_GET_BLOCK_SIZE( const void *arg, size_t *ret_size)
{ 
   MEM_ENTRY *ret; 

   if ( MEM_ENTRY_check_exact_ptr( arg, &ret, "DBGMEM_GET_BLOCK_SIZE" )) {
     return -1;
   }

   if (ret_size) {
     *ret_size = ret->udata_size;
   }
   return 0;

}

typedef enum {
  MREAD,
  MWRITE,
} 
  MACCESS_TYPE;

typedef enum {
  MSTRREAD,
  MSTRCPY,
  MSTRNCPY,
  MSTRCAT,
} 
  MACCESS_TYPE_STR;


static const char *access_name( MACCESS_TYPE access)
{
    switch( access ) {
      case MREAD:
        return "Reading";
      case MWRITE:
        return "Writing";
    }
    return "???";
}

#if 0
static const char *access_name_str( MACCESS_TYPE_STR access)
{
    switch( access ) {
      case MSTRREAD:
        return "Reading";
      case MSTRCPY:
      case MSTRNCPY: 
      case MSTRCAT: 
        return "Writing"; 
    }
    return "???";
}
#endif


EXPORT_C int DBGMEM_CHECK_MEMORY_RANGE(const void *arg_ptr, size_t length,  MACCESS_TYPE atype, const char *function_name)
{
    MEM_ENTRY *ptr;
    int status;
    
    if (!g_mem_entry_alloc) {
      return 0;   
    }
    
    ptr = MEM_ENTRY_find( arg_ptr, BPTR_AT( arg_ptr, length ) );
    if (ptr) {
	unsigned char *range_eof;  
        unsigned char *data_eof; 

	status = is_entry_ok( ptr );
	if (status == 1) {

	    dbgmem_log_err("DBGMEM: Error in %s: lower bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;

	}

	if (status == 2 )  {
	    dbgmem_log_err("DBGMEM: Error in %s: upper bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
	}

	range_eof = BPTR_AT( ptr->ptr, ptr->udata_size );
	data_eof = BPTR_AT( arg_ptr, length );

	if (ptr->ptr <= arg_ptr) {
	     if (range_eof < data_eof) {
	       dbgmem_log_err("DBGMEM: Error in %s: %s access skips %zu bytes past end of allocated block. [allocated block %p length %zd ]\n",
	         function_name,
	         access_name( atype ),
	         data_eof - range_eof,
	         ptr->ptr,
	         ptr->udata_size );
	       goto eof_error;
	     }
	} else {
	     dbgmem_log_err("DBGMEM: Error in %s: %s %zu bytes before start of allocated block. [allocated block %p length %zd]\n",
	       function_name,
	       access_name( atype ),
	       (size_t) ptr->ptr - (size_t) arg_ptr,
	       ptr->ptr,
	       ptr->udata_size);
	     goto eof_error;
	}
       
        check_adjacent_node_list(ptr);  
       	
    } else {
	if (atype == MWRITE) {
     	  if (check_stack_pointer( arg_ptr, length)) {

	     dbgmem_log_err("DBGMEM: Error in %s: overwriting stack frame [argument pointer %p argument size %zd]\n",
	       function_name,
	       arg_ptr,
	       length);
	     goto eof_error;
 	  }
	}

	    
    }
 
    return 0;

eof_error:
    DBG_ERRROR_REPORT(ptr,2); 
 
    return 1;
}

#define MEM_MIN(a,b) ((a) < (b) ? (a) : (b))


EXPORT_C size_t  DBGMEM_CHECK_STRING_RANGE(const void *arg_ptr, size_t str_bound, size_t string_length, MACCESS_TYPE_STR atype, const char *function_name)
{
    MEM_ENTRY *ptr;
    unsigned char *ptr_eof,*rt;
    size_t max_size;
    int status;

    if (!g_mem_entry_alloc) {
      return (size_t) -1;   
    }
  
    ptr = MEM_ENTRY_find( arg_ptr, arg_ptr );
    if (!ptr) { // No entry is covering argument pointer
      if (atype == MSTRREAD) { // read only access
        void *rs;
	
	if (str_bound == (size_t) -1) {
	  return (size_t) strlen((const char *) arg_ptr);
	}

	rs = memchr( (void *)  arg_ptr, 0, str_bound );
	if (rs) {
	  return (size_t) rs - (size_t) arg_ptr;
	}
	return (size_t) str_bound;
	
      }	else { // modify accesss (string_length not -1 in this case)
	  if (check_stack_pointer( arg_ptr, string_length )) {    // check if stack overwrite.
	     dbgmem_log_err("DBGMEM: Error in %s: access is overwriting stack frame [argument pointer %p argument size %zd]\n",
	       function_name,
	       arg_ptr,
	       str_bound);
	     goto eof_error;
 	  }
      }
      return 0;
    }
    
    // have entry that covers this pointer
    status = is_entry_ok( ptr );
    if (status == 1) {

	    dbgmem_log_err("DBGMEM: Error in %s: lower bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;

    }

    if (status == 2 )  {
	    dbgmem_log_err("DBGMEM: Error in %s: higher bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
    }

    
    ptr_eof = BPTR_AT( ptr->ptr, ptr->udata_size );
    max_size = (size_t) ptr_eof - (size_t) arg_ptr;
   

    switch(atype) {
     case MSTRREAD: {  // source argument of str functions
        rt = (unsigned char *) memchr( arg_ptr, '\0', max_size );
	if (str_bound != (size_t) -1) { // src of strncpy ; n in strncpy is equal to str_bound
	  if (rt == 0) {
	     if (max_size < str_bound) {
	       dbgmem_log_err("DBGMEM: Error in %s: argument string %p is not null terminated. will read past heap block! [allocated block %p length %zd]\n", 
	         function_name,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	       goto eof_error;
	     }
	     return str_bound;
	  } 
	  return BPTR_DIFF(rt, arg_ptr); // real string length
 	
	} else { // src of strcpy

          if (rt == 0) {
	    dbgmem_log_err("DBGMEM: Error in %s: argument string %p is not null terminated. will read past heap block! [allocated block %p length %zd]\n", 
	         function_name,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	   goto eof_error;
	}
        return BPTR_DIFF(rt, arg_ptr); // real string length
    }
    break;
 
    case MSTRNCPY:{  // target argument of strncpy
 
	 if (max_size < str_bound ) { // overwriting the target string.
	    dbgmem_log_err("DBGMEM: Error in %s: will overwrite buffer. writing %zd bytes, target buffer is %zd bytes long at %p [allocated block %p length %zd]\n",
	         function_name,
		 str_bound, 
		 max_size,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
	 }

#if 0
	 if (str_bound == string_length) { // strcpy will leave target non null terminated
	    dbgmem_log_err("DBGMEM: Warning in %s: result is not null terminated string. writing %zd bytes into target length of equal size at %p [allocated block %p length %zd]\n",
	         function_name,
		 str_bound,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
	} 
#endif
	}
	break;

    case MSTRCPY: { // target of strcpy
         if (max_size <  (string_length + 1) ) { // will overwrite target buffer.
	    dbgmem_log_err("DBGMEM: Error in %s: will overwrite target, writing %zd bytes, target buffer is %zd bytes long at %p [allocated block %p length %zd]\n",
	         function_name,
		 string_length+1,
		 max_size,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
	 }
	}
	return 0;
    } break;

    case MSTRCAT: { // target of strcat or strncat
      size_t nsz,new_string;
      
      rt = (unsigned char *) memchr( arg_ptr, '\0', max_size ); 
      if (rt == 0) { //target buffer not null terminated, will now overwrite target.
	dbgmem_log_err("DBGMEM: Error in %s: will overwrite buffer %p that is not null terminated within allocated block! [allocated block %p length %zd]\n", 
	         function_name,

		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	goto eof_error;
      }
      nsz  = BPTR_DIFF(rt, arg_ptr);
      new_string = nsz  + string_length + 1; 
      if ( new_string > max_size) {
	dbgmem_log_err("DBGMEM: Error in %s: will now overwrite target, writing %zd bytes, target buffer is %zd bytes long at %p [allocated block %p length %zd]\n", 
	         function_name,
		 new_string,
	         max_size,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	goto eof_error;
	
      }
      return 0;
   }
   break;
   }
   return 0;

eof_error:
    DBG_ERRROR_REPORT(ptr,2); 
    return 1;
}


EXPORT_C size_t  DBGMEM_CHECK_WSTRING_RANGE(const wchar_t *arg_ptr, size_t str_bound, size_t string_length, MACCESS_TYPE_STR atype, const char *function_name)
{
    MEM_ENTRY *ptr;
    wchar_t *ptr_eof;
    const wchar_t *rt;
    size_t max_size;
    int status;

    if (!g_mem_entry_alloc) {
      return (size_t) -1;   
    }
  
    ptr = MEM_ENTRY_find( arg_ptr, arg_ptr );
    if (!ptr) { // No entry is covering argument pointer
      if (atype == MSTRREAD) { // read only access
        const wchar_t *rs;
	
	if (str_bound == (size_t) -1) {
	  return (size_t) wcslen(arg_ptr);
	}

	rs = wmemchr( arg_ptr, 0, str_bound );
	if (rs) {
	  return rs - arg_ptr;
	}
	return (size_t) str_bound;
	
      }	else { // modify accesss (string_length not -1 in this case)
	  if (check_stack_pointer( arg_ptr, string_length * sizeof(wchar_t))) {    // check if stack overwrite.
	     dbgmem_log_err("DBGMEM: Error in %s: access is overwriting stack frame [argument pointer %p argument size %zd]\n",
	       function_name,
	       arg_ptr,
	       str_bound);
	     goto eof_error;
 	  }
      }
      return 0;
    }
    
    // have entry that covers this pointer
    status = is_entry_ok( ptr );
    if (status == 1) {

	    dbgmem_log_err("DBGMEM: Error in %s: lower bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;

    }

    if (status == 2 )  {
	    dbgmem_log_err("DBGMEM: Error in %s: higher bound of memory block is overwritten. [allocated block %p length %zd]\n",
	         function_name,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
    }

    
    ptr_eof = (wchar_t *) BPTR_AT( ptr->ptr, ptr->udata_size );
    max_size = ((size_t) ptr_eof - (size_t) arg_ptr) / sizeof(wchar_t);
   

    switch(atype) {
     case MSTRREAD: {  // source argument of str functions
        rt = wmemchr( arg_ptr, '\0', max_size  );
	if (str_bound != (size_t) -1) { // src of strncpy ; n in strncpy is equal to str_bound
	  if (rt == 0) {
	     if (max_size < str_bound) {
	       dbgmem_log_err("DBGMEM: Error in %s: argument string %p is not null terminated. will read past heap block! [allocated block %p length %zd]\n", 
	         function_name,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	       goto eof_error;
	     }
	     return str_bound;
	  } 
	  return rt - arg_ptr; // real string length
 	
	} else { // src of strcpy

          if (rt == 0) {
	    dbgmem_log_err("DBGMEM: Error in %s: argument string %p is not null terminated. will read past heap block! [allocated block %p length %zd]\n", 
	         function_name,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	   goto eof_error;
	}
        return BPTR_DIFF(rt, arg_ptr) / sizeof(wchar_t); // real string length
    }
    break;
 
    case MSTRNCPY:{  // target argument of strncpy
 
	 if (max_size < str_bound ) { // overwriting the target string.
	    dbgmem_log_err("DBGMEM: Error in %s: will overwrite buffer. writing %zd bytes, target buffer is %zd bytes long at %p [allocated block %p length %zd]\n",
	         function_name,
		 str_bound, 
		 max_size,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
	 }

#if 0
	 if (str_bound == string_length) { // strcpy will leave target non null terminated
	    dbgmem_log_err("DBGMEM: Warning in %s: result is not null terminated string. writing %zd bytes into target length of equal size at %p [allocated block %p length %zd]\n",
	         function_name,
		 str_bound,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
	} 
#endif
	}
	break;

    case MSTRCPY: { // target of strcpy
         if (max_size <  (string_length + 1) ) { // will overwrite target buffer.
	    dbgmem_log_err("DBGMEM: Error in %s: will overwrite target, writing %zd bytes, target buffer is %zd bytes long at %p [allocated block %p length %zd]\n",
	         function_name,
		 string_length+1,
		 max_size * sizeof(wchar_t),
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	    goto eof_error;
	 }
	}
	return 0;
    } break;

    case MSTRCAT: { // target of strcat or strncat
      size_t nsz,new_string;
      
      rt = wmemchr( arg_ptr, '\0', max_size ); 
      if (rt == 0) { //target buffer not null terminated, will now overwrite target.
	dbgmem_log_err("DBGMEM: Error in %s: will overwrite buffer %p that is not null terminated within allocated block! [allocated block %p length %zd]\n", 
	         function_name,

		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	goto eof_error;
      }
      nsz  = BPTR_DIFF(rt, arg_ptr);
      new_string = nsz  + string_length + 1; 
      if ( new_string > max_size) {
	dbgmem_log_err("DBGMEM: Error in %s: will now overwrite target, writing %zd bytes, target buffer is %zd bytes long at %p [allocated block %p length %zd]\n", 
	         function_name,
		 new_string,
	         max_size,
		 arg_ptr,
  	         ptr->ptr,
	         ptr->udata_size);
	goto eof_error;
	
      }
      return 0;
   }
   break;
   }
   return 0;

eof_error:
    DBG_ERRROR_REPORT(ptr,2); 
    return 1;
}


static int is_in_range(const void *test, const void *from, size_t length)
{
  if ( test > from && test < (const void *) BPTR_AT(from, length)) {
    return 1;
  }  
  return 0;	 
}
 
STATIC_C void CHECK_OVERLAP(const void *dest, size_t dest_length, const void *src, size_t src_length, const char *function_name)
{
  if (! dest_length ) {
    return;
  }

  if (is_in_range( dest, src, src_length) || 
      is_in_range( BPTR_AT( dest, dest_length - 1), src, src_length) ) {
	 
	 dbgmem_log_err("DBGMEM: Warning in in %s: overlap of source range %p length %zd and destination buffer %p.\n",
	         function_name,
		 src,
		 src_length,
		 dest);
 
	DBG_ERRROR_REPORT(0,2); 
  }
}







