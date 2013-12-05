#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

const char *arr[]={ "-", "\\", "|", "/", 0 };


void sigint(int signo)
{
  //_exit(0); - _exit does not call atexit handlers - and apparently it does not call destructors. 
  exit(0); // this one works!
}

main()
{

   // turns out the destructor of the shared library is only called when exit is called.
   // so signal handler that does not do exit - no destructor.
   // in older systems this was different (maybe this is Redhat feature?)

   signal( SIGINT, sigint);
   //signal( SIGINT, SIG_DFL );

   int i = 0;
   while(1) {

     fprintf(stderr,"%s\r", arr[ i++ ] );
     if (arr[i]==0) {
       i = 0;
     }
     sleep(1);
   }
   return 0;
}
