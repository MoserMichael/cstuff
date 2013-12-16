#include <stdio.h>
#include <stdint.h>


class MyEx
{
public:
  MyEx(int error_code_) : error_code( error_code_ ) {
  }

  int error_code;
};

int64_t fact(int n) {
   int one = 1;

   fprintf(stderr,"--fact arg: %p stackvar: %p\n", &n, &one );
   if ( n == 1 ) {
      fprintf(stderr,"Throwing exception\n");

      throw MyEx(1);
      return 1;
   }
   return n * fact( n - 1 );
}


int main()
{
   int a  = 0;

   fprintf(stderr," *** Main func: %p sp: %p\n", main, &a );
   try {
     printf("Result is %lld\n", fact( 10 ) );
   } catch( MyEx &msg ) {
      fprintf(stderr,"Exception caught. error code %d\n", msg.error_code );
   }

   return 0;
}


