#ifndef __lexer_xx_yy__
#define __lexer_xx_yy__

#include <cutils/array.h>
#include <cutils/dbuf.h>
#include "inc.h"

//--------------------------------------------------------

/* if defined then the parser writes lots of traces to stderr */
//#define TRACE_PARSER


/* if defined then we are working with reentrant parser */
#define IS_REENTRANT

#ifdef IS_REENTRANT
  typedef void* yyscan_t;
#endif

//--------------------------------------------------------

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

M_INLINE void YYLTYPE_init( YYLTYPE *location )
{
  location->file_id  = 0;
  location->first_line = location->first_column =
  location->last_line = location->last_column = 1;
}

#define DEFINE_NULL_YYLTYPE { 0, 0, 0, 0, 0 }

//--------------------------------------------------------

typedef struct tagSTRING_PART {
  
  DBUF part_data;
  YYLTYPE loc;  
  int is_expression;

} STRING_PART;

STRING_PART *STRING_PART_init( int is_expression , YYLTYPE *location );
void STRING_PART_free( STRING_PART *part  );

M_INLINE const char * STRING_PART_get( STRING_PART *part )
{
  return (const char *) part->part_data.buf;
}

//--------------------------------------------------------

typedef struct tagLEXCONTEXT {

  int send_init_token;  // force lex to emit an init token.
  int init_token_value;

  int set_init_location; // if set then lex will pick up init_location during first call.
  YYLTYPE init_location;

#ifdef IS_REENTRANT
  yyscan_t yyscanner; // the reentrant scanner.
#endif
	
  ARRAY nested_buffers; // stack of included files.
  ARRAY file_name_table; // all files that have been parsed.
  ARRAY string_parts;   // multipart strings.
  int   open_count;
  INC_PATH *inc_path;

} LEXCONTEXT;



int LEXER_init(LEXCONTEXT *pc, INC_PATH *inc_path);

int LEXER_free(LEXCONTEXT *pc);

int LEXER_open_string(LEXCONTEXT *pc, const char *string, int init_token_value, YYLTYPE *location );

int LEXER_scan_file( LEXCONTEXT *pc, int init_token_value, const char *file_name );

const char *LEXER_get_file_name(LEXCONTEXT *pc, int file_index);

const char *LEXER_get_current_file_name(LEXCONTEXT *pc);


void LEXER_clean_string_parts( LEXCONTEXT *ctx);

void LEXER_set_location( LEXCONTEXT *ctx, YYLTYPE *location );

void LEXER_set_file_name( LEXCONTEXT *ctx, const char *file_name );

YYLTYPE LEXER_get_location( LEXCONTEXT *ctx );

void  LEXER_set_next_token( LEXCONTEXT *pc, int init_token_value );

#endif
