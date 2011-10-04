#ifndef _VALUES_H_
#define _VALUES_H_

#include <cutils/array.h>
#include <stdarg.h>

typedef enum {
  VAL_TYPE_UINT8,
  VAL_TYPE_INT8,
  VAL_TYPE_UINT16,
  VAL_TYPE_INT16,
  VAL_TYPE_UINT32,
  VAL_TYPE_INT32,
  VAL_TYPE_UINT64,
  VAL_TYPE_INT64,
  VAL_TYPE_STRING,
  VAL_TYPE_PTR,
} VAL_TYPE;

typedef struct tagVAL {
  VAL_TYPE   type;
  union {
    uint8_t  u8;
    int8_t   n8;
    uint16_t u16;
    int16_t  n16;
    uint32_t u32;
    int32_t  n32;
    uint64_t u64;
    int64_t  n64;

    char    *sval;
    void    *pval;

  } u;

} VAL;

typedef struct tagVALUES {
  ARRAY values;
} VALUES;


M_INLINE int VALUES_init( VALUES *val)
{
  return ARRAY_init( &val->values, sizeof(VAL), 10);
}
 
M_INLINE void VALUES_free( VALUES *val)
{
  ARRAY_free( &val->values );
}

int VALUES_printv( VALUES *val, const char *format, va_list ap );

int VALUES_scanv( VALUES *val, const char *format, va_list ap );

M_INLINE int VALUES_print( VALUES *val, const char *format, ... )
{
  va_list vlist;
  va_start( vlist, format );
  return VALUES_printv( val,  format, vlist );
}

M_INLINE int VALUES_scan(  VALUES *val, const char *format, ... )
{
  va_list vlist;
  va_start( vlist, format );
  return VALUES_scanv( val, format, vlist );
}

M_INLINE VAL * VALUES_at( VALUES *val, size_t pos)
{
  return (VAL *) ARRAY_at( &val->values, pos );
}


M_INLINE size_t VALUES_size( VALUES *val)
{
  return ARRAY_size( &val->values );
}

/*
  %format 

         hhd - int8_t
	 hhu - uint8_t

	 hd -  int16_t
	 hu -  uint16_t

	 d  -  int32_t
	 u  -  uint32_t

	 qd -  int64_t
	 qu -  uint64_t

	 s  -  null terminated string
	 p  -  pointer value.
*/
	 

#endif


