#include <stdio.h>
#include <cutils/properties.h>
#include "vtest/vtest.h"
#include <string.h>

void TEST_properties_basic()
{
  PROPERTIES props;
  const char *val;

  VASSERT( ! PROPERTIES_init( &props, 10 ) );

  VASSERT( ! PROPERTIES_put( &props, "hello", "world" ) );  
  VASSERT( ! PROPERTIES_put( &props, "hello2", "world2" ) );  
  VASSERT( ! PROPERTIES_put( &props, "hello4", "world4" ) );  
  VASSERT( ! PROPERTIES_put( &props, "hello8", "world8" ) );  

  VASSERT( PROPERTIES_get( &props, "hello5" ) == 0 );

  val = PROPERTIES_get( &props, "hello" );
  VASSERT( strcmp(val,"world" ) == 0 );

  val = PROPERTIES_get( &props, "hello2" );
  VASSERT( strcmp(val,"world2" ) == 0 );

  val = PROPERTIES_get( &props, "hello4" );
  VASSERT( strcmp(val,"world4" ) == 0 );

  val = PROPERTIES_get( &props, "hello8" );
  VASSERT( strcmp(val,"world8" ) == 0 );

  PROPERTIES_free( &props ); 
}

void TEST_properties_file()
{
  PROPERTIES props;
  const char *val;
  int32_t vn;
  int rt;

  VASSERT( ! PROPERTIES_init( &props, 10 ) );
  
  VASSERT( ! PROPERTIES_load( &props, "props.props" ) );

  val = PROPERTIES_get( &props, "bbb" );
  VASSERT( val != 0 );
  VASSERT( strcmp( val, "hello world" ) == 0 );
 
  rt = PROPERTIES_hex_int32_t( &props, "ccc", &vn );  
  VASSERT( rt == 0 );
  VASSERT( vn == 0x1a );

  rt = PROPERTIES_dec_int32_t( &props, "aaa", &vn );  
  VASSERT( rt == 0 );
  VASSERT( vn == 42 );

 
  PROPERTIES_free( &props ); 

}
