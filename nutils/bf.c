#include "bf.h"
#include <string.h>


int BF_compact( BF * bf )
{
    size_t bs;

    if (bf->get_pos == bf->put_pos) {
      bf->get_pos = bf->put_pos = bf->start;
      return 1;
    }
    if (bf->get_pos != bf->start) {
      bs = bf->put_pos - bf->get_pos;	  
      memmove( bf->start, bf->get_pos, bs );
      bf->get_pos = bf->start;
      bf->put_pos = bf->start + bs;
      return 0;
    } 
    return 1;
}

char *  BF_get_line( BF * bf, int eof)
{  
   char *eof_line, *ret;

   eof_line =  memchr(bf->get_pos, eof, bf->put_pos - bf->get_pos );
   if (eof_line) {
      ret = (char *) bf->get_pos;
      *eof_line = '\0';
      bf->get_pos = (uint8_t *) eof_line + 1;
      return ret;
   }
   return 0;
}

char *  BF_get_line_ext( BF * bf, const char *eof_line, size_t eof_line_size)
{
   char *pos,*next,*ret;
   size_t nsearch;
   
   nsearch = bf->put_pos - bf->get_pos;

   if (nsearch  < eof_line_size) {
     return 0;
   }
   
   nsearch -= eof_line_size - 1; 
   pos = (char *) bf->get_pos;

   do { 
     next = (char *) memchr( pos, (int) *eof_line, nsearch);
     if (!next) {
       return 0;
     }
     if (memcmp(next, eof_line, eof_line_size) == 0) {
        ret = (char *) bf->get_pos;
        *next = '\0';
	bf->get_pos = (uint8_t *) (next + eof_line_size);
	return (char *) ret;
     }
     pos = next + 1;
   } while( pos < (char *) bf->put_pos );

   return 0;
}

 int BF_check( BF * bf )
{
    uint8_t * eof;
    
    if (bf->bf > bf->start) {
      return 1;
    }
    if (bf->start > bf->get_pos)  {
      return 2;
    }
    if (bf->get_pos > bf->put_pos) {
      return 3;
    }
    if (bf->put_pos > bf->end) {
      return 4;
    }
    eof = bf->bf + bf->bf_size;
    if (bf->end > eof) {
      return 5;
    }
    return 0;
}


int BF_set_start( BF * bf, size_t pos )
{
  if (pos > bf->bf_size) {
    return -1;
  }
  bf->start = bf->bf + pos;

  if (bf->start > bf->get_pos) {
     bf->get_pos = bf->start;
  }
  if (bf->start > bf->put_pos) {
     bf->put_pos = bf->start;
  }
  if (bf->start > bf->end) {
    bf->end = bf->start; 
  }
  return 0;
}

int BF_set_end( BF * bf, size_t pos )
{
  if (pos > bf->bf_size) {
    return -1;
  }
  bf->end = bf->bf + pos;

  if (bf->end < bf->get_pos) {
     bf->get_pos = bf->end;
  }
  if (bf->end < bf->put_pos) {
     bf->put_pos = bf->end;
  }
  if (bf->end <  bf->start) {
    bf->start = bf->end; 
  }
  return 0;
}

int BF_set_get( BF * bf, size_t pos )
{
  size_t bfs = bf->end - bf->start; 
  if (pos > bfs) {
    return -1;
  }
  bf->get_pos = bf->start + pos;
  if (bf->get_pos > bf->put_pos) {
     bf->put_pos = bf->get_pos;
  }
  return 0;
}

int BF_set_put( BF * bf, size_t pos )
{
  size_t bfs = bf->end - bf->start; 
  if (pos > bfs) {
    return -1;
  }
  bf->put_pos = bf->start + pos;
  if (bf->put_pos < bf->get_pos) {
     bf->get_pos = bf->put_pos;
  }
  return 0;
}


