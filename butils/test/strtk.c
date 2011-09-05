#include <butils/strtk.h>
#include "vtest/vtest.h"


#define TSTRING "ab-cd1.f3g "
#define TPATTERN "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-[]"

void STRTK_test()
{

  STRTK tok;

  STRTK_init( &tok, TPATTERN );

  VASSERT( (STRTK_tok( &tok, TSTRING ) - TSTRING) == 10 );
  VASSERT( (STRTK_ntok( &tok, TSTRING,  11 ) - TSTRING) == 10 );
}
