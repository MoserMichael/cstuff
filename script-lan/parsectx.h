#ifndef __PARSE_CTX_H_Y__
#define __PARSE_CTX_H_Y__


#include <cutils/array.h>
#include <cutils/dbuf.h>

struct tagAST_BASE;

/***  global context structure *** */

typedef struct tagPARSECONTEXT {

	/*  *** error status *** */
	int my_yy_is_error;
	struct tagAST_BASE * my_ast_root;	

	/*  *** lexical analysis *** */
	DBUF  token_buf;
	ARRAY nested_buffers;
	ARRAY file_name_table;
	int   open_count;

} PARSECONTEXT;



extern PARSECONTEXT *parse_context;

#endif
