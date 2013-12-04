#include <stdio.h>
#include <limits.h>

static __thread int is_in_malloc = 0;

main()
{
  printf("%d\n",is_in_malloc);
  return 0;
}
