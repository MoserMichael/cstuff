#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include "inc.h"
#include <butils/pname.h>
#include <butils/fn.h>


void init_path( INC_PATH *path, const char *exe_path )
{
  char *pname, *dname, *dname2, *lname;

  
  pname = get_current_process_name( exe_path );
  if (!pname)
    return;

  dname = FN_dir_name( pname );
  if (!dname)
    return;

  dname2 = FN_dir_name( dname );
  if (!dname2)
    return;

  lname = FN_make_path( dname2, "lib/pooh" );
  if (!lname)
     return;

  INC_PATH_add( path, lname, 1 );

  free(lname);
  free(dname);
  free(dname2);
 }


INC_PATH *INC_PATH_init( const char *exe_path )
{
  INC_PATH *path;

  path = (INC_PATH *) malloc( sizeof( INC_PATH ) );
  if (!path) 
    return 0;
  ARRAY_init( path, sizeof( char *), 0 );

  if (exe_path != 0 )
    init_path( path, exe_path );
  return path;
}

void INC_PATH_add( INC_PATH *inc_path, const char *path, int add_last )
{ 
  char *sadd = strdup( path );
  if (add_last) 
    ARRAY_push_back( inc_path, &sadd, sizeof( void * ) );
  else
    ARRAY_insert_at( inc_path, 0, &sadd, sizeof( void * ) );
}



char * INC_PATH_resolve( INC_PATH *inc_path, const char *name )
{
  size_t i;
  char path[ PATH_MAX + 1];

  for( i = 0; i < ARRAY_size( inc_path ); i ++ ) {
    char **tmp, *def;
    int num;

    tmp = (char **) ARRAY_at( inc_path, i );
    assert( tmp != 0 );
    def = *tmp;

    num = snprintf( path, sizeof( path ) - 1, "%s/%s", def, name );
    path[ num ] = '\0';
    
    if (!access( path, R_OK )) {
      return strdup( path );
    }  
  }
  return 0;
}

char * INC_PATH_to_text( INC_PATH *inc_path )
{
  size_t i, len = 0;
  char *ret, *stmp, **tmp, *def;
   
  if ( ARRAY_size( inc_path ) == 0 ) 
    return 0;

  for( i = 0; i < ARRAY_size( inc_path ); i ++ ) {
    
    tmp = (char **) ARRAY_at( inc_path, i );
    def = *tmp;
    len += strlen( def ) + 1;
  }

  stmp = ret = (char *) malloc( len + 1 ); 

  for( i = 0; i < ARRAY_size( inc_path ); i ++ ) {
    
    tmp = (char **) ARRAY_at( inc_path, i );
    def = *tmp;

    strcpy( stmp, def );
    strcat( stmp, " " );

    stmp += strlen( stmp );

  }

  return ret;
}


