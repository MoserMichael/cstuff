#ifndef __SUTILS_H_Y_Z_
#define __SUTILS_H_Y_Z_


/*
  If you look at wikipedia then white spaces is such an ambiguous subject (by such I mean suuuuch).
  Well we deal with ASCII chars mainly (as do most RFC's so that's what matters in this universe.
  We honour the laws of thermodynamics (and gravity) in this universe ;-)
  - all that to say that after a few beers I value the constance of convention over uncertainty ;;--))
*/

M_INLINE int is_space(char ch)
{
  return ch == ' ' || ch == '\t';
}

M_INLINE char * skip_spaces( char *line )
{
  char *pos;

  for( pos = line; is_space( *pos ) ; ++pos );

  return pos;
}

M_INLINE char *skip_non_spaces( char *line )
{
  char *pos;

  for( pos = line; *pos != '\0' && ! is_space( *pos ) ; ++pos );

  return pos;
}

M_INLINE char *get_token(char *line , char **eof )
{
  char *ret;

  ret = skip_spaces( line );
  if (*ret == '\0') {
    *eof = ret;
    return 0;
  }
  *eof = skip_non_spaces( ret );
  return ret;
}


#endif
