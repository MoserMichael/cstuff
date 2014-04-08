#include <stdio.h>
#include <string.h>
#include "sbuf.c"

main()
{

    char str[ 200 ];
    SBUF sbuf;

    SBUF_init( &sbuf, str, sizeof(str) );

    SBUF_add_s( &sbuf, "test " );
    SBUF_fmt_size_t( &sbuf, 0  );
    SBUF_add_s( &sbuf, " " );
    SBUF_fmt_size_t( &sbuf, 9  );
    SBUF_add_s( &sbuf, " " );
    SBUF_fmt_size_t( &sbuf, 123456 );
    SBUF_add_s( &sbuf, " " );
    SBUF_fmt_size_t( &sbuf, -1 );
    SBUF_add_s( &sbuf, " " );
    SBUF_fmt_ptr( &sbuf, (void *) 0xABCDEFBA);

    printf( "%s\n", sbuf.buf );

    return 0;

}
