/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>

void __cyg_profile_func_exit( void *func_addr, void *call_site ) 
{
#if 0
  (void) func_addr;
  (void) call_site;

  write(1,"enter\n",6);
#endif

#if 1
  void *ptr =  __builtin_frame_address(1);
 

  fprintf(stderr, "exit ip: %p call-site: %p bp: %p - [bp]: %p\n", func_addr, call_site,  ptr,  * ((void **) ptr) );
#endif  

}

void __cyg_profile_func_enter(  void *func_addr,  void *call_site )
{
  int rt;
  void *rptr[1];
 #if 0
  (void) func_addr;
  (void) call_site;
  write(1,"exit\n",5);

  fprintf(stderr, "enter %p %p\n", func_addr, call_site );
#endif

#if 1 
  void *ptr =  __builtin_frame_address(1); 
  if (ptr == 0) {
    ptr = rptr[2]; // one off the stack used to be the return address.
  }

  rt = backtrace(rptr, 2 );
  if (rt > 0) {
     if (rptr[0] != ptr) {
	fprintf(stderr,"backtrace and __builtin_frame_addr not the same (backtrace %p builtin_frame_addr %p)\n", rptr[0], ptr);
     }
  } else {
     fprintf(stderr,"No backtrace.\n");
  }
 
  (void) call_site;

  fprintf(stderr, "enter ip: %p call-site: %p bp: %p - [bp]: %p\n", func_addr, call_site, ptr,  * ((void **) ptr) );
#endif  
}
