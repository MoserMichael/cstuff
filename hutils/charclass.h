#ifndef __CHARCLASS_H_
#define __CHARCLASS_H_

#include <cutils/base.h>


M_INLINE int is_digit( int8_t ch )
{
  return (ch >= '0' && ch <= '9');
}

M_INLINE int is_upper_case( int8_t ch )
{
  return (ch >= 'A' && ch <= 'Z');
}

M_INLINE int is_hex( int8_t ch ) 
{
  return is_digit(ch) || (ch >= 'a' && ch <='f') || (ch >='A' && ch <= 'F');
}


M_INLINE int is_hex_ext( int8_t ch ) 
{
  if (is_digit(ch)) {
    return ch - '0';
  }
  if ( ch >= 'a' && ch <='f') { 
    return ch - 'a';
  }
  if ( ch >='A' && ch <= 'F') {
    return ch - 'A';
  }
  return 0;
}

M_INLINE int is_alpha( int8_t ch )
{
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

M_INLINE int is_alphanum( int8_t ch )
{
  return is_alpha( ch ) || is_digit( ch );
}




#endif



