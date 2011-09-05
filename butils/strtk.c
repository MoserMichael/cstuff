#include "strtk.h"
#include <string.h>

void STRTK_init( STRTK *tok , const char *stop_chars )
{
  uint8_t *pos;
  memset( tok->pattern, 0, STRTK_PATTERN_SIZE );

  for( pos = (uint8_t *) stop_chars; *pos != '\0'; ++pos ) {
    STRTK_add_char( tok, *pos );  
  }
}


