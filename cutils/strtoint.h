#ifndef __STRTOINT__H_
#define __STRTOINT__H_


#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <cutils/base.h>

/**
 * @defgroup strtoint
 * @brief an attempt to normalize the wild strtol/stroll/strtoul/strtoull set of functions for the stdint.h world.
 *
 * @{
 */

M_INLINE int16_t strto_int16_t (const char *nptr, char **endptr, int base )
{
  long rval = strtol( nptr, endptr, base );
  if (rval > SHRT_MAX) {
    errno = ERANGE;
  }
  return (int16_t) rval;
}

M_INLINE uint16_t strto_uint16_t (const char *nptr, char **endptr, int base )
{
  long rval = strtoul( nptr, endptr, base );
  if (rval > USHRT_MAX) {
    errno = ERANGE;
  }
  return (uint16_t) rval;
}


M_INLINE int32_t strto_int32_t (const char *nptr, char **endptr, int base )
{
  long rval = strtol( nptr, endptr, base );
  if (rval > INT_MAX) {
    errno = ERANGE;
  }
  return (int32_t) rval;
}

M_INLINE uint32_t strto_uint32_t (const char *nptr, char **endptr, int base )
{
  uint32_t rval = strtol( nptr, endptr, base );
  if (rval > UINT_MAX) {
    errno = ERANGE;
  }
  return (int32_t) rval;
}

#define strto_int64_t   strtoll
#define strto_uint64_t  strtoull

/**
 * @}
 */

#endif


