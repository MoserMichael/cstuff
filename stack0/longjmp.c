/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

jmp_buf targ;

int64_t fact(int n) {
   int one = 1;

   fprintf(stderr,"--fact arg: %p stackvar: %p\n", &n, &one );
   if ( n == 1 ) {
#if 0  
      memset(&one,100,100);    
#endif      
      fprintf(stderr,"lets do the time jump again...\n");
      longjmp(targ, 1);
      return 1;
   }
   return n * fact( n - 1 );
}


int main()
{
   int a  = 0;

   fprintf(stderr," *** Main func: %p sp: %p\n", main, &a );

   if (setjmp(targ) != 0) {
     fprintf(stderr,"*** Returned from setjmp ***\n");
     return 0;
   }

   printf("Result is %lld\n", fact( 10 ) );
   return 0;
}
