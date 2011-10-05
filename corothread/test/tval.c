#include "vtest/vtest.h"
#include <corothread/val.h>
#include <stdio.h>



void VAL_test()
{
  VALUES val;
  uint8_t n8 = 250, _n8;
  uint16_t n16 = 65000, _n16;
  int32_t  n32 = 42, _n32;
  int64_t n64 = 0xBEEFBEEFBEEFBEEF, _n64;
  char *sval = "aaa", *_sval;

  VASSERT( VALUES_init( &val ) == 0 );
  VASSERT( VALUES_print( &val, "%hhu%hd%d%qd%s", n8, n16, n32, n64, sval )  == 0 );
  VASSERT( VALUES_scan( &val, "%hhu%hd%d%qd%s", &_n8, &_n16, &_n32, &_n64, &_sval ) == 0 );

  VASSERT( n8 == _n8 );
  VASSERT( n16 == _n16 );
  VASSERT( n32 == _n32 );
  VASSERT( n64 == _n64 );
  VASSERT( sval == _sval );
  VASSERT( strcmp( sval, _sval ) == 0 );

  // can't get more values than what there are in the bag.
  VASSERT( VALUES_scan( &val, "%hhu%hd%d%qd%s%d", &_n8, &_n16, &_n32, &_n64, &_sval, &_n32 ) == -1 );

  // can't get different type
  VASSERT( VALUES_scan( &val, "%d", &_n32 ) == -1 );

  // can't get different type
  VASSERT( VALUES_scan( &val, "%hhd", &_n8 ) == -1 );

  // can get less values of the same type
  VASSERT( VALUES_scan( &val, "%hhu%hd%d", &_n8, &_n16, &_n32 ) == 0 );

  VASSERT( n8 == _n8 );
  VASSERT( n16 == _n16 );
  VASSERT( n32 == _n32 );
 
  VALUES_free( &val );

}
