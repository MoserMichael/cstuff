#ifndef _INC_H_Y_Z_
#define _INC_H_Y_Z_

#include <cutils/array.h>

typedef ARRAY INC_PATH;;

INC_PATH *INC_PATH_init( const char *exe_path );
void INC_PATH_add( INC_PATH *inc_path, const char *path, int add_last );
char * INC_PATH_resolve( INC_PATH *inc_path, const char *name );

char * INC_PATH_to_text( INC_PATH *inc_path );


#endif

