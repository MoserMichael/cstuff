/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _V_HEAP_H_
#define _V_HEAP_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>


typedef int (*HEAP_compare) (void *lhs, void *rhs, size_t elmsize);

/**
 * @brief Heap data structure; all elements in the heap must be of the same size.
 * A heap data structure is an ordered tree where the root node contains the smallest value.
 * The smallest element in the whole heap is alway the top element.
 * This heap is implemented by an array.
 */
typedef struct  
{
	size_t		elmcount;
	size_t		elmmaxcount;
	size_t		elmsize;

	unsigned char	*buffer;

	HEAP_compare	compare_func;

}  
	HEAP;


/* 
 * @brief construct a heap object.
 * @param ctx (in) allocator interface. (if null we are using the default allocator)
 * @param heap (in) the object
 * @param elmcount (in) maximum number of elements kept in this heap.
 * @param elmsize (in)  size of one element kept in this heap.
 * @param grow_at (in) 
 * @param compare_func (in) compares two entries in the heap - used to establish heap property.
 */
M_INLINE int HEAP_init(  HEAP *heap, size_t elmcount, size_t elmsize, HEAP_compare compare_func)
{
	
	heap->elmsize = elmsize;
	heap->elmcount = 0;
	heap->elmmaxcount = elmcount;
	heap->compare_func = compare_func;

	heap->buffer = (unsigned char *) malloc( elmcount * elmsize );
	if (!heap->buffer) {
		return -1;
	}

	return 0;
}



/** 
 * @brief free all memory used by the heap (object destructor).
 * @heap heap (in) the object.
 */
M_INLINE void HEAP_free( HEAP *heap ) 
{
	if (heap->buffer) {
		free(heap->buffer);
		heap->buffer = 0;
		heap->elmcount = 0;
		heap->elmmaxcount = 0;
		heap->elmsize = 0;
	}
}

M_INLINE size_t HEAP_size( HEAP *heap)
{
	return heap->elmcount;
}

M_INLINE size_t HEAP_maxsize( HEAP *heap)
{
	return heap->elmmaxcount;
}

M_INLINE size_t HEAP_elmsize( HEAP *heap)
{
	return heap->elmsize;
}

/** 
 * @brief returns pointer to the top element of the heap.
 * @param heap		(in) the object
 * @return pointer to the top element of the heap or 0 if heap is empty.
 */
M_INLINE void *HEAP_top( HEAP *heap)
{
	if (heap->elmcount == 0) {
		return 0;
	}
	return heap->buffer;
}


M_INLINE void *HEAP_get_at( HEAP *heap, int index)
{
	if (heap->elmcount == 0) {
		return 0;
	}
	if (index > (int) heap->elmcount || index < 0) {
		return 0;
	}

	return heap->buffer + index * heap->elmsize;
}


/** 
 * @brief remove the top element from the heap
 * @param heap		(in) the object
 */
int HEAP_pop( HEAP *heap);

/** 
 * @brief insert a new element into the heap
 * @param heap		(in) the object
 * @param element	(in) data of object to be inserted into heap
 * @param elmsize	(in) size of data area for element pointer.
 */
int HEAP_push( HEAP *heap, void *element, size_t elmsize );

/** 
 * @brief check consistency of heap object.
 */
int HEAP_check( HEAP *heap );


typedef int  (*HEAP_VISITOR)   (int index, void *elm, size_t elmsize, void *context);



void HEAP_foreach_sorted( HEAP *heap, HEAP_VISITOR eval, void *context);


M_INLINE void HEAP_foreach( HEAP *heap, HEAP_VISITOR eval, void *context)
{
	size_t i;
	size_t sz = heap->elmcount;
	size_t elmsz = heap->elmsize;
	uint8_t *pos;

	for(pos = heap->buffer, i = 0; i < sz; i++, pos += elmsz) {
		eval( i, pos, elmsz, context ); 
	}
}

M_INLINE int HEAP_findif( HEAP *heap, HEAP_VISITOR eval, void *context, int32_t *retval)
{
	size_t i;
	size_t sz = heap->elmcount;
	size_t elmsz = heap->elmsize;
	int32_t ret;
	uint8_t *pos;

	if (!retval) {
	  *retval = 0;
	}

	for(i = 0, pos = heap->buffer; i < sz; i++, pos += elmsz) {
		if ( (ret = eval( i, pos, elmsz, context )) != 0) {
			if (retval) {
				*retval = ret;
			}
			return i;
		}
	}
	return -1;
}


#ifdef  __cplusplus
}
#endif

#endif

