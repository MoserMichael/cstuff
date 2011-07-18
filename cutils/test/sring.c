/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/sring.h>
#include <vtest/vtest.h>

typedef struct
{
  SRING base;
  int num;
}
  INT_DLIST_ENTRY;

static int compare_entry(INT_DLIST_ENTRY *a1, INT_DLIST_ENTRY *a2)
{
	
	if (a1->num < a2->num) {
		return -1;
	}

	if (a1->num > a2->num) {
		return 1;
	}
	return 0;
}

static int *shuffle(int n)
{
	int *arr,i,tmp,pos;
	
	arr = (int *) malloc(n * sizeof(int));
	for(i = 0; i < n; i++) {
		arr[i] = i;
	}

	for(i = 0; i < n; i++) {

		pos = (rand() % (n - 1)) + 1;
		
		tmp = arr[0];
		arr[0] = arr[pos];
		arr[pos] = tmp;

	}
	return arr;

}

void SRING_test()
{
	SRING list;
	INT_DLIST_ENTRY *tmp;
	SRING *prev, *pos;
	int i, *arr;


	SRING_init( &list );

	for(i=9;i>=0;i--) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2);
		SRING_push_front(&list, (SRING *) tmp);
	}

	VASSERT( SRING_check( &list ) );

	i = 0;
	SRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20 );


    pos = SRING_get_first( &list );
	for(i=0;i<10;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2) + 1;

  	    SRING_insert_after( pos, (SRING *) tmp);
		pos = SRING_get_next( &list, pos );
		pos = SRING_get_next( &list, pos );
	}

	VASSERT( SRING_check( &list ) );

	i = 0;
	SRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i++;
	}
	VASSERT( i == 20 );


	i = 0;
	prev=  &list;
	SRING_FOREACH(pos, &list ) {
		if (i & 1) {
			free( SRING_unlink_after( prev ) );
			VASSERT( SRING_check( &list ) );
			pos = prev;
		}
		prev = pos;
		i+= 1;
	}
	VASSERT( i == 20 );
	
	VASSERT( SRING_check( &list ) );

	i = 0;
	SRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20);
	
	VASSERT( SRING_check( &list ) );

	while(!SRING_isempty( &list)) {
			free( SRING_unlink_after( &list));
	}

	VASSERT( SRING_check( &list ) );
	VASSERT( SRING_isempty( &list ) );


	arr = shuffle(100);
	for(i=0; i<100; i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = arr[i];

		SRING_insert_sorted(&list, (SRING_COMPARE) compare_entry,(SRING *) tmp);
	}
	free(arr);

	VASSERT( SRING_check( &list ) );

	i = 0;
	SRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);
		
	while(!SRING_isempty( &list)) {
			free( SRING_unlink_after( &list ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		SRING_push_front(&list, (SRING *) tmp);
	}
	VASSERT( SRING_check( &list ) );

	i = 99;
	SRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i -= 1;
	}
	VASSERT( i == -1);

	while(!SRING_isempty( &list)) {
			free( SRING_unlink_after( &list ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		SRING_push_front(&list, (SRING *) tmp);
	}

	VASSERT( SRING_check( &list ) );

	i = 99;
	SRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i -= 1;
	}
	VASSERT( i == -1);

	while(!SRING_isempty( &list)) {
			free( SRING_unlink_after( &list ));
	}	
}
