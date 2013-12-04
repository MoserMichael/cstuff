
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

#include <stdlib.h>
#include <string.h>
#include "load.h"

static int	dlsym_nesting = 0;
static size_t	alloc_generation = 1;
static int	check_adjacent_nodes = 10;
static int 	fill_byte = 1;

#define		STACK_FRAMES  3

PFN_malloc get_malloc() { return malloc; }
PFN_free   get_free()   { return free; }


#ifdef WIN32
#define backtrace( a, b ) (0)
#endif

#include "simple_test.c"

static int attach_debugger()
{
  CTEST_FAIL("test failed");
  return 0;
}

#include "log.c"

#include "fixed_size_alloc.c"

#include "dlist.c"

#include "radix_tree.c"

#include "mem_range.c"

#include "mem_stack.c"
	
#include "mem_arena.c"

#include "area_alloc.c"

#define  ALLOC_NUM 10000

#if 1
#define SANITY { char *tmp; tmp = malloc(8192); free(tmp); }
#else
#define SANITY 
#endif

void fixed_mem_test()
{
  FIXED_SIZE_ALLOC_HDR * hdr;
  int i;
  size_t all[ ALLOC_NUM ],  all2[ ALLOC_NUM ];
  int *val;
  int cval = 0;
  
  CTEST_INIT("fixed memory allocator");
  
  hdr = FIXED_SIZE_ALLOC_init(5,sizeof(int));

  for(i=0;i<ALLOC_NUM;i++) {
    all[i] = FIXED_SIZE_ALLOC_malloc(&hdr);
    CTEST_ASSERT_NEQ( all[i], -1 ); 
      
    val = (int *) FIXED_SIZE_GET_PTR_CHECKED(hdr,all[i]);
    CTEST_ASSERT_0( val ); 
	
	*val = cval ++;
  }
  
  for(i=0;i<ALLOC_NUM;i++) {
    FIXED_SIZE_ALLOC_free(hdr, all[i]);
  }

  for(i=0;i<ALLOC_NUM;i++) {
    all2[i] = FIXED_SIZE_ALLOC_malloc( &hdr);
    CTEST_ASSERT_NEQ( all2[i], -1 ); 

    val = (int *) FIXED_SIZE_GET_PTR_CHECKED(hdr,all2[i]);
    CTEST_ASSERT_0( val ); 
    
    *val = cval ++;
  }

  for(i=0;i<ALLOC_NUM;i++) {
	CTEST_ASSERT_EQ( all[i], all2[ ALLOC_NUM - i - 1] );
  }

  SANITY
  CTEST_OK();
}

#define MAX_RANGES 1000

struct tagRange
{
	size_t ptr_low;
	size_t ptr_high;
}
    range[ MAX_RANGES ];

#define MAX_PTR 100000

void alloc_test()
{
  void *ptr[ MAX_PTR ];
  size_t i;

  CTEST_INIT("alloc text");

  MENTRY_SIZE = sizeof(MEM_ENTRY) + sizeof(void *) * STACK_FRAMES;
  CTEST_ASSERT_EQ( MEM_ENTRY_init(STACK_FRAMES) , 0 );

  for(i=0;i<MAX_PTR;i++) {
     size_t sz;
	 void **frames;
	 size_t j;

	 sz = 1 + (i % 100);
	 ptr[i] = alloc_mem( sz, 0, &frames, ALLOC_MALLOC);
     for(j=1;j < STACK_FRAMES; j++) { frames[j] = (void *) 2; } 

	 memset( ptr[i], 0, sz );
  }

  for(i=0;i<MAX_PTR;i++) {
  
	  free_mem( ptr[i], ALLOC_MALLOC );
  }

  SANITY
  CTEST_OK();
}

#define TEST_ALIGN 32

void  memalign_alloc_test()
{
  void *ptr[ MAX_PTR ];
  size_t i;

  CTEST_INIT("memalign text");

  MENTRY_SIZE = sizeof(MEM_ENTRY) + sizeof(void *) * STACK_FRAMES;
  CTEST_ASSERT_EQ( MEM_ENTRY_init(STACK_FRAMES) , 0 );

  for(i=0;i<MAX_PTR;i++) {
     size_t sz;
	 void **frames;
	 size_t j;

	 sz = 1 + (i % 100);
	 ptr[i] = alloc_mem( sz, TEST_ALIGN,  &frames , ALLOC_MALLOC);

	 CTEST_ASSERT_EQ(  ((size_t) ptr[i]) & (TEST_ALIGN - 1), 0);

     for(j=1;j < STACK_FRAMES; j++) { frames[j] = (void *) 2; } 

	 memset( ptr[i], 0, sz );
  }

  for(i=0;i<MAX_PTR;i++) {
  
	  free_mem( ptr[i] , ALLOC_MALLOC );
  }

  SANITY
  CTEST_OK();
}

void mem_range_test()
{
  size_t i,sz=10,low=5,ret;
  int rval, is_in_range;
  
  CTEST_INIT("memory ranges");

  CTEST_ASSERT_EQ( MEM_ENTRY_init(3) , 0 );

  for(i=0;i<MAX_RANGES;i++) {
	range[ i ].ptr_low = low;
	range[ i ].ptr_high =  low + sz;
	
	low = range[ i ].ptr_high + sz;
	sz += 1;

	CTEST_ASSERT_0( 
		MEM_ENTRY_new_range( (void *) range[ i ].ptr_low, 
							 range[ i ].ptr_high - range[ i ].ptr_low,
							 &ret, 
							 ALLOC_MALLOC) );

  }
  low += 10;

  for(i=0;i<low;i++) {
	  int j;
	  MEM_ENTRY *ret;

	  ret = MEM_ENTRY_find( (const void *) i, (const void *) i );
	  rval = ret != 0 ? 1 : 0;

	  if (ret) {
		CTEST_ASSERT_TRUE( (size_t) ret->ptr <= (size_t) i && ((size_t) ret->ptr + (size_t) ret->udata_size) > (size_t) i);
	  }

	  is_in_range = 0;
	  for(j=0;j<MAX_RANGES;j++) {
		  if (i >= range[j].ptr_low && i<range[j].ptr_high) {
			is_in_range = 1;

			if ((void *) range[j].ptr_low != ret->ptr || 
				ret->udata_size != BPTR_DIFF(range[j].ptr_high,range[j].ptr_low) )  {
				is_in_range = 0;
			}

			break;
		  }
	  }
	  CTEST_ASSERT_EQ( rval, is_in_range ); 
  }

  MEM_ENTRY_release();
  
  SANITY
  CTEST_OK();
}

void mem_range_with_delete()
{
  size_t i,j,sz=10,low=5,ret;
  int rval, is_in_range, max_range;
  //FILE *fp = fopen("ranges.txt","w");
  
  CTEST_INIT("memory ranges with delete");

  CTEST_ASSERT_EQ( MEM_ENTRY_init(3) , 0 );

  for(j=i=0;i<MAX_RANGES;i++) {
	
	if (i % 5 != 0) {  
	  range[ j ].ptr_low = low;
	  range[ j ].ptr_high =  low + sz;

	  //fprintf(fp,"+[%d %d]\n", range[ j ].ptr_low,  range[ j ].ptr_high );
 
	  low = range[ j ].ptr_high + sz;
	  sz += 1;

	  CTEST_ASSERT_0( 
		MEM_ENTRY_new_range( (void *) range[ j ].ptr_low, 
							 range[ j ].ptr_high - range[ j ].ptr_low,
							 &ret, 
							 ALLOC_MALLOC) );
	  j++;
	} else {
#if 1
	  int high = low + sz;
	  
	  low = high + 10;
	  
	  //fprintf(fp,"-[%d %d]\n", low,  high );

	  CTEST_ASSERT_0( 
		MEM_ENTRY_new_range( (void *) low, 
							 high-low,
							 &ret, 
							 ALLOC_MALLOC) );
		
	  CTEST_ASSERT_NEQ(
	    MEM_EMTRY_free_range( (void *) low ), -1 );
#endif
	}
  }
  //fclose(fp);
  low += 10;
  max_range = j;

  for(i=0;i<low;i++) {
	  int j;
	  MEM_ENTRY *ret;
	
	  ret =  MEM_ENTRY_find( (const void *) i, (const void *) i );
	  rval = ret != 0 ? 1 : 0;

	  if (ret) {
		CTEST_ASSERT_TRUE( (size_t) ret->ptr <= (size_t) i && ((size_t) ret->ptr + (size_t) ret->udata_size) > (size_t) i);
	  }

	  is_in_range = 0;
	  for(j=0;j<max_range;j++) {
		  if (i >= range[j].ptr_low && i<range[j].ptr_high) {
			is_in_range = 1;
			if ((void *) range[j].ptr_low != ret->ptr || 
				ret->udata_size != BPTR_DIFF(range[j].ptr_high,range[j].ptr_low) )  {
				is_in_range = 0;
			}
			break;
		  }
	  }
	  CTEST_ASSERT_EQ( rval, is_in_range ); 
  }
  
  MEM_ENTRY_release();
  
  SANITY
  CTEST_OK();
}

void func(int depth, char *ptr);

void do_func(int depth, char *aptr)
{
      char ptr[10];

      //fprintf(stderr, "# %x %x %x #\n", (unsigned long) &depth, (unsigned long)  &aptr, (unsigned long) &ptr ); 
     func( depth, ptr );
 
}

void  func(int depth, char *ptr)
{  
   	
   if (depth == 3) {
     do_func( depth+1, ptr );
     return;
   }
   if (depth == 5) {
      int i;
      for( i=0;i<11;i++) {
         CTEST_ASSERT_EQ( check_stack_pointer( ptr, i ) , 0 ); 
      }
      // have to add for function argument, and there is some padding between args and locals!
       CTEST_ASSERT_EQ( check_stack_pointer( ptr, 50  ) , -1 );	   // 25    
      return;
   }
   func( depth+1, ptr );
}

void stack_test()
{
  CTEST_INIT("stack check");
  func(0,0);
  CTEST_OK();
}

int main()
{	 
  fixed_mem_test();	 

  mem_range_test();
  
  mem_range_with_delete();

  alloc_test();

  memalign_alloc_test();

#ifndef WIN32
  stack_test();
#endif	  

  CTEST_EXIT();

  return 0;
}

