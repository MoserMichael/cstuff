#ifndef __LDL_H_
#define __LDL_H_

typedef struct tagSHLIB {
  void *handle;
  char *last_error;
} SHLIB;

int SHLIB_load( SHLIB *lib, const char *name );
void * SHLIB_get_proc_addr( SHLIB *lib, const char *symbol );
int SHLIB_unload( SHLIB *lib );

#endif


