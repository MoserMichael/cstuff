#include "circbufn.h"
#include <string.h>

int CIRCBUFN_resize( CIRCBUFN *buf, size_t maxcount )
{
    uint8_t *mem;
    size_t sz;

    sz = CIRCBUFN_maxsize( buf );
    if (maxcount < sz) // can't reduce size
	return -1;

    mem = (uint8_t *) malloc( maxcount * buf->elmsize );
    if (!mem) 
	return -1;

    sz = CIRCBUFN_size( buf );

    if (buf->data && sz != 0) {
      if (buf->read_pos < buf->write_pos) {
         memcpy( mem, buf->data + buf->read_pos, sz * buf->elmsize );
      } else {
         size_t sz1 = buf->maxcount - buf->read_pos; 

         memcpy( mem, buf->data + buf->read_pos, sz1 * buf->elmsize );
         memcpy( mem + sz1, buf->data, buf->write_pos * buf->elmsize ); 
      }
    }

    buf->data = mem;
    buf->read_pos = 0;
    buf->write_pos = sz;
    buf->maxcount = maxcount;
    return 0;
}

