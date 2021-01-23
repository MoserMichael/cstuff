#include <stdio.h>
#include <stdlib.h>

void * really_do_leak(size_t sz)
{
   return malloc( sz );
}

void * do_leak(size_t sz)
{

    return really_do_leak( sz );
}    
