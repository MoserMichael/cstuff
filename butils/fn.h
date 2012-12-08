#ifndef _FN_H_
#define _FN_H_

char *FN_dir_name( const char *path_name );
char *FN_file_name( const char *path_name );
char *FN_make_path( const char *dir_name, const char *file_name );
char *FN_get_extension( const char *fname_or_path );
char *FN_set_extension( const char *fname_or_path, const char *extension );
char *FN_normalize( const char *relpath );

#endif
