#include <unistd.h>



void __attribute__((constructor)) mdbg_init_mem_alloc(void)
{
  write(2,"init\n",5);
}

void __attribute((destructor)) mdbg_cleanup_mem_alloc(void)
{
  write(2,"destr\n",6);
}

