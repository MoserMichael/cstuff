#include "circbuf.h"
#include <string.h>

int CIRCBUF_resize( CIRCBUF *buf, size_t maxcount )
{
    uint8_t *mem;
    size_t sz;

    sz = CIRCBUF_maxsize( buf );
    if (maxcount < sz) // can't reduce.
	return -1;

    mem = (uint8_t *) malloc( maxcount );
    if (!mem) 
	return -1;

    if (buf->data) {
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

