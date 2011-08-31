#ifndef __HEX_Y_Z__
#define __HEX_Y_Z__

#include <stdint.h>

/**
 * @defgroup HEX
 * @brief work with hexadecimal string
 * @{
 */


/**
 * @brief parsing of hexadecimal string
 */
void *parse_hexa_string( const char *str, int len, uint32_t *rsize );

/**
 * @brief format of hexadecimal dump
 */
char *make_hex_dump(const char *data, size_t len );

/**
 * @}
 */


#endif


