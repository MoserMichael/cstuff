/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <heap.h>
#include <stdlib.h>
#include <memory.h>

int HEAP_push( HEAP *heap, void *element, size_t elmsize )
{
	size_t pos, pos_offset;
	size_t parent, parent_offset;

	if (elmsize != heap->elmsize) {
		return -1;
	}


	if (heap->elmcount >= (heap->elmmaxcount - 1)) {
		unsigned char *pbuffer;
		size_t sz = heap->elmmaxcount * 2;

		
		pbuffer = realloc( heap->buffer, (sz) * heap->elmsize );	
		if (!pbuffer) {
			return -1;
		}
		heap->buffer = pbuffer;
		heap->elmmaxcount = sz;
	}

	elmsize = heap->elmsize;

	heap->elmcount ++;

	pos = heap->elmcount;
	while(pos > 1) {

		parent = pos / 2;

		pos_offset = (pos - 1) * elmsize;
		parent_offset = (parent - 1) * elmsize;

		if ( heap->compare_func( heap->buffer + parent_offset, 
								 element, 
								 elmsize ) > 0) {
			
			memcpy( heap->buffer + pos_offset, heap->buffer + parent_offset, elmsize);
			pos = parent;
		} else {
			break;
		}
	}

	memcpy( heap->buffer + (pos - 1) * elmsize, element, elmsize);


	return 0;
}	


int HEAP_pop( HEAP *heap)
{
	size_t pos, pos_offset;
	size_t left_child, left_child_offset;
	size_t right_child, right_child_offset;
	size_t next_pos, next_pos_offset;
	size_t last_element, last_element_offset;
	size_t elmsize;

	if (heap->elmcount == 0) {
		return -1;
	}

	elmsize = heap->elmsize;

	last_element = heap->elmcount;
	last_element_offset = (last_element -1) * elmsize;
	heap->elmcount--;
	
	pos = 1;
	while( pos <= heap->elmcount) {
		
		left_child = (pos << 1);
		if (left_child > heap->elmcount) {
			break;
		}


		right_child = left_child + 1;

		right_child_offset = (right_child - 1) * elmsize;
		left_child_offset = (left_child - 1) * elmsize;

		if (left_child != heap->elmcount &&
			heap->compare_func( heap->buffer + right_child_offset, 
								heap->buffer + left_child_offset, 
								elmsize) < 0) {

			next_pos = right_child;
		} else {
			next_pos = left_child;
		}


		next_pos_offset = (next_pos-1) * elmsize;
		if (heap->compare_func(heap->buffer + last_element_offset,
							   heap->buffer + next_pos_offset,
							   elmsize) > 0) {

			pos_offset = (pos - 1) * elmsize;
			memcpy( heap->buffer + pos_offset, heap->buffer + next_pos_offset, elmsize);
		} else {
			break;
		}
  		pos = next_pos;
		
	}
	
	pos_offset = (pos - 1) * elmsize;
	if (pos_offset != last_element_offset) {
	    memcpy( heap->buffer + pos_offset, heap->buffer + last_element_offset, elmsize);
	}



	return 0;
}


static int check_recursive(HEAP *heap, size_t pos)
{
	size_t elmsize = heap->elmsize;
	size_t pos_left;
	size_t pos_right;

	pos_left = (2 * pos);
	if (pos_left <= heap->elmcount) {

		if (heap->compare_func( heap->buffer + (pos - 1) * elmsize,  
							    heap->buffer + (pos_left - 1) * elmsize, 
								elmsize) > 0) {
			return 0;
		}
		if (!check_recursive( heap, pos_left)) {
			return 0;
		}
	}


	pos_right =   (2 * pos + 1) * elmsize;
	if (pos_right <= heap->elmcount) {

		if (heap->compare_func( heap->buffer + (pos - 1) * elmsize,  
							    heap->buffer + (pos_right - 1) * elmsize, 
							    elmsize) > 0) {
			return 0;
		}
		if (!check_recursive( heap, pos_right)) {
			return 0;
		}
	}
	return 1;
}


int HEAP_check( HEAP *heap )
{

	if (heap->elmcount > heap->elmmaxcount) {
		return 0;
	}
	if (!check_recursive(heap, 1)) {
		return 0;
	}
	return 1;
}


static void visit_sorted(HEAP *heap, size_t pos, HEAP_VISITOR eval, void *context)
{
	size_t elmsize = heap->elmsize;
	size_t pos_left;
	size_t pos_right;

	if (heap->elmcount > heap->elmmaxcount) {
		return;
	}

	pos_left = (2 * pos);
	if (pos_left <= heap->elmcount) {

		check_recursive( heap, pos_left );
	}

	pos_right =   (2 * pos + 1) * elmsize;
	if (pos_right <= heap->elmcount) {

		check_recursive( heap, pos_right );
	}

        eval( pos, heap->buffer + (pos - 1) * elmsize, elmsize, context );
}



void HEAP_foreach_sorted( HEAP *heap, HEAP_VISITOR eval, void *context)
{
 	visit_sorted( heap, 1, eval, context );
}

	
