#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned char *ptr;
    int i;

    ptr = malloc(123);

    for(i = 0; i < 123; i++) {
	if (ptr[i] != 0xDD) {
	    printf("malloc: Allocated memory is not initialized with 0xDD. offset %d\n", i );
	    exit(1);
	}
	
    }

    ptr = realloc( ptr, 500 );

    for(i = 0; i < 500; i++) {
	if (ptr[i] != 0xDD) {
	    printf("realloc: Allocated memory is not initialized with 0xDD. offset %d\n", i );
	    exit(1);
	}
    }

    free(ptr);

    for(i = 0; i < 123; i++) {
	if (ptr[i] != 0xEE) {
	    printf("free: Freed memory is not initialized with 0xEE. offset %d\n", i );
	    exit(1);
	}
	
    }


    return 0;
}
