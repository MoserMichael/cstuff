#ifndef _DBUF_X_Y_Z_
#define _DBUF_X_Y_Z_

#ifdef  __cplusplus
extern "C" {
#endif


#include <cutils/base.h>

/**
 * @defgroup DBUF
 * @brief buffer that is owner  of stuff that grows dynamically as you add stuff to it;  
 * This object owns the memory that it is holding.
 * it also has length/size fields, no assumption data includes zeros or not - any data (not just strings).
 * @{
 */
typedef struct tagDBUF {

  uint8_t *buf;
  size_t buf_used;
  size_t buf_size;

} DBUF;

/**
 * @brief initialise a buffer
 */
M_INLINE int DBUF_init( DBUF *buf, size_t init_size )
{
  buf->buf =  0;
  if (init_size) {
    buf->buf = malloc( init_size );
    if (!buf->buf) {
      return -1;
    }
  }
  buf->buf_used = 0;
  buf->buf_size = init_size;
  return 0;
}

M_INLINE void DBUF_reset(DBUF *buf) {
  buf->buf_used = 0;
}

/**
 * @brief free memory owned by a buffer
 */
M_INLINE void DBUF_free(DBUF *buf) {
  if (buf->buf) {
    free(buf->buf); 
    buf->buf = 0;
    buf->buf_used = buf->buf_size = 0;
  }
}

/**
 * @brief add stuff to a buffer
 */
int DBUF_add(  DBUF* buf, const void *data, size_t data_size );

M_INLINE int DBUF_add_null(  DBUF* buf )
{
  char n = 0;
  return DBUF_add( buf, &n, 1 );
}



M_INLINE  void * DBUF_buffer( DBUF *buf )
{
  return buf->buf;
}

M_INLINE size_t DBUF_size( DBUF *buf )
{
  return buf->buf_used;
}
 
/**
 * @}
 */

#ifdef  __cplusplus
}
#endif


#endif

