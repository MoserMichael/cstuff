#define _USE_BSD
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <malloc.h>
#include "fn.h"

#define SEP_CHAR '/'



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

char *FN_normalize( const char *relpath )
{
  char tmp[ PATH_MAX + 1];
  char *rtp;

  strcpy( tmp, "" );
  rtp = realpath( relpath, tmp );
  if (strcmp(tmp,"") == 0)
    return 0;

  return strdup( tmp );
}
