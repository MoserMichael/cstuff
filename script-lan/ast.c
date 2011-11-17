#include "ast.h"
#include "pars.h"

int AST_EXPRESSION_check_types( AST_EXPRESSION *scl, int op,  AST_EXPRESSION *lhs, AST_EXPRESSION *rhs )
{
    M_UNUSED( scl );
    M_UNUSED( op );
    M_UNUSED( lhs );
    M_UNUSED( rhs );

#if 0
    if (lhs->value_type ==  S_VAR_ANY && rhs->value_type == S_VAR_ANY) {
      return 0;
    }

    if (lhs->value_type ==  S_VAR_CODE || rhs->value_type == S_VAR_CODE) {
       // can't mix lambda function with operator
       return -1;   
    }
    
    // if division - check that divisor is not constant with value 0
    if (op == TK_OP_NUM_DIV && rhs->exp_type == S_EXPR_CONSTANT) {
    
	if (rhs->value_type == S_VAR_INT && rhs->val.const_value.int_value == 0) {
	    // divide by zero error.
	}

	if (rhs->value_type == S_VAR_DOUBLE && rhs->val.const_value.double_value == 0) {
	    // divide by zero error.
	}
    }
 
    if (is_numeric_operator( op) ) {

	if (lhs->value_type != S_VAR_CODE && is_numeric_type( lhs->value_type ) ) {
	  // numeric operator expects numeric value
	}

	if (rhs->value_type != S_VAR_CODE && is_numeric_type( rhs->value_type ) ) {
	  // numeric operator expects numeric value
	}
    }
    if (is_string_operator( op) ) {

	if (lhs->value_type != S_VAR_CODE && is_string_tiype( lhs->value_type ) ) {
	  // numeric operator expects numeric value
	}

	if (rhs->value_type != S_VAR_CODE && is_numeric_type( rhs->value_type ) ) {
	  // numeric operator expects numeric value
	}
    }
#endif
    return 0;
}

int AST_EXPRESSION_fold_constants( AST_EXPRESSION *scl, int op,  AST_EXPRESSION *lhs, AST_EXPRESSION *rhs )
{
    M_UNUSED( scl );
    M_UNUSED( op );
    M_UNUSED( lhs );
    M_UNUSED( rhs );
#if 0
    if (lhs->exp_type != S_EXPR_CONSTANT || rhs->exp_type != S_EXPR_CONSTANT) {
      return -1;
    } 

    switch( op ) {
      case TK_ADD:
        break;
    }
#endif
    return 0;
   
}


