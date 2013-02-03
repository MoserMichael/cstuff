#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "fn.h"

char *posix_search_in_path( const char *file )
{
  char *env, *path, *dir, *test, *ret;

  env = getenv( "PATH" );
  if (!env)
    return 0;
 
  path = strdup( env );
  for (dir = strtok( path, ":"); dir != 0; dir = strtok( 0, ":")) {
     test = FN_make_path( dir, file );     
     if (test) {
       if ( !access( test, R_OK ) ) {
         ret = FN_normalize( test );
	 free( test );
	 return ret;
       }
       free(test);
     }
  }
  free( path );
  return 0;
}


char *posix_get_process_name( const char *argv )
{
  char dir[ PATH_MAX + 1 ], *path, *ret;

  if (argv[0] == '/') 
    return strdup( argv );
 

  if (getcwd( dir, sizeof( dir ) ) == 0)
    return 0;
 
  path = FN_make_path( dir, argv );  
  if (!access( path, R_OK )) {
    ret = FN_normalize( path );
    free( path );
    return ret;
  }
  free(path);

  return posix_search_in_path( argv );
}


char *get_current_process_name( const char *argv0)
{
#ifdef __linux__
  char buf[ PATH_MAX + 1 ];
  
  (void) argv0;
  memset(buf, 0, sizeof(buf));
  if (readlink( "/proc/self/exe", buf, sizeof( buf ) ) == -1)
    return 0;
  if (strcmp(buf,"")!=0)
    return strdup( buf );
  return posix_get_process_name( argv0 );
#else
  return posix_get_process_name( argv0 );
#endif
}

