    #include <nutils/bf.h>
#include "vtest/vtest.h"



void BF_test()
{
  BF bf;
  void *mem;
  int16_t val16;
  int32_t val32;
  int64_t val64;

  mem = malloc(18 );

  BF_init( &bf, mem, 18 );

  BF_put_int32_t( &bf, -4 );
  BF_put_int32_t( &bf, -10 );
  BF_put_int64_t( &bf, -8000000001ll );
  BF_put_int16_t( &bf, -10001 );
   
  BF_get_mode( &bf );

  VASSERT( ! BF_get_int32_t( &bf, &val32 ) );
  VASSERT( val32 == -4 );

  VASSERT( ! BF_get_int32_t( &bf, &val32 ) );
  VASSERT( val32 == -10 );

  VASSERT( ! BF_get_int64_t( &bf, &val64 ) );
  VASSERT( val64 == -8000000001ll );

  VASSERT( ! BF_get_int16_t( &bf, &val16 ) );
  VASSERT( val16 == -10001 );

}
