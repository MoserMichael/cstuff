/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>
#include <ctype.h>

M_INLINE void UTIL_swapbytes(unsigned char *src, unsigned char *dst, size_t size)
{
	unsigned char tmp;

	while(size>0) {
		tmp = *src;
		*src = *dst;
		*dst = tmp;

		++src;
		++dst;
		--size;
	}
}

/*
 * @brief returns 1 if num is a power of two.
 */
M_INLINE int UTIL_is_power_of_two( uint32_t num )
{
	return (num & (num - 1)) == 0;
	/* num == 2 ^ n -1 | all ones
	 * num == 2 ^ n    | one on next position after the last one.
	 * & will return 0 (no overlap) if num is 2 ^ n
	 */
}	

/* 
 * @brief align integer (alignment must be power of 2)
 */
M_INLINE uint32_t UTIL_align( uint32_t num, uint32_t align)
{
	/* assert( UTIL_is_power_of_two( align ) ); */
	return (num  + align - 1 ) & (~(align - 1));
	/*
	 * & (x) (~(align -1 )) set align to the closers alignment (x % align == 0) (round down to closest alignment)
	 * num + align - 1      make sure that the argument crosses alignment border, 
	 */
}

/*
 * @brief align pointer integer (alignment must be power of 2)
 */
M_INLINE void * UTIL_ptr_align( void *ptr, size_t align)
{
	/* assert( UTIL_is_power_of_two( align ) ); */
	return (void *) ( (((ptrdiff_t) ptr) & ~(align - 1)) + align );
}
/*
 * @brief return pointer to start of "page" ("page" size is power of two).
 */
M_INLINE void * UTIL_ptr_page_start( void *ptr, size_t page_size)
{
	/* assert( UTIL_is_power_of_two( align ) ); */
	return (void *) ( ((ptrdiff_t) ptr) & ~ ((ptrdiff_t) page_size - 1) );
	/*
	 * round argument pointer down to the closes pagesize (assuming pagesize is power of two)
	 */
}

/*
 * @brief checks if pointer is aligned to given power of two
 */
M_INLINE int UTIL_ptr_is_aligned( void *ptr, size_t page_size)
{
	/* assert( UTIL_is_power_of_two( align ) ); */
	return ( ((ptrdiff_t) ptr) & ((ptrdiff_t) page_size - 1) ) == 0;
}

/*
 * @brief check if argument pointer (ptr) is in memory range specified by [from ... to)
 */
M_INLINE int UTIL_ptr_in_range(void *ptr, void *from , void *to )
{
	return (uint8_t *) ptr >= (uint8_t *) from && (uint8_t *) ptr < (uint8_t *) to;
}


M_INLINE size_t UTIL_round_to_power_of_n( size_t num )
{
	size_t n = 1;

	while( n < num ) {
		n <<= 1;
	}
	return n;
}

M_INLINE size_t UTIL_log_base_2_of_n( size_t num )
{
	size_t ret = 0;

	while( num ) {
		num >>= 1;
		ret ++;
	}
	return ret;
}



M_INLINE char *UTIL_skip_spaces( const char *p) {
	while( *p != '\0' && isspace((int) *p)) {
		++p;
	}
	return (char *) p;
}

M_INLINE char *UTIL_skip_nspace( const char *p) {
	while( *p != '\0' && !isspace((int) *p)) {
		++p;
	}
	return (char *) p;
}

#ifdef  __cplusplus
}
#endif

#endif

