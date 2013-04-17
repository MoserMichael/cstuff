#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "pars.h"
#include "printast.h"
#include <eval.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

extern AST_XFUNC_DECL xlib[];


int eval( PARSECONTEXT *ctx, EVAL_OPTIONS *opts );
void dump_ast( const char *file_path, AST_BASE *base, int idx );
void  delete_temp_results(const char *file_path);


int loadRTLIB( PARSECONTEXT *ctx )
{ 
  size_t i;

  for(i=0; xlib[i].f_name != 0 ; i++ )
  {
    PARSECONTEXT_add_function_def(  ctx, &xlib[i] );
  }
  return 0;
}

int run(EVAL_OPTIONS *opts)
{
  PARSECONTEXT *ctx;
  AST_BASE *rval;

  ctx = PARSER_init( opts->inc_path );

  loadRTLIB( ctx );

  if (LEXER_scan_file( 
	&ctx->lexctx, 
	opts->mode == EVAL_MODE_GRAMMAR ? TK_START_PGRAMMAR : TK_START_STATEMENT, 
	opts->file_name)) {
    fprintf(stderr,"Can't open file %s\n", opts->file_name);
    goto err;
  }

  if (PARSER_run( ctx, &rval )) {
    fprintf(stderr,"Parsing failed\n");
    goto err;
  }
 
  if (!ctx->my_ast_root) { 
    goto ok;
  }

  // dump ast before checker.
  if (opts->is_verbose) {
    dump_ast( opts->file_name, ctx->my_ast_root, 1 );
  }

  if ( CHECKER_run( &ctx->chkctx, ctx->my_ast_root ) ) {
    fprintf(stderr,"Type checking failed\n");
    goto err;
  }
 
  // dump ast after checker
  if (opts->is_verbose) {
    dump_ast( opts->file_name, ctx->my_ast_root, 2 );
  }


  if (eval( ctx, opts )) {
    goto err;
  }
#if 0    
  } else {
    FILE *fp;
    PEG_PARSER parser; 
    PP_BASE_INFO rinfo;
    EVAL_CTX ectx;

    if ( !EVAL_init( &ectx, ctx, opts->is_trace_on  )) {
      goto err;
    }      
   
    fp = fopen( opts->input_file, "r" );
    if (!fp) {
      printf("Can't open file for parsing %s", opts->input_file );
      goto err;
    }

    if ( PEG_PARSER_init( &parser, ctx->my_ast_root , 4096, PARSER_DATA_SRC_file, fp, &ectx ) ) {
      printf( "Failed to initialize parser\n" );
      goto err;
    }

    if ( PEG_PARSER_run( &parser, &rinfo ) ) {
      printf("Parsing failed\n");
    } else {
      printf("Parsing ok");
    }

    EVAL_free( &ectx );
  }
#endif  
ok:
  PARSER_free(ctx);
  return 0;
  
err:

  delete_temp_results(opts->file_name);
  PARSER_free(ctx);
  return 1;
}

char *make_path( const char *file_path, const char *pattern, int idx)
{
  char *name;

  name = (char *)  malloc( strlen(file_path) + strlen(pattern) + 20 );
  assert(name);
  if (!name) {
    return 0;
  }
  sprintf( name, "%s%s%d", file_path, pattern, idx );
  return name;
}
 

void  delete_temp_results(const char *file_path)
{
  char *path;

  path = make_path( file_path, ".ast.", 1 );
  if (path) {
    //unlink( path );
  }
}

void dump_ast( const char *file_path, AST_BASE *base, int idx )
{
  char *name;
  FILE *fout;


  name = make_path( file_path, ".ast.", idx );
  if (!name) {
    return;
  }
  fout = fopen( name, "w" );
  if (fout) {
    AST_print( fout, base );
    fclose(fout);
  }
  free(name);
}

void print_msg(const char *msg)
{
  if (msg) {
    fprintf( stderr, "Error: %s\n", msg );
  }

  fprintf( stderr, "Usage: pooh [switches] <script-file>\n"
		  " -i <directory>  prepend directory to include path\n"
		  " -I <directory>  append directory to include path\n"
		  " -x		    Trace execution of program\n"
		  " -v		    print syntax tree into file. the file has the same name\n"
		  "		    as script with extension .ast.1 .ast.2\n"
		  " -h		    show this help message and exit\n" );
  exit(1);	      
}


void init_options( EVAL_OPTIONS *opts )
{
  memset( opts, 0, sizeof( EVAL_OPTIONS ) );
  opts->mode = EVAL_MODE_SCRIPT;
}

void parse_cmd_line( int argc, char *argv[], EVAL_OPTIONS *opts )
{
  int i;

  memset( opts, 0, sizeof( EVAL_OPTIONS ) ); 

  opts->inc_path = INC_PATH_init( argv[0] );

  for( i = 1; i < argc; i++ ) {
     if (strcmp( argv[ i ], "-h" ) == 0) {
        print_msg( 0 );
     } else
     if (strcmp( argv[ i ], "-v" ) == 0) {
        opts->is_verbose = 1;
     } else if (strcmp( argv[ i ], "-x" ) == 0) {
	opts->is_trace_on = 1;
     } else if (strcmp( argv[ i ], "-i" ) == 0 || strcmp( argv[ i ], "-I" ) == 0) {
        struct stat statbuf;
	char msg[ PATH_MAX * 2 ];

	if ((i + 1) >= argc) {
	    print_msg("Missing argument of -i option");	    
	}
	if (stat( argv[ i + 1 ], &statbuf )) {
	    sprintf( msg, "False value of %s option, file %s is not a directory.", argv[ i ], argv[ i + 1 ] );
	    print_msg( msg );
	}
	if (! S_ISDIR( statbuf.st_mode ) ) {
	    sprintf( msg, "False value of %s option, file %s is not a directory.", argv[ i ], argv[ i + 1 ] );
	    print_msg( msg );
	}

	if (strcmp( argv[ i ], "-i" ) == 0) 
	    INC_PATH_add( opts->inc_path, argv[ i + 1 ], 0 );
	else 
	    INC_PATH_add( opts->inc_path, argv[ i + 1 ], 1 );

	i += 1; // skip argument
     } else {
        opts->file_name = argv[ i ];

        opts->argv = argv + i;
        opts->argc = argc - i;
	break;
     }
   }

   if (opts->file_name == 0) {
      print_msg( "No script file has been specified" );
   }

   if ( access( opts->file_name, R_OK ) ) {
      print_msg( "Can't read script file / script file is missing" );
   }
}

int main(int argc, char *argv[])
{
  EVAL_OPTIONS opts; 
  
  init_options( &opts );
  parse_cmd_line( argc, argv, &opts );

  if (run( &opts )) {
    exit(1);
  }
  return 0;
}
