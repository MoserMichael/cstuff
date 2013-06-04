#include "ast.h"
#include "parser.h"
#include <ctype.h>
#include <rtlib/rtlib.h>
#include "eval.h"

typedef struct tagGRULE_ENTRY {
  HASH_Entry entry;
  const char *rule_name;
  struct tagAST_PP_RULE *rule;

} GRULE_ENTRY;



int GRAMMAR_add_rule( GRAMMARCHECKERCTX *ctx,  const char *rule_name, AST_PP_RULE *rule )
{
    GRULE_ENTRY *entry, *efirst;

    entry = (GRULE_ENTRY *)  malloc( sizeof( GRULE_ENTRY ) );
    if (!entry) {
	return -1;
    }	
    entry->rule = rule;
    entry->rule_name = rule_name;

    if ( (efirst = (GRULE_ENTRY *) HASH_find( &ctx->rule_defs, rule_name, (ssize_t) -1 )) != 0) {
	do_yyerror( &rule->base.base.location, ctx->ctx, "The rule %s is already defined", rule_name );
	do_yyerror( &efirst->rule->base.base.location, ctx->ctx, "First definition of rule %s", rule_name);
    }	

    return HASH_insert( &ctx->rule_defs, &entry->entry, rule_name, (ssize_t) -1 );
}

AST_PP_RULE *GRAMMAR_find_rule( GRAMMARCHECKERCTX *ctx, const char *rule_name )
{
    GRULE_ENTRY *entry;

    entry = (GRULE_ENTRY *) HASH_find( &ctx->rule_defs, (void *) rule_name, -1);
    if (!entry) {
	return 0;
    }
    return entry->rule;
}

static int hash_compare(HASH_Entry *entry, const void * key, ssize_t key_length)
{
  GRULE_ENTRY *lhs;	

  (void) key_length;

  lhs = (GRULE_ENTRY *) entry;

  if (strcmp(  lhs->rule_name, key) == 0) {
    return 0;
  }
  return 1;
}


GRAMMARCHECKERCTX * GRAMMAR_init( struct tagPARSECONTEXT *pctx  )
{
    GRAMMARCHECKERCTX * ctx;

    ctx = (GRAMMARCHECKERCTX *) malloc( sizeof( GRAMMARCHECKERCTX ) );
    if (!ctx) {
        return 0;
    }
    memset( ctx, 0, sizeof( GRAMMARCHECKERCTX ) ); 
    if (HASH_init( &ctx->rule_defs, 10, 0, hash_compare, 0 )) {
	return 0;
    }
    ARRAY_init( &ctx->visited, sizeof( void *) , 0 );
    ARRAY_init( &ctx->current_alt_stack, sizeof( void *) , 0 );
    ctx->ctx = pctx;
    ctx->rules_list = 0;
    pctx->grctx = ctx;

    return ctx;
}

void GRAMMAR_free( GRAMMARCHECKERCTX * ctx )
{
  HASH_deleteall( &ctx->rule_defs, 0, 0, 0 );
  HASH_free( &ctx->rule_defs );
  ARRAY_free( &ctx->visited );
  ARRAY_free( &ctx->current_alt_stack );
  free( ctx );
}


void print_range_char( int ch )
{
   if (isprint( ch ) && ! isspace( ch ) ) {
     fprintf( stderr, "%c", ch );
   } else {
     fprintf( stderr, "\\%x", ch );
   }
}


void GRAMMAR_show_alt( AST_PP_ALTERNATIVE *alt, int level )
{
   DRING *cur, *cur_seq;
   AST_BASE_LIST *lst;
   AST_BASE *elm;
   int show_bracket = 1;
   AST_PP_BASE *base = &alt->base;
   int first_alt = 1;

   if (level == 0 && (base->from == base->to && base->from == 1))  
     show_bracket = 0;

   if (show_bracket)
     fprintf( stderr, "( ");
  
   DRING_FOREACH( cur, &alt->alternatives ) {
     lst = (AST_BASE_LIST *) _OFFSETOF( cur, AST_BASE, entry );
     if (first_alt) {
       first_alt = 0;
     } else {
       fprintf( stderr, " |" );
     }       
     DRING_FOREACH( cur_seq, &lst->statements ) {
	elm = _OFFSETOF( cur_seq, AST_BASE, entry );
	fprintf(stderr," ");
	switch( elm->type ) {
	  case S_PP_RULE_REF:
	    fprintf( stderr,"<%s>", ((AST_PP_RULE_REF *) elm)->rule_name );
	    break;
	  case S_PP_ALTERNATIVE:
	    GRAMMAR_show_alt( (AST_PP_ALTERNATIVE *) elm, level + 1 );
	    break;
	  case S_PP_RULE_CONSTANT:
	    fprintf( stderr, "'%s'", ((AST_PP_CONSTANT *) elm)->const_string );
	    break;
	  case S_PP_CHAR_CLASS: { 
	    AST_PP_CHAR_CLASS *scl = (AST_PP_CHAR_CLASS *) elm;
	    ARRAY *arr = &scl->ranges;
	    size_t i;

	    for( i = 0; i < ARRAY_size( arr ); ++i ) {
		AST_PP_RANGE *range = (AST_PP_RANGE *) ARRAY_at( arr, i );
		print_range_char( range->from );
		if (range->to != (size_t) -1) {
		   fprintf( stderr, "-" );
		   print_range_char( range->to );
		}
	    }
	    }
	    break;
	  default: 
	    assert(0);
	}
     }
   }
  
   if (show_bracket)
     fprintf( stderr, " )");

   if ( ! (base->from == base->to && base->from == 1) ) {

     fprintf( stderr, " " );
     if (base->from == 0 && base->to == 1)
	fprintf( stderr, "optional" );
     else if (base->from == 1 && base->to == -1) 
	fprintf( stderr, "some" );
     else if (base->from == 0 && base->to == 0) 
	fprintf( stderr, "none" );
     else if (base->from == 1 && base->to == -1) 
	fprintf( stderr, "some" );
     else if (base->from == 0 && base->to == -1) 
	fprintf( stderr, "any" );
     else if (base->from != base->to) 
	fprintf( stderr, " %d - %d ", base->from, base->to );
     else 	
        fprintf( stderr, "%d", base->from );
			   
   }
}

void GRAMMAR_on_left_recursion( GRAMMARCHECKERCTX *ctx, AST_PP_RULE *rule )
{
   AST_PP_RULE *tmp;
   size_t i;
  
   fprintf( stderr, "Error: The grammar has left recursion. can't use such a grammar.\n" );
   ctx->ctx->my_yy_is_error = 1;


   // search for rule up on the stack of invocations.
   if (ARRAY_size( &ctx->visited ) == 0) {
     return;
   }

   for( i = ARRAY_size( &ctx->visited ) - 1; ; --i ) {
     tmp = * ( (AST_PP_RULE **) ARRAY_at( &ctx->visited, i ) );
     if (tmp == rule) {
       break;
     }
     if (i == 0) {
       break;
     }
   }	

   for ( ; i < ARRAY_size( &ctx->visited ); ++i ) {
      // print all rules along the path
      tmp = * ( (AST_PP_RULE **) ARRAY_at( &ctx->visited, i ) );
      tmp->flags |= RULE_FLAGS_CIRCLE_REPORTED;

      fprintf(stderr, " %6d | %s %c",  
		tmp->base.base.location.first_line, 
		tmp->rule_name,
		tmp->rtype == S_PP_RULE_GRAMMAR ? '=' : ':' );

      AST_PP_ALTERNATIVE *alt = &tmp->rhs;
      GRAMMAR_show_alt( alt, 0 );	

      fprintf(stderr,"\n\n");
   }
}

void GRAMMAR_check_rule( GRAMMARCHECKERCTX *ctx, AST_PP_RULE *rule );
void GRAMMAR_check_alt( GRAMMARCHECKERCTX *ctx, AST_PP_ALTERNATIVE *alt, int is_start );


void GRAMMAR_check_sequence( GRAMMARCHECKERCTX *ctx, AST_BASE_LIST *lst,  int is_start)
{
    DRING *cur;
    AST_PP_BASE *elm;
    int is_first = 1;
    //int is_top = ARRAY_size( &ctx->visited ) == 1;  
    int tmp_is_start = is_start;

    assert( lst->base.parent != 0 );

    DRING_FOREACH( cur, &lst->statements ) {
	elm = (AST_PP_BASE *) _OFFSETOF( cur, AST_BASE, entry );
	
	if (elm->base.type == S_PP_RULE_REF) {
		AST_PP_RULE *rule;
		AST_PP_RULE_REF *rref = (AST_PP_RULE_REF *) elm; 


		assert( elm->base.parent != 0 );

		rule = GRAMMAR_find_rule( ctx, rref->rule_name );

		if (!rule) {
		  do_yyerror( &rref->base.base.location, ctx->ctx, "The rule %s is not defined", rref->rule_name );
		} else {
		  rref->rule_ref_resolved = rule;
  
		  // is the rule refs parent in token or in grammar land? this determines if this one is a token or not.
		  if (rule->rtype & S_PP_RULE_TOKEN) {
		      AST_PP_BASE *tmp = &rref->base;
		      while( tmp != 0 ) {
			 if (tmp->base.type == S_PP_RULE ) {
			    AST_PP_RULE *rule = (AST_PP_RULE *) tmp;
			    if (rule->rtype & S_PP_RULE_TOKEN) {
				rref->is_token = 0;  
			    } else {
				rref->is_token = 1;
			    }
			    break;
			 }
			 tmp = (AST_PP_BASE *)  tmp->base.parent;
		      }
		  }
		}

	}	
	if (elm->base.type == S_PP_ALTERNATIVE) {
	    AST_PP_ALTERNATIVE *alt = (AST_PP_ALTERNATIVE *) elm; 
	    GRAMMAR_check_alt( ctx, alt, tmp_is_start );
	}
	tmp_is_start = 0;
    }
 
    if (is_start)
    DRING_FOREACH( cur, &lst->statements ) {
	elm = (AST_PP_BASE *) _OFFSETOF( cur, AST_BASE, entry );
	
	if (elm->base.type == S_PP_RULE_REF) {
		AST_PP_RULE *rule;
		AST_PP_RULE_REF *rref = (AST_PP_RULE_REF *) elm; 
		rule = rref->rule_ref_resolved;

		if (is_first && rule) {

		    // check for left recursion.
		    if (rule->flags & RULE_FLAGS_VISITED ) {
			if ( !(rule->flags & RULE_FLAGS_CIRCLE_REPORTED) ) {
			  // here we have left recursion
			  GRAMMAR_on_left_recursion( ctx, rule );
			  rule->flags |= RULE_FLAGS_CIRCLE_REPORTED;
			}			  
		    } else 
			GRAMMAR_check_rule( ctx, rule );
		}
	}
	if (elm->from != 0) {
	    is_first = 0;
	    break;
	}
    }
}

void GRAMMAR_check_alt( GRAMMARCHECKERCTX *ctx, AST_PP_ALTERNATIVE *alt, int  is_start )
{
   DRING *cur;
   AST_BASE_LIST *lst;

   assert( alt->base.base.parent != 0 );
   
   DRING_FOREACH( cur, &alt->alternatives ) {
     lst = (AST_BASE_LIST *) _OFFSETOF( cur, AST_BASE, entry );
     GRAMMAR_check_sequence( ctx, lst, is_start );
   }
}

void GRAMMAR_check_rule( GRAMMARCHECKERCTX *ctx, AST_PP_RULE *rule )
{
   void *tmp;

   ARRAY_push_back( &ctx->visited, &rule, sizeof(void *) );
   rule->flags |= RULE_FLAGS_VISITED; 
   GRAMMAR_check_alt( ctx, & rule->rhs, 1 );

   ARRAY_pop_back( &ctx->visited, &tmp, sizeof(void *) );
   rule->flags &= ~RULE_FLAGS_VISITED;
}
  
 
// Tasks to do:
// Check that all rule references are defined.
// check that there are no unreferenced rules
// check that there are no left recursive rules, no direct / indirect recursion
// try to optimize the rules where required.
int GRAMMAR_checker( GRAMMARCHECKERCTX *ctx, struct tagAST_BASE *program) 
{
   AST_BASE_LIST *slist;
   DRING *cur;
   AST_PP_RULE *rule;
   int cnt = 0;

   slist = (AST_BASE_LIST *) program;
   assert( slist->base.type == S_AST_LIST );
   	
   DRING_FOREACH( cur, &slist->statements ) {

	rule = (AST_PP_RULE *)  _OFFSETOF( cur, AST_BASE, entry );
	assert( rule->base.base.type ==  S_PP_RULE );
	GRAMMAR_check_rule( ctx, rule );
        cnt++;
   }

   if (cnt == 0) {
      YYLTYPE loc = DEFINE_NULL_YYLTYPE; 
      do_yyerror( &loc, ctx->ctx, "The grammar does not define any rules" );
   }
   return 0;
}

#if 0
#define ADD_ERROR( gr, ty, trm, sp, ce, ca ) do { PEG_ERROR_ENTRY e; e.type = ty; e.term = trm; e.start_pos = sp; e.num1 = ce; e.num2 = ca; ARRAY_push_back( &(gr)->error_stack, &e, sizeof( e ) ); } while( 0 );
#define ERROR_RESET( gr ) do { ARRAY_reset( &(gr)->error_stack ); } while( 0 );
#endif


PEG_ERROR_ENTRY * NEXT_ERROR_ENTRY( PEG_PARSER *parser,  PEG_PARSER_POS cursor_iteration, AST_PP_BASE *current , PEG_PARSER_ERROR type ) 
{ 
   PEG_ERROR_ENTRY e, *entry;
   
   ARRAY_push_back( &(parser)->error_stack, &e, sizeof( e ) ); 
   
   entry = ( PEG_ERROR_ENTRY *) ARRAY_at( &(parser)->error_stack, ARRAY_size( &(parser)->error_stack ) - 1 );    
   entry->start_pos = cursor_iteration;
   entry->term = current;
   entry->type = type;

   return entry;
}   

int PEG_PARSER_init( PEG_PARSER *parser, struct tagAST_BASE *program, size_t lookahead_size, PEG_PARSER_DATA_SRC data_source, void *data_src_ctx , EVAL_CTX *eval_ctx )
{
  AST_BASE_LIST *slist;
  
  
  slist = (AST_BASE_LIST *) program;
  parser->root = (AST_PP_BASE *) _OFFSETOF(  DRING_get_first( &slist->statements ), AST_BASE, entry ) ; // first rule is the top rule to parse.
 

  if (CIRCBUF_init( &parser->lookahead, lookahead_size )) {
    return -1;
  }

  //memset( &parser->error_entry, 0, sizeof( PEG_ERROR_ENTRY ) );
  //parser->error_entry_set = 0;
  
  ARRAY_init( &parser->error_stack, sizeof( PEG_ERROR_ENTRY), 0 );	
  parser->error_set_enabled = 1;
  parser->num_non_terminal = 0;

  parser->rtype = S_PP_RULE_GRAMMAR; 
  PEG_PARSER_POS_init( &parser->offset );
  parser->data_src = data_source;
  parser->data_src_ctx = data_src_ctx;
  parser->undo_nesting = 0;
  parser->eval_ctx = eval_ctx;
  parser->trace_flags = eval_ctx->grammar_trace_flags;
  parser->current_rhs_clause = 0;
  parser->current_rhs_clause_len = 0;

  parser->fail_reason = 0;
  parser->is_fail_parsing = 0;

  DBUF_init( &parser->trace_clause, 0 );

  return 0;
}

int PEG_PARSER_next_char( PEG_PARSER *parser, PP_CHAR *rt )
{
  uint8_t ch;
  size_t have_room;
  size_t has_read;

  if (! CIRCBUF_at( &parser->lookahead, parser->offset.lookahead_offset, &ch ) ) {
    PEG_PARSER_POS_add( &parser->offset, ch );
    *rt = (PP_CHAR) ch;
    return 0;
  }

  // do we have room in the buffer ?
  have_room = CIRCBUF_maxsize( &parser->lookahead ) - parser->offset.lookahead_offset;

  if (!have_room) { 
     // resize the lookahead buffer.
     if (CIRCBUF_resize( &parser->lookahead, CIRCBUF_maxsize( &parser->lookahead ) * 2 ) ) {
       return -1;
     }
  }

  // read some data.
  if (! parser->data_src( &parser->lookahead, &has_read, parser->data_src_ctx ) ) {
    return -1; // read error.
  }
 
  if (! CIRCBUF_at( &parser->lookahead, parser->offset.lookahead_offset, &ch ) ) {
    PEG_PARSER_POS_add( &parser->offset, ch );
    *rt = (PP_CHAR) ch;
    return 0;
  }


  
  return  -1;

}

void PEG_PARSER_skip_spaces( PEG_PARSER *parser )
{
  int skip = 0;
  PP_CHAR ch;

  if (PEG_PARSER_next_char( parser, &ch ) == 0)  {
    skip = 1;
    do {
      if (!isspace( ch )) {
        break;
      }
    } while(PEG_PARSER_next_char( parser, &ch ) == 0);  
  }

  if (skip) {
    parser->offset.lookahead_offset --;
    parser->offset.column --;

  }
}

#define INIT_PP_DATA( ridx ) memset( ridx, 0, sizeof( PP_BASE_INFO ));
#define SET_PP_RET_DATA( ridx, sidx, eidx, mcount, rdata ) do { (ridx)->start_idx = (sidx); (ridx)->end_idx = (eidx); (ridx)->match_count = mcount; (ridx)->data = (rdata);  } while( 0 );

void display_trace( PEG_PARSER *parser, size_t from, size_t to )
{
  DBUF *buf;

  fprintf( stderr, "\n" );
  
  fprintf( stderr, "|Line: %d Column: %d|", parser->offset.line, parser->offset.column );
 
  buf = &parser->trace_clause; 
  fprintf( stderr, "%.*s", (int) buf->buf_used, buf->buf );

  if (from != (size_t) -1) { 
    DBUF out;

    DBUF_init( &out, 0 );
    CIRCBUF_extract_text( &parser->lookahead, from, to, &out );
    fprintf( stderr, "%.*s", (int) out.buf_used, out.buf );
    DBUF_free( &out );
  }

  fprintf( stderr, "\n" );
}

int is_non_trivial_error( ARRAY *error_stack )
{
    PEG_ERROR_ENTRY *entry;

    if (ARRAY_size( error_stack ) == 0 ) {
      return 0;
    }
    entry = (PEG_ERROR_ENTRY *) ARRAY_at( error_stack, 0 );
    return (int) entry->type <= 2; // not very nice.
}   

int PEG_PARSER_run_impl( PEG_PARSER *parser, AST_PP_BASE *current, PP_BASE_INFO *rinfo )
{
  PEG_PARSER_POS cursor_iteration, cursor;
  PP_CHAR ch;
  int repetition = 0;
  BINDING_DATA *rdata = 0;
  int rdata_count = 0;
  PP_BASE_INFO cur_pp_info;
  int rt;
  int is_token = 0;

  cursor_iteration = cursor = parser->offset;

parse_start:

  INIT_PP_DATA( &cur_pp_info );
  switch( current->base.type) {
    case S_PP_RULE_REF: {
      AST_PP_RULE_REF *scl = (AST_PP_RULE_REF *) current;

      is_token = scl->is_token;

      rt = PEG_PARSER_run_impl( parser,  &scl->rule_ref_resolved->base, &cur_pp_info );

      if (rt) {
        goto parse_error;
      }
      
      if (scl->is_token)
	parser->num_non_terminal ++;
      goto parse_ok;
      }
      break;
      
     case S_PP_RULE: {
      AST_PP_RULE *scl = (AST_PP_RULE *) current;
      S_PP_RULE_TYPE rtype;
      size_t trace_pos = (size_t) -1;
      int tmp;
      
      rtype = parser->rtype;
      parser->rtype = scl->rtype; 

      if ( parser->trace_flags & scl->rtype ) {
        trace_pos = parser->trace_clause.buf_used;
        DBUF_add( &parser->trace_clause, "(?", 2 );
        DBUF_add( &parser->trace_clause, scl->rule_name, strlen( scl->rule_name ) );
        DBUF_add( &parser->trace_clause, "? ", 2 );
      }

      tmp = parser->num_non_terminal;
      parser->num_non_terminal = 0;

      rt = PEG_PARSER_run_impl( parser, (AST_PP_BASE *) &scl->rhs, &cur_pp_info );
       
      parser->rtype = rtype;
      parser->num_non_terminal = tmp;
      
      if (rt) 
      {
        if (trace_pos != (size_t) -1) {
            parser->trace_clause.buf_used = trace_pos;
        }            
        goto parse_error;
      }	
 
      if ( parser->trace_flags ) {
        if (trace_pos != (size_t) -1) {
            parser->trace_clause.buf_used = trace_pos;
        }            
        DBUF_add( &parser->trace_clause, "<", 1 );
        DBUF_add( &parser->trace_clause, scl->rule_name, strlen( scl->rule_name ) );
        DBUF_add( &parser->trace_clause, "> ", 2 );

        display_trace( parser, -1, -1 );
      }

      //ADD_ERROR( parser, PERR_RULE_FAILED, current, cursor_iteration, 0, 0 )
      
      goto parse_ok;
      }
      break;
 
    case S_PP_ALTERNATIVE: { 
      AST_PP_ALTERNATIVE *scl = (AST_PP_ALTERNATIVE *) current;
      AST_PP_BASE *pbase;
      DRING *cur, *curs;
      AST_BASE_LIST *sequence = 0;
      int sequence_ok = 1;
      PEG_PARSER_POS alt_pos;
      int tmp_num_non_terminal;
      S_PP_RULE_TYPE rtype = parser->rtype;
      PP_BASE_INFO *rhs_terms_info = 0;
      size_t rhs_terms_info_count = (size_t) -1;
      size_t max_length_sequence = 0;
      size_t trace_pos;

      max_length_sequence = scl->max_length_sequence;
      if ( max_length_sequence != 0) {
	rhs_terms_info = (PP_BASE_INFO *) alloca( sizeof(PP_BASE_INFO) * max_length_sequence );
      }

      parser->undo_nesting ++; 
     
      sequence_ok =  1;
      DRING_FOREACH( cur, &scl->alternatives ) {
        sequence = (AST_BASE_LIST *) _OFFSETOF(  cur, AST_BASE, entry );
 
	if (!sequence_ok) {
	  // backtrack: there is another sequence to check
	  parser->offset = alt_pos;
	  parser->num_non_terminal = tmp_num_non_terminal;
	  if ( is_non_trivial_error( & parser->error_stack ) != 0) {
	     parser->error_set_enabled = 0;
	  } else {
	     ARRAY_reset( &parser->error_stack );
	     parser->error_set_enabled = 1;
	  }
          //ERROR_RESET( parser );
	  //parser->error_entry_set = 0;
	  if ( parser->trace_flags & parser->rtype ) {
            // trace backtracking action.
            parser->trace_clause.buf_used = trace_pos;
            display_trace( parser, -1, -1 );
          }       
	  sequence_ok = 1;
	}

	alt_pos = parser->offset;
	tmp_num_non_terminal = parser->num_non_terminal; 

	rhs_terms_info_count = 0;
	
        if ( parser->trace_flags & parser->rtype ) {
          trace_pos = parser->trace_clause.buf_used ;
        }  
	DRING_FOREACH( curs, &sequence->statements) {
	  pbase = (AST_PP_BASE *) _OFFSETOF(  curs, AST_BASE, entry );

	  if (rtype == S_PP_RULE_GRAMMAR) {
	    PEG_PARSER_skip_spaces( parser );
	  }

	  if (pbase->base.type == S_PP_SCRIPT) {
	    AST_PP_SCRIPT *scl_scr = (AST_PP_SCRIPT *) pbase;
	    BINDING_DATA *res;
	    // prior to invocation - the current clauses and their values have to be made available to the script
	    // the script can get them by calling some functions.
	    parser->current_rhs_clause = rhs_terms_info; 
	    parser->current_rhs_clause_len = rhs_terms_info_count;

	    // run the script attached to the rule.
	    res = EVAL_function(  parser->eval_ctx, (AST_FUNC_CALL *) scl_scr->rule_script );

	    if (parser->is_fail_parsing) {
		//ADD_ERROR( parser, PERR_STOPPED_BY_SEMANTIC_ACTION, current, cursor_iteration, 0, 0 );
		return -1;
	    }

	    switch( rdata_count) {
		case 0: {
		    rdata = BINDING_DATA_MEM_new( S_VAR_NULL );
		    BINDING_DATA_copy( rdata, res, CP_REF );
		    }
		    break;
		case 1: {
		    BINDING_DATA *vect = BINDING_DATA_MEM_new( S_VAR_LIST );
		    VALARRAY_set( &vect->b.value.array_value, 0, rdata, CP_REF );
		    rdata = vect;
		    }
		    //break;
		default:
		    VALARRAY_set( &rdata->b.value.array_value, rdata_count, res, CP_REF );
		    break;
	    }		
	    rdata_count++;


 	    continue;
	  }

	  INIT_PP_DATA( rhs_terms_info + rhs_terms_info_count );
	  if (PEG_PARSER_run_impl( parser, pbase, rhs_terms_info + rhs_terms_info_count )) {
	     sequence_ok = 0;
	     break;
	  }

	  rhs_terms_info_count ++;
	} // eof current sequence

	if (sequence_ok)
	{
	  break;
	}

 

      } // eof all alternatives

      parser->undo_nesting --;
      
      if (sequence_ok) {
        goto parse_ok;
      }
      // set error...ddd
      goto parse_error;
      }
      break;
    case S_PP_RULE_CONSTANT: {
        AST_PP_CONSTANT *scl = (AST_PP_CONSTANT *) current;
        char *p;

	is_token = 1;

        for( p = scl->const_string; *p != '\0'; ++p ) {
	  if (PEG_PARSER_next_char( parser, &ch ) == -1) {
	    goto parse_error;
	  }
	  if (*p != (char) ch) {
	    goto parse_error;
	  }
	}
        if ( parser->trace_flags & parser->rtype ) {  
          DBUF_add( &parser->trace_clause, "'", 1 );
          DBUF_add( &parser->trace_clause, scl->const_string, strlen( scl->const_string ) );
          DBUF_add( &parser->trace_clause, "' ", 2 );
          display_trace( parser, -1, -1 );
	}    
	parser->num_non_terminal ++;
	goto parse_ok;
      }
      break;
    case S_PP_CHAR_CLASS: {
      AST_PP_CHAR_CLASS *scl = (AST_PP_CHAR_CLASS *) current;
      AST_PP_RANGE *cur; 
      size_t i;

      if (PEG_PARSER_next_char( parser, &ch ) == -1) {
	// parser error; premature input.
	goto parse_error;
      }
      for( i = 0; i < ARRAY_size( &scl->ranges ); i++ ) {
	cur = (AST_PP_RANGE *) ARRAY_at( &scl->ranges, i ); 
	if (cur->to == (uint32_t) -1 ) {
	    if (cur->from == ch) 
		goto parse_ok;
	} else {
	    if (cur->from <= ch && ch <= cur->to) {
		goto parse_ok;
	    }		
	}
      }
      goto parse_error;
    }
      break;
    default:
      assert(0);
  }

parse_ok: 

  // we have a match.
  repetition ++;

  
  if (current->to == 0) { // match none; but we have a match -> fail.
     parser->offset = cursor;
     if (parser->num_non_terminal != 0 && parser->error_set_enabled && parser->rtype == S_PP_RULE_GRAMMAR ) {
	NEXT_ERROR_ENTRY( parser, cursor_iteration, current,  PERR_TERM_NOT_EXPECTED_HERE );
     }	
     //ADD_ERROR( parser, PERR_TERM_NOT_EXPECTED_HERE, current, cursor_iteration, 0, 0 )
     return -1;
  }
  
  if ( current->to == -1 || repetition < current->to ) {
	cursor_iteration = parser->offset;
    goto parse_start;
  } 
 
  // advance read position when possible (not nested within an alternative)
  if (parser->undo_nesting == 0 && parser->offset.lookahead_offset != 0) {
    CIRCBUF_inc_read_pos( &parser->lookahead, parser->offset.lookahead_offset );
    parser->offset.lookahead_offset = 0;
  } 

  if (repetition >= current->from) {
    SET_PP_RET_DATA( rinfo, cursor, parser->offset, repetition, rdata )
    return 0;
  }

  assert(0);
  return -1;    

parse_error:
  if ( repetition >= current->from ) {
    SET_PP_RET_DATA( rinfo, cursor, parser->offset, repetition, rdata )
    parser->offset = cursor_iteration;
    return 0;
  }

  // IF not start of the sequence && no error has been set && this is a grammar (not token)
  if (parser->error_set_enabled ) { //&& parser->rtype == S_PP_RULE_GRAMMAR ) {
    
    if (current->base.type ==  S_PP_ALTERNATIVE) {
       NEXT_ERROR_ENTRY( parser, cursor_iteration, current, PERR_ALTERNATIVE_NOT_PARSED );
       if ( is_non_trivial_error( & parser->error_stack ) != 0) {
	     parser->error_set_enabled = 1;
       } 
    } else if (is_token && current->from == current->to && parser->num_non_terminal != 0 && current->from == 1) {
       ARRAY_reset( &parser->error_stack );
       NEXT_ERROR_ENTRY( parser, cursor_iteration, current, PERR_TERM_EXPECTED_HERE  );
    } else if ( parser->num_non_terminal != 0 )  { // non trivial error (other then the first symbol did not match)
       ARRAY_reset( &parser->error_stack );
       PEG_ERROR_ENTRY *entry = NEXT_ERROR_ENTRY( parser, cursor_iteration, current, PERR_TERM_NOT_ENOUGH_OCCURANCES  );
       entry->u.multiplicity.expected = current->from;
       entry->u.multiplicity.actual  = repetition;
    } 
  }
  return -1;
}

#if 0
static const char *parser_msg[] = {
  //PERR_RULE_FAILED, 
  "Parsing of rule %s failed",
  //PERR_TERM_EXPECTED_HERE,
  "Term %s expected here",
  //PERR_TERM_NOT_EXPECTED_HERE,
  "Term %s not expected here",
  //PERR_TERM_NOT_ENOUGH_OCCURANCES,
  "Not enough occurances for %s",
  //PERR_STOPPED_BY_SEMANTIC_ACTION,
  "Term %s failed due to semantic action",
};
#endif

void PEG_PARSER_print_error( PEG_PARSER *parser )
{
  if ( ARRAY_size( &parser->error_stack ) != 0 ) {
     PEG_ERROR_ENTRY *entry = (PEG_ERROR_ENTRY *) ARRAY_at( &parser->error_stack, 0 );
     fprintf( stderr,"Error: ");
     switch( entry->type ) {
        case PERR_ALTERNATIVE_NOT_PARSED:
	    fprintf( stderr, "PERR_ALTERNATIVE_NOT_PARSED\n" );
	    break;
	case PERR_TERM_EXPECTED_HERE:
	    fprintf( stderr, "PERR_TERM_EXPECTED_HERE\n" );
	    break;
	case PERR_TERM_NOT_EXPECTED_HERE:
	    fprintf( stderr, "PERR_TERM_NOT_EXPECTED_HERE\n" );
	    break;
	case PERR_TERM_NOT_ENOUGH_OCCURANCES:
	    fprintf( stderr, "PERR_TERM_NOT_ENOUGH_OCCURANCES\n" );
	    break;
	case PERR_STOPPED_BY_SEMANTIC_ACTION:
	    fprintf( stderr, "PERR_STOPPED_BY_SEMANTIC_ACTION\n" );
	    break;
     }
  }
}


int PEG_PARSER_run( PEG_PARSER *parser, PP_BASE_INFO *rinfo)
{
  int rt = PEG_PARSER_run_impl( parser, parser->root, rinfo );
  if (rt) {
    PEG_PARSER_print_error( parser );
  }
  return rt;
} 


