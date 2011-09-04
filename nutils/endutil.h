#ifndef __ENDIAN_H_Y_Z__
#define __ENDIAN_H_Y_Z__

#include <cutils/base.h>
#include <arpa/inet.h>


typedef union {
  struct {
     uint32_t word_a;
     uint32_t word_b;
  } n32;
  uint64_t n64;
} QUAD;

M_INLINE uint64_t n2h_uint64_t(uint64_t val)
{
  QUAD a,b;
  
  a.n64 = val;

  b.n32.word_a = ntohl( a.n32.word_b );
  b.n32.word_b = ntohl( a.n32.word_a );

  return b.n64;
}

M_INLINE uint64_t h2n_uint64_t(uint64_t val)
{
  QUAD a,b;
  
  a.n64 = val;

  b.n32.word_a = htonl( a.n32.word_b );
  b.n32.word_b = htonl( a.n32.word_a );

  return b.n64;
}


#define n2h_uint8_t(val)  (val)
#define n2h_int8_t(val)   (val)
#define n2h_uint16_t(val) ntohs( val )
#define n2h_int16_t(val)  ntohs( val )
#define n2h_uint32_t(val) ntohl( val )
#define n2h_int32_t(val)  ntohl( val )
#define n2h_int64_t(val)  n2h_uint64_t( val )

#define h2n_uint8_t(val)  (val)
#define h2n_int8_t(val)   (val)
#define h2n_uint16_t(val) htons( val )
#define h2n_int16_t(val)  htons( val )
#define h2n_uint32_t(val) htonl( val ) 
#define h2n_int32_t(val)  htonl( val )
#define h2n_int64_t(val)  h2n_uint64_t( val ) 

#endif

