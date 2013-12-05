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

typedef void * (*PFN_malloc) (size_t sz);
typedef void   (*PFN_free) (void *ptr);
typedef void * (*PFN_calloc) (size_t count, size_t size );
typedef void * (*PFN_realloc) ( void *ptr, size_t size );
typedef char * (*PFN_strdup) (const char *arg);
typedef char * (*PFN_strndup) (const char *arg, size_t nlen);
typedef char * (*PFN_getcwd) (char *ptr, size_t sz);

typedef int    (*PFN_mallopt) (int param, int value);

typedef void * (*PFN_memcpy)  (void *__restrict __dest, __const void *__restrict __src, size_t __n);
typedef void * (*PFN_memmove) (void *__dest, __const void *__src, size_t __n);
typedef void * (*PFN_memset) (void *__s, int __c, size_t __n);
typedef char * (*PFN_strcpy)  (char *__restrict __dest, __const char *__restrict __src);
typedef char * (*PFN_strncpy) (char *__restrict __dest, __const char *__restrict __src, size_t __n);
typedef char * (*PFN_strcat) (char *__restrict __dest, __const char *__restrict __src);
typedef char * (*PFN_strncat) (char *__restrict __dest, __const char *__restrict __src, size_t __n);
typedef int    (*PFN_strcmp) (__const char *__s1, __const char *__s2);
typedef int    (*PFN_strncmp) (__const char *__s1, __const char *__s2, size_t __n);

typedef pid_t  (*PFN_fork) (void);

#endif

