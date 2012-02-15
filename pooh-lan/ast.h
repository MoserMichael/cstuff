#ifndef _AST_H_
#define _AST_H_

#include <stdio.h>
#include <assert.h>
#include <cutils/dring.h>
#include <cutils/array.h>
#include <cutils/dlist.h>
#include <cutils/tree.h>
#include "yystype.h"
#include "parsectx.h"
#include <stdlib.h>


/***************************************************/



typedef enum 
{
  S_EXPRESSION,

  S_AST_LIST,

  S_AST_VECTOR,
  
  S_ASSIGNMENT,
  
  S_IF,

  S_WHILE,

  S_FOR,
  
  S_FUN_CALL,

  S_FUN_DECL,
  
  S_RETURN, 

  S_YIELD,

  S_NEXT,
  
  S_BREAK,

} 
  S_TYPE;




#define IS_EXPR_TYPE_REF(x) (/*(x) >= S_EXRP_SCALAR_REF && */ (x) <= S_EXPR_CODE_REF)


/***************************************************/

typedef struct tagAST_BASE {
	S_TYPE    type;
	YYLTYPE   location; 
	struct tagAST_BASE *parent;
	DRING entry; // this statement embedded in statement list.

}	AST_BASE;


M_INLINE void AST_BASE_init(AST_BASE *base, S_TYPE type, YYLTYPE *location)
{
	base->type = type;
	base->location = *location;
	base->parent = 0;
	DRING_init( &base->entry );
}

M_INLINE void AST_BASE_last_clause( AST_BASE *base, AST_BASE *last)
{
	base->location.last_line = last->location.last_line;
	base->location.last_column = last->location.last_column;
}

M_INLINE void AST_BASE_first_clause( AST_BASE *base, AST_BASE *first)
{
	base->location.first_line = first->location.first_line;
	base->location.first_column = first->location.first_column;
}


/***************************************************/

typedef struct tagAST_BASE_LIST {
  AST_BASE base;
  DRING statements;

} AST_BASE_LIST;

M_INLINE  AST_BASE_LIST * AST_BASE_LIST_init( YYLTYPE *location )
{
  AST_BASE_LIST *scl;

  scl = (AST_BASE_LIST *) malloc( sizeof( AST_BASE_LIST ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_AST_LIST, location );
  DRING_init( &scl->statements );
  return scl;
}

M_INLINE void AST_BASE_LIST_add( AST_BASE_LIST *scl, AST_BASE *add )
{
  add->parent = &scl->base;
  DRING_push_back( &scl->statements, &add->entry );
  AST_BASE_last_clause( &scl->base, add );

}

/***************************************************/

typedef struct tagAST_VECTOR {
  AST_BASE base;
  ARRAY refs;

} AST_VECTOR;


M_INLINE AST_VECTOR * AST_VECTOR_init( YYLTYPE *location )
{
 AST_VECTOR *scl;

 scl = (AST_VECTOR *) malloc( sizeof( AST_VECTOR ) );
 if (!scl) {
   return 0;
 }
 
 AST_BASE_init( &scl->base, S_AST_VECTOR, location );

 if (ARRAY_init( &scl->refs, sizeof( void * ), 10 )) {
   free(scl);
   return 0;
 }
 return scl;
}

M_INLINE int AST_VECTOR_add( AST_VECTOR *scl, AST_BASE *add)
{
  AST_BASE_last_clause( &scl->base, add );
  return ARRAY_push_back( &scl->refs, &add, sizeof( void * ) );
}


M_INLINE AST_BASE * AST_VECTOR_get( AST_VECTOR *scl, size_t idx )
{
  uint8_t *ret;
  
  ret = ARRAY_at( &scl->refs, idx );
  if (ret) {
     return * ((AST_BASE **) ret);
  }
  return 0;
}  

/***************************************************/

typedef enum tagS_EXPR_TYPE {

  S_EXPR_BINARY,

  S_EXPR_UNARY,

  S_EXPR_REFERENCE,

  S_EXPR_CONSTANT,

  S_EXPR_FUNCALL,

  S_EXPR_HASH_INDEX,

  S_EXPR_ARRAY_INDEX,

  S_EXPR_LIST_VALUES, // array constuctor

  S_EXPR_HASH_VALUES, // hash constructor

  S_EXPR_PLACEHOLDER,

  S_EXPR_LAMBDA,  // unamed function

  S_EXPR_ERROR,  // one of the types in an expression evaluated to an error, don't propagate the error further

}
  S_EXPR_TYPE;

typedef enum {
   
  S_VAR_INT = 1,

  S_VAR_DOUBLE = 2,

  S_VAR_STRING = 4,

  S_VAR_CODE = 8,

  S_VAR_HASH = 16,

  S_VAR_LIST = 32,

  S_VAR_ANY = S_VAR_INT | S_VAR_DOUBLE | S_VAR_STRING | S_VAR_CODE | S_VAR_HASH | S_VAR_LIST,

} AST_VAR_TYPE;

M_INLINE int is_numeric_type( AST_VAR_TYPE ty )
{
  return ty == S_VAR_INT || ty == S_VAR_DOUBLE;
}

M_INLINE int is_numeric_or_string_type( AST_VAR_TYPE ty )
{
  return ty == S_VAR_INT || ty == S_VAR_DOUBLE || ty == S_VAR_STRING;
}

typedef union {
  long   long_value;

  double double_value;
		
  char   *string_value;
} Simple_value_type;

typedef struct tagAST_EXPRESSION {
  AST_BASE base;

  S_EXPR_TYPE exp_type;
  AST_VAR_TYPE value_type;
 
  union {
	struct {
		int    op;
		struct tagAST_EXPRESSION *expr_left, *expr_right;
	} expr;

	struct {
		int    op;
		struct tagAST_EXPRESSION *expr;
		int    is_prefix;
	} unary;

	struct {
		char  *lhs;
		AST_VECTOR *indexes;
		struct tagBINDING_DATA *binding;
	} ref;

	Simple_value_type const_value;

	AST_VECTOR *index_expressions;

        struct tagAST_EXPRESSION *index_expr;

	struct tagAST_FUNC_CALL *fcall;

	struct tagAST_FUNC_DECL *fdecl;

  } val;

} AST_EXPRESSION;

M_INLINE AST_EXPRESSION *AST_EXPRESSION_init( S_EXPR_TYPE exp_type, AST_VAR_TYPE value_type, YYLTYPE *location  )
{
  AST_EXPRESSION *scl;

  scl = (AST_EXPRESSION *) malloc( sizeof( AST_EXPRESSION ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_EXPRESSION, location );
  scl->exp_type = exp_type;
  scl->value_type = value_type;

  return scl;
}

M_INLINE void AST_EXPRESSION_free( AST_EXPRESSION * expr )
{
  free(expr );
}

int AST_EXPRESSION_unary_op_check_types( PARSECONTEXT *parse_context, AST_EXPRESSION *scl );
int AST_EXPRESSION_unary_fold_constants( AST_EXPRESSION *scl);

int AST_EXPRESSION_binary_op_check_types( PARSECONTEXT *parse_context, AST_EXPRESSION *scl );
int AST_EXPRESSION_binary_fold_constants( AST_EXPRESSION *scl );


M_INLINE AST_EXPRESSION * AST_EXPRESSION_init_binary( int op, AST_EXPRESSION *lhs, AST_EXPRESSION *rhs  )
{
  AST_EXPRESSION *scl; 

  assert( lhs->base.type == S_EXPRESSION );
  assert( rhs->base.type == S_EXPRESSION );
 
  scl = AST_EXPRESSION_init( S_EXPR_BINARY, S_VAR_ANY, &lhs->base.location );
  if (!scl) {
    return 0;
  }

  scl->val.expr.op = op;
  
  lhs->base.parent = &scl->base;
  scl->val.expr.expr_left = lhs;
  
  rhs->base.parent = &scl->base;
  scl->val.expr.expr_right = rhs;

  if (! AST_EXPRESSION_binary_fold_constants( scl ) ) {
     AST_EXPRESSION_free( lhs );
     AST_EXPRESSION_free( rhs );
     return 0;
  }

  return scl;
}


M_INLINE AST_EXPRESSION * AST_EXPRESSION_init_unary( int op, AST_EXPRESSION *lhs, int prefix, YYLTYPE *location  )
{
  AST_EXPRESSION *scl; 

  scl = AST_EXPRESSION_init( S_EXPR_UNARY, S_VAR_ANY, location );
  if (!scl) {
    return 0;
  }

  scl->exp_type = S_EXPR_BINARY;
  scl->val.unary.op = op;

  assert( lhs->base.type == S_EXPRESSION );
  lhs->base.parent = &scl->base;
  scl->val.unary.expr = lhs;

  scl->val.unary.is_prefix = prefix;

  if (! AST_EXPRESSION_unary_fold_constants( scl ) ) {
     AST_EXPRESSION_free( lhs );
  }
  return scl;
}


M_INLINE AST_EXPRESSION * AST_EXPRESSION_init_ref( const char *name, AST_VECTOR *indexes, YYLTYPE *location  )
{
  AST_EXPRESSION *scl,*first_idx;

  scl = AST_EXPRESSION_init( S_EXPR_REFERENCE, S_VAR_ANY, location );
  if (!scl) {
    return 0;
  }

  scl->val.ref.lhs = strdup( name );
  scl->val.ref.binding = 0;

  if (indexes) {
    assert( indexes->base.type == S_AST_VECTOR );
    indexes->base.parent = &scl->base;
    
    first_idx = (AST_EXPRESSION *) AST_VECTOR_get( indexes, 0 );   
    if (first_idx) {
      if (first_idx->exp_type == S_EXPR_ARRAY_INDEX) {
	scl->value_type = S_VAR_LIST;
      } else {
	scl->value_type = S_VAR_HASH;
      }
    }
  }
  scl->val.ref.indexes = indexes;

  return scl;
}

AST_EXPRESSION *AST_compile_multi_part_string( PARSECONTEXT *pc );

/****************************************************/

typedef enum { 
  ASSIGNMENT_DEEP_COPY,
  ASSIGNMENT_REF_COPY,

} ASSIGNMENT_TYPE;
typedef struct tagAST_ASSIGNMEN {
  AST_BASE base;
  struct tagAST_BASE *left_side;
  struct tagAST_EXPRESSION *right_side;

  ASSIGNMENT_TYPE type; 

} AST_ASSIGNMENT;
 

M_INLINE AST_ASSIGNMENT * AST_ASSIGNMENT_init( ASSIGNMENT_TYPE type,
		struct tagAST_BASE *lhs, struct tagAST_EXPRESSION *rhs, YYLTYPE *location  )
{
  AST_ASSIGNMENT * scl;

  scl = (AST_ASSIGNMENT *) malloc( sizeof( AST_ASSIGNMENT ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_ASSIGNMENT, location );
  
  assert(lhs->type == S_EXPRESSION || lhs->type == S_AST_VECTOR );
  lhs->parent = &scl->base;
  scl->left_side = lhs;
  
  assert(rhs->base.type == S_EXPRESSION );
  rhs->base.parent = &scl->base;
  scl->right_side = rhs;

  scl->type = type;

  return scl;
}

/***************************************************/
typedef struct tagAST_COND {
  AST_BASE base;
  
  AST_EXPRESSION *condition;
  AST_BASE_LIST *block;
  struct tagAST_COND *elsecond;
 
} AST_COND;


M_INLINE AST_COND * AST_COND_init( AST_EXPRESSION *expr, AST_BASE_LIST *block, YYLTYPE *location   )
{
  AST_COND * scl;

  scl = (AST_COND * ) malloc( sizeof(AST_COND) );
  if (!scl) {
    return 0;
  }

  AST_BASE_init( &scl->base, S_IF, location );
  
  if (expr != 0 ) { 
    assert( expr->base.type == S_EXPRESSION );
    expr->base.parent = &scl->base;
  }
  scl->condition = expr;

  assert( block->base.type == S_AST_LIST );
  block->base.parent = &scl->base;
  scl->block = block;

  scl->elsecond = 0;

  return scl;
}

M_INLINE void AST_COND_set_else( AST_COND *scl, AST_COND *elsecond )
{
  assert( scl->base.type == S_IF );
  assert( elsecond->base.type == S_IF );
  
  elsecond->base.parent = &scl->base;
  scl->elsecond = elsecond;
}


M_INLINE int AST_COND_set_else_block( AST_COND *scl, AST_BASE_LIST *block )
{
  AST_COND *else_cond;
  
  assert( scl->base.type == S_IF );
  
  else_cond = AST_COND_init( 0, block, &block->base.location );
  if (!else_cond) {
    return -1;
  }
  AST_COND_set_else( scl, else_cond );
  return 0;
}

/***************************************************/
typedef struct tagAST_FOR_LOOP {
  AST_BASE base;
  
  struct tagAST_EXPRESSION *loop_var; 
  struct tagAST_EXPRESSION *loop_expr;
  AST_BASE_LIST  *block; 
 
} AST_FOR_LOOP;

M_INLINE AST_FOR_LOOP *AST_FOR_LOOP_init( AST_EXPRESSION *loop_var, AST_EXPRESSION *loop_expr, AST_BASE_LIST *block, YYLTYPE *location   )
{
  AST_FOR_LOOP *scl;

  scl = (AST_FOR_LOOP *) malloc( sizeof( AST_FOR_LOOP ) );
  if (!scl) {
    return 0;
  }

  AST_BASE_init( &scl->base, S_WHILE, location );

  assert(loop_var->base.type == S_EXPRESSION );
  loop_var->base.parent = &scl->base;  
  scl->loop_var = loop_var;

  assert(loop_expr->base.type == S_EXPRESSION );
  loop_expr->base.parent = &scl->base;
  scl->loop_expr = loop_expr;

  assert(block->base.type == S_AST_LIST );
  block->base.parent = &scl->base;
  scl->block = block;

  return scl;
}

/***************************************************/
typedef struct tagAST_WHILE_LOOP {
  AST_BASE base;
  
  struct tagAST_EXPRESSION *condition;
  AST_BASE_LIST *block; 
  int post_condition;
 
} AST_WHILE_LOOP;


M_INLINE AST_WHILE_LOOP *AST_WHILE_LOOP_init(AST_EXPRESSION *expr, AST_BASE_LIST *block, int post_condition, YYLTYPE *location  )
{
  AST_WHILE_LOOP *scl;

  scl = (AST_WHILE_LOOP * ) malloc( sizeof(AST_WHILE_LOOP) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_WHILE, location );

  assert(expr->base.type == S_EXPRESSION );
  expr->base.parent = &scl->base;

  assert(block->base.type == S_AST_LIST );
  block->base.parent = &scl->base;
  scl->block = block;

  scl->post_condition = post_condition;
  
  return scl;
}

/***************************************************/

typedef struct tagBINDING_DATA {

  AST_VAR_TYPE value_type;

  union {
    long   long_value;

    double double_value;
		
    char   *string_value;
    
    ARRAY   array_value;

    HASH    hash_value;

    struct {
       struct tagBINDING_DATA *this_environment;
       struct tagAST_FUNC_DECL *fdecl;
    }  func_value;

  } value;

} BINDING_DATA;

/* entry maps a binding name to a binding value; Not very space efficient in deed very space inefficient that is */
typedef struct tagBINDING_ENTRY {
  HASH_Entry entry;
  const char *binding_name;
 
  YYLTYPE def_location;  // location where binding is defined in code.
  AST_VAR_TYPE value_type;
  int stack_offset;

} BINDING_ENTRY;


M_INLINE int binding_hash_compare( HASH_Entry  *entry, void * key, ssize_t key_length)
{
  BINDING_ENTRY *lhs;	

  (void) key_length;

  lhs = (BINDING_ENTRY *) entry;

  if (strcmp(  lhs->binding_name, key) == 0) {
    return 0;
  }
  return 1;
}

#define CHKR_RETURN_VALUE_UNKNOWN 1
#define CHKR_WORKING  2
#define CHKR_RETURN_VALUE_KNOWN 4

#define CHKR_REPEAT_ON_FINISH 8
#define CHKR_REPEAT_NOW 16

typedef struct tagAST_FUNC_DECL {

  AST_BASE base;
  TREENODE funcs; // all functions (by nesting of declaration)
  
  const char *f_name;
  AST_VECTOR *func_params;
  AST_BASE_LIST *func_body;

  HASH scope_map_name_to_binding;
    
  int checker_state;  

} AST_FUNC_DECL;



M_INLINE AST_FUNC_DECL * AST_FUNC_DECL_init(const char *f_name, AST_VECTOR *func_param, PARSECONTEXT *ctx, YYLTYPE *location  )
{  
  AST_FUNC_DECL *scl;

  scl = (AST_FUNC_DECL *) malloc( sizeof( AST_FUNC_DECL ) );
  if (!scl) {
    return 0;
  }
  
  AST_BASE_init( &scl->base, S_FUN_DECL, location );
  TREE_init_root( &scl->funcs );

  scl->f_name = f_name ? strdup( f_name ) : 0;
  
  assert( func_param->base.type == S_AST_VECTOR );
  func_param->base.parent = &scl->base;
  scl->func_params = func_param;

  scl->checker_state = 0;  
  
#if 0
  if (HASH_init( &ctx->scope_map_name_to_binding, 10, 0, binding_hash_compare, 0 ) ) {
    return -1;
  }
#endif

  if (scl->f_name) {
    PARSECONTEXT_add_function_def( ctx, scl ); 
  }
  return scl;
}


M_INLINE void AST_FUNC_DECL_set_body( AST_FUNC_DECL *scl, PARSECONTEXT *ctx, AST_BASE_LIST *body )
{
    assert( body->base.type == S_AST_LIST );
    body->base.parent = &scl->base;
    scl->func_body = body;

    ctx->current = _OFFSETOF( scl->funcs.parent,  AST_FUNC_DECL, funcs );
}

/***************************************************/
typedef struct tagAST_FUNC_CALL {
  
  AST_BASE base;
  

  AST_FUNC_DECL *func_decl;
  
  AST_EXPRESSION *f_name;
  AST_VECTOR *call_params;

} AST_FUNC_CALL;



M_INLINE AST_FUNC_CALL * AST_FUNC_CALL_init( AST_EXPRESSION *f_name, AST_VECTOR *func_params, YYLTYPE *location  )
{  
  AST_FUNC_CALL *scl;

  scl = (AST_FUNC_CALL *) malloc( sizeof( AST_FUNC_CALL ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_FUN_CALL, location );

  assert( func_params->base.type == S_EXPRESSION );  
  scl->f_name = f_name ;
  
  assert( func_params->base.type == S_AST_VECTOR );
  scl->call_params = func_params;
  scl->func_decl = 0;

  return scl;
}


/***************************************************/
typedef struct tagAST_RETURN {
  
  AST_BASE base;
 
  AST_EXPRESSION *rvalue;

} AST_RETURN;
  

M_INLINE AST_RETURN *AST_RETURN_init( S_TYPE type, AST_EXPRESSION *expr, YYLTYPE *location  )
{
  AST_RETURN *ret;

  ret = (AST_RETURN *) malloc( sizeof( AST_RETURN ) );
  if (!ret) {
    return 0;
  }
  AST_BASE_init( &ret->base, type, location );
  
  if (expr) {
    assert(expr->base.type == S_EXPRESSION );
    expr->base.parent = &ret->base;
  }
  ret->rvalue = expr;

  return ret;
}

// --------------------------------------------------------------------------------------------------

M_INLINE AST_EXPRESSION * AST_EXPRESSION_fcall_init( struct tagAST_FUNC_CALL *func, YYLTYPE *location  )
{
  AST_EXPRESSION *scl; 

  scl = AST_EXPRESSION_init(  S_EXPR_FUNCALL, S_VAR_ANY, location );
  if (!scl) {
    return 0;
  }

  assert( func->base.type == S_FUN_CALL );
  func->base.parent = &scl->base;
  scl->val.fcall = func;

  return scl;
}




#endif


  









