/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#ifdef  __cplusplus
extern "C" {
#endif


#include <cutils/base.h>
#include <stdlib.h>
#include <string.h>

/**
 * @defgroup ARRAY
 * @brief Dynamic array, all elements contained in this structure are of the same size.
 *
 * A dynamic array, growable array, resizable array, dynamic table,
 * or array list is a data structure, an array which is automatically expanded to 
 * accommodate new objects if filled beyond its current size. 
 * In most ways the dynamic array has performance similar to an array, with the addition 
 * of new operations to add and remove elements from the end:
 *
 *   - Getting or setting the value at a particular index (constant time)
 *   - Iterating over the elements in order (linear time, good cache performance)
 *   - Add an element to the end (constant amortized time)
 *
 * From http://en.wikipedia.org/wiki/Dynamic_array
 *
 *
 * @{
 */
typedef struct  {

	size_t   elmmaxcount;
	size_t   elmsize;
	size_t   elmcount;

	uint8_t  *buffer;

} ARRAY;


M_INLINE int ARRAY_init( ARRAY *arr, size_t elmsize, size_t numofelem)
{
	
	arr->elmsize	= elmsize;
	arr->elmcount	= 0;
	arr->elmmaxcount= numofelem;
	arr->buffer	= 0;


	if (numofelem) {
		if ((arr->buffer = (uint8_t *) malloc( elmsize * numofelem)) == 0) {
			return -1;
		}
	}
	return 0;
}


/**
 * @brief Allocates a dynamic array from already allocated memory area; 
 *
 * The memory is not owned by this object, meaning that this dynamic array cannot be resized 
 * (does not grow as maximum size is exceed); data are is not freed in object destructor.
 * 
 * @param arr		(out) the object
 * @param elmsize	(in)  size of one element
 * @param ptr		(in)  memory area that will contain
 * @param busize	(in)  size of memory area pointed to by ptr (size of Data area passed has to be greater or equal to the size of one element elmsize).
 * @return 0 if ok -1 on failure.
 */
M_INLINE  int ARRAY_init_fixed( ARRAY * arr, size_t elmsize, void *ptr, size_t bufsize)
{
	if (bufsize < elmsize) {
		return -1;
	}

	arr->elmsize = elmsize;

	arr->elmcount = 0;
	arr->elmmaxcount = bufsize / elmsize;
	arr->buffer = ptr;

	return 0;
}

/** 
 * @brief Macro: Allocate dynamic array on stack
 *
 * Warning! this macro uses alloca standard library function - this is not always portable, 
 * and it bombs out when it causes stack overflow.
 *
 * @param arr		(out) (type ARRAY *) the object.
 * @param elmsize	(in)  (type size_t) size of one element
 * @param numofelem (in)  (type size_t) maximum number of elements.
 */
#define ARRAY_init_stack( arr, elmsize, numofelem ) \
do { \
	(arr)->ctx = 0; \
	(arr)->elmcount = 0; \
	(arr)->elmsize = (elmsize); \
	(arr)->elmmaxcount = (elmsize) * (numofelem); \
	(arr)->buffer = (char *) alloca( (str)->elmmaxcount ); \
} while(0);


/** 
 * @brief free all memory held by dynamic array (destructor).
 * @param buf (in out) the object
 */
M_INLINE void ARRAY_free( ARRAY *arr) 
{
	if (arr->buffer) {
		free( arr->buffer );
		arr->buffer = 0;
	}
}

M_INLINE  void ARRAY_reset( ARRAY * arr)
{
	arr->elmcount = 0;
}

/**
 * @brief returns number of objects that are currently held by this collection.
 * @param  arr (in) the object
 * @return 
 */
M_INLINE size_t ARRAY_size( ARRAY *arr) 
{
	return arr->elmcount;
}

/** 
 * @brief returns maximum number of elements that can currently be held by this collection.
 * @param  arr (in) the object
 * @return 
 */
M_INLINE size_t ARRAY_maxsize( ARRAY *arr )
{
	return arr->elmmaxcount;
}

/**
 * @brief change size of dynamic array (i.e. change size of memory allocated for array)
 * If requested size is smaller then the current size, then all skipping elements are discarded.
 *
 * @param arr (in out) the object
 * @param num_elem (in) change maximum number of element to this number. 
 */
M_INLINE int ARRAY_resize( ARRAY *arr, size_t num_elem) 
{
	uint8_t * ptr;

	if (num_elem > arr->elmmaxcount) {
		ptr = realloc( arr->buffer, num_elem * arr->elmsize);
		if (!ptr) {
			return -1;
		}
		arr->buffer = ptr;
		arr->elmmaxcount = num_elem;
		if (num_elem < arr->elmcount) {
			arr->elmcount = num_elem;
		}
	} else {
		arr->elmcount = num_elem;
	}
	return 0;
}

M_INLINE uint8_t * ARRAY_at( ARRAY * arr, size_t index)                
{
	if (index >= arr->elmcount) {
		return 0;
	}

	return arr->buffer + (index * arr->elmsize);
}

M_INLINE int ARRAY_copy_at( ARRAY * arr, size_t index, void *elm, size_t elmsize) 
{
	if (index >= arr->elmcount) {
		return -1;
	}
	
	if (elmsize != arr->elmsize) {
		return -1;
	}

	memcpy( elm, arr->buffer + (index * arr->elmsize), elmsize );
	return 0;
}

int ARRAY_insert_at( ARRAY *arr, size_t index, void *elm, size_t elmsize);

int ARRAY_set_at( ARRAY *arr, size_t index, void *elm, size_t elmsize);


M_INLINE int ARRAY_delete_at( ARRAY *arr, size_t index)
{
	if (index >= arr->elmcount) {
		return -1;
	}
	
	if (index < (arr->elmcount - 1)) {
		size_t elmsize = arr->elmsize;

		memmove( arr->buffer + index * elmsize,
				 arr->buffer + (index + 1) * elmsize, 
				 (arr->elmcount - index - 1) * elmsize);
		
	} 
	--arr->elmcount;
	return 0;
}


M_INLINE int ARRAY_push_back( ARRAY *arr, void *elm, size_t elmsize)
{	
	return ARRAY_insert_at(arr,arr->elmcount,elm, elmsize);		
}


M_INLINE int ARRAY_pop_back( ARRAY *arr, void *ptr, size_t elmsize )
{
	if (arr->elmcount <=0) {
		return -1;
	}

	if (ptr) {
		if (elmsize != arr->elmsize) {
			return -1;
		}

		memcpy( ptr, arr->buffer + (--arr->elmcount) * arr->elmsize, elmsize);
	}
	return 0;
}

M_INLINE int ARRAY_stack_top( ARRAY *arr, void *ptr, size_t elmsize )
{
	if (arr->elmcount <=0) {
		return -1;
	}

	if (ptr) {
		if (elmsize != arr->elmsize) {
			return -1;
		}

		memcpy( ptr, arr->buffer + ((arr->elmcount - 1) * arr->elmsize), elmsize);
	}
	return 0;
}

#if 0

#define ARRAY_FOREACH( loopvarname, array )
  
#define ARRAY_FOREACH_RANGE( loopvarname, from_idx, to_idx, array )

#define ARRAY_FOREACH_REVERSE( loopvarname, array )

#define ARRAY_FOREACH_REVERSE_RANGE( loopvarname, from_idx, to_idx, array )

#endif

/* Ups: can do this only with gcc typeof, very bad, not portable

#define ARRAY_FOREACH( loopvarname, array )	{\
	char *ARRAY_FOREACH_##loopvarname_eof = (array)->buffer + ((array)->elmcount * (array)->elmsize); \
	char *ARRAY_FOREACH_##loopvarname_ptr = (array)->buffer; \
	for(;ARRAY_FOREACH_##loopvarname_ptr < ARRAY_FOREACH_##loopvarname_eof; ARRAY_FOREACH_##loopvarname_ptr += (array)->elmsize) { \
		loopvarname = (cast-to-type-of-loopvarname) ARRAY_FOREACH_##loopvarname_ptr
*/


typedef int  (*ARRAY_VISITOR)   (int index, void *elm, size_t elmsize, void *context);

M_INLINE void ARRAY_foreach( ARRAY *arr, ARRAY_VISITOR eval, void *context)
{
	size_t i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint8_t *pos;

	for(pos = arr->buffer, i = 0; i < sz; i++, pos += elmsz) {
		eval( i, pos, elmsz, context ); 
	}
}

M_INLINE int ARRAY_foreach_range( ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx)
{
	size_t i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint8_t *pos;

	if (!(from_idx > 0 && from_idx < to_idx && (size_t) to_idx <  sz)) {
		return -1;
	}

	for(pos = arr->buffer + (from_idx * elmsz), i = from_idx; i < (size_t) to_idx; i++, pos += elmsz) {
		eval( i, pos, elmsz, context ); 
	}

	return 0;
}



M_INLINE void ARRAY_foreach_reverse( ARRAY *arr, ARRAY_VISITOR eval, void *context)
{
	size_t i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint8_t *pos;

	for(pos = arr->buffer + (elmsz * (sz-1)), i = sz-1; ; i--, pos -= elmsz) {
		eval( i, pos, elmsz, context ); 
		if (i == 0) {
			break;
		}
	}
}


M_INLINE int ARRAY_foreach_reverse_range( ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx)
{
	int i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint8_t *pos;

	if (!(from_idx > 0 && from_idx < to_idx && (size_t) to_idx < sz)) {
		return -1;
	}

	for(i = (to_idx - 1), pos = arr->buffer + (elmsz * i); ; i--, pos -= elmsz) {
		eval( i, pos, elmsz, context ); 
		if (i == from_idx) {
			break;
		}
	}

	return 0;
}


M_INLINE int ARRAY_findif( ARRAY *arr, ARRAY_VISITOR eval, void *context, uint32_t *retval)
{
	size_t i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint32_t ret;
	uint8_t *pos;

	if (!retval) {
	  *retval = 0;
	}

	for(i = 0, pos = arr->buffer; i < sz; i++, pos += elmsz) {
		if ( (ret = eval( i, pos, elmsz, context )) != 0) {
			if (retval) {
				*retval = ret;
			}
			return i;
		}
	}
	return -1;
}

M_INLINE int ARRAY_findif_range( ARRAY *arr, ARRAY_VISITOR eval, void *context,  
							    int from_idx, int to_idx, uint32_t *retval)
{
	int  i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint32_t ret;
	uint8_t *pos;

	if (!retval) {
	  *retval = 0;
	}

	if (!(from_idx > 0 && from_idx < to_idx && (size_t) to_idx < sz)) {
		return -1;
	}

	for(i = from_idx, pos = arr->buffer + (from_idx * elmsz); i < to_idx; i++, pos += elmsz) {
		if ((ret = eval( i, pos, elmsz, context )) != 0) {
			if (retval) {
				*retval = ret;
			}			
			return i;
		}
	}

	return -1;
}



M_INLINE int ARRAY_findif_reverse( ARRAY *arr, ARRAY_VISITOR eval, void *context, uint32_t *retval)
{
	size_t i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint32_t ret;
	uint8_t *pos;

	if (!retval) {
	  *retval = 0;
	}

	for(i = sz-1, pos = arr->buffer + (elmsz * (sz-1)); ; i--, pos -= elmsz) {
		if ((ret = eval( i, pos, elmsz, context )) != 0) {
			if (retval) {
				*retval = ret;
			}			
			return i;
		}
		if (i == 0) {
			break;
		}
	}
	return -1;
}


M_INLINE int ARRAY_findif_reverse_range( ARRAY *arr, ARRAY_VISITOR eval, void *context, 
									  int from_idx, int to_idx, uint32_t *retval)
{
	int i;
	size_t sz = arr->elmcount;
	size_t elmsz = arr->elmsize;
	uint32_t ret;
	uint8_t *pos;

	if (!retval) {
	  *retval = 0;
	}

	if (!(from_idx > 0 && from_idx < to_idx && (size_t) to_idx < sz)) {
		return -1;
	}

	for(i = to_idx - 1, pos = arr->buffer + (elmsz * i); ; i--, pos -= elmsz) {
		if ((ret = eval( i, pos, elmsz, context )) != 0) {
			if (retval) {
				*retval = ret;
			}			
			return i;
		}
		if (i == from_idx) {
			break;
		}
	}

	return -1;
}


/** 
 * @brief iterate over all entries of the array and delete entries that match predicate from the array.
 * @param list (in) the object.
 * @param check_if (in) predicate function; the function returns 1 then inspected argument element will be deleted; if argument pointer is NULL then all entries will be deleted. 
 * @param context (in) data pointer passed to every invocation of check_if
 */
#if 0
M_INLINE void ARRAY_deleteif( ARRAY *list, VDLIST_VISITOR check_if, void *context)
{
    VDLIST_entry *cur,*next,*del;

    VDLIST_FOREACH_SAVE(cur,next,list) {
		if (!check_if || check_if( list, cur, context))  {
			del = VDLIST_unlink( list, cur );
			if (free_ctx) {
				free_ctx->free( M_MEMBEROF(del,offset_of_link) );
			}
		}
	}
}
#endif

/**
 * @}
 */

#ifdef  __cplusplus
}
#endif

#endif
