
/* 
 * DBGMEM - memory allocation leak tracker and debugging tool.
 *
 *  (c) 2008 by Michael Moser
 * 
 * This code is available under the GNU Lesser Public License version 2.1
 *
 * See LICENSE file for the full copy of the license
 *
 */

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <pthread.h>

#include <dbgmemutil.h>


void  * test_malloc(int num, int tst);

void * test_malloc2(int num, int tst)
{
  if (num <= 0) {
    switch(tst) {
      case 0:
         return  (void *) malloc(8); // just produce a leak
      
      case 1: {
      
         char *tst = (char *) malloc(10);
	 
	 tst[11] = 'a'; //memory overwrite
         return tst;
      }
      
      case 2: {
      
         char *tst = (char *) malloc(10);
	 
	 tst[-1] = 'a'; //memory underwrite
         return tst;
      }

      default:
         return 0;

    }	 

  } else {
    return test_malloc( num - 1, tst );
  }
  
}

void * test_malloc(int num, int tst)
{
  return test_malloc2(num, tst);
}



int main()
{
  int i;

  for(i=0;i<10;i++) {

   test_malloc(10+i,0);
  }

  mallopt(1002,0);
  
  test_malloc(3,1);
  test_malloc(3,2);
  test_malloc(2,0);
   
  return 0;
}
