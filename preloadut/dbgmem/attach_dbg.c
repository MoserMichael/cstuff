
/* 
 * DBGMEM - memory allocation leak tracker and debugging tool.
 *
 *  (c) 2008 by Michael Moser
 * 
 * 3-clause BSD License applies  
 *
 * See LICENSE file for the full copy of the license
 *
 */

#include "memalloc.h"
#include <unistd.h>

STATIC_C  int in_attach_dbg, in_dbg;





static int attach_dbg()
{
   pid_t pid;
   int   line_num,line_num_a;
   char pname[ PATH_MAX +1 ];

   //* TODO: check if in debugger - if yes then , sigh, do nothing. *

	
   line_num_a = __LINE__;
   if (in_dbg) {
	 return 0;
   }

   get_process_name( pname, sizeof(pname) );

   in_dbg = in_attach_dbg = 1;

   line_num = __LINE__;
   pid = fork();
   
   if (pid == -1) {
       dbgmem_log_err("DBGMEM: can't attach debugger. fork failed errno %d\n", errno);
       return -1; 
   }

   if (pid != 0) {
       // parent proces
       while(in_attach_dbg) { sleep(1); }
       return 1;
   }	

   if (pid == 0) {
       pid_t parent; 
       FILE *fp;

       ignore_this_process = 1;

       // child process
       parent = getppid();

       // write gdb script.
       fp = fopen("gdb.scr","w");
       if (fp) {

         fprintf(fp,"attach %d\nb %s:%d\nb %s:%d\nset in_attach_dbg=0\nc\n", 
			parent, 
			__FILE__, line_num + 11, 
			__FILE__, line_num_a + 2 );	
         fclose(fp);

         // attach gdb to parent process.
         execlp("/usr/bin/gdb", "gdb", pname, "--command=gdb.scr", (const char *) 0);
         dbgmem_log_err("DBGMEM: failed to run the debugger errno %d\n", errno);
       } else { 
         dbgmem_log_err("DBGMEM: failed to run the debbugger - can't write dbg script errno %d\n", errno);
       }
       exit(1);
   } 
   return -2;
}

static void dump_core()
{
  char *data = 0;

  *data = 0;
}


STATIC_C int attach_debugger()
{

  if (attach_gdb_on_errors==1) {
     dump_core();
  }
  
  if (attach_gdb_on_errors==2) {
    return attach_dbg();
  }
  return 0;
}

STATIC_C const char *get_attach_debugger_mode()
{
  switch( attach_gdb_on_errors) {
    case 1: 
      return "dump core";
    case 2:
      return "attach gdb";
    default:
      return "continue";
  }
  return "";
}
