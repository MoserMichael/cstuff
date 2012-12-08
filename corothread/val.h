#ifndef _VALUES_H_
#define _VALUES_H_

#include <cutils/array.h>
#include <stdarg.h>

/** 
 * @defgroup VAL
 * @brief one typed value
 * A typed value, a tuple of typed values is conveniently passed around by the VALUES class
 * @{
 */
 
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

/**
 * @}
 */
 
/** 
 * @defgroup VALUES
 * @brief A class for conveniently passing of typed tuple of values 
 *
 * A typed tuple of values is stored into the VALUES object by a printf like function.
 *
 * VALUES_print( values, "%hhu%hd%d%qd%s", n8, n16, n32, n64, sval ); 
 
 * Once inserted, the values can be retrieved back, also by a scanf like function

 * VALUES_scan( &val, "%hhu%hd%d%qd%s", &_n8, &_n16, &_n32, &_n64, &_sval );

 * Note that the type specifiers of values retrieved must be strictly the same as that of the values inserted previously.

 * Type specifiers:

 * %hhd - signed byte (int8_t)
 * %hhu - unsigned byte (uint8_t)
 * %hd  - signed short (int16_t)
 * %hhu - unsigned short (uint16_t)
 * %d   - signed integer (int32_t)
 * %u   - unsigned integer (uint32_t)
 * %qd  - signed long long (int64_t)
 * %qu  - unsiend long long (uint64_t
 * %s   - null terminated string pointer
 * %p   - void pointer.
 *
 * @{
 */

 typedef struct tagVALUES {
  ARRAY values;
} VALUES;


/**
 * @brief initialises a typed tuple of values object
 */
M_INLINE int VALUES_init( VALUES *val)
{
  return ARRAY_init( &val->values, sizeof(VAL), 10);
}
 
/**
 * @brief frees a typed tuple of values object
 */
M_INLINE void VALUES_free( VALUES *val)
{
  ARRAY_free( &val->values );
}

/**
 * @brief insert typed tuple of values
 * @param val -  the VALUES object that is modified
 * @param format - Format specifier (for details see documentation of VALUES class)
 * @param ap -
 */
int VALUES_printv( VALUES *val, const char *format, va_list ap ); // __VALIST  
 
/**
 * @brief retrieve typed tuple of values; like scanf receives varying number of arguments
 * @param val -   the VALUES object that is modified 
 * @param format -  Format specifier (for details see documentation of VALUES class) 
 * @param ap -  va_list - pointer to stack frame of variadic function. 
 */
int VALUES_scanv( VALUES *val, const char *format, va_list ap );

/**
 * @brief set typed tuple of values; like prinf receives varying number of arguments 
 * @param val -  the VALUES object that is modified 
 * @param format - Format specifier (for details see documentation of VALUES class) 
 * @param ap - va_list - pointer to stack frame of variadic function.
 */
M_INLINE int VALUES_print( VALUES *val, const char *format, ... )
{
  va_list vlist;
  va_start( vlist, format );
  return VALUES_printv( val,  format, vlist );
}

/**
 * @brief retrieve typed tuple of values; like scanf receives varying number of arguments 
 * @param val -  the VALUES object that is modified 
 * @param format - Format specifier (for details see documentation of VALUES class) 
 */
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

/**
 * @}
 */

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


