#include "strtk.h"
#include <string.h>

void STRTK_init( STRTK *tok , const char *stop_chars )
{
  uint8_t *pos;
  uint8_t idx, off;

  memset( tok->pattern, 0, STRTK_PATTERN_SIZE );

  for( pos = (uint8_t *) stop_chars; *pos != '\0'; ++pos ) {
     idx = (*pos) >> 3;
     off = (*pos) &  7;

     tok->pattern[ idx ] |= (1 << off);
  }
}


