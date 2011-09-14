#ifndef __BUFF_H_
#define __BUFF_H_ 

#include <nutils/endutil.h>

/**
 @defgroup BF
 @brief protocol buffer. idea similar to skb_buff, does not own it's data.
 The following invariants hold

 START_OF_BUFFER <= start <= get_pos <= put_pos <= end <= END_OF_BUFFER.

 The effective buffer ranges from start up to end pointers - the range [start end)
 start may be larger than START_OF_BUFFER, as a way of reserving room for a header.
 end may be less than END_OF_BUFFER, as a way of reserving root for a footer.
 
 the get_pos position will advance as data is returned from the buffer 
 the pus_pos position will advance, as data is placed int the buffer.

 @{
 */
typedef struct tagBF {
  uint8_t *bf;
  size_t   bf_size;

  uint8_t *start, *get_pos, *put_pos, *end;

} BF;


/**
 * @brief initialise an empty protocol buffer object. 
 * start and end cover the whole memory range.
     * nitiay get and put pointer are set to start of range.
 */
M_INLINE void BF_init( BF *bf, void *buffer, size_t buffer_size )
{
   bf->bf =  (uint8_t *) buffer;
   bf->bf_size = buffer_size;

   bf->start =   (uint8_t *) buffer;
   bf->end = buffer + buffer_size;

   bf->get_pos = bf->put_pos = bf->start;
}

/**
 * @brief validates a buffer - checks all invariants.
 */
int BF_check( BF * bf );

/**
 * @brief set pointers so that we can 'get' the whole range from start to end.
 */
M_INLINE void BF_get_mode( BF *bf )
{
    bf->get_pos = bf->start;
    bf->put_pos = bf->end;
}


/**
 * @brief set pointers so that we can 'put' the whole range from start to end.
 */
M_INLINE void BF_put_mode( BF *bf )
{
    bf->get_pos = bf->start;
    bf->put_pos = bf->start;
}

/**
 * @brief returns number of bytes that can be returned by _get_
 */
M_INLINE size_t BF_get_size( BF *bf )
{
  return bf->put_pos - bf->get_pos;
}


/**
 * @brief returns number of bytes that can be written by _put_
 */
M_INLINE size_t BF_put_size( BF *bf )
{
  return bf->end - bf->put_pos;
}


/** 
 * @brief set start position, position is relative to start of memory range.
 */
int BF_set_start( BF * bf, size_t pos );

/** 
 * @brief set end position, position is relative to start of memory range.
 */
int BF_set_end( BF * bf, size_t pos );


/** 
 * @brief set get position, position is relative to 'start' pointer.
 */
int BF_set_get( BF * bf, size_t pos );

/** 
 * @brief set put position, position is relative to 'start' pointer.
 */
int BF_set_put( BF * bf, size_t pos );

/**
 * @brief returns number of bytes that cam be read from this buffer (i.e. that BF_get_(type) return ) 
 */
M_INLINE size_t BF_can_get( BF *bf )
{
    return bf->put_pos - bf->get_pos;
}

/**
 * @brief returns number of bytes that cam be placed into this buffer (i.e. that BF_put_(type) places into this buffer ) 
 */
M_INLINE size_t BF_can_put( BF *bf )
{
    return bf->end - bf->put_pos;
}

/**
 * @brief return 1 if the whole buffer is fulled with data (get_pos == start && put_pos == end)
 */
M_INLINE int BF_is_full( BF *bf )
{
    return bf->get_pos == bf->start && bf->end == bf->put_pos;
}

/**
 * @brief moves get_pos to start position; moves content between get_pos and put_pos if not empty.a
 * Always succeeds
 * returns 0 if bytes were moved, 1 if buffer was empty to begin with.
 */
int BF_compact( BF * bf );


/**
  @ brief get the next line.
 */
char *  BF_get_line( BF * bf, int eof_line);

/**
  @ brief get the next line.
 */
char *  BF_get_line_ext( BF * bf, const char *eof_line, size_t eof_line_size);


#define BF_DECLARE_PUT_MACRO( type ) \
M_INLINE int BF_put_##type ( BF *bf, type val ) \
{ \
   if ((bf->end - bf->put_pos) < (ptrdiff_t) sizeof( type ) ) { \
     return -1 ; \
   } \
   do { \
     type nval = h2n_##type ( val ); \
     *( ( type *) bf->put_pos) = nval; \
     bf->put_pos += sizeof ( type ); \
   } while(0); \
   return 0; \
} 

BF_DECLARE_PUT_MACRO( uint8_t )
BF_DECLARE_PUT_MACRO( int8_t )
BF_DECLARE_PUT_MACRO( uint16_t )
BF_DECLARE_PUT_MACRO( int16_t )
BF_DECLARE_PUT_MACRO( uint32_t )
BF_DECLARE_PUT_MACRO( int32_t )
BF_DECLARE_PUT_MACRO( uint64_t )
BF_DECLARE_PUT_MACRO( int64_t )


#define BF_DECLARE_GET_MACRO( type ) \
M_INLINE int BF_get_##type ( BF *bf, type *pval ) \
{ \
   if ((bf->put_pos - bf->get_pos) < (ptrdiff_t) sizeof( type ) ) { \
     return -1 ; \
   } \
   *pval = n2h_##type( *( ( type *) bf->get_pos) ); \
   bf->get_pos += sizeof( type ); \
   return 0; \
}

BF_DECLARE_GET_MACRO( uint8_t )
BF_DECLARE_GET_MACRO( int8_t )
BF_DECLARE_GET_MACRO( uint16_t )
BF_DECLARE_GET_MACRO( int16_t )
BF_DECLARE_GET_MACRO( uint32_t )
BF_DECLARE_GET_MACRO( int32_t )
BF_DECLARE_GET_MACRO( uint64_t )
BF_DECLARE_GET_MACRO( int64_t )

#define BF_DECLARE_PUT_UNSAFE_MACRO( type ) \
M_INLINE void BF_put_unsafe_##type ( BF *bf, type val ) \
{ \
  type nval = h2n_##type ( val ); \
  *( ( type *) bf->put_pos) = nval; \
  bf->put_pos += sizeof ( type ); \
} 

BF_DECLARE_PUT_UNSAFE_MACRO(uint8_t)
BF_DECLARE_PUT_UNSAFE_MACRO(int8_t) 
BF_DECLARE_PUT_UNSAFE_MACRO(uint16_t)
BF_DECLARE_PUT_UNSAFE_MACRO(int16_t) 
BF_DECLARE_PUT_UNSAFE_MACRO(uint32_t)
BF_DECLARE_PUT_UNSAFE_MACRO(int32_t)
BF_DECLARE_PUT_UNSAFE_MACRO(uint64_t)
BF_DECLARE_PUT_UNSAFE_MACRO(int64_t)

#define BF_DECLARE_GET_UNSAFE_MACRO( type ) \
M_INLINE type BF_get_unsafe_##type ( BF *bf ) { \
   type rval = n2h_##type( *( ( type *) bf->get_pos) ); \
   bf->get_pos += sizeof( type ); \
   return rval; \
}

BF_DECLARE_GET_UNSAFE_MACRO(uint8_t)
BF_DECLARE_GET_UNSAFE_MACRO(int8_t)
BF_DECLARE_GET_UNSAFE_MACRO(uint16_t)
BF_DECLARE_GET_UNSAFE_MACRO(int16_t)
BF_DECLARE_GET_UNSAFE_MACRO(uint32_t)
BF_DECLARE_GET_UNSAFE_MACRO(int32_t)
BF_DECLARE_GET_UNSAFE_MACRO(uint64_t)
BF_DECLARE_GET_UNSAFE_MACRO(int64_t)

/**
 @}
*/

#endif
