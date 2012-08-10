#include "parsectx.h"
#include "parser.h"
#include "lexer.h"

#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include "yystype.h"
#include "ast.h"
#include <butils/strtk.h>
#include <cutils/dbuf.h>

#define ERROR_MSG_LEN 4096


/* global used throughout lexer and parser */
//PARSECONTEXT *parse_context;

#ifndef IS_REENTRANT
YYLTYPE yyloc;
#endif

#define YYLLOC_DEFAULT(Current, Rhs, N)                                      \
         do                                                                  \
           if (N)                                                            \
             {								     \
		(Current).file_id	  = YYRHSLOC(Rhs, 1).file_id;	     \
		(Current).first_line   = YYRHSLOC(Rhs, 1).first_line;        \
		(Current).first_column = YYRHSLOC(Rhs, 1).first_column;      \
		(Current).last_line    = YYRHSLOC(Rhs, N).last_line;         \
		(Current).last_column  = YYRHSLOC(Rhs, N).last_column;       \
             }                                                               \
           else                                                              \
             {                                                               \
	       (Current).file_id	  = YYRHSLOC(Rhs, 0).file_id;        \
               (Current).first_line   = (Current).last_line   =              \
                 YYRHSLOC(Rhs, 0).last_line;                                 \
               (Current).first_column = (Current).last_column =              \
                 YYRHSLOC(Rhs, 0).last_column;                               \
             }                                                               \
         while (0)

/* include bison generated code */

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define YYERROR_VERBOSE

#ifdef TRACE_PARSER
#define YYDEBUG 1
#endif

#include "pars.txt"


/*
  Function name - MY_YY_fancy_error
 
  Description - write error report line to stderr, where the error token is underlined with ^^^ characters
 
  Input -    line			input line
			 line_num		line number
			 column_start   column start of error token
			 column_end     column end of error token
			 
 
  Return Code/Output 
 */
static void fancy_error(const char *line, int line_num, int column_start, int column_end, int show_highlight )
{
	int pos;

	fprintf(stderr, " %6d |", line_num );

	for(pos = 0; pos < 80; ++pos ) {
	   if (pos % 10 == 0) {
	     fprintf(stderr,":");
	   } else if (pos % 5 == 0) {
	     fprintf(stderr,"+");
	   } else {
	     fprintf(stderr,".");
	   }
	}
	fprintf(stderr , "\n        |%s\n        |" , line);
	

	if ( show_highlight )
	{
	for(pos = 0; pos < column_start; pos++) {
		fprintf(stderr,".");
	}

	do {
		fprintf(stderr,"^");
	} while( ++pos <= column_end);
	
	fprintf(stderr,"\n");
	}
}
 

/*
  Function name - MY_YY_fancy_error
 
  Description - write error report line to stderr, where the error token is underlined with ^^^ characters
 
  Input -    location location of error (columns, lines)
			 file filename of source file that contains the error
			 
 
  Return Code/Output - 0 - ok, -1 error
 */
static int fancy_error_report(YYLTYPE *location, const char *file, int show_highlight)
{
	int	 line_num = 1;
	int	 ch;
	char	 by;
        DBUF	 buf;

	FILE *fp = fopen(file,"r");
	if (!fp) {
		return -1;
	}

	DBUF_init( &buf, 10 );

	while( (ch = fgetc( fp ) ) != -1 ) {
	   if (ch == '\n') {
	     if (line_num == location->first_line) {
	       break;
	      }
	      ++line_num;
	      continue;
	   }

	   if (line_num == location->first_line) {
	     by = (char ) ch;
	     DBUF_add( &buf, &by, 1);
	   }
	}
	DBUF_add_null( &buf );

	fancy_error( (char *) buf.buf, location->first_line, location->first_column, location->last_column,  show_highlight  );
	fclose(fp);
	return 0;
}

void SHOW_SOURCE_LINE_impl (YYLTYPE *loc, PARSECONTEXT *parse_context )
{
   YYLTYPE lloc = *loc;
   while( lloc.first_line <= lloc.last_line)
   {
     fancy_error_report( &lloc, 
	LEXER_get_file_name( &parse_context->lexctx, lloc.file_id ), 0 );
     lloc.first_line ++;
   }  
}


#define S_TOKEN_STRART "TK_"
#define S_TOKEN_START_LEN 3


char *translate_token(const char *msg) {

    char *tok;
    char *news, *ch;
    char *copy;

    news = malloc( strlen( msg ) + 1 );
    if (!news) {
  	return 0;
    }
    strcpy(news,"");

    copy = strdup( msg + S_TOKEN_START_LEN ); 
    if (!copy) {
	free(news);
	return 0;
    }
    for( tok = strtok( copy, "_"); tok != 0; tok = strtok( 0, "_" ) ) {
	
	for( ch = tok; *ch != '\0'; ch++) {
	  *ch = tolower( * ch );
	}  
	strcat(news, tok);  
        strcat(news," ");
    }
    free(copy);
    return news;
}
/*
   The token names start with TK_ by convention; this function turns them into something nice and readable.
 */
char *translate_message(const char *msg)
{
    const char *pos, *tok;
    char *news, *npos;
    STRTK stok;
    DBUF dbuf;

    STRTK_init( &stok, " \t" );
    DBUF_init( &dbuf, 10 );

    for( pos = msg, tok = STRTK_tok_mod( &stok, pos, &npos ); 
	    tok != 0; pos = npos, tok = STRTK_tok_mod( &stok, pos, &npos ) ) { 

       news = 0;
       if (strncmp( tok, S_TOKEN_STRART, S_TOKEN_START_LEN ) == 0) {
          news = translate_token( tok  );
          DBUF_add( &dbuf, news, strlen(news) );
          free(news);
      } else {
        DBUF_add( &dbuf, tok, strlen(tok) );
        DBUF_add( &dbuf, " ", 1 );
      }

      pos = npos;
    }
    DBUF_add_null( &dbuf );
    
    return (char *) dbuf.buf;
}

/*
  Function name - do_yyerror
 
  Description - called by yac/lex as well as syntax analysis/code generation to display an error.
 
  Input -    loc location of error (columns, lines, file_id)
			 fomat format spec of error message (variable line error message)
			 
 
  Return Code/Output - none
 */
void do_yyerror (YYLTYPE *loc, PARSECONTEXT *parse_context, const char  *format, ...)
{
  char msg[ERROR_MSG_LEN];
  int len;
  va_list ap;
  YYLTYPE local_loc;
	
  if (! parse_context->report_errors ) {
    return;
  }


  if (!loc) {
    local_loc = LEXER_get_location( & parse_context->lexctx ); 
    loc = &local_loc;
  }


  va_start(ap, format);
  len = 
#ifdef WIN32  
	_vsnprintf
#else
	vsnprintf
#endif
	
	(msg, sizeof(msg) - 1, format, ap);
  va_end(ap);

  msg[len] = '\0';

  parse_context->my_yy_is_error = 1;
  fprintf (stderr, "\n%s(%d): Error: %s\n", 
	LEXER_get_file_name( &parse_context->lexctx , loc->file_id ), 
	loc->first_line, 
	msg);

  
  fancy_error_report( loc, 
	LEXER_get_file_name( &parse_context->lexctx, loc->file_id), 1 );
}

void do_yywarning (YYLTYPE *loc, PARSECONTEXT *parse_context, const char  *format, ...)
{
  char msg[ERROR_MSG_LEN];
  int len;
  va_list ap;

  if (! parse_context->report_errors ) {
    return;
  }

  va_start(ap, format);
  len = 
#ifdef WIN32  
	_vsnprintf
#else
	vsnprintf
#endif
	
	(msg, sizeof(msg) - 1, format, ap);
  va_end(ap);

  msg[len] = '\0';

  parse_context->my_yy_is_error = 1;
  fprintf (stderr, "\n%s(%d): Warning: %s\n", 
	LEXER_get_file_name( &parse_context->lexctx , loc->file_id ), 
	loc->first_line, 
	msg);

  
  fancy_error_report( loc, 
	LEXER_get_file_name( &parse_context->lexctx, loc->file_id), 1 );
}


/*
  Function name - yyerror
 
  Description - called by yac/lex as to display an error.
 
  Input -    msg - error message text 
			  
  Return Code/Output - none
 */
void yyerror (YYLTYPE *loc, PARSECONTEXT *parse_context, char const *msg)
{
  char *nmsg;

  nmsg = translate_message( msg );
  do_yyerror( loc, parse_context, "%s", nmsg );
  free( nmsg );
}

/*
  Function name - MY_YY_parse
 
  Description - parse a source file with flex/bison.
				during this process the AST - abstract syntax treee - is built.
 
  Input -    
			  
  Return Code/Output -  0 - ok, != 0 failure.
						root base node of abstract syntax tree.
					   

 */

int PARSER_run(PARSECONTEXT *ctx, struct tagAST_BASE **rval  )
{
	int ret = 0;

#ifdef TRACE_PARSER
	yydebug = 1;
#endif

	ret = yyparse( ctx );
	if (ret) {

#ifdef TRACE_PARSER
	  fprintf( stderr, "---yyparer failed; returns %d\n", ret );
#endif
	  return ret;
	}

#ifdef TRACE_PARSER
	fprintf( stderr, "---yyparse ok\n" );
#endif


	*rval = ctx->my_ast_root;
	return ctx->my_yy_is_error; 
}



PARSECONTEXT * PARSER_init()
{
  PARSECONTEXT *ret;


  ret = (PARSECONTEXT *) malloc( sizeof( PARSECONTEXT ) );
  if (!ret) {
    return 0;
  } 

  memset( ret, 0, sizeof( PARSECONTEXT ) );

  if (PARSECONTEXT_init( ret )) { 
    return 0;
  }

  return ret;

}

int PARSER_free(PARSECONTEXT *ctx)
{
  if (LEXER_free( &ctx->lexctx )) {
    return -1;
  }
  return 0;
}
