#include <vtest/vtest.h>
#include <butils/fn.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void FN_test()
{

//TODO: function wrappers on top of realpath
//realpath just does not work on Cygwin.
//so either do my own realpath or not use these functions on windows.
//
#ifndef WIN32
  char *tmp;

  tmp = FN_normalize(".././abcd/efg");

  VASSERT( tmp != 0 );
  VASSERT( strchr(tmp,'.') == 0 );
  free(tmp);
 
  tmp = FN_make_path( "a/b/c", "d" );
  VASSERT( strcmp( tmp, "a/b/c/d" ) == 0 );
  free( tmp );


  tmp = FN_make_path( "a/b/c/", "d" );
  VASSERT( strcmp( tmp, "a/b/c/d" ) == 0 );
  free( tmp );

  tmp = FN_make_path( "a/b/c", "/d" );
  VASSERT( strcmp( tmp, "a/b/c/d" ) == 0 );
  free( tmp );

  tmp = FN_get_extension( "a.b/cde" );
  VASSERT( tmp == 0 );
 
  tmp = FN_get_extension( "a/b/cd.ef" );
  VASSERT( strcmp(tmp,".ef") == 0 );
  free( tmp );

  tmp = FN_set_extension( "a/b/cd.ef", "ghijk" );
  VASSERT( strcmp(tmp,"a/b/cd.ghijk") == 0 );
  free( tmp );

  tmp = FN_set_extension( "a/b/cd.ef", ".ghijk" );
  VASSERT( strcmp(tmp,"a/b/cd.ghijk") == 0 );
  free( tmp );

  tmp = FN_set_extension( "a/b/cd", "ghijk" );
  VASSERT( strcmp(tmp,"a/b/cd.ghijk") == 0 );
  free( tmp );
  
  tmp = FN_set_extension( "a/b/cd", ".ghijk" );
  VASSERT( strcmp(tmp,"a/b/cd.ghijk") == 0 );
  free( tmp );


  tmp = FN_dir_name( "a/b/c/d.ef" );
  VASSERT( strcmp( tmp, "a/b/c/" ) == 0 );
  free( tmp );

  tmp = FN_file_name( "a/b/c/d.ef" );
  VASSERT( strcmp( tmp, "d.ef" ) == 0 );
  free( tmp );
#endif
}


