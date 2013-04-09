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


int GRAMMAR_init(  GRAMMARCHECKERCTX * ctx,  struct tagPARSECONTEXT *pctx  )
{
    if (HASH_init( &ctx->rule_defs, 10, 0, hash_compare, 0 )) {
	return -1;
    }
    ARRAY_init( &ctx->visited, sizeof( void *) , 0 );
    ARRAY_init( &ctx->current_alt_stack, sizeof( void *) , 0 );
    ctx->ctx = pctx;
    return 0;
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

   if (level == 0 && (base->from == base->to && base->from == 1))  
     show_bracket = 0;

   if (show_bracket)
     fprintf( stderr, "( ");
  
   DRING_FOREACH( cur, &alt->alternatives ) {
     lst = (AST_BASE_LIST *) _OFFSETOF( cur, AST_BASE, entry );
     
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

   if ( ! (base->from == base->to && base->from == 1) ) {

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
   if (show_bracket)
     fprintf( stderr, " )");
}

void GRAMMAR_on_left_recursion( GRAMMARCHECKERCTX *ctx, AST_PP_RULE *rule )
{
   AST_PP_RULE *tmp;
   size_t i;
  
   fprintf( stderr, "Error: The grammar has left recursion. can't use such a grammar.\n" );

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


void GRAMMAR_check_sequence( GRAMMARCHECKERCTX *ctx, AST_BASE_LIST *lst )
{
    DRING *cur;
    AST_PP_BASE *elm;
    int is_first = 1;
    int is_top = ARRAY_size( &ctx->visited ) == 1;  

    DRING_FOREACH( cur, &lst->statements ) {
	elm = (AST_PP_BASE *) _OFFSETOF( cur, AST_BASE, entry );
	
	if (elm->base.type == S_PP_RULE_REF) {
		AST_PP_RULE *rule;
		AST_PP_RULE_REF *rref = (AST_PP_RULE_REF *) elm; 

		rule = GRAMMAR_find_rule( ctx, rref->rule_name );

		if (!rule && is_top) {
		  do_yyerror( &rref->base.base.location, ctx->ctx, "The rule %s is not defined", rref->rule_name );
		}

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
	    if (!is_top) {
		break;
	    }
	}
    }
}

void GRAMMAR_check_alt( GRAMMARCHECKERCTX *ctx, AST_PP_ALTERNATIVE *alt )
{
   DRING *cur;
   AST_BASE_LIST *lst;
   
   DRING_FOREACH( cur, &alt->alternatives ) {
     lst = (AST_BASE_LIST *) _OFFSETOF( cur, AST_BASE, entry );
     GRAMMAR_check_sequence( ctx, lst );
   }
}

void GRAMMAR_check_rule( GRAMMARCHECKERCTX *ctx, AST_PP_RULE *rule )
{
   void *tmp;

   ARRAY_push_back( &ctx->visited, &rule, sizeof(void *) );
   rule->flags |= RULE_FLAGS_VISITED; 
   GRAMMAR_check_alt( ctx, & rule->rhs );

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

   slist = (AST_BASE_LIST *) program;
   assert( slist->base.type == S_AST_LIST );
   	
   DRING_FOREACH( cur, &slist->statements ) {

	rule = (AST_PP_RULE *)  _OFFSETOF( cur, AST_BASE, entry );
	assert( rule->base.base.type ==  S_PP_RULE );
	GRAMMAR_check_rule( ctx, rule );
   }
   return 0;
}

/* ----------------- */

int PEG_PARSER_init( PEG_PARSER *parser, struct tagAST_BASE *program, size_t lookahead_size, PEG_PARSER_DATA_SRC data_source, void *data_src_ctx , EVAL_CTX *eval_ctx )
{
  AST_BASE_LIST *slist;
  
  
  slist = (AST_BASE_LIST *) program;
  parser->root = (AST_PP_BASE *) DRING_get_first( &slist->statements ); // first rule is the top rule to parse.
 
  if (CIRCBUF_init( &parser->lookahead, lookahead_size )) {
    return -1;
  }
  parser->rtype = S_PP_RULE_GRAMMAR; 
  PEG_PARSER_POS_init( &parser->offset );
  parser->data_src = data_source;
  parser->data_src_ctx = data_src_ctx;
  parser->undo_nesting = 0;
  parser->eval_ctx = eval_ctx;
  
  parser->current_rhs_clause = 0;
  parser->current_rhs_clause_len = 0;

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
  have_room = CIRCBUF_size( &parser->lookahead ) - parser->offset.lookahead_offset;

  if (!have_room) { 
     // resize the lookahead buffer.
     if (CIRCBUF_resize( &parser->lookahead, CIRCBUF_maxsize( &parser->lookahead ) * 2 ) ) {
       return -1;
     }
  }

  // read some data.
  if (parser->data_src( &parser->lookahead, &has_read, parser->data_src_ctx ) ) {
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


int PEG_PARSER_run_impl( PEG_PARSER *parser, AST_PP_BASE *current, PP_BASE_INFO *rinfo )
{
  PEG_PARSER_POS cursor_iteration, cursor;
  PP_CHAR ch;
  int repetition = 0;
  BINDING_DATA *rdata = 0;
  PP_BASE_INFO cur_pp_info;
  int rt;
  PP_BASE_INFO *rhs_terms_info = 0;
  size_t rhs_terms_info_count = (size_t) -1;

  cursor_iteration = cursor = parser->offset;

parse_start:

  INIT_PP_DATA( &cur_pp_info );
  switch( current->base.type) {
    case S_PP_RULE_REF: {
      AST_PP_RULE_REF *scl = (AST_PP_RULE_REF *) current;

      rt = PEG_PARSER_run_impl( parser,  &scl->rule_ref_resolved->base, &cur_pp_info );

      if (rt)  {
        goto parse_error;
      }
      	
      rhs_terms_info  = &cur_pp_info;
      rhs_terms_info_count = 1;
      goto parse_ok;
      }
      break;
      
     case S_PP_RULE: {
      AST_PP_RULE *scl = (AST_PP_RULE *) current;
      S_PP_RULE_TYPE rtype;
      
      rtype = parser->rtype;
      parser->rtype = scl->rtype;
      rt = PEG_PARSER_run_impl( parser, (AST_PP_BASE *) &scl->rhs, &cur_pp_info );
      parser->rtype = rtype;

      if (rt) 
        goto parse_error;
      
      rhs_terms_info = &cur_pp_info;
      rhs_terms_info_count = 1;
      goto parse_ok;
      }
      break;

    case S_PP_ALTERNATIVE: { 
      AST_PP_ALTERNATIVE *scl = (AST_PP_ALTERNATIVE *) current;
      AST_PP_BASE *pbase;
      DRING *cur, *curs;
      AST_BASE_LIST *sequence;
      int sequence_ok = 1;
      PEG_PARSER_POS alt_pos;
      S_PP_RULE_TYPE rtype = parser->rtype;
      size_t max_length_sequence = 0;

      max_length_sequence = scl->max_length_sequence;
      if ( max_length_sequence != 0) {
	rhs_terms_info = (PP_BASE_INFO *) alloca( sizeof(PP_BASE_INFO) * max_length_sequence );
      }

      parser->undo_nesting ++; 
      // the main challenge.
      DRING_FOREACH(  cur, &scl->alternatives ) {
	sequence = (AST_BASE_LIST *) cur;

	sequence_ok = 1;

	alt_pos = parser->offset;

	rhs_terms_info_count = 0;

	DRING_FOREACH( curs, &sequence->statements) {
	  pbase = (AST_PP_BASE *) curs;

	  if (rtype == S_PP_RULE_GRAMMAR) {
	    PEG_PARSER_skip_spaces( parser );
	  }

	  INIT_PP_DATA( rinfo + rhs_terms_info_count );
	  if (PEG_PARSER_run_impl( parser, pbase, rhs_terms_info + rhs_terms_info_count )) {
	     sequence_ok = 0;
	     break;
	  }
	  rhs_terms_info_count ++;
	}
	if (sequence_ok)
	{
	  break;
	}
    
	parser->offset = alt_pos;
      }

      parser->undo_nesting --;
      
      if (sequence_ok) {
        goto parse_ok;
      }
      goto parse_error;
      }
      break;
    case S_PP_RULE_CONSTANT: {
        AST_PP_CONSTANT *scl = (AST_PP_CONSTANT *) current;
        char *p;

        for( p = scl->const_string; *p != '\0'; ++p ) {
	  if (PEG_PARSER_next_char( parser, &ch ) == -1) {
	    goto parse_error;
	  }
	  if (*p != (char) ch) {
	    goto parse_error;
	  }
	}
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
     return -1;
  }
  
  if ( current->to == -1 || repetition <= current->to ) {
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
    goto parser_ok_script;
  }
  return -1;    

parse_error:
  if ( repetition >= current->from ) {
    SET_PP_RET_DATA( rinfo, cursor, parser->offset, repetition, rdata )
    parser->offset = cursor_iteration;
    goto parser_ok_script;
  }
  return -1;

parser_ok_script:
  if (current->rule_script != 0) {
    // prior to invocation - the current clauses and their values have to be made available to the script
    // the script can get them by calling some functions.
    if (rhs_terms_info_count != (size_t) -1)  {
      parser->current_rhs_clause = rhs_terms_info; 
      parser->current_rhs_clause_len = rhs_terms_info_count;
    } else {
      parser->current_rhs_clause = rinfo;
      parser->current_rhs_clause_len = 1;
    }

    // run the script attached to the rule.
    rdata = EVAL_function(  parser->eval_ctx, (AST_FUNC_CALL *) current->rule_script );
  } 

  return 0;
}

int PEG_PARSER_run( PEG_PARSER *parser, PP_BASE_INFO *rinfo)
{
  return PEG_PARSER_run_impl( parser, parser->root, rinfo );
} 


