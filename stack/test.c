/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


char * crash_me()
{
      int one = 1;
      char *ptr = malloc(3);

#if 0    
      //gcc 4.5.2 with optimization says that this will kill the program ! 
      memset(ptr,10,50);
      memset(&one,10,50);
#else

     static int i;
     static char *p;

     for(p= (char *) &one,i=0;i<150;i++) { *p++ = 10; }
     //for(p= (char *) ptr,i=0;i<50;i++) { *p++ = 10; }

#endif
      return ptr;
}

int64_t fact(int n) {
   int one = 1;

   fprintf(stderr,"--fact arg: %p stackvar: %p\n", &n, &one );
   if ( n == 1 ) {
     crash_me();
     return 1;
   }
   return n * fact( n - 1 );
}


int64_t do_it()
{
  return fact( 10 );
}


int main()
{
   int a  = 0;

   fprintf(stderr," *** Main func: %p sp: %p\n", main, &a );

   printf(
#if __WORDSIZE == 64
   "Result is %ld\n"
#else
   "Result is %lld\n"
#endif
   , do_it( 10 ) );
   return 0;
}
