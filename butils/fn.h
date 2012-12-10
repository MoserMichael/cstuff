#ifndef _FN_H_
#define _FN_H_


/**
 * @defgroup HEX
 * @brief functions for working with path names
 * @{
 */

/**
 * @brief returns directory name from file path (like dirname in baseutils)
 * Allocates memory for the new string.
 * returns 0 if path_name is empty string or is file name (has an empty directory component)
 */
char *FN_dir_name( const char *path_name );

/**
 * @brief returns file name from file path (like basename in baseutils)
 * Allocates memory for the new string.
 */
char *FN_file_name( const char *path_name );

/**
 * @brief adds component to path name and returns the new path name
 * Allocates memory for the new string.
 */
char *FN_make_path( const char *dir_name, const char *file_name );

/**
 * @brief returns the file extension of a file.
 * Allocates memory for the new string.
 */
char *FN_get_extension( const char *fname_or_path );

/**
 * @brief returns a new path name with changed file extension
 * Allocates memory for the new string.
 */
char *FN_set_extension( const char *fname_or_path, const char *extension );

/**
 * @brief returns normalized file name (wrapper around realpath)
 * Allocates memory for the new string.
 */
char *FN_normalize( const char *relpath );


/**
 * @}
 */

#endif
