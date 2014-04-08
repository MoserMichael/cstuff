#include <string.h>

typedef struct tagSBUF
{
  char *buf;
  size_t buf_size;
  size_t pos;

} SBUF;

static void SBUF_init( SBUF *sbuf, char *buf, size_t size)
{
  sbuf->buf = buf;
  sbuf->buf_size = size;
  sbuf->pos = 0;
  sbuf->buf[ 0 ] = '\0';
}

#if 0
static void SBUF_reset( SBUF *buf )
{
  sbuf->pos = 0;
  sbuf->buf[ 0 ] = '\0';
}
#endif

static int SBUF_add( SBUF *sbuf, const char *buf, size_t size)
{
    size_t left = sbuf->buf_size - sbuf->pos;

    if (left == 0)
    {
	return -1;
    }

    if (left <= size)
    {
	size = left - 1;
    }

    memcpy( sbuf->buf + sbuf->pos, buf, size);
    sbuf->pos += size;
    sbuf->buf[ sbuf->pos ] = '\0';
 
    return 0;
}

static int SBUF_add_s( SBUF *sbuf, const char *str)
{
    return SBUF_add( sbuf, str, strlen( str ) );
}


static char * fmt_string( char *str, size_t str_len, size_t val, int radix)
{
    int digit;
    char ch_digit;
    size_t pos  = str_len - 1;

    str[ pos ] = '\0';

    while(val > 0)
    {
        digit = val % radix;
	val /= radix;

	if (digit <= 9) {
	  ch_digit = (char) ( digit + (int) '0' ); // assumes ASCII.
	} else {
	  ch_digit = (char) ( digit - 10 + (int) 'A' ); // assumes ASCII
        }	  
	
	if (pos == 0)
	{
	    return  0;
	}

	str[ --pos ] = ch_digit;
    }

    if (pos == (str_len - 1))
    {
	str[ --pos ] = '0';
    }	
    return str + pos;
}

static int SBUF_fmt_size_t( SBUF *sbuf, size_t size)
{
   char buf_s[ 128 ];
   char *snum;

   snum = fmt_string( buf_s, sizeof(buf_s), size, 10);
   if (snum)
   {
	return SBUF_add_s( sbuf, snum );
   }
   return -1;
}

static int SBUF_fmt_ptr( SBUF *sbuf, void *ptr)
{
    char buf_s[ sizeof(size_t) * 2 + 1 ];
    char *snum;

    snum = fmt_string( buf_s, sizeof(buf_s), (size_t) ptr, 16 );
    if (snum)
    {
	return SBUF_add_s( sbuf, snum );
    }
    return -1;
}

