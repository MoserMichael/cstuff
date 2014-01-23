/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/array.h>
#include "vtest/vtest.h"



void ARRAY_test()
{
	ARRAY arr;
	int  elm;

	VASSERT( ! ARRAY_init( &arr, sizeof(int), 0 ) );

	elm = 1;
	VASSERT( ! ARRAY_push_back( &arr, &elm, sizeof(elm) ) );

	elm = 3;
	VASSERT( ! ARRAY_push_back( &arr, &elm, sizeof(elm) ) );

	elm = 4;
	VASSERT( ! ARRAY_push_back( &arr, &elm, sizeof(elm) ) );

	elm = 2;
	VASSERT( ! ARRAY_insert_at( &arr, 1, &elm, sizeof(elm) ) );


	VASSERT( ARRAY_maxsize(&arr) == 6);

	VASSERT (  ARRAY_size(&arr) == 4 );
	VASSERT ( *( (int *) ARRAY_at( &arr, 0)) == 1 );
	VASSERT ( *( (int *) ARRAY_at( &arr, 1)) == 2 );
	VASSERT ( *( (int *) ARRAY_at( &arr, 2)) == 3 );
	VASSERT ( *( (int *) ARRAY_at( &arr, 3)) == 4 );

	
	VASSERT( ! ARRAY_delete_at( &arr, 1 ) );
	VASSERT( ! ARRAY_pop_back( &arr, &elm, sizeof(int) ) );

	VASSERT (  ARRAY_size(&arr) == 2 );
	VASSERT ( *( (int *) ARRAY_at( &arr, 0)) == 1 );
	VASSERT ( *( (int *) ARRAY_at( &arr, 1)) == 3 );


	ARRAY_free( &arr );


}

