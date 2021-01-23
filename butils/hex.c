#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "hex.h"

#define HEX_LINE_LENGTH  16

char *make_hex_dump(const char *data, size_t len)
{
    int lines = (len / HEX_LINE_LENGTH) + 1;
    char  *buf = malloc(  lines * ( HEX_LINE_LENGTH * 3 + HEX_LINE_LENGTH + 2 ) + 2 );
    size_t i,j,k,ch,nl ;
    int cc;

    ch = 0;

    for(i = 0; i < len; ) {
      nl = j = i + HEX_LINE_LENGTH;
      if (j > len) {
	j = len;
      }

      for(k=i ; k < j; k++) {
        sprintf(buf + ch, "%02x ", (unsigned char) data[k] );
        ch += 3;
      }

      for( ; k < nl ; k ++ ) {
        sprintf(buf + ch, "   ");
	ch += 3;
      }
  
 
      buf[ch++] = ' ';

      for(k=i; k < j; k++) {
        cc = data[ k ];
        if ( isprint( cc ) ) {
          buf[ch++] = data[k];
        } else {
          buf[ch++] = '.';
        }      
      }	

      i = j;
      buf[ch++] = '\n';     	
    }
    buf[ch] = '\0';
	 
    return buf;
}

void *parse_hexa_string( const char *str, int len, uint32_t *rsize )
{
  uint8_t * buf, *pos;
  int low_val, high_val;
  char ch;

  if (len  == -1) {
    len = strlen( str );
  }

  buf = (uint8_t *) malloc( len );
  if (!buf) {
    return 0;
  }


  for( pos = buf ; *str != '\0'; ++str) {
     ch = *str;
     if (ch >= 'a' && ch <= 'z') {
       high_val = (ch - 'a') + 10;
     } else if (ch >= 'A' && ch <= 'Z') {
       high_val = (ch - 'A') + 10;
     } else if (ch >= '0' && ch <= '9') {
       high_val = (ch - '0');
     } else {
       goto err;
     }
     ++str;

     if (*str != '\0') {
       ch = *str;
       if (ch >= 'a' && ch <= 'z') {
         low_val = (ch - 'a') + 10;
       } else if (ch >= 'A' && ch <= 'Z') {
         low_val = (ch - 'A') + 10;
       } else if (ch >= '0' && ch <= '9') {
         low_val = (ch - '0');
       } else {
         goto err;
       }
    } else {
      high_val = 0;
      low_val = 0;
    }
   
    *pos++ = (uint8_t) ( (high_val << 4) | low_val );  
  }

  *rsize = pos - buf;
  return buf;
err:
  return 0;
}    



