#include "ast.h"
#include "parser.h"
#include <ctype.h>

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
    ARRAY_init( &ctx->visited, sizeof(void *), 0 );
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
	elm = _OFFSETOF( cur, AST_BASE, entry );
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

      fprintf(stderr, " %6d | %s %c",  
		tmp->base.base.location.first_line, 
		tmp->rule_name,
		tmp->rtype == S_PP_RULE_GRAMMAR ? '=' : ':' );

      AST_PP_ALTERNATIVE *alt = &tmp->rhs;
      GRAMMAR_show_alt( alt, 0 );	

      fprintf(stderr,"\n\n");
   }
  

}


void GRAMMAR_check_sequence( GRAMMARCHECKERCTX *ctx, AST_BASE_LIST *lst )
{
    DRING *cur;
    AST_BASE *elm;
    int is_first = 1;

    DRING_FOREACH( cur, &lst->statements ) {
	elm = _OFFSETOF( cur, AST_BASE, entry );
	if (elm->type == S_PP_RULE_REF) {
		AST_PP_RULE *rule;
		AST_PP_RULE_REF *rref = (AST_PP_RULE_REF *) elm; 

		rule = GRAMMAR_find_rule( ctx, rref->rule_name );
		if (!rule) {
		  do_yyerror( &rref->base.base.location, ctx->ctx, "The rule %s is not defined", rref->rule_name );
		}

		if (is_first) {
		    // check for left recursion.
		    if (rule->flags & RULE_FLAGS_VISITED) {
			 // here we have left recursion
			 GRAMMAR_on_left_recursion( ctx, rule );
		    }

		}
	}

	is_first = 0;
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

