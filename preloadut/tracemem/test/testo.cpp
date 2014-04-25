#include <string.h>
#include <stdio.h>


char *foo(int nrec)
{
    if (nrec == 0) 
    {
    char *p = new char[ 10 ];
    
    strcpy(p, "111111111" );
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

    char *p = foo( 10 );

    delete p;
    
    return 0;
}
