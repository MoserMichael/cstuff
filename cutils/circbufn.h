#ifndef _CIRCBUF_H_
#define _CIRCBUF_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>
#include <cutils/dbuf.h>
#include <assert.h>
#include <string.h>

/**
 * @defgroup CIRCBUF
 * @brief circular buffer, elements are one byte long; can be resized.
 */
typedef struct tagCIRCBUFN {
  size_t read_pos, write_pos, maxcount;
  size_t elmsize;  // size of one data item.
  uint8_t *data;
} CIRCBUFN;

int CIRCBUFN_resize( CIRCBUFN *buf, size_t maxcount );

M_INLINE void CIRCBUFN_init_mem(CIRCBUFN *buf, uint8_t *data, size_t elmsize, size_t maxcount)
{
    buf->data = data;
    buf->maxcount = maxcount;
    buf->read_pos = buf->write_pos = 0;
    buf->elmsize = elmsize;
}

M_INLINE int CIRCBUFN_init( CIRCBUFN *buf, size_t elmsize, size_t maxcount )
{
   CIRCBUFN_init_mem( buf, 0, elmsize, 0 );
   return CIRCBUFN_resize( buf, maxcount );
}

M_INLINE void CIRCBUFN_free( CIRCBUFN *buf )
{
    free( buf->data );
    buf->data = 0;
}

M_INLINE size_t CIRCBUFN_maxsize( CIRCBUFN *buf )
{
    return buf->maxcount;
}


M_INLINE int CIRCBUFN_empty( CIRCBUFN *buf)
{
  return buf->read_pos == buf->write_pos;
}

M_INLINE int CIRCBUFN_isfull( CIRCBUFN *buf )
{
  size_t next_write = (buf->write_pos + 1) % buf->maxcount;
  return (next_write == buf->read_pos);
}

M_INLINE size_t CIRCBUFN_size( CIRCBUFN *buf )
{
    size_t rpos = buf->read_pos;
    size_t wpos = buf->write_pos;

    if (wpos >= rpos) 
	return wpos - rpos;
    return (buf->maxcount - rpos) + wpos;
}

M_INLINE size_t CIRCBUFN_add( CIRCBUFN *buf, void *elm, size_t elmsize)
{
    if (CIRCBUFN_isfull( buf ))
	return -1;
    
    assert( elmsize == buf->elmsize );
    memcpy( &buf->data[ buf->write_pos  * buf->elmsize ], elm, elmsize );
    buf->write_pos = (buf->write_pos + 1) % buf->maxcount;

    return 0;
}

M_INLINE void *CIRCBUFN_at(CIRCBUFN *buf, size_t offset )
{  
   if (offset >= CIRCBUFN_size( buf )) 
     return 0;
   return &buf->data[ buf->elmsize * ( ( buf->read_pos + offset ) % buf->maxcount ) ];
}

M_INLINE void CIRCBUFN_inc_read_pos( CIRCBUFN *buf, size_t pos )
{
    if (pos > CIRCBUFN_size( buf )) {
      buf->read_pos = buf->write_pos;
      return;
    }
    buf->read_pos = (buf->read_pos + pos) % buf->maxcount;
}





/**
 * @}
 */

#ifdef  __cplusplus
}
#endif

#endif

