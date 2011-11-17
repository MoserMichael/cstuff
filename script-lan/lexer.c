

#include "lexer.h"
#include "yystype.h"
#include "pars.h"


/* ****************************************************************************** 
	   support for BISON Locations - declarations

  bison allows you to track position of token (row + column start + column end)
  this feature is not quire enabled by default, so here are some helpers.
 * ****************************************************************************** */

extern YYLTYPE yyloc;
int  yycolumn;
extern int yylineno;



int  MY_YY_get_next(char *buff, int max_buffer);


/*
	Invoked from lex file when parsing one token: 
	  - advance current column counter by one length of current token
	  - copy position of current token in bison global yyloc (required for position tracking)
*/

#ifndef TRACE_TOKENIZER

#define MY_YY_RETURN(x) \
	return(x);

#else 

#define MY_YY_RETURN(x) \
	MY_YY_begin_token( yytext ); \
	fprintf(stdout,"\n!->Token %s(%d) (%d,%d)-(%d,%d)\n", yytext, x,  yylloc.first_line,yylloc.first_column,yylloc.last_line,yylloc.last_column ); fflush(stdout); \
	return(x);

#endif

int MY_YY_input();
int LEXER_close_file();
void yyerror (char const *);


/*** include the flex parser ***/


#define ECHO
#define YY_NO_UNPUT


#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-function"
 
#define POSITION_RESET_ON_NEW_FILE do { yylineno = yycolumn = 1; } while( 0 );


/**
 * macro is invoked before an action returns; here yyloc (bison current position global) ise set; 
 * also the column position is advanced by the length of the token that has been recognized; 
 */
#define YY_USER_ACTION  do { \
  yyloc.first_line = yyloc.last_line = yylineno; \
  yyloc.first_column = yycolumn; \
  yyloc.last_column = yycolumn + yyleng - 1; \
} while( 0 );

#define MY_YY_NEWLINES do {  \
  yyloc.first_column = yyloc.last_column = 1; \
  yylineno += yyleng; \
  yyloc.first_line = yyloc.last_line = yylineno; \
} while(0);


#define MY_YY_NEWLINE do {  \
  yyloc.first_column = yyloc.last_column = 1; \
  yylineno += 1; \
  yyloc.first_line = yyloc.last_line = yylineno; \
} while(0);

#include "lex.txt"



/*
  Function name - MY_YY_input
 
  Description - get next char of input and keep track of current line number / current column number.
 
  Input -       
 
  Return Code/Output - current input char.
 */
int MY_YY_input()
{
  int c;
	
  while( (c = input()) == 0xD );

  if (c == '\n') {
    yylineno ++;
    yycolumn = 0;
  } else {
    yycolumn ++;
  }
  return c;
}




/* ****************************************************** 
		nested file support (include directives)
 * ****************************************************** */

typedef struct tagMY_YY_BUFFER_STATE {
	
	YYLTYPE pos;
	int	file_id;
	YY_BUFFER_STATE state;

} MY_YY_BUFFER_STATE;

#define MAX_INCLUDE_DEPTH 60



static int is_file_already_opened(const char *file_name);
static char *copy_file_name(const char *file_name);
                


int LEXER_init()
{
	yyloc.file_id = 0;
	
	POSITION_RESET_ON_NEW_FILE;

	DBUF_init( &parse_context->token_buf, 256 ) ;

	ARRAY_init( &parse_context->nested_buffers, sizeof(MY_YY_BUFFER_STATE), 1 ) ;
	ARRAY_init( &parse_context->file_name_table, sizeof(char *), 1 );
	return 0;
}

int LEXER_free()
{
	DBUF_free( &parse_context->token_buf ) ;
	ARRAY_free( &parse_context->nested_buffers ) ;
	ARRAY_free( &parse_context->file_name_table );
	return 0;
}

int LEXER_scan_file(const char *file_name)
{
	MY_YY_BUFFER_STATE state;
	char *scopy;
	FILE *fp;

	parse_context->open_count++;

	/* is this file already opened ? can't a.v:include b.v -> b.v:include a.v
           The second inclide of a.v will be ignored.
	 */
	if (is_file_already_opened( file_name)) {
		return 1;
	}

	/* check if file can be opened before pushing include stack */
	fp = fopen( file_name, "r" );
	if (!fp) {
		return -1;
	}
	

	/* enter file name into file name table */
	scopy = copy_file_name(file_name);
	
	ARRAY_push_back( &parse_context->file_name_table, (void *) &scopy , sizeof(char *) );

	/* first time open - just use this file descriptor */
	if (parse_context->open_count == 1) {
		yyin = fp;
		return 0;
	}
	
	/* insert buffer into stack; so that we can return to current file contex, once we are finished */
	//state.state = yy_current_buffer;
	state.pos  = yyloc; 

	POSITION_RESET_ON_NEW_FILE;

	ARRAY_push_back( &parse_context->nested_buffers, & state, sizeof(MY_YY_BUFFER_STATE) );

	/* lex globel: the input stream */
	yyin = fp;
	
	/* switch lex context buffers */
	yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) );

	BEGIN(INITIAL);

	return 0;
}


int LEXER_close_file()
{
	if (ARRAY_size( &parse_context->nested_buffers ) > 0) {

		MY_YY_BUFFER_STATE state;
		
		
		ARRAY_pop_back( &parse_context->nested_buffers, &state, sizeof(MY_YY_BUFFER_STATE) );
		
        	yy_delete_buffer( YY_CURRENT_BUFFER );
        	yy_switch_to_buffer(  state.state );

		yyloc = state.pos;


	} else  {

		ARRAY_free( &parse_context->nested_buffers );
       		return -1;
    	}  
    return 0;
}

const char *LEXER_get_file_name(int file_index)
{
	 char **ptr = (char **) ARRAY_at( &parse_context->file_name_table, file_index );
	 return *ptr;
}



static int is_file_already_opened(const char *file_name)
{
	unsigned int  i;
	char **ptr;
	char *scopy = copy_file_name(file_name);

	for(i=0; i<ARRAY_size( &parse_context->file_name_table);  i++) {
		ptr = (char **) ARRAY_at( &parse_context->file_name_table, i );

		if (strcmp(*ptr, scopy) == 0) {
			return 1;
		}
	}

	free(scopy);
	return 0;
}

static char *copy_file_name(const char *file_name)
{
	char *ret = strdup(file_name);
	char *tmp;

	tmp = ret;
	while(*tmp != '\0') {
		if (*tmp == '\\') {
			*tmp = '/';
		}
		tmp ++;
	}
    return ret;
}


