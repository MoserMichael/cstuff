#include <dlfcn.h>
#include <string.h>
#include "ld.h"

int SHLIB_load( SHLIB *lib, const char *name )
{
  lib->last_error = 0;
  lib->handle = dlopen( name, RTLD_NOW );
  if (!lib->handle) {
    char *ret;
    ret = dlerror();
    if (ret)
	lib->last_error = strdup( ret );
  }
  return lib->handle == 0;
}
void * SHLIB_get_proc_addr( SHLIB *lib, const char *symbol )
{
  void *ret;
 
  ret = dlsym( lib->handle, symbol );
  if (!ret) {
    char *ret;
    ret = dlerror();
    if (ret)
	lib->last_error = strdup( ret );
  }
  return ret;
}

int SHLIB_unload( SHLIB *lib )
{
  if (lib->handle) {
    dlclose( lib->handle );
    lib->handle = 0;
  }
  return 0;
}



