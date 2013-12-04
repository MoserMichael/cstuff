
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

#include <unistd.h>

static  int get_line( int fd, char *buf, size_t buf_size )
{
  size_t pos;
  char ch = 'a';

  (void) buf_size;

  for(pos = 0; ch != '\n';) {

    if (read(fd, &ch, 1) == 1) {
      buf[ pos ++ ] = (char) (ch == '\n' ? '\0' : ch);
    } else {
      buf[ pos ] = '\0';
      break;
    }
   }	
   return pos;
}


STATIC_C int get_process_name(char *rbuf, size_t buf_size)
{
  char buf[ PATH_MAX * 2 + 1 ];

  int fd = open( "/proc/self/maps", O_RDONLY);
  int ret = -1;
  
  if (fd != -1) {
    while( get_line( fd, buf, sizeof(buf) ) ) {
      char *tok;	
      int i;
      
      // is this executable mapping?			
      tok = strtok( buf, " \t");
      if (!tok) {
        break;
      }
      tok = strtok( 0, " \t");
      if (tok && strlen(tok) == 4 && tok[2] == 'x') {
				
        // if it is then get the module path
	for(i=0;i<4 && (tok = strtok( 0, " \t")) != 0;i++);
					
	  if (tok) {
	    strncpy(rbuf, tok, buf_size);
	    ret = 0;
	  }
      }
    }
    close(fd);
  }
  return ret;
}

#if 0
STATIC_C int get_lib_address(const char *lib_pattern, void **low_address, void **high_address)
{  
  int ret = 0;
  int fd, i; 
  char buf[ PATH_MAX * 2 + 1 ];
  char *tok, *from_addr, *to_addr;

  fd = open( "/proc/self/maps", O_RDONLY);  

  *low_address = 0;
  *high_address = 0;
  
  if (fd == -1) {
    return -1;
  }
  while( get_line( fd, buf, sizeof(buf) ) ) {
    // parse line 
    from_addr = strtok( buf, " -\t");
    to_addr = strtok( 0, "-"); // \t") 
	
    for(i=0;i<5 && (tok = strtok( 0, " \t")) != 0;i++);
 
    // does the file name match the pattern ?
    if (tok && strstr(tok,lib_pattern) != 0) {
      void *val;

      sscanf( from_addr, "%p", &val );
      *low_address = val;

      sscanf( to_addr, "%p", &val );     
      *high_address = val;

      ret = 1; 
      break;
    }
  }
  close(fd);
  return ret;
}
#endif
