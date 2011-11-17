#ifndef __lexer_xx_yy__
#define __lexer_xx_yy__


#include "parsectx.h"

/* value of lexical token - passed back by flex. */
#define YYLTYPE_IS_DECLARED

typedef struct YYLTYPE
{
	int file_id;	// offset of file entry object (what is the file that parsed this one here)
	
	int first_line;
	int first_column;
	int last_line;
	int last_column;

} YYLTYPE;

#define YYLTYPE_set_null(x) \
	do { (x).file_id = (x).first_line =  \
		 (x).last_line = (x).first_column =  \
		 (x).last_column = -1; } while(0);



/*
  Function name - VSCRIPTCTX_init_tokenizer 
 
  Description - initialise tokeniser
 
  Input -       
 
  Return Code/Output 
 */
int LEXER_init();

/*
  Function name - VSCRIPTCTX_free_tokenizer
 
  Description - free tokeniser
 
  Input -       
 
  Return Code/Output 
 */
int LEXER_free();

/*
  Function name - LEXER_scan_file
 
  Description - open a source file.
				supports nested include directives, 
				i.e. maintains a stack of currently parsed files; when finishing with nested file, 
				then we return to file that had include directive.

				therefore each open file has its numeric id, this id is kept as part of token position.
				 
  Input -       
 
  Return Code/Output 
 */
int LEXER_scan_file(const char *file_name);


/*
  Function name - LEXER_get_file_name
 
  Description - given a file id, return the file name of that file.
 
  Input -       
 
  Return Code/Output 
 */
const char *LEXER_get_file_name(int file_index);


#endif
