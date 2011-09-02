#include "bf.h"


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


