#define _USE_BSD
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <malloc.h>
#include <stdio.h>
#include "fn.h"
#define SEP_CHAR '/'

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


char *FN_dir_name( const char *path_name  )
{
   size_t len, slen;
   const char *str;
   char *ret;

   if (!path_name || (slen = strlen(path_name)) == 0)
     return 0;

   slen -= 1;
   if (slen > 0 && path_name[ slen ] == SEP_CHAR)
     slen -= 1;

   do {
     if (path_name[ slen ] == SEP_CHAR) {
       break;
     }
     slen -= 1;
   } while( slen != 0 ); 

   if (slen == 0)
     return 0;   

   str = path_name + slen;

   len = (str + 1) - path_name;  
   ret = (char *) malloc( len + 1 );
   if (!ret)
     return 0;

   memcpy( ret, path_name, len );
   ret[ len ] = '\0';
   
   return ret;
}

char *FN_file_name( const char *path_name )
{
   size_t len;
   char *str;
   char *ret;

   if (!path_name || strlen(path_name) == 0)
     return 0;
   
   str = strrchr( path_name, SEP_CHAR );
   if (!str)
     return 0;

   str += 1;  
   len = strlen( str ); 
   
   ret = (char *) malloc( len + 1 );
   if (!ret)
     return 0;
   
   strcpy( ret, str );
   
   return ret;
}

char *FN_make_path( const char *dir_name, const char *file_name )
{
  size_t len,add_sep = (size_t) -1;
  char *ret;

  if (!dir_name || strlen(dir_name) == 0) 
    return 0;
  if (!file_name || strlen(file_name) == 0) 
    return 0;

  len = strlen(dir_name);

  if (dir_name[ len - 1 ] != SEP_CHAR) {
    if (!file_name || file_name[0] != SEP_CHAR) {
      add_sep = len;
      len += 1;
    }
  }

  if (file_name) {
    len += strlen( file_name );
  }

  ret = malloc( len + 1 );

  strcpy(ret, dir_name );
  if (add_sep != (size_t) -1) {
    ret[ add_sep ] = SEP_CHAR;
    ret[ add_sep + 1 ] = '\0';
  }
  
  if (file_name)
    strcat( ret, file_name );

  return ret;
}

static char *FN_get_extension_imp( const char *fname_or_path )
{
   char *dot;
   char *sep;

   if (!fname_or_path || strlen(fname_or_path) == 0)
     return 0;

   dot = strrchr( fname_or_path, '.' );
   if (!dot)
     return 0;

   sep = strrchr( fname_or_path, SEP_CHAR );
   if (!sep)
     return dot;
   if (sep < dot)
     return dot;

   return 0;
}

char *FN_get_extension( const char *fname_or_path )
{
   char *ret;

   ret = FN_get_extension_imp( fname_or_path );
   if (!ret)
     return 0;

   return strdup( ret );
}


char *FN_set_extension( const char *fname_or_path, const char *extension )
{
  char *ext;
  size_t add_dot;
  size_t len;
  char *ret;

  if (!fname_or_path || !extension)
    return 0;

  add_dot = 0;
  if (extension[0] != '.')
    add_dot = 1;

  ext = FN_get_extension_imp( fname_or_path );
  if (!ext) {
    len = strlen( fname_or_path ) + strlen( extension ) + add_dot + 1;
    ret = malloc( len + 1 );
    if (!ret)
      return 0;

    strcpy( ret, fname_or_path );
    if (add_dot)
      strcat( ret, "." );
    strcat( ret, extension );

    return ret;
  }
  
  len = (ext - fname_or_path) + strlen( extension ) + 1;

  ret = malloc( len + 1 );
  if (!ret)
    return 0;

  memcpy( ret, fname_or_path, ext - fname_or_path + 1 );
  ret[ ext - fname_or_path + 1 ] = '\0';
  strcpy( ret + (ext - fname_or_path) + add_dot , extension );

  return ret;
}


//#ifdef __CYGWIN__
// cygwin function does not work for whatever reaso, so do it here
static char *my_normalize( const char *arg )
{
  const char *pos, *last;
  struct stat stat_buf;
  char *res;
  size_t len = 0;
  size_t max_len = PATH_MAX;

  //printf("my_normalize %s\n", arg);

  res = malloc( max_len + 1 );
  if (!res) {
     //printf("my ass\n");
     return 0;
  }

  if (getcwd( res, max_len ) == 0) {
    //printf("no cwd?\n");
    goto err;
  }
  len = strlen( res );
  //printf("cwd %s\n", res );

  for(last = pos = arg; ; ++pos) {
     if (*pos == '/' || *pos == '\0') {
        if (last[0] == '.' && (pos - last) == 1) {
	   // nothing to do
        } else if ((pos - last) == 2 && last[0] == '.' && last[1] == '.') {
	   // one dir up.
	   int len; 
	   
	   for( len = strlen( res ) - 1; len > 0 && res[ len ] != '/'; --len);
	   res[ len ] = '\0';  	       

	   //printf("Up - cwd %s\n", res );
        } else {
	   // is this a symlink?
           if ( stat( res, &stat_buf ) ) {
              goto add;
 	   }
	   if (S_ISLNK( stat_buf.st_mode )) {
	     char tmpbuf[ PATH_MAX + 1 ];
	     
	     if (readlink(res, tmpbuf, sizeof(tmpbuf)) < 0) {
		goto err;
	     }
	     strcpy(res, tmpbuf);
	     len = strlen( res );

	   } else if (!S_ISDIR( stat_buf.st_mode ) && *pos != '\0') {
	     //printf("Ups %s\n", pos );
	     goto err;
	   } else {
add:	   
	     if ( (len + 1 + (pos - last)) > max_len) {
		goto err;
	     }
	     strcat( res, "/" );
	     strncat( res, last, pos - last); 
	     len = strlen( res );
	     //printf("add %s\n", res );
	   }	   
        }
        last = pos + 1;
     }
     if (*pos == '\0') {
	break;
     }
  }
  
  return res;
err:
  free(res);
  return 0;
}
//#endif

char *FN_normalize( const char *relpath )
{
  char *tmp;

//#ifdef __CYGWIN__ 
  tmp = my_normalize( relpath );
//#else
//  tmp = realpath( relpath, 0 );
//#endif
                  
  //fprintf(stderr,"%s -> %s\n", relpath, tmp);
  return tmp;
}
