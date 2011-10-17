#include "dbuf.h"
#include <string.h>

int DBUF_add(  DBUF* buf, void *data, size_t data_size )
{
  size_t bs;
  uint8_t *nb;
  
  bs = buf->buf_size;
  if ( (buf->buf_used + data_size) > bs) {
    if (bs == 0) {
      bs = 1;
    }
    do {
      bs *= 2;
    } while( (buf->buf_used + data_size) > bs );

    nb = (uint8_t *) realloc( buf->buf, bs );
    if (!nb) {
      return -1;
    }
    buf->buf = nb;
    buf->buf_size = bs;
  }
  
  memcpy( buf->buf + buf->buf_used, data, data_size );
  buf->buf_size = bs;
  buf->buf_used += data_size;

  return 0; 
}


