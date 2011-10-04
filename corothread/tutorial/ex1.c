#include <stdio.h>
#include <stdint.h>

void show_fibonacci_sequence() 
{
  uint64_t a=0, b=1, n, i;


  for(i=0; i < 90; i++) {
    n = a + b;

    printf( "%lld\n", n );
    
    a=b;
    b=n;
  } 
}

int main()
{
  printf("A sequence of fibonacci numbers\n");
  show_fibonacci_sequence();
  return 0;
}
