#ifndef _AST_H_
#define _AST_H_

#include <stdio.h>
#include <assert.h>
#include <cutils/dring.h>
#include <cutils/array.h>
#include <cutils/dlist.h>
#include "yystype.h"
#include "lexer.h"
#include <stdlib.h>


extern YYLTYPE yyloc;

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

  S_CONTINUE,
  
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


M_INLINE void AST_BASE_init(AST_BASE *base, S_TYPE type, YYLTYPE location)
{
	base->type = type;
	base->location = location;
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

M_INLINE  AST_BASE_LIST * AST_BASE_LIST_init( )
{
  AST_BASE_LIST *scl;

  scl = (AST_BASE_LIST *) malloc( sizeof( AST_BASE_LIST ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_AST_LIST, yyloc );
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


M_INLINE AST_VECTOR * AST_VECTOR_init( )
{
 AST_VECTOR *scl;

 scl = (AST_VECTOR *) malloc( sizeof( AST_VECTOR ) );
 if (!scl) {
   return 0;
 }
 
 AST_BASE_init( &scl->base, S_AST_VECTOR, yyloc );

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

  S_EXPR_LIST_VALUES,

  S_EXPR_HASH_VALUES,

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
	} ref;

	union {
	  long   long_value;

	  double double_value;
		
	  char   *string_value;
	} const_value;

	AST_VECTOR *index_expressions;

        struct tagAST_EXPRESSION *index_expr;

	struct tagAST_FUNC_CALL *fcall;

	struct tagAST_FUNC_DECL *fdecl;

  } val;

} AST_EXPRESSION;

M_INLINE AST_EXPRESSION *AST_EXPRESSION_init( S_EXPR_TYPE exp_type, AST_VAR_TYPE value_type )
{
  AST_EXPRESSION *scl;

  scl = (AST_EXPRESSION *) malloc( sizeof( AST_EXPRESSION ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_EXPRESSION, yyloc );
  scl->exp_type = exp_type;
  scl->value_type = value_type;

  return scl;
}

int AST_EXPRESSION_check_types( AST_EXPRESSION *scl, int op,  AST_EXPRESSION *lhs, AST_EXPRESSION *rhs );
int AST_EXPRESSION_fold_constants( AST_EXPRESSION *scl, int op,  AST_EXPRESSION *lhs, AST_EXPRESSION *rhs );


M_INLINE AST_EXPRESSION * AST_EXPRESSION_init_binary( int op, AST_EXPRESSION *lhs, AST_EXPRESSION *rhs )
{
  AST_EXPRESSION *scl; 

  assert( lhs->base.type == S_EXPRESSION );
  assert( rhs->base.type == S_EXPRESSION );
 
  scl = AST_EXPRESSION_init( S_EXPR_BINARY, S_VAR_ANY );
  if (!scl) {
    return 0;
  }

  if (AST_EXPRESSION_check_types( scl, op, lhs, rhs ) ) {
     scl->exp_type = S_EXPR_ERROR;
     return 0;
  }

  if (AST_EXPRESSION_fold_constants( scl, op, lhs, rhs ) ) {
    scl->exp_type = S_EXPR_ERROR;
    return 0;
  }
 
  scl->val.expr.op = op;
  
  lhs->base.parent = &scl->base;
  scl->val.expr.expr_left = lhs;
  
  rhs->base.parent = &scl->base;
  scl->val.expr.expr_right = rhs;

  return scl;
}


M_INLINE AST_EXPRESSION * AST_EXPRESSION_init_unary( int op, AST_EXPRESSION *lhs, int prefix )
{
  AST_EXPRESSION *scl; 

  scl = AST_EXPRESSION_init( S_EXPR_UNARY, S_VAR_ANY );
  if (!scl) {
    return 0;
  }

  scl->exp_type = S_EXPR_BINARY;
  scl->val.unary.op = op;

  assert( lhs->base.type == S_EXPRESSION );
  lhs->base.parent = &scl->base;
  scl->val.unary.expr = lhs;

  scl->val.unary.is_prefix = prefix;

  return scl;
}


M_INLINE AST_EXPRESSION * AST_EXPRESSION_init_ref( const char *name, AST_VECTOR *indexes )
{
  AST_EXPRESSION *scl; 

  scl = AST_EXPRESSION_init( S_EXPR_REFERENCE, S_VAR_ANY );
  if (!scl) {
    return 0;
  }

  scl->val.ref.lhs = strdup( name );
  
  if (indexes) {
    assert( indexes->base.type == S_AST_VECTOR );
    indexes->base.parent = &scl->base;
  }
  scl->val.ref.indexes = indexes;

  return scl;
}

/****************************************************/

typedef enum { 
  ASSIGNMENT_DEEP_COPY,
  ASSIGNMENT_REF_COPY,

} ASSIGNMENT_TYPE;
typedef struct tagAST_ASSIGNMEN {
  AST_BASE base;
  struct tagAST_EXPRESSION *left_side;
  struct tagAST_EXPRESSION *right_side;

  ASSIGNMENT_TYPE type; 

} AST_ASSIGNMENT;
 

M_INLINE AST_ASSIGNMENT * AST_ASSIGNMENT_init( ASSIGNMENT_TYPE type,
		struct tagAST_EXPRESSION *lhs, struct tagAST_EXPRESSION *rhs )
{
  AST_ASSIGNMENT * scl;

  scl = (AST_ASSIGNMENT *) malloc( sizeof( AST_ASSIGNMENT ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_ASSIGNMENT, yyloc );
  
  assert(lhs->base.type == S_EXPRESSION );
  lhs->base.parent = &scl->base;
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


M_INLINE AST_COND * AST_COND_init( AST_EXPRESSION *expr, AST_BASE_LIST *block  )
{
  AST_COND * scl;

  scl = (AST_COND * ) malloc( sizeof(AST_COND) );
  if (!scl) {
    return 0;
  }

  AST_BASE_init( &scl->base, S_IF, yyloc );
  
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
  
  else_cond = AST_COND_init( 0, block );
  if (!else_cond) {
    return -1;
  }
  AST_COND_set_else( scl, else_cond );
  return 0;
}

/***************************************************/
typedef struct tagAST_FOR_LOOP {
  AST_BASE base;
  
  const char *loop_var;
  struct tagAST_EXPRESSION *loop_expr;
  AST_BASE_LIST  *block; 
 
} AST_FOR_LOOP;

M_INLINE AST_FOR_LOOP *AST_FOR_LOOP_init( const char *loop_var, AST_EXPRESSION *loop_expr, AST_BASE_LIST *block  )
{
  AST_FOR_LOOP *scl;

  scl = (AST_FOR_LOOP *) malloc( sizeof( AST_FOR_LOOP ) );
  if (!scl) {
    return 0;
  }

  AST_BASE_init( &scl->base, S_WHILE, yyloc );

  scl->loop_var = strdup( loop_var );

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


M_INLINE AST_WHILE_LOOP *AST_WHILE_LOOP_init(AST_EXPRESSION *expr, AST_BASE_LIST *block, int post_condition )
{
  AST_WHILE_LOOP *scl;

  scl = (AST_WHILE_LOOP * ) malloc( sizeof(AST_WHILE_LOOP) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_WHILE, yyloc );

  assert(expr->base.type == S_EXPRESSION );
  expr->base.parent = &scl->base;

  assert(block->base.type == S_AST_LIST );
  block->base.parent = &scl->base;
  scl->block = block;

  scl->post_condition = post_condition;
  
  return scl;
}

/***************************************************/
typedef struct tagAST_FUNC_DECL {

  AST_BASE base;
  
  const char *f_name;
  AST_VECTOR *func_params;
  AST_BASE_LIST *func_body;
} AST_FUNC_DECL;



M_INLINE AST_FUNC_DECL * AST_FUNC_DECL_init(const char *f_name, AST_VECTOR *func_param )
{  
  AST_FUNC_DECL *scl;

  scl = (AST_FUNC_DECL *) malloc( sizeof( AST_FUNC_DECL ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_FUN_DECL, yyloc );

  scl->f_name = f_name ? strdup( f_name ) : 0;
  
  assert( func_param->base.type == S_AST_VECTOR );
  func_param->base.parent = &scl->base;
  scl->func_params = func_param;

  return scl;
}


M_INLINE void AST_FUNC_DECL_set_body( AST_FUNC_DECL *scl, AST_BASE_LIST *body )
{
    assert( body->base.type == S_AST_LIST );
    body->base.parent = &scl->base;
    scl->func_body = body;
}

/***************************************************/
typedef struct tagAST_FUNC_CALL {
  
  AST_BASE base;

  AST_FUNC_DECL *func_decl;
  
  const char *f_name;
  AST_VECTOR *call_params;

} AST_FUNC_CALL;



M_INLINE AST_FUNC_CALL * AST_FUNC_CALL_init(const char *f_name, AST_VECTOR *func_params )
{  
  AST_FUNC_CALL *scl;

  scl = (AST_FUNC_CALL *) malloc( sizeof( AST_FUNC_CALL ) );
  if (!scl) {
    return 0;
  }
  AST_BASE_init( &scl->base, S_FUN_CALL, yyloc );

  scl->f_name = strdup( f_name );
  
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
  

M_INLINE AST_RETURN *AST_RETURN_init( S_TYPE type, AST_EXPRESSION *expr )
{
  AST_RETURN *ret;

  ret = (AST_RETURN *) malloc( sizeof( AST_RETURN ) );
  if (!ret) {
    return 0;
  }
  AST_BASE_init( &ret->base, type, yyloc );
  
  if (expr) {
    assert(expr->base.type == S_EXPRESSION );
    expr->base.parent = &ret->base;
  }
  ret->rvalue = expr;

  return ret;
}



#endif


  









