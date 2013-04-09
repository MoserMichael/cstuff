#ifndef _CIRCBUF_H_
#define _CIRCBUF_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>


/**
 * @defgroup CIRCBUF
 * @brief circular buffer, elements are one byte long; can be resized.
 */
typedef struct tagCIRCBUF {
  size_t read_pos, write_pos, maxcount;
  uint8_t *data;
} CIRCBUF;

int CIRCBUF_resize( CIRCBUF *buf, size_t maxcount );

M_INLINE void CIRCBUF_init_mem(CIRCBUF *buf, uint8_t *data, size_t maxcount)
{
    buf->data = data;
    buf->maxcount = maxcount;
    buf->read_pos = buf->write_pos = 0;
}

M_INLINE int CIRCBUF_init( CIRCBUF *buf, size_t maxcount )
{
   CIRCBUF_init_mem( buf, 0, 0 );
   return CIRCBUF_resize( buf, maxcount );
}

M_INLINE void CIRCBUF_free( CIRCBUF *buf )
{
    free( buf->data );
}

M_INLINE size_t CIRCBUF_maxsize( CIRCBUF *buf )
{
    return buf->maxcount;
}


M_INLINE int CIRCBUF_empty( CIRCBUF *buf)
{
  return buf->read_pos == buf->write_pos;
}

M_INLINE int CIRCBUF_isfull( CIRCBUF *buf )
{
  size_t next_write = (buf->write_pos + 1) % buf->maxcount;
  return (next_write == buf->read_pos);
}

M_INLINE size_t CIRCBUF_size( CIRCBUF *buf )
{
    size_t rpos = buf->read_pos;
    size_t wpos = buf->write_pos;

    if (wpos >= rpos) 
	return wpos - rpos;
    return (buf->maxcount - rpos) + wpos;
}

M_INLINE size_t CIRCBUF_add( CIRCBUF *buf, uint8_t ch )
{
    if (CIRCBUF_isfull( buf ))
	return -1;
    
    buf->data[ buf->write_pos ] = ch;
    buf->write_pos = (buf->write_pos + 1) % buf->maxcount;

    return 0;
}

M_INLINE size_t CIRCBUF_get( CIRCBUF *buf, uint8_t *ch )
{  
   if (CIRCBUF_empty( buf )) 
     return -1;
   *ch = buf->data[ buf->read_pos ];
   buf->read_pos = (buf->read_pos + 1) % buf->maxcount;
   return 0;
}

M_INLINE uint8_t CIRCBUF_at(CIRCBUF *buf, size_t offset, uint8_t *ch )
{  
   if (offset >= CIRCBUF_size( buf )) 
     return -1;
   *ch = buf->data[ ( buf->read_pos + offset ) % buf->maxcount ];
   return 0;
}

M_INLINE void CIRCBUF_inc_read_pos( CIRCBUF *buf, size_t pos )
{
    if (pos > CIRCBUF_size( buf )) {
      buf->read_pos = buf->write_pos;
      return;
    }
    buf->read_pos = (buf->read_pos + pos) % buf->maxcount;
}



//CIRCBUF_at( CIRCBUF *buf, size_t offset, uint8_t *ch )


/**
 * @}
 */

#ifdef  __cplusplus
}
#endif

#endif

