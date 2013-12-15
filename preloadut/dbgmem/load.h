#ifndef __LOAD_H__
#define __LOAD_H__
 
#ifdef WIN32
#define __off_t size_t
#define __const const
#endif

typedef void * (*PFN_mmap) (void *__addr, size_t __len, int __prot,
		             int __flags, int __fd, __off_t __offset);
typedef int    (*PFN_munmap) (void *__addr, size_t __len);
typedef void * (*PFN_mremap)(void *__addr, size_t __old_len, size_t __new_len, int __may_move);


/* -- memory allocations -- */

typedef void * (*PFN_malloc) (size_t sz);
typedef void   (*PFN_free) (void *ptr);
typedef void * (*PFN_calloc) (size_t count, size_t size );
typedef void * (*PFN_realloc) ( void *ptr, size_t size );

typedef char * (*PFN_strdup) (const char *arg);
typedef char * (*PFN___strdup) (const char *arg);

typedef wchar_t * (*PFN_wcsdup) (const wchar_t *arg);

typedef char * (*PFN_strndup) (const char *arg, size_t nlen);
typedef char * (*PFN___strndup) (const char *arg, size_t nlen);

typedef char * (*PFN_getcwd) (char *ptr, size_t sz);
typedef char * (*PFN___getcwd_chk) (char *ptr, size_t sz, size_t sz2);

typedef int    (*PFN_mallopt) (int param, int value);

/* pointer checks */

typedef void * (*PFN_memcpy)  (void *__restrict __dest, __const void *__restrict __src, size_t __n);
typedef void * (*PFN___memcpy_chk)  (void *__restrict __dest, __const void *__restrict __src, size_t __n, size_t nnn);

typedef wchar_t * (*PFN_wmemcpy)  (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n);
typedef wchar_t * (*PFN___wmemcpy_chk)  (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n, size_t nnn);

typedef void * (*PFN_memmove) (void *__dest, __const void *__src, size_t __n);
typedef void * (*PFN___memmove_chk) (void *__dest, __const void *__src, size_t __n, size_t nnn);

typedef wchar_t * (*PFN_wmemmove) (wchar_t *__dest, __const wchar_t *__src, size_t __n);
typedef wchar_t * (*PFN___wmemmove_chk) (wchar_t *__dest, __const wchar_t *__src, size_t __n, size_t nnn);

typedef void * (*PFN_memset) (void *__s, int __c, size_t __n);
typedef void * (*PFN___memset_chk) (void *__s, int __c, size_t __n, size_t nnn);

typedef wchar_t * (*PFN_wmemset) (wchar_t *__s, wchar_t __c, size_t __n);
typedef wchar_t * (*PFN___wmemset_chk) (wchar_t *__s, wchar_t __c, size_t __n, size_t nnn);

typedef char * (*PFN_strcpy)  (char *__restrict __dest, __const char *__restrict __src);
typedef char * (*PFN___strcpy_chk)  (char *__restrict __dest, __const char *__restrict __src, size_t nnn);

typedef wchar_t * (*PFN_wcscpy)  (wchar_t *__restrict __dest, __const wchar_t *__restrict __src);
typedef wchar_t * (*PFN___wcscpy_chk)  (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t nnn);

typedef char * (*PFN_strncpy) (char *__restrict __dest, __const char *__restrict __src, size_t __n);
typedef char * (*PFN___strncpy_chk) (char *__restrict __dest, __const char *__restrict __src, size_t __n, size_t nnn);

typedef wchar_t * (*PFN_wcsncpy) (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n);
typedef wchar_t * (*PFN___wcsncpy_chk) (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n, size_t nnn);

typedef char * (*PFN_strcat) (char *__restrict __dest, __const char *__restrict __src);
typedef char * (*PFN___strcat_chk) (char *__restrict __dest, __const char *__restrict __src, size_t nnn);

typedef wchar_t * (*PFN_wcscat) (wchar_t *__restrict __dest, __const wchar_t *__restrict __src);
typedef wchar_t * (*PFN___wcscat_chk) (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t nnn);

typedef char * (*PFN_strncat) (char *__restrict __dest, __const char *__restrict __src, size_t __n);
typedef char * (*PFN___strncat_chk) (char *__restrict __dest, __const char *__restrict __src, size_t __n, size_t nnn);

typedef wchar_t * (*PFN_wcsncat) (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n);
typedef wchar_t * (*PFN___wcsncat_chk) (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n, size_t nnn);

typedef int    (*PFN_strcmp) (__const char *__s1, __const char *__s2);
typedef int    (*PFN_wcscmp) (__const wchar_t *__s1, __const wchar_t *__s2);

typedef int    (*PFN_strncmp) (__const char *__s1, __const char *__s2, size_t __n);
typedef int    (*PFN_wcsncmp) (__const wchar_t *__s1, __const wchar_t *__s2, size_t __n);

typedef pid_t  (*PFN_fork) (void);

#endif

