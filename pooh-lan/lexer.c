

#include "parser.h"
#include "lexer.h"
#include "yystype.h"
#include "pars.h"
#include <butils/strtk.h>


/* ****************************************************************************** 
	   support for BISON Locations - declarations

  bison allows you to track position of token (row + column start + column end)
  this feature is not quire enabled by default, so here are some helpers.
 * ****************************************************************************** */

#if 0
/* doesn't work with older versions of gcc */
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif 

#ifdef IS_REENTRANT 
  

  #define YY_TYPEDEF_YY_SCANNER_T

  #define YY_EXTRA_TYPE LEXCONTEXT * 

  #define SET_YY_STYPE( field, value ) do { yylval_param -> field = value; } while(0);

  struct yyguts_t;   
  int MY_YY_input(  yyscan_t yyscanner );
  #define MY_YY_INPUT MY_YY_input( yyscanner )

  #define POSITION_RESET_ON_NEW_FILE do { \
    struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; \
    yylineno = yycolumn = 1; } while( 0 );

  /**
   * macro is invoked before an action returns; here yyloc (bison current position global) ise set; 
   * also the column position is advanced by the length of the token that has been recognized; 
   */
  #define YY_USER_ACTION  do { \
    yylloc_param->first_line =  yylloc_param->last_line = yylineno; \
    yylloc_param->first_column = yycolumn; \
    yylloc_param->last_column = yycolumn + yyleng - 1; \
  } while( 0 );

  #define MY_YY_NEWLINES do {  \
  } while(0);


  #define MY_YY_NEWLINE do {  \
  } while(0);

 
#else
  #define SET_YY_STYPE( field, value ) do { yylval . field = value; } while(0);
 
  int MY_YY_input();
  #define MY_YY_INPUT MY_YY_input()

 
  extern YYLTYPE yyloc;
  int  yycolumn;
  extern int yylineno;


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


#endif		




int  MY_YY_get_next(char *buff, int max_buffer);


/*
	Invoked from lex file when parsing one token: 
	  - advance current column counter by one length of current token
	  - copy position of current token in bison global yyloc (required for position tracking)
*/

#undef  TRACE_TOKENIZER
#ifndef TRACE_TOKENIZER

#define MY_YY_RETURN(x) \
	struct yyguts_t *yyg = (struct yyguts_t *) yyscanner; \
	yycolumn += yyleng; \
	return(x);

#else 


#define MY_YY_RETURN(x) \
	struct yyguts_t *yyg = (struct yyguts_t *) yyscanner; \
	yycolumn += yyleng; \
	fprintf(stdout,"\n->Token %s(%d) (%d,%d)-(%d,%d)\n", yytext, x,  yylloc->first_line, yylloc->first_column, yylloc->last_line, yylloc->last_column ); \
	fflush(stdout); \
	return(x);

#endif

int LEXER_close_file( LEXCONTEXT *pc );
void yyerror (char const *);


/*** include the flex parser ***/


#define ECHO
#define YY_NO_UNPUT



#include "lex.txt"


#if 0
/* doesn't work with older versions of gcc */
#pragma GCC diagnostic warning "-Wsign-compare"
#pragma GCC diagnostic warning "-Wunused-function"
#pragma GCC diagnostic warning "-Wunused-parameter"
#endif

#ifdef IS_REENTRANT 
  #define yyloc  (* yyg->yylloc_r)
#endif

/*
  Function name - MY_YY_input
 
  Description - get next char of input and keep track of current line number / current column number.
 
  Input -       
 
  Return Code/Output - current input char.
 */

#ifdef IS_REENTRANT 
int MY_YY_input(  yyscan_t yyscanner )  
#else 
int MY_YY_input( )
#endif
{
  int c;
#ifdef IS_REENTRANT 
  struct yyguts_t *yyg = (struct yyguts_t *) yyscanner;

  while( (c = input( yyscanner ) ) == 0xD );
#else
  while( (c = input( ) ) == 0xD );
#endif		

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




int LEXER_init( LEXCONTEXT *pc , INC_PATH *inc_path)

{
	pc->inc_path = inc_path;

#ifdef IS_REENTRANT
	if (yylex_init( &pc->yyscanner ) ) {
	  return -1;
	}
	yyset_extra( pc, pc->yyscanner );

//	yyscanner = pc->yyscanner;
//	yyget_lloc( yyscanner )->file_id = 0;
	

#else	
	yyloc.file_id = 0;
	POSITION_RESET_ON_NEW_FILE;
#endif		
	
	ARRAY_init( &pc->nested_buffers, sizeof(MY_YY_BUFFER_STATE), 1 ) ;
 

	ARRAY_init( &pc->file_name_table, sizeof(char *), 1 );
        ARRAY_init( &pc->string_parts, sizeof(char *), 1 );
		
	/* tell lex to return the first token as TK_START_STATEMENT - this to disambiguate mutiple start symbol in grammar */
        pc->send_init_token = 1;
	pc->init_token_value = TK_START_STATEMENT;
        pc->set_init_location = 0;
  
	return 0;
}


static int is_file_already_opened( LEXCONTEXT *pc, const char *file_name);
static char *copy_file_name(const char *file_name);
                


int LEXER_free( LEXCONTEXT *pc )
{
	LEXER_clean_string_parts( pc );

	ARRAY_free( &pc->nested_buffers ) ;
	ARRAY_free( &pc->file_name_table );
	ARRAY_free( &pc->string_parts );
 
	//DBUF_free( &pc->token_buf ) ;
	return 0;
}

YYLTYPE LEXER_get_location( LEXCONTEXT *pc )
{
   
#ifdef IS_REENTRANT
    YYLTYPE rt;
    yyscan_t yyscanner = pc->yyscanner;  
    struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
    rt = yyloc;
//  return *yyg->yylloc_r;
    return rt;
#else
    M_UNUSED( pc );
    return yyloc;
#endif
}

void LEXER_set_location( LEXCONTEXT *pc, YYLTYPE *location )
{
#ifdef IS_REENTRANT
    pc->set_init_location = 1;
    pc->init_location = *location;
#else
    M_UNUSED( pc );
    yyloc = *location;
#endif
}

int LEXER_open_string( LEXCONTEXT *pc, const char *string, int init_token_value, YYLTYPE *location  )
{
    FILE *fp;
    int len;
#if 0    
    MY_YY_BUFFER_STATE state;
#endif
#ifdef IS_REENTRANT
    yyscan_t yyscanner = pc->yyscanner;  
    struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif

#ifdef TRACE_PARSER
    fprintf( stderr, "---scanning string %s | init token %d\n", string , init_token_value );
#endif


    len = strlen( string );
    fp = fmemopen( (char *) string, len, "r" );

  //yyloc = start_loc;
    yyin = fp;
	
    /* switch lex context buffers */
#ifdef IS_REENTRANT
    struct yy_buffer_state * bstate =  yy_create_buffer( yyin, YY_BUF_SIZE, yyscanner ); 
   
    bstate->yy_bs_lineno =  location->first_line;
    bstate->yy_bs_column =  location->first_column;
    
    yy_switch_to_buffer( bstate ,yyscanner );
#else
    yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) );
#endif 

    // set current location 
    //yyset_lloc (YYLTYPE * yylloc_param , yyscan_t yyscanner)


#if 0
#ifdef IS_REENTRANT
    state.pos = * yyg->yylloc_r;
#else    
    state.pos  = yyloc; 
#endif    
    state.state = YY_CURRENT_BUFFER;
    
    ARRAY_push_back( &pc->nested_buffers, & state, sizeof(MY_YY_BUFFER_STATE) );
#endif
 
    LEXER_set_next_token( pc, init_token_value );

    BEGIN(INITIAL);

    return 0;
}

int LEXER_scan_file( LEXCONTEXT *pc, const char *file_name )
{
	MY_YY_BUFFER_STATE state;
	char *scopy;
	FILE *fp;
        STRING_PART **cur;

#ifdef IS_REENTRANT
	yyscan_t yyscanner = pc->yyscanner;  
        struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif

	if ( file_name == 0) {
	  cur = (STRING_PART **) ARRAY_at( &pc->string_parts, 0 );
	  file_name = STRING_PART_get(*cur);
	}

	pc->open_count++;

	/* is this file already opened ? can't a.v:include b.v -> b.v:include a.v
           The second inclide of a.v will be ignored.
	 */
	if (is_file_already_opened( pc, file_name )) {
		return 1;
	}

	/* check if file can be opened before pushing include stack */
	fp = fopen( file_name, "r" );
	if (!fp) {
	    char * fname = INC_PATH_resolve( pc->inc_path, file_name );
	    if (!fname) 
		return -1;
	    
	    fp = fopen( fname, "r" );
	    
	    free( fname );

	    if (!fp)
		return -1;
	}

#ifdef TRACE_PARSER
	fprintf( stderr, "---scanning file %s\n", file_name );
#endif

	/* enter file name into file name table */
	scopy = copy_file_name(file_name);
	LEXER_set_file_name( pc, scopy );


	/* first time open - just use this file descriptor */
	if (pc->open_count == 1) {
		yyin = fp;
#ifdef IS_REENTRANT
		yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE, yyscanner ), yyscanner );
#endif	
    
	
		return 0;
	}
	
	/* insert buffer into stack; so that we can return to current file contex, once we are finished */
	//state.state = yy_current_buffer;
#ifdef IS_REENTRANT
	state.pos = * yyg->yylloc_r;
#else    
	state.pos  = yyloc; 
#endif    
 
	state.state = YY_CURRENT_BUFFER;
	POSITION_RESET_ON_NEW_FILE;

	ARRAY_push_back( &pc->nested_buffers, & state, sizeof(MY_YY_BUFFER_STATE) );

	/* lex globel: the input stream */
	yyin = fp;
	
	/* switch lex context buffers */
#ifdef IS_REENTRANT
	yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE, yyscanner ), yyscanner );
#else	
	yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) );
#endif




	BEGIN(INITIAL);

	return 0;
}

int LEXER_close_file( LEXCONTEXT *pc )
{
#ifdef IS_REENTRANT
	yyscan_t yyscanner = pc->yyscanner;  
        struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif

	if (ARRAY_size( &pc->nested_buffers ) > 0) {

		MY_YY_BUFFER_STATE state;
		
		
		ARRAY_pop_back( &pc->nested_buffers, &state, sizeof(MY_YY_BUFFER_STATE) );
		
        	yy_delete_buffer( YY_CURRENT_BUFFER, yyscanner );
        	yy_switch_to_buffer(  state.state, yyscanner );

#ifdef IS_REENTRANT
		* yyg->yylloc_r = state.pos;
#else
		yyloc = state.pos;
#endif		


	} else  {

		ARRAY_free( &pc->nested_buffers );
       		return -1;
    	}  
    return 0;
}

const char *LEXER_get_file_name( LEXCONTEXT *pc, int file_index)
{
	 char **ptr = (char **) ARRAY_at( &pc->file_name_table, file_index );
	 return *ptr;
}

const char *LEXER_get_current_file_name(LEXCONTEXT *pc)
{
	 size_t sz = ARRAY_size( &pc->file_name_table ) - 1;
	 return LEXER_get_file_name( pc, sz );
}

void LEXER_set_file_name( LEXCONTEXT *pc, const char *file_name )
{
	ARRAY_push_back( &pc->file_name_table, (void *) &file_name , sizeof(char *) );
}

void  LEXER_set_next_token( LEXCONTEXT *pc, int init_token_value )
{
    if (init_token_value != -1) {
      pc->send_init_token = 1;
      pc->init_token_value = init_token_value;
    }
}



static int is_file_already_opened(LEXCONTEXT *pc, const char *file_name)
{
	unsigned int  i;
	char **ptr;
	char *scopy = copy_file_name(file_name);

	for(i=0; i<ARRAY_size( &pc->file_name_table);  i++) {
		ptr = (char **) ARRAY_at( &pc->file_name_table, i );

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

void LEXER_clean_string_parts( LEXCONTEXT *ctx)
{
  size_t i;
  STRING_PART **cur;

  for(i =0; i< ARRAY_size(&ctx->string_parts);i++) {
    cur = (STRING_PART **) ARRAY_at( &ctx->string_parts, i );
    STRING_PART_free( (*cur) );
  }
  ARRAY_reset( &ctx->string_parts );
}

//------------------------------------------------------------------------

int do_parse_string( LEXCONTEXT *pc, DBUF *parent, char start_char );
int parse_string_header( LEXCONTEXT *pc, DBUF *parent, DBUF *buf, char start_char, char **token_delimiter, char **start_of_expression, char **end_of_expression );
int parse_string_data( LEXCONTEXT *pc, DBUF *parent,  char *token_delimiter, char *start_of_expression, char *end_of_expression  );
STRING_PART * parse_string_sequence( LEXCONTEXT *pc, DBUF *parent,  const char *string_term, const char *start_of_expression, int *has_follow_up );
STRING_PART *parse_expression_sequence( LEXCONTEXT *pc, DBUF *parent,  char *end_of_expression );
const char * dbuf_ends_with( DBUF *buf, const char *term_string );
void print_string( LEXCONTEXT *pc );

int parse_string(  LEXCONTEXT *pc, char start_char )
{
  int rt;

  rt = do_parse_string( pc, 0, start_char );
#if 0  
  print_string( pc );
#endif  
  return rt;
}

#if 0
void print_string( LEXCONTEXT *pc )
{ 
  size_t i;
  STRING_PART *cur,**tmp;

  for( i = 0; i < ARRAY_size(&pc->string_parts ) ; i++ ) {

      tmp = (STRING_PART **) ARRAY_at( &pc->string_parts, i );
      cur = *tmp;
      fprintf(stderr,"\t\tis_exp %d (%d,%d-%d,%d) >>%s<<\n",
		    cur->is_expression,
		    cur->loc.first_line, cur->loc.first_column, cur->loc.last_line, cur->loc.last_column,
		    (char *) cur->part_data.buf );
  }

}
#endif

int do_parse_string( LEXCONTEXT *pc, DBUF *parent, char start_char )
{
  DBUF header;
  char *token_delimiter, *start_of_expression = 0, *end_of_expression = 0;

  DBUF_init( &header, 0 );

  if (parse_string_header( pc, parent, &header, start_char, &token_delimiter, &start_of_expression, &end_of_expression )) {
    return -1;
  }

  return parse_string_data( pc, parent, token_delimiter, start_of_expression, end_of_expression );
 
}
int parse_string_header( LEXCONTEXT *pc, DBUF *parent, DBUF *buf, char start_char, char **token_delimiter, char **start_of_expression, char **end_of_expression ) 
{
  int c;
#ifdef IS_REENTRANT
  yyscan_t yyscanner = pc->yyscanner;
  struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif	
  YYLTYPE strdefloc = yyloc;

  DBUF_init(buf,0);
  DBUF_add( buf, &start_char, sizeof(char) );
   
  while ( 1 ) {
    c =  MY_YY_INPUT;
    if (c == -1) {
      do_yyerror( &strdefloc, (PARSECONTEXT *) pc, "Wrong string, starting at");
      return -1;	
    }
    if (c != start_char) {
      unput(c);
      //yycolumn -- ;
      break;
    }

    if (parent) {
      DBUF_add( parent, &c, sizeof(char) );
    }
    DBUF_add( buf, &c, sizeof(char) );
  }
  DBUF_add_null( buf );

  *token_delimiter = (char *) buf->buf;
  *start_of_expression = strdup( *token_delimiter );
  memset( *start_of_expression, '[', strlen( *token_delimiter ) );
  *end_of_expression = strdup( *token_delimiter );
  memset( *end_of_expression, ']', strlen( *token_delimiter ) );

  return 0;
} 

int parse_string_data( LEXCONTEXT *pc, DBUF *parent, char *token_delimiter, char *start_of_expression, char *end_of_expression  )
{
  int has_follow_up = 1;
  int parts = 0;
  STRING_PART *cur;
#ifdef IS_REENTRANT
  yyscan_t yyscanner = pc->yyscanner;
  struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif	
 
  while(1) {
    cur = parse_string_sequence( pc, parent, token_delimiter, start_of_expression, &has_follow_up );
    if ( !cur ) {
      return -1;
    }
    
    cur->loc.last_line  =  yylineno;
    cur->loc.last_column = yycolumn;
    if (!parent) {
      ARRAY_push_back( &pc->string_parts, &cur, sizeof(void *) );
    } else {
      STRING_PART_free( cur );
    }

    ++parts;
    if (!has_follow_up) {
      break;
    }

    cur = parse_expression_sequence( pc, parent, end_of_expression );
    if (!cur) {
      return -1;
    }

    cur->loc.last_line  =  yylineno;
    cur->loc.last_column = yycolumn;
    if (!parent) {
      ARRAY_push_back( &pc->string_parts, &cur, sizeof(void *) );
    } else {
      STRING_PART_free( cur );
    }

    ++parts;
  }

  return parts;
}

STRING_PART *parse_expression_sequence( LEXCONTEXT *pc, DBUF *parent,  char *end_of_expression )
{
  STRING_PART *part; 
  int c;
  YYLTYPE start;
  char *eof;
#ifdef IS_REENTRANT
  yyscan_t yyscanner = pc->yyscanner;
  struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif	
  int countb = 0;

//start = yyloc;
  start.file_id = yyloc.file_id;
  start.first_line = start.last_line = yylineno;
  start.first_column = start.last_column = yycolumn;

  part = STRING_PART_init( 1, &start );
  if (!part) {
    return 0;
  }
   		
  while ( (c =  MY_YY_INPUT) != -1) {

    if (parent) {
      DBUF_add( parent, &c, sizeof(char) );
    }
    
    DBUF_add(  &part->part_data, &c, sizeof(char) );
    
    if (c == '[') {
      countb ++;
      continue;
    }
    if (c == ']') {
      if (countb > 0) {
        countb --;
        continue;
      }
    }

    if (c == '\'' || c == '"') {
	do_parse_string( pc, &part->part_data, c );
    }

    eof = (char *) dbuf_ends_with( &part->part_data, end_of_expression );  
    if (eof) {
      *eof = '\0';
      break;
    }
  }

  if (c == -1) {
     do_yyerror ( &start, (PARSECONTEXT *) pc,  "The string constant is not closed, it goes on and on until the end of the file" );
     return  0;
  }

 
  return part;
}

STRING_PART * parse_string_sequence( LEXCONTEXT *pc, DBUF *parent, const char *string_term, const char *start_of_expression, int *has_follow_up )
{
  int c;
  STRING_PART *part;
  char *eof;
#ifdef IS_REENTRANT
  yyscan_t yyscanner = pc->yyscanner;
  struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif
  YYLTYPE start;

  *has_follow_up = 0;
  
  start.file_id = yyloc.file_id;
  start.first_line = start.last_line = yylineno;
  start.first_column = start.last_column = yycolumn;

 
  part = STRING_PART_init( 0, &start );
  if (!part) {
    return 0;
  }
  
  		
  while ( (c =  MY_YY_INPUT) != -1) {
	
    if (parent) {
      DBUF_add( parent, &c, sizeof(char) );
    }
   
   // add c to returned input
    DBUF_add(  &part->part_data, &c, sizeof(char) );
 
    eof = (char *) dbuf_ends_with( &part->part_data, string_term );  
    if ( eof ) {
      *eof = '\0';
      *has_follow_up = 0;
       goto ret;
    }

    eof = (char *) dbuf_ends_with( &part->part_data, start_of_expression );  
    if ( eof ) {
      *eof = '\0';
      *has_follow_up = 1;
       goto ret;
    }

  }

  if (c == -1 ) {
     do_yyerror ( &start, (PARSECONTEXT *) pc,  "The string constant is not closed, it goes on and on until the end of the file" );
     return 0;
  }

ret:
  
  part->loc = start;
  part->loc.last_line = yylineno; //yyloc.last_line;
  part->loc.last_column = yycolumn; //yyloc.last_column;

  return part;
}

const char * dbuf_ends_with( DBUF *buf, const char *term_string )
{
  size_t term_string_len;
  const char *ptr_occur;

  term_string_len = (size_t) strlen( term_string );
  if (buf->buf_used < term_string_len ) {
    return 0;
  }

  ptr_occur = (const char *) buf->buf + buf->buf_used - term_string_len; 
  return strncmp( ptr_occur, term_string, term_string_len ) == 0 ? ptr_occur : 0;
}

#if 0
int parse_string_oneline( LEXCONTEXT *pc ) 
{
  int c;
  STRING_PART *part;
#ifdef IS_REENTRANT
  yyscan_t yyscanner = pc->yyscanner;
  struct yyguts_t * yyg = (struct yyguts_t*) yyscanner; 
#endif
  YYLTYPE start;

  start = yyloc;
 
  part = STRING_PART_init( 0, &start );
  if (!part) {
    return TK_ERROR;
  }
		
  while ( (c = MY_YY_INPUT) != '"' && c != EOF) {
    if (c == '\\') {
      c = MY_YY_INPUT;
      if (c==EOF) {
        break;
      }
      if (c=='\n') {
        continue;
      }
      switch(c) {
        case 'n': 
          c = '\n';
          break;
        case 't':
          c = '\t';
          break;
        case 'v':
          c = '\v';
          break;
        case 'b': 
          c = '\r';
          break;
        case 'r':
          c = '\r';
          break;
        case 'f':
          c = '\f';
          break;
        case 'a':
          c = '\a';
          break;
      }			   			   			   
    } else {
      if (c == '\n') {
        /*TODO: write where string constant started. */			  
        yyerror( "this string constant may not span multiple lines" );
        return TK_ERROR;
      }
    }
    DBUF_add( &part->part_data, &c, sizeof(char) );
  }
			
  part->loc = start;
  part->loc.last_column = yyloc.last_column;

  if (c == '"') {
    ARRAY_push_back( &pc->string_parts, &part, sizeof(void *) );
    return TK_STRING_CONSTANT;
  }

  return TK_ERROR;
}
#endif




