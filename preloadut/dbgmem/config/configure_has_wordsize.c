#include <limits.h>
#include <stdio.h>

int main()
{
#if 0
  fprintf(stdout,"%d",__WORDSIZE);
#endif
  
#ifdef __WORDSIZE
  fprintf(stdout,"0");
#else
  fprintf(stdout,"%d",sizeof(int) * 8 );
#endif
  return 0;
}
