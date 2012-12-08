/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __X_Y_Z_BASE_H_
#define __X_Y_Z_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>

#define _MEMBEROF(ptr,offset)			(((uint8_t *) ptr) - offset)

#define _OFFSETOF( ptr, strct, membrr )  (strct *) _MEMBEROF( ptr, offsetof( strct, membrr ) )

#define M_INLINE  static inline
#define M_UNUSED(x) (void) (x)


#endif
