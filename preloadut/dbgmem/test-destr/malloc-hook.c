#ifndef __GNUC__
#define __GNUC__
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>




typedef void * (*PFN_hook) (size_t sz, const void *caller );

PFN_hook init_hook;

__thread int is_enabled;

void *my_malloc_hook( size_t sz, const void *caller )
{
    void *ret;
    
    if (!is_enabled) {
	return init_hook( sz, caller );
    }
    is_enabled = 0;
    ret = init_hook( sz, caller );
    printf( "malloc hook size=%llu caller=%p ret=%p\n", (long long unsigned int) sz, caller, ret );
    is_enabled = 1;
    return ret;
}

static void my_init_hook()
{
  //init_hook = __malloc_hook;
    init_hook = __libc_malloc;
    __malloc_hook =  my_malloc_hook;
    is_enabled = 1;
}


void (*__malloc_initialize_hook) (void) = my_init_hook;



main(int argc, char *argv[])
{
    char *str;

    str = strdup( argv[0] );

    printf("the duplicate %p, the original %p\n", str, argv );

    return 0;
}
