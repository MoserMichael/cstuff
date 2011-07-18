/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */


#include <cutils/array.h>


static int ARRAY_grow_default(ARRAY *arr,size_t request_size)
{
	size_t sz;
	void *rt;
	


	sz = 2 * request_size;

	rt = realloc( arr->buffer, sz * arr->elmsize);
	if (!rt) {
		return -1;
	}
	arr->buffer = rt;
	arr->elmmaxcount = sz;
	return 0;
}





int ARRAY_insert_at( ARRAY *arr, size_t index, void *elm, size_t elmsize)
{	
	if (elmsize != arr->elmsize) {
		return -1;
	}

 
	if (index >= arr->elmmaxcount) {
		if (ARRAY_grow_default(arr,index+1)) {
			return -1;
		}
	}


	if (arr->elmcount == arr->elmmaxcount) {
		if (ARRAY_grow_default(arr,arr->elmmaxcount+1)) {
			return -1;
		}
	}


	if (index < arr->elmcount) {
		memmove( arr->buffer + (index + 1) * elmsize, 
				 arr->buffer + index * elmsize, (arr->elmcount - index) * elmsize);

	} 

	if (index <= arr->elmcount) {
		arr->elmcount++;
	} else {
		memset( arr->buffer + arr->elmcount * arr->elmsize, 
				0, 
				arr->elmsize * (index - arr->elmcount) );		
		arr->elmcount = index + 1;
	}

	memcpy(arr->buffer + (index * elmsize), elm, elmsize);


	return 0;
}

int ARRAY_set_at( ARRAY *arr, size_t index, void *elm, size_t elmsize)
{	
	if (elmsize != arr->elmsize) {
		return -1;
	}

 
	if (index >= arr->elmmaxcount) {
		if (ARRAY_grow_default(arr,index+1)) {
			return -1;
		}
	}


	memcpy(arr->buffer + (index * elmsize), elm, elmsize);
	if (index >= arr->elmcount) {
		memset( arr->buffer + arr->elmcount * arr->elmsize, 
				0, 
				arr->elmsize * (index - arr->elmcount) );	
		arr->elmcount = index + 1;

	}

	return 0;
}
