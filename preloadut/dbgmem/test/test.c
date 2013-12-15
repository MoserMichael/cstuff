
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
#include <sys/types.h>
#include <sys/wait.h>

void  * test_malloc(int num);
void consume_tdata(void *);
void memset_wrap( void *trg, int ch, size_t n);
void memcpy_wrap( void *trg, void *src, size_t n);

void no_core()
{
  struct rlimit l;

  l.rlim_cur = 0;
  l.rlim_max = 0;
  
  if (setrlimit( RLIMIT_CORE, &l )) {
    fprintf(stderr,"Warning: Failed to set core limit\n");
  }
}

void smash_stack() 
{
  char buf[1];

  memset_wrap(buf, 100, 100);
}

#if 0
void *uninitialised_stack(void *a) 
{
  void *tst;

  printf("uninitialised pointer: %p", tst );  
  return 0;
}
#endif


int main(int argc, char *argv[])
{
  int tst_num,num_ch;
  void *tst,*val;


#if 1 
  no_core();
#endif  
  tst_num = argc > 1 ? atoi(argv[1]) : 0;
  num_ch = argc > 2 ? atoi(argv[2]) : 0;
  

  mallopt(M_CHECK_ACTION, tst_num != 13 ? 1 : 0); // on memory errors - print error but malloc does not call exit.

  tst = test_malloc(4);

  fprintf(stderr,"start test %d\n",tst_num);
  switch(tst_num) {

   case 0: {
     memcpy(&val,tst,sizeof(void *));
     // use uninitialised memory (printout 0xDDDDDDDD expected)
     printf("%p", val); 
   }
   break;
    
   case 1: {
     memset_wrap(tst,0,8);
     free(tst);
     
     memcpy(&val,tst,sizeof(void *));
     printf("%p", val);
     // use freed memory (printout 0xEEEEEEEE expected)
     //printf("%p", * ((void **) tst) ); 
   }
   break;
    
   case 2: {
        // memory underwrite in checked function
     memset_wrap( ((char *) tst) - 5, 10, 10);
   }
   break;

   case 3: {
     // memory overwrite in checked function 
     memset_wrap( ((char *) tst) + 5 , 10, 10);
   }
   break;

   case 4: {
     // overlapped copy
     memcpy_wrap( tst, ((char *)tst) + 2, 4 );
   }
   break;

   case 5: {
     // stack smashed
     smash_stack();
   }
   break;

   case 6: {
     // illegal free
     free( ((char *) tst) + 5);
   }
   break;

   case 7: {
     // free twice 
     free(tst);
     free(tst);
   }
   break;
  
   case 8: {
      // leak test
      mallopt(1002,0);
      tst = test_malloc(3);  
   }
   break;

#if 0
   case 9: {
      // uninitialised pthread stack
      pthread_t tid;
      pthread_create( &tid, 0, uninitialised_stack, 0 );   
      pthread_join(tid, 0);
   }
   break;
#endif

   case 10: {
       char *ptr;
       size_t rsize;
       
       printf("DBGMEM_is_installed() returns %d\n", DBGMEM_is_installed());
 
       if (!DBGMEM_GET_SIZE( tst, &rsize)) {
          printf("The size is %zd\n",rsize);
       } else { 
          printf("can't get size - invalid memory block");
       }
       
       // attempting to overwrite end of memory
       DBGMEM_CHECK_MEMORY(tst, 9,  DBGMEM_WRITE, "test1");       

       ptr = (char *) tst;
#if 1
       ptr[ 9 ] = 0;
       // memory violated
       DBGMEM_CHECK_MEMORY(tst, 1,  DBGMEM_WRITE, "testy");       
#endif  
       --ptr;
       * ptr = 0;
       // memory violated
       DBGMEM_CHECK_MEMORY(tst, 1,  DBGMEM_WRITE, "test2");       
       
       ptr = (char *) malloc(10);
       memcpy(ptr,"abcdeABCDE",10);
       
       // unterminated string
       DBGMEM_CHECK_STRING( ptr, (size_t) -1, (size_t) -1, DBGMEM_STRREAD, "test3");

   }
   break;

#ifdef  __cplusplus
  case 11: {
    char *tst;
    unsigned int *ltst;
    
    // ok
    tst = (char *) malloc(10);
    free(tst);

    // ok 
    tst = new char;
    delete  tst;

    // ok
    tst = new char[20];
    delete [] tst;

    // fail
    tst = (char *) malloc(11);
    delete tst;

    // fail 
    ltst = new unsigned int(1);
    delete  [] ltst;

    // fail
    tst = new char[21];
    delete tst;

  } break; 
#endif
  case 12: {
       char *ptr,*nptr = 0;

       // do a memory overwrite
       ptr = (char *) tst;
       ptr[ 9 ] = 0;

       // now crash
       *nptr = 0;
   }
   break;
   
   case 13: {
      char *ptr1,*ptr2;
     
      ptr1 = (char *) malloc(3);
      ptr2 = (char *) malloc(4);

      strcpy(ptr1,"");
      strcpy(ptr2,"abc");

      // overwrite
      strcpy(ptr1, ptr2);

      // glibc in release mode exits here.
#if 0
fprintf(stderr,"chk2\n");

      ptr1 = (char *) malloc(3);
      strcpy(ptr1,"a");


fprintf(stderr,"chk3\n");

      // overwrite
      strcat(ptr1,ptr2);  

fprintf(stderr,"chk4\n");

      ptr1 = (char *) malloc(3);
      strcpy(ptr1,"");

      // warning: results in null terminated string.
      strncpy( ptr1, ptr2, 3 );

      ptr1 = (char *) malloc(3);
      strcpy(ptr1,"a");
 
      //overwrite	 
      strncat(ptr1, ptr2, 2);
#endif
   }
   break;

   case 14:
   {
      static int repeat = 3;
      pid_t pid;
      int status;
ag:
     
      if (repeat > 0) {
         
	if ((pid = fork()) == 0) {
	  --repeat;
	  goto ag;

	} else {
	  waitpid(pid,&status,0);
	}
      }
   }
   break;

   case 15:
   {
     int i;
     char *ptr1;
     
     for(i=0;i<3;i++) { 
       DBGMEM_increase_generation_tag();
       ptr1 = (char *) malloc(10);
       strcpy(ptr1,"kuku");
     }
     fprintf(stderr,"Finish test 15\n");

   }
   break;

  }
  fprintf(stderr,"eof test %d\n",tst_num);


  if (num_ch > 0) {
    int rt;

    char cmd[30];

    sprintf(cmd,"./test %d %d", tst_num, num_ch-1);
    fprintf(stderr,">system(%s)\n",cmd);
    rt = system(cmd);
    if ( rt != 0 ) {
      printf("System failed!\n");
      exit(1);
    }
  }
  consume_tdata( tst );
  return 0;
}
