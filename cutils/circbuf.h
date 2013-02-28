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
  size_t read_pos, write_pos, size;
  uint8_t *data;
} CIRCBUF;


M_INLINE void CIRCBUF_init_mem(CIRCBUF *buf, uint8_t *data, size_t size)
{
    buf->data = data;
    buf->size = size;
    buf->read_pos = buf->write_pos = 0;
}

M_INLINE void CIRCBUF_free( CIRCBUF *buf )
{
    free( buf->data );
}

int CIRCBUF_resize( CIRCBUF *buf, size_t size );

M_INLINE int CIRCBUF_init( CIRCBUF *buf, size_t size )
{
   CIRCBUF_init_mem( buf, 0, 0 );
   return CIRCBUF_resize( buf, size );
}


M_INLINE int CIRCBUF_empty( CIRCBUF *buf)
{
  return buf->read_pos == buf->write_pos;
}

M_INLINE int CIRCBUF_isfull( CIRCBUF *buf )
{
  size_t next_write = (buf->write_pos + 1) % buf->size;
  return (next_write == buf->read_pos);
}

M_INLINE size_t CIRCBUF_size( CIRCBUF *buf )
{
    size_t rpos = buf->read_pos;
    size_t wpos = buf->write_pos;

    if (wpos >= rpos) 
	return wpos - rpos;
    return (buf->size - rpos) + wpos;
}


M_INLINE size_t CIRCBUF_add( CIRCBUF *buf, uint8_t ch )
{
    if (CIRCBUF_isfull( buf ))
	return -1;
    
    buf->data[ buf->write_pos ] = ch;
    buf->write_pos = (buf->write_pos + 1) % buf->size;

    return 0;
}

M_INLINE size_t CIRCBUF_get( CIRCBUF *buf, uint8_t *ch )
{  
   if (CIRCBUF_empty( buf )) 
     return -1;
   *ch = buf->data[ buf->read_pos ];
   buf->read_pos = (buf->read_pos + 1) % buf->size;
   return 0;

}

/**
 * @}
 */

struct tagCIRC_BUF_INPUT_SRC;

typedef int (*FN_CIRC_BUF_INPUT_SRC) ( CIRCBUF *buf, struct tagCIRC_BUF_INPUT_SRC *src );

typedef struct tagCIRC_BUF_INPUT_SRC
{
    void *ctx;
    FN_CIRC_BUF_INPUT_SRC next_data;
}
    CIRC_BUF_INPUT_SRC;


#ifdef  __cplusplus
}
#endif

#endif

