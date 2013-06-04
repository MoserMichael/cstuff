#include "circbuf.h"
#include <string.h>

int CIRCBUF_resize( CIRCBUF *buf, size_t maxcount )
{
    uint8_t *mem;
    size_t sz;

    sz = CIRCBUF_maxsize( buf );
    if (maxcount < sz) // can't reduce size
	return -1;

    mem = (uint8_t *) malloc( maxcount );
    if (!mem) 
	return -1;

    sz = CIRCBUF_size( buf );

    if (buf->data && sz != 0) {
      if (buf->read_pos < buf->write_pos) {
         memcpy( mem, buf->data + buf->read_pos, sz );
      } else {
         size_t sz1 = buf->maxcount - buf->read_pos; 

         memcpy( mem, buf->data + buf->read_pos, sz1  );
         memcpy( mem + sz1, buf->data, buf->write_pos ); 
      }
    }

    buf->data = mem;
    buf->read_pos = 0;
    buf->write_pos = sz;
    buf->maxcount = maxcount;
    return 0;
}

int CIRCBUF_extract_text( CIRCBUF *buf, size_t spos, size_t epos, DBUF *copy_to )
{
  size_t sz, maxs;

  DBUF_reset( copy_to );
  sz = CIRCBUF_size( buf ); 

  if (spos >= sz || epos >= sz)
    return -1;


  if (spos > epos) {
    maxs = CIRCBUF_maxsize( buf );  
    if (DBUF_init( copy_to, maxs - spos + epos + 1 )) 
        return -1;
    DBUF_add( copy_to, buf->data, epos ); 
    DBUF_add( copy_to, buf->data + spos , maxs - spos );
  } else {
    if (DBUF_init( copy_to, epos - spos + 1 ))
        return -1;
    DBUF_add( copy_to, buf->data + spos, epos - spos );
  }
  DBUF_add_null( copy_to );
  return 0;
}

