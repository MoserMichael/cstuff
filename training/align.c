#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

struct a {
  uint8_t a;
  uint16_t b;
  void *c;
};

struct a_1 {
  void *a;
  uint16_t b;
  uint8_t c;
};

struct a_2 {
  uint8_t a;
  void *b;
  uint16_t c;
};

struct b {
 uint16_t a;
 uint8_t b;
};

struct b_1 {
 uint8_t a;
 uint16_t b;
};

struct c {
 uint8_t a[ sizeof(void *) + 1 ];
 uint8_t b;
};

struct c_1 {
 uint8_t a;
 uint8_t b[ sizeof(void *) + 1 ];
};

struct cc {
 uint16_t a[ 3 ];
 uint8_t b;
};

struct cc_1 {
 uint8_t a;
 uint16_t b[ 3 ];
};


struct d {
  char a;
  struct d_inner {
    void *a;
    uint16_t b;
  } b;
};

struct d_1 {
  void *a;
  struct d_1_inenr {
    uint8_t a;
    uint16_t b;
  } b;
};

struct e {
  uint8_t a:3;
  uint8_t b:4;
  uint8_t c:2;
};

struct f 
{ 
  uint16_t a:15; 
  uint8_t b:2; 
};


#define INT_FIELD_MINUS_ONE_BIT ((sizeof(int) * 8) - 1)
struct g
{
  int a:INT_FIELD_MINUS_ONE_BIT;
  uint8_t b:2;
};


main()
{
  printf("! Purpose: #pragma(pack) produces small structures, but slow code to access them! (interrupt to handle misaligned access)\n");
  printf("! Purpose: want to use unaligned structs in a space efficient way\n\n");

  printf("! a structure field of uint8_t  can be on any address\n");
  printf("! a structure field of uint16_t must be on address divisible by two.\n");
  printf("! a structure field of uint32_t must be on address divisible by four. (etc)\n\n");
  printf( "sizeof({ uint16_t, uint8_t })=%zu (%zu,%zu) sizeof({ uint8_t, uint16_t })=%zu (%zu,%zu)\n", 
	sizeof(struct b), offsetof(struct b,a), offsetof(struct b,b), 
	sizeof(struct b_1), offsetof(struct b_1,a), offsetof(struct b_1,b));

  printf("\n");
  printf("! to minimize space: try to pack more than one structure field into the holes between the structure fields\n\n");

  printf( "sizeof({ char8_t , uint16_t, void* })=%zu (%zu,%zu,%zu)  sizeof({void *, uint16_t, uint8_t })=%u (%zu,%zu,%zu)  sizeof({uint8, void *, uint16 })=%zu (%zu,%zu,%zu)\n", 
	sizeof(struct a), offsetof(struct a,a), offsetof(struct a,b), offsetof(struct a,c), 
	sizeof(struct a_1), offsetof(struct a_1,a), offsetof(struct a_1,b), offsetof(struct a_1,c),
	sizeof(struct a_2), offsetof(struct a_2,a), offsetof(struct a_2,b), offsetof(struct a_2,c) );
  
  printf("\n");
  printf("! an array of uint8_t can be on any address (same as uint8_t field)\n");
  printf("! an array of uint16_t must start on an address divisibe by two\n\n");

  printf( "sizeof({uint8[sizeof(void*)+1], uint8_t})=%zu (%zu,%zu) sizeof({uint8_t, uint8_t[sizeof(void*)+1]})=%zu (%zu,%zu)\n", 
	sizeof(struct c), offsetof(struct c,a), offsetof(struct c,b), 
	sizeof(struct c_1), offsetof(struct c_1,a), offsetof(struct c_1,b));

  printf( "sizeof({uint16_t[3], uint8_t})=%zu (%zu,%zu) sizeof({uint8_t,uint16_t[3]})=%zu (%zu,%zu)\n", 
	sizeof(struct cc), offsetof(struct cc,a), offsetof(struct cc,b), 
	sizeof(struct cc_1), offsetof(struct cc_1,a), offsetof(struct cc_1,b));


  printf("\n");
  printf("! nested structures are a source of some waste of memory due to padding/alignment (hi c++)\n");
  printf("! One way is to reorder the structure to fit into a smaller size\n\n");

  printf("sizeof({uint8_t, { void *, uint16_t }})=%zu  sizeof({void *, {uint8_t, uint16_t }})=%zu\n",
	sizeof(struct d),
	sizeof(struct d_1));

  printf("\n");
  printf("! bitfields: a series of bitfields is aligned to word size (sizeof(int)=%d)\n", sizeof(int));
  printf("! because bitwise instructions are used to implement them (these work on ints)\n");
  printf("! common convention to have a padding field that pads up to word size\n");
  printf("! also: always use unsigned type for bitfield values\n");
  printf("! still: they try to pack them as close as possible (!)\n\n");

  printf("sizeof({ uint_t a:3; uint_t b:4; uint8_t c:2; }) = %zu (!!!)\n", sizeof(struct e)); 
  printf("sizeof({ uint16_t a:15; uint8_t b:2; })=%zu; sizeof(int)=%zu\n", sizeof(struct f), sizeof(int) );

  printf("\n");
  printf("! bitfield member that does not fit the current word is moved to the next word\n\n");
  printf("sizeof({ int a:%d; uint8_t b:2; })=%zu\n", 
    INT_FIELD_MINUS_ONE_BIT, sizeof(struct g) );

  printf("\n");
  printf("! When in doubt obtain the layout of a struct by means of offsetof (can't take offsetof bitfield members)\n");
  printf("! or use clang's -Wpadding or use pahole.\n");
  printf("! double or long double fields can have surprising behavior\n");
  
  printf("\n");
  printf("! thank you !\n");
  return 0;
}
  
