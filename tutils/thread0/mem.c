#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/user.h>

int main()
{
  int i;
  void *ret;

  for(i = 0; ; i++) {
         ret = mmap(0, PAGE_SIZE * 2, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
	 if (ret == (void *) -1) {
	    fprintf(stderr," Allocation failed #%d. errno %d\n", i, errno);
	    exit(1);
         }
#if 1	 
	 * (( char *) ret) = 0;
#endif	 
  }
  return 0;
}

