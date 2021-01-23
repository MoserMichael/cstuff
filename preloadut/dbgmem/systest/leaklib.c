#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

char  *str_dup_and_rev(char *str, size_t *rlen )
{
#if 0
    char *src, *trg;
#endif    
    size_t n  = strlen( str );
    char *ret = strdup( str );

    
    if (!ret) {
      return 0;
    }
#if 1    
    *rlen = n;
    return ret;
#else
    *rlen = n;
    
    for( src = str + n, trg = ret; ; --src, ++trg ) {
      *trg = *src;

      if (src == str) 
        break;
    }

    return ret;
#endif    
}
