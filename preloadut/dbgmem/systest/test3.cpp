#include <malloc.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

class foo_bar 
{
public:
  foo_bar()
  {
     char *ptr;
     int i;

     for(i = 0; i < 5; i++ ) {
       ptr = new char[99];   
       v.push_back( ptr );
     }  
  }

  void clear()
  {
    size_t i;
    for( i=0; i < v.size(); i++) 
       delete [] v[i];

  }

  vector<char *> v;
}
  global;


static const char *vect[] = { "-", "\\", "|", "/", (char *) 0 };
int do_cont = 1;


void *leak_it(int rec,size_t size)
{
  if (rec) { 
    return leak_it(--rec,size);
  }
  return malloc( 100 + size * 10);
}

static void sigint_handler(int sig)
{
#define MESSAGE "SIGINT received\n"

  (void) sig;

  write(2, MESSAGE, strlen(MESSAGE) );
  exit(0);
}

int main()
{
   size_t pos = 0;
   FILE *fp;

   fp = fopen("test3.pid","w");
   if (fp) {
     fprintf(fp,"%u",getpid());
     fclose(fp);
   }

  
   signal( SIGINT, sigint_handler);
  
   while(do_cont) {
 
     leak_it(3+pos, pos);

     if (vect[pos] == 0) {
       pos = 0;
     }
     fprintf(stderr,"%s\r",vect[pos]);
     ++pos;
     sleep(1);
  }
  
  global.clear();
  fprintf(stderr,"test3 exit\n");
  return 0;
}
