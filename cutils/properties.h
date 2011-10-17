#ifndef __BHASHPROPS_H__
#define __BHASHPROPS_H__

#include <cutils/bhash.h>
#include <cutils/strtoint.h>
#include <ctype.h>

/**
 * @defgroup PROPERTIES
 * @brief implements the commonly used idiom of a map from names to values (strings to strings), a hash with a signle values (not multimap).
 *
 * Also supported is reading the properties from a file ( format just like java.lang.Properties )
 * @{
 */

typedef struct tagNAMEVALUE_HASH_ENTRY  {
  HASH_Entry entry;

  char *name;
  char *value;

} NAMEVALUE_HASH_ENTRY;


typedef struct tagPROPERTIES {
  HASH props;
} PROPERTIES;

/**
 * @brief initialise the object 
 */
int PROPERTIES_init( PROPERTIES *props, int num_buckets );

/**
 * @brief loads properties from file, a java properties text file. 
 */
int PROPERTIES_load(PROPERTIES *props, const char *file_name);

/**
 * @brief free a properties object
 */
void PROPERTIES_free( PROPERTIES *props );


/**
 * @brief add a name to value mapping
 */
int PROPERTIES_put( PROPERTIES *props, const char *name , const char *value );



/**
 * @brief get the value for a name, return 0 if no such mapping exists.
 */
const char * PROPERTIES_get( PROPERTIES *props, const char *name );


#define MAKE_PROPERTIES_DECIMAL_INT_GET( type ) \
M_INLINE int PROPERTIES_dec_##type (PROPERTIES *props, const char *name, type *rret) \
{ \
  const char *value; \
  char *eptr; \
  type ret; \
  \
  value = PROPERTIES_get( props, name ); \
  if (!value) { \
    return -1; \
  } \
  for( ;isspace(*value); ++value ); \
 \
  errno = 0; \
   \
  ret = strto_##type ( value, &eptr, 10 ); \
 \
  if (errno) {  \
    return -2; \
  } \
 \
  if (*eptr == '\0' || isspace(*eptr)) { \
    *rret = ( type ) ret; \
    return 0; \
  } \
  return -3; \
}

MAKE_PROPERTIES_DECIMAL_INT_GET( int16_t )
MAKE_PROPERTIES_DECIMAL_INT_GET( uint16_t )
MAKE_PROPERTIES_DECIMAL_INT_GET( int32_t )
MAKE_PROPERTIES_DECIMAL_INT_GET( uint32_t )
MAKE_PROPERTIES_DECIMAL_INT_GET( int64_t )
MAKE_PROPERTIES_DECIMAL_INT_GET( uint64_t )

#define MAKE_PROPERTIES_HEX_INT_GET( type ) \
M_INLINE int PROPERTIES_hex_##type (PROPERTIES *props, const char *name, type *rret) \
{ \
  const char *value; \
  char *eptr; \
  type ret; \
  \
  value = PROPERTIES_get( props, name ); \
  if (!value) { \
    return -1; \
  } \
  for( ;isspace(*value); ++value ); \
 \
  errno = 0; \
   \
  ret = strto_##type ( value, &eptr, 16 ); \
 \
  if (errno) {  \
    return -2; \
  } \
  if (*eptr == '\0' || isspace(*eptr)) { \
    *rret = ( type ) ret; \
    return 0; \
  } \
  return -3; \
 \
}

MAKE_PROPERTIES_HEX_INT_GET( int16_t )
MAKE_PROPERTIES_HEX_INT_GET( uint16_t )
MAKE_PROPERTIES_HEX_INT_GET( int32_t )
MAKE_PROPERTIES_HEX_INT_GET( uint32_t )
MAKE_PROPERTIES_HEX_INT_GET( int64_t )
MAKE_PROPERTIES_HEX_INT_GET( uint64_t )



/**
 * @}
 */

#endif


