#include <malloc.h>
#include <signal.h>
#include <string.h>

int main()
{
    int i;
    char *ptr;

    //for(i=0; i < 32 * 1024; i +=16)
    for(i=0; i < 8192; i += 1024 )
    {
	ptr = (char *) malloc( i );
	memset(ptr, 0xFF, i );
	fprintf(stderr,"%p ", ptr );
    }

    ptr[-1] = '0';

    //free(ptr);
    //free(ptr);
    raise(SIGSEGV);
    
    return 0;
}
