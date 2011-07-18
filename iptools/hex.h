#ifndef __HEX_Y_Z__
#define __HEX_Y_Z__


#include <stdint.h>

void *parse_hexa_string( const char *str, int len, uint32_t *rsize );
char *make_hex_dump(const char *data, size_t len );


#endif


