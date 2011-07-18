/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/heap.h>
#include <vtest/vtest.h>
#include <stdio.h>
#include <string.h>


typedef struct HEntry {
	int Key;
	char Name[6];
} HEntry;

int HEntry_compare(void *lh, void *rh, size_t entry)
{
	HEntry *le = (HEntry *) lh;
	HEntry *re = (HEntry *) rh;

	M_UNUSED(entry);

	if (le->Key < re->Key) {
		return -1;
	}
	if (le->Key > re->Key) {
		return 1;
	}
	return 0;
}

void check_heap(void *data, size_t elsize, void *context)
{
	HEntry *pentry = (HEntry *) data;
	int *num = (int *) context;

	M_UNUSED(elsize);

	VASSERT( *num < pentry->Key);
	*num = pentry->Key;
}


typedef struct tagVISITCTX {
	HEntry *entry;
} 
	VISITCTX;

int  heap_visitor_check_sorted(int index, void *elm, size_t elmsize, void *context)
{
 	VISITCTX * ctx = (VISITCTX *) context;	
	HEntry *pentry = (HEntry *) elm;

	M_UNUSED( index );
	M_UNUSED( elmsize );


	if (ctx->entry) {
		VASSERT_RET( HEntry_compare( ctx->entry, elm, 0 ) < 0 , -1);
	}
	ctx->entry = pentry;
	return  0;
}




static void *shuffle(int n)
{
	int *arr,i,tmp,pos;
	
	arr = (int *) malloc(n * sizeof(int));
	for(i = 0; i < n; i++) {
		arr[i] = i;
	}

	for(i = 0; i < n; i++) {

		pos = (rand() % (n - 1)) + 1;

		VASSERT_RET(pos > 0 && pos < n, 0);
		tmp = arr[0];
		arr[0] = arr[pos];
		arr[pos] = tmp;

	}
	return arr;
}

#ifdef SHOW_RESULTS
void printheap(HEAP *heap)
{

	size_t i;

	for(i=0;i<heap->elmcount;i++) {
		HEntry *entry = (HEntry *) (heap->buffer + i * heap->elmsize);
		printf(" %03d ", entry->Key);
	}
	printf("\n");
}
#endif


#define TEST_SIZE 20000

void HEAP_test()
{
	HEAP heap;
	HEntry entry,*pentry;
	int *tmp,i;
        VISITCTX sorted_ctx;

	VASSERT( !HEAP_init( &heap, 10, sizeof(HEntry),  HEntry_compare) );

	tmp = shuffle(TEST_SIZE);
#ifdef SHOW_RESULTS
	printheap(&heap);
#endif

	for(i=0;i<TEST_SIZE;i++) {
			entry.Key = tmp[i];
			sprintf(entry.Name,"N:%03d",tmp[i]);

			HEAP_push(  &heap, (unsigned char *) &entry, sizeof(entry) );

#ifdef SHOW_RESULTS
			printheap(&heap);
#endif
			
			VASSERT(  HEAP_check(&heap) );

	}
	free(tmp);

#ifdef SHOW_RESULTS
	printf("pop!\n");
#endif


	VASSERT( HEAP_check(&heap) );
	VASSERT( HEAP_size(&heap) == TEST_SIZE );

	i = -1;
	while( (pentry = (HEntry *) HEAP_top( &heap )) != 0 ) {

#ifdef SHOW_RESULTS
		printheap(&heap);
		printf("->%d\n",pentry->Key);
#endif
		VASSERT(pentry->Key > i);


		sprintf(entry.Name,"N:%03d",pentry->Key);
		VASSERT( pentry->Key >= i);
		VASSERT( strcmp(pentry->Name,entry.Name) == 0);
		i = pentry->Key;
		
		HEAP_pop(  &heap);
	}

	VASSERT(HEAP_check(&heap));


	sorted_ctx.entry = 0;
	HEAP_foreach_sorted( &heap, heap_visitor_check_sorted, &sorted_ctx);

	HEAP_free(&heap);
}

