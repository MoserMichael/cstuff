#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *foo(int nrec)
{
    if (nrec == 0) 
    {
    char *p = strdup( "aaaaaaaaaaaaaa" );
    fprintf(stderr,"%s\n", p );
    return p;
    }
    else
    {
	return foo(nrec-1);
    }
}

int main()
{
    foo( 5 );

    void *ptr = foo(10);
    fprintf(stderr,"after foo\n");
    free( ptr );
    
    return 0;
}
