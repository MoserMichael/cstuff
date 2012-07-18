%locations
%debug
%pure-parser
%parse-param { PARSECONTEXT *parse_context }
%lex-param { LEX_PARAMS }


%{

#define TOKEN_END_OF_INPUT 0

void yyerror ( YYLTYPE *loc, PARSECONTEXT *parse_context, char const *);

#ifdef IS_REENTRANT
  
  extern int yylex (YYSTYPE * yylval_param, YYLTYPE * yylloc_param, yyscan_t yyscanner);
  
  #define YYLEX_PARAM  parse_context->lexctx.yyscanner
  #define YYLOCPTR &yyloc
#else 
  #define YYLOCPTR &yyloc
#endif


%} 

/*
%left  TK_WHILE TK_UNTIL TK_FOR TK_IF TK_DO TK_ASSIGN TK_ASSIGN_REF TK_ELSE TK_ELSIF TK_END TK_RETURN TK_BREAK TK_CONTINUE TK_INTEGER_CONSTANT TK_STRING_CONSTANT TK_MULTI_PART_STRING_CONSTANT TK_DOUBLE_CONSTANT TK_GOTO TK_SUB TK_INCLUDE

%token TK_OP_NUM_EQ TK_OP_NUM_NE TK_OP_NUM_LT TK_OP_NUM_GT TK_OP_NUM_LE TK_OP_NUM_GE TK_OK_DOT

%token TK_OP_STR_EQ TK_OP_STR_NE TK_OP_STR_LT TK_OP_STR_GT TK_OP_STR_LE TK_OP_STR_GE

%token TK_OP_STR_CAT

%token TK_OP_NUM_ADD TK_OP_NUM_SUBST TK_OP_NUM_DIV TK_OP_NUM_MULT TK_OP_NUM_MOD  

%token TK_OP_NUM_AUTOINCR TK_OP_NUM_AUTODECR

%token TK_OP_TOSTR TK_OP_TOINT

%token TK_VAR_DEF TK_VAR_UNDEF TK_ARR_DEF TK_CODEREF_DEF TK_IDENTIFIER

%token TK_OP_STR_REGEXMATCH TK_HASH_IT  

%token TK_COLON TK_SEMICOLON TK_COMMA TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE TK_BRACE_OPEN TK_BRACE_CLOSE  TK_BRACKET_OPEN  TK_BRACKET_CLOSE TK_CLASS TK_INTERFACE TK_NEW

%token TK_START_STATEMENT TK_START_EXPRESSION 
*/

%token TK_ERROR

%right TK_WHILE TK_FOR TK_IF TK_LOOP TK_ASSIGN TK_ASSIGN_REF TK_ELSE TK_ELSIF TK_END TK_RETURN TK_BREAK TK_NEXT TK_IDENTIFIER TK_INTEGER_CONSTANT TK_STRING_CONSTANT TK_MULTI_PART_STRING_CONSTANT TK_DOUBLE_CONSTANT TK_NIL TK_FALSE TK_TRUE TK_EMPTY_STRING TK_SUB TK_INCLUDE 


%token TK_OP_LOGICAL_AND TK_OP_LOGICAL_OR

%token TK_OP_NUM_EQ TK_OP_NUM_NE TK_OP_NUM_LT TK_OP_NUM_GT TK_OP_NUM_LE TK_OP_NUM_GE TK_OP_DOT

%token TK_OP_STR_EQ TK_OP_STR_NE TK_OP_STR_LT TK_OP_STR_GT TK_OP_STR_LE TK_OP_STR_GE

%token TK_OP_STR_CAT TK_HASH_IT

%token TK_OP_NUM_ADD TK_OP_NUM_SUBST TK_OP_NUM_DIV TK_OP_NUM_MULT TK_OP_NUM_MOD  

%token TK_OP_LOGICAL_NEGATE 

%token TK_VAR_DEF TK_VAR_UNDEF TK_ARR_DEF TK_CODEREF_DEF 

%token TK_OP_STR_REGEXMATCH 

%token TK_COLON TK_SEMICOLON TK_COMMA TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE TK_BRACE_OPEN TK_BRACE_CLOSE  

%token TK_BYREF TK_OPTIONAL TK_THREE_DOTS 

%left TK_BRACKET_OPEN  TK_BRACKET_CLOSE TK_UNDERSCORE  
 

%token TK_START_STATEMENT TK_START_EXPRESSION TK_PARAMETER_LABEL 

%token TK_END_OF_FILE 0 "end of file"
%right TK_END_OF_FILE

/*
%token TK_ERROR

%token TK_WHILE TK_FOR TK_IF TK_DO TK_ASSIGN TK_ASSIGN_REF TK_ELSE TK_ELSIF TK_END TK_RETURN TK_BREAK TK_CONTINUE TK_INCLUDE 

%token TK_SUB

%token TK_IDENTIFIER TK_INTEGER_CONSTANT TK_STRING_CONSTANT TK_MULTI_PART_STRING_CONSTANT TK_DOUBLE_CONSTANT 

%token TK_OP_NUM_EQ TK_OP_NUM_NE TK_OP_NUM_LT TK_OP_NUM_GT TK_OP_NUM_LE TK_OP_NUM_GE

%token TK_OP_STR_EQ TK_OP_STR_NE TK_OP_STR_LT TK_OP_STR_GT TK_OP_STR_LE TK_OP_STR_GE

%token TK_OP_STR_CAT TK_HASH_IT

%token TK_OP_NUM_ADD TK_OP_NUM_SUBST TK_OP_NUM_DIV TK_OP_NUM_MULT TK_OP_NUM_MOD  

%token TK_OP_NUM_AUTOINCR TK_OP_NUM_AUTODECR

%token TK_OP_STR_REGEXMATCH 

%token TK_COLON TK_COMMA TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE TK_BRACE_OPEN TK_BRACE_CLOSE  TK_BRACKET_OPEN  TK_BRACKET_CLOSE TK_UNDERSCORE  

%token TK_START_STATEMENT TK_START_EXPRESSION 
*/

%%

prog : 
	TK_START_STATEMENT stmtList	
     	    {
	       AST_FUNC_DECL *scl;

	       // global scope is a function (so that it has context for looking up globals and function names.)
	       scl = parse_context->root_ctx;
	       AST_FUNC_DECL_set_body( scl, parse_context, (AST_BASE_LIST *) $<ast>2 );

	       parse_context->my_ast_root = &scl->base;
	    }
	    |
	TK_START_EXPRESSION expr 
	    {
		parse_context->my_ast_root = $<ast>2;
	    }
	    |
	TK_START_STATEMENT 
	    {
		parse_context->my_ast_root = 0;
	    }
	 ;

stmtList : stmtList stmt	
	    {
		AST_BASE_LIST *slist;

		slist = (AST_BASE_LIST *) $<ast>1;
	        AST_BASE_LIST_add( slist, $<ast>2 );
	        $<ast>$ = &slist->base;
		
		AST_BASE_set_location( $<ast>$, YYLOCPTR );  
	    }
	 | stmt 
	    {
		AST_BASE_LIST *slist;

		slist = AST_BASE_LIST_init( YYLOCPTR );
                AST_BASE_LIST_add( slist, $<ast>1 );
	        $<ast>$ = &slist->base;
	    }
	 ;


stmt : valueDeepCopyAssignStmt
     | referenceCopyAssignmentStmt
     | includeStmt
     | functionCallStmt
     | functionDefStmt
     | ifStmt			
     | forStmt
     | whileStmt
     | doWhileStmt
     | loopStmt
     | returnStmt
     | breakStmt
     | nextStmt
     | TK_END 
     {
	  do_yyerror( &yylloc, parse_context, "too many end keywords - this one does not belong to any statement. previous statement all closed with end" );
	  return -1;
     }
     ;     


stmtInnerList : stmtInnerList stmtInner
	    {
		AST_BASE_LIST *slist;

		slist = (AST_BASE_LIST *) $<ast>1;

		if ($<ast>2 != 0) {
		  AST_BASE_LIST_add( slist, $<ast>2 );
	        }
	        $<ast>$ = &slist->base;

		AST_BASE_set_location( $<ast>$, YYLOCPTR );  
	    }
	 | stmtInner 
	    {
		AST_BASE_LIST *slist;

		slist = AST_BASE_LIST_init( YYLOCPTR );
		if ($<ast>1 != 0) {
                  AST_BASE_LIST_add( slist, $<ast>1 );
		}
	        $<ast>$ = &slist->base;
	    }
	 ;

stmtInner : valueDeepCopyAssignStmt
     | referenceCopyAssignmentStmt
     | includeStmt
     | functionCallStmt
     | ifStmt			
     | forStmt
     | whileStmt
     | doWhileStmt
     | loopStmt
     | returnStmt
     | breakStmt
     | nextStmt
     | TK_SUB 
	{
	  do_yyerror( &yylloc, parse_context,  "some previous statement is not closed with end keyword" );
	  $<ast>$ = 0;
	  parse_context->stmt_not_closed = 1;
	
	  yychar = YYEMPTY;
	  LEXER_set_next_token( &parse_context->lexctx , TK_END );
	}
     | TK_END_OF_FILE
 	{

	//do_yyerror( &yylloc, parse_context,  "some previous statement is not closed with end keyword" );
	  $<ast>$ = 0;
	  parse_context->stmt_not_closed = 1;

	  //unput(TK_END_OF_FILE);
	  yychar = YYEMPTY;
	  LEXER_set_next_token( &parse_context->lexctx , TK_END );
  	}
      ;     



forStmt : TK_FOR TK_IDENTIFIER expr stmtInnerList TK_END
	{
	    AST_FOR_LOOP *scl;
	    AST_EXPRESSION *loopID;

	    loopID = AST_EXPRESSION_init_ref( $<string_value>2, 0, YYLOCPTR );

            scl = AST_FOR_LOOP_init( loopID, (AST_EXPRESSION *) $<ast>3, (AST_BASE_LIST *) $<ast>4, YYLOCPTR );
	    $<ast>$ = &scl->base;

	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "for loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }
	    
	}
		|
	 TK_FOR multiValueLeftHandSide expr stmtInnerList TK_END
  	{
	    AST_FOR_LOOP *scl;

            scl = AST_FOR_LOOP_init( (AST_EXPRESSION *) $<ast>2, (AST_EXPRESSION *) $<ast>3, (AST_BASE_LIST *) $<ast>4, YYLOCPTR );
	    $<ast>$ = &scl->base;
	    
	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "for loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }

	} 
		|
	 TK_FOR multiValueLeftHandSide expr TK_END
  	{
	    AST_FOR_LOOP *scl;

	    do_yywarning( &yylloc, parse_context,  "empty body of for statement" );

            scl = AST_FOR_LOOP_init( (AST_EXPRESSION *) $<ast>2, (AST_EXPRESSION *) $<ast>3, 0 , YYLOCPTR );
	    $<ast>$ = &scl->base;
	} 
		|
	 TK_FOR TK_IDENTIFIER expr TK_END
	{
	    AST_FOR_LOOP *scl;
	    AST_EXPRESSION *loopID;

	    do_yywarning( &yylloc, parse_context,  "empty body of for statement" );

	    loopID = AST_EXPRESSION_init_ref( $<string_value>2, 0, YYLOCPTR );

            scl = AST_FOR_LOOP_init( loopID, (AST_EXPRESSION *) $<ast>3, (AST_BASE_LIST *) 0, YYLOCPTR );
	    $<ast>$ = &scl->base;
	}
  ;     



whileStmt : TK_WHILE expr stmtInnerList TK_END
	{
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>2, (AST_BASE_LIST *) $<ast>3, LOOP_PRECOND_WHILE, YYLOCPTR  );
	    $<ast>$ = &scl->base;

	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "while loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }

	}
	    |
	    TK_WHILE expr TK_END
	{

	    do_yywarning( &yylloc, parse_context,  "empty body of while statement" );
#if 1	    
	    return -1;
#else	    
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>2, 0, LOOP_PRECOND_WHILE, YYLOCPTR  );
	    $<ast>$ = &scl->base;
#endif	    
	
	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "while loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }
	
	}
	    ;


doWhileStmt : TK_LOOP stmtInnerList TK_WHILE expr
	{
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>4, (AST_BASE_LIST *) $<ast>2, LOOP_POSTCOND_WHILE, YYLOCPTR  );
	    $<ast>$ = &scl->base;

	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }

	}  | 

	TK_LOOP  TK_WHILE expr
	{
	    do_yywarning( &yylloc, parse_context,  "empty body of loop statement" );
	    
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>3, 0, LOOP_POSTCOND_WHILE, YYLOCPTR  );
	    $<ast>$ = &scl->base;

	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }

	}
	    ;
  
loopStmt : TK_LOOP stmtInnerList TK_END
	{
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( 0, (AST_BASE_LIST *) $<ast>2, LOOP_INFINITE, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	    
	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }
	}   |
	    TK_LOOP TK_END
	{
	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "loop not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }
	    do_yyerror( &yylloc, parse_context,  "Infinite loop - empty body of loop statement" );
	    
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( 0, (AST_BASE_LIST *) 0, LOOP_INFINITE, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	}
	
	    ;

valueDeepCopyAssignStmt : assignmentLeftHandSide TK_ASSIGN expr
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( CP_VALUE, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	}  |
	    assignmentLeftHandSide TK_ASSIGN error
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( CP_VALUE, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) 0, YYLOCPTR  );
	    $<ast>$ = &scl->base;


	    do_yyerror( & @2, parse_context,  "in assignment; right hand side (value of assignment) is wrong" );
	} 	
	;

referenceCopyAssignmentStmt : assignmentLeftHandSide TK_ASSIGN_REF  expr
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( CP_REF, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	}  | 
	    assignmentLeftHandSide TK_ASSIGN_REF  error
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( CP_REF, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3, YYLOCPTR  );
	    $<ast>$ = &scl->base;

	    do_yyerror( & @2, parse_context,  "in assignment; right hand side (value of assignment) is wrong" );
	} 	
	;

assignmentLeftHandSide : varRef 
		|   
	multiValueLeftHandSide
	    ;

multiValueLeftHandSide : TK_BRACKET_OPEN multiValueLeftHandSideList TK_BRACKET_CLOSE
	{
	  AST_EXPRESSION *scl;
	  
	  scl = AST_EXPRESSION_init( S_EXPR_LIST_VALUES, S_VAR_LIST, YYLOCPTR   ); 

	  scl->val.index_expressions = (AST_VECTOR *) $<ast>2;
	  $<ast>$ = &scl->base;
	}	|
	     TK_BRACKET_OPEN multiValueLeftHandSideList error
	{
	  AST_EXPRESSION *scl;

	  do_yyerror( & @2, parse_context, "] expected after sequence" );
	  
	  scl = AST_EXPRESSION_init( S_EXPR_LIST_VALUES, S_VAR_LIST, YYLOCPTR   ); 

	  scl->val.index_expressions = (AST_VECTOR *) $<ast>2;
	  $<ast>$ = &scl->base;
	}
	
	    ;

	    

multiValueLeftHandSideList :  multiValueLeftHandSideList TK_COMMA multiValueLeftHandSideClause
	    {
	  AST_VECTOR *sval;

	  sval = (AST_VECTOR *) $<ast>1;
	  AST_VECTOR_add( sval, $<ast>3 );
	  
	  $<ast>$ = &sval->base;

	  AST_BASE_set_location( $<ast>$, YYLOCPTR );  

	}
		|
	multiValueLeftHandSideClause
	{
	  AST_VECTOR *sval;
	  
	  sval = AST_VECTOR_init( YYLOCPTR );
	  AST_VECTOR_add( sval, $<ast>1 );

	  $<ast>$ = &sval->base;
	
	
	}
;

multiValueLeftHandSideClause : varRef
		|
	TK_UNDERSCORE 
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_PLACEHOLDER, S_VAR_ANY, YYLOCPTR  ); 
	  $<ast>$ = &scl->base;
	    }
	;

ifStmt  : TK_IF condClause elseClauses TK_END
	{
	    if ( $<ast>3 != 0 ) {
 	      AST_COND_set_else( (AST_COND *) $<ast>2, (AST_COND *) $<ast>3 );
	    }
	    $<ast>$ = $<ast>2;

	    AST_BASE_set_location( $<ast>$, YYLOCPTR );  

	    if (parse_context->stmt_not_closed) {
	      do_yyerror( & @1 , parse_context,  "if statement not closed with end keyword" );
	      parse_context->stmt_not_closed = 0;
	    }
	}	;

elseClauses : elsifClause TK_ELSE  conditionContent
	{  
	    if ($<ast>1 != 0) {
	      $<ast>$ = (AST_BASE *) AST_COND_set_else_block( (AST_COND *) $<ast>1, (AST_BASE_LIST *) $<ast>3 );
            } else {
	      $<ast>$ = (AST_BASE *) AST_COND_init( 0, (AST_BASE_LIST *) $<ast>3, YYLOCPTR );
	    }
	    AST_BASE_set_location( $<ast>$, YYLOCPTR );  
	}
	    | elsifClause 
	    ;

elsifClause : elsifClause TK_ELSIF condClause
	{
	    if ($<ast>1 == 0) {
	      $<ast>$ = $<ast>3;
	    } else {
	      AST_COND_set_else( (AST_COND *) $<ast>1, (AST_COND *) $<ast>3 );
	      $<ast>$ = $<ast>1;
	    }
	    AST_BASE_set_location( $<ast>$, YYLOCPTR );  
	}
	    |
	{
	    $<ast>$ = 0;	    
	}
	    ;

condClause  : expr conditionContent 	
	{
	  AST_COND *scl;

	  scl = AST_COND_init( (AST_EXPRESSION *) $<ast>1, (AST_BASE_LIST *) $<ast>2, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
	    ;

conditionContent : stmtInnerList
		|
	{
	    do_yywarning( &yylloc, parse_context,  "empty body of condition statement" );
            AST_BASE_LIST *slist;

	    slist = AST_BASE_LIST_init( YYLOCPTR );
            $<ast>$ = &slist->base;
	}
	    ;
		
	                                    

includeStmt : TK_INCLUDE TK_STRING_CONSTANT
	{
		int ret = LEXER_scan_file( &parse_context->lexctx, 0 );
	        free( $<string_value>2 );
		if (ret < 0) {
		  do_yyerror( &yylloc, parse_context,  "Can't open include file %s", $<string_value>2 );
		  return -1;
		}
		$<ast>$ = 0;
	}
	;


functionDefStmt  :  functionPrototypeDecl stmtInnerList TK_END  
	{
	  AST_FUNC_DECL *scl;

	  scl = (AST_FUNC_DECL *) $<ast>1;

	  AST_FUNC_DECL_set_body( scl, parse_context, (AST_BASE_LIST *) $<ast>2 );

	  $<ast>$ = &scl->base;

	  if (parse_context->stmt_not_closed) {
	    do_yyerror( & @1 , parse_context,  "function definition not closed with end keyword" );
	    parse_context->stmt_not_closed = 0;
	  }

	}
	    |
		 functionPrototypeDecl TK_END  
	{
	  do_yywarning( &yylloc, parse_context,  "empty body of function definition" );
	
	  AST_FUNC_DECL *scl;

	  scl = (AST_FUNC_DECL *) $<ast>1;

	  AST_FUNC_DECL_set_body( scl, parse_context, 0 );

	  $<ast>$ = &scl->base;
	}

	;

	

functionPrototypeDecl : TK_SUB TK_IDENTIFIER TK_PARENTHESES_OPEN funcPDecl TK_PARENTHESES_CLOSE 
	{
	  AST_FUNC_DECL *scl;
	  
          if ($<ast>4 == (AST_BASE *) -1) {
	    scl = AST_FUNC_DECL_init( $<string_value>2, 0, parse_context, YYLOCPTR );
	    scl->var_arguments = 1;
         } else {
	    scl = AST_FUNC_DECL_init( $<string_value>2, (AST_VECTOR *) $<ast>4, parse_context, YYLOCPTR );
          }
          $<ast>$ = &scl->base;
	}
		| TK_SUB TK_IDENTIFIER error 
	{
	   do_yyerror( &yylloc, parse_context,  "In a function definition the ( sign should be right after the function name" );
           return -1;
        }
		;

funcPDecl : funcParamDecls 
        | 
            TK_THREE_DOTS 
        { 
            $<ast>$ = (AST_BASE *) -1; 
        }
            ;


funcParamDecls : funcParamDecls  TK_COMMA funcParamDecl 
	{
	  AST_VECTOR *sval;

	  sval = (AST_VECTOR *) $<ast>1;
	  
	  AST_VECTOR_add( sval, $<ast>3 );

	  $<ast>$ = &sval->base;

	  AST_BASE_set_location( $<ast>$, YYLOCPTR );  
	}
	    | funcParamDecl
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR );

	  AST_VECTOR_add( scl, $<ast>1 );

	  $<ast>$ = &scl->base;
	}
            |
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR );

	  $<ast>$ = &scl->base;
	}
	;

funcParamDecl : TK_IDENTIFIER optParamSpecs2 {
	  AST_EXPRESSION *expr;

	  expr = AST_EXPRESSION_init_ref( $<string_value>1, 0, YYLOCPTR );
          
          //expr->var_type |= $<int_value>2;

	  $<ast>$ = &expr->base;
	}
	;

optParamSpecs2 : optParamSpecs
        {
            $<int_value>$ = $<int_value>1;
        }
            |
        {
            $<int_value>$ = 0;
        }
        ;


optParamSpecs : optParamSpecs optParamSpec
	    {
		$<int_value>$ = $<int_value>1 | $<int_value>2;
	    }
        |
            optParamSpec
            {
                $<int_value>$ = $<int_value>1;
            }
	;

optParamSpec : TK_BYREF
	    {
		$<int_value>$ = S_VAR_PARAM_BYREF;
	    }
        | TK_OPTIONAL
	    {
		$<int_value>$ = S_VAR_PARAM_OPTIONAL;
	    }
         ;


breakStmt : TK_BREAK
	{
	    AST_BASE *scl;

	    scl = (AST_BASE *) malloc( sizeof( AST_BASE ) );
	    AST_BASE_init( scl, S_BREAK,  YYLOCPTR  );
	    $<ast>$ = scl;
	}
	;

nextStmt : TK_NEXT
	{
	    AST_BASE *scl;

	    scl = (AST_BASE *) malloc( sizeof( AST_BASE ) );
	    AST_BASE_init( scl, S_NEXT,  YYLOCPTR );
	    $<ast>$ = scl;
	}
	 ;

returnStmt : TK_RETURN expr
	{
	  AST_RETURN *scl;

	  scl = AST_RETURN_init( S_RETURN, (AST_EXPRESSION *) $<ast>2, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
	   | TK_RETURN
	{
	  AST_RETURN *scl;

	  scl = AST_RETURN_init( S_RETURN, 0, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
	   ;


/* *** expression clause **** */

expr : logicalExp
     | hashConstructor
     | listConstructor
     | anonymousFunction
     ;   

anonymousFunction : TK_SUB TK_PARENTHESES_OPEN funcParamDecls TK_PARENTHESES_CLOSE stmtInnerList TK_END
	{
	  AST_EXPRESSION *scl; 
	  AST_FUNC_DECL *fdecl;
	  
	  fdecl = AST_FUNC_DECL_init( 0,  (AST_VECTOR *) $<ast>3, parse_context, YYLOCPTR);

	  AST_FUNC_DECL_set_body( fdecl, parse_context, (AST_BASE_LIST *) $<ast>5 );
	  scl = AST_EXPRESSION_init( S_EXPR_LAMBDA, S_VAR_CODE, YYLOCPTR);
	  scl->val.fdecl =  (AST_BASE *) fdecl;

	  $<ast>$ = &scl->base;

	  if (parse_context->stmt_not_closed) {
	    do_yyerror( & @1 , parse_context,  "function definition not closed with end keyword" );
	    parse_context->stmt_not_closed = 0;
	  }
  
	} 
	    |
	TK_SUB TK_PARENTHESES_OPEN funcParamDecls TK_PARENTHESES_CLOSE TK_END
	{
	    do_yywarning( &yylloc, parse_context,  "empty body of anonymous function (function without a name)" );
	}
	;

logicalExp : logicalExp logExpOp compExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	  $<ast>$ = &scl->base;
	}
	| compExp
	;

logExpOp : TK_OP_LOGICAL_AND { $<int_value>$ = TK_OP_LOGICAL_AND; }  
	 | TK_OP_LOGICAL_OR  { $<int_value>$ = TK_OP_LOGICAL_OR; }  
	 ;


compExp : compExp compExpOp addExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	  $<ast>$ = &scl->base;
	}
	| addExp
	;

compExpOp : TK_OP_NUM_EQ { $<int_value>$ = TK_OP_NUM_EQ; }  
	  | TK_OP_NUM_NE { $<int_value>$ = TK_OP_NUM_NE; }  
	  | TK_OP_NUM_LT { $<int_value>$ = TK_OP_NUM_LT; }  
	  | TK_OP_NUM_GT { $<int_value>$ = TK_OP_NUM_GT; }  
	  | TK_OP_NUM_LE { $<int_value>$ = TK_OP_NUM_LE; }  
	  | TK_OP_NUM_GE { $<int_value>$ = TK_OP_NUM_GE; }  
  	  | TK_OP_STR_EQ { $<int_value>$ = TK_OP_STR_EQ; }  
	  | TK_OP_STR_NE { $<int_value>$ = TK_OP_STR_NE; }  
	  | TK_OP_STR_LT { $<int_value>$ = TK_OP_STR_LT; }  
	  | TK_OP_STR_GT { $<int_value>$ = TK_OP_STR_GT; }  
	  | TK_OP_STR_LE { $<int_value>$ = TK_OP_STR_LE; }  
	  | TK_OP_STR_GE { $<int_value>$ = TK_OP_STR_GE; }               
	  ;

addExp  : addExp addExpOp multExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	  $<ast>$ = &scl->base;
	}
	| multExp
	;

addExpOp : TK_OP_NUM_SUBST { $<int_value>$ = TK_OP_NUM_SUBST; }  
	 | TK_OP_STR_CAT   { $<int_value>$ = TK_OP_STR_CAT; }  
	 | TK_OP_NUM_ADD   { $<int_value>$ =  TK_OP_NUM_ADD; }  
	 ; 

multExp : multExp multExpOp unaryExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	  $<ast>$ = &scl->base;
	}
	| unaryExp
	;

multExpOp : TK_OP_NUM_DIV   { $<int_value>$ = TK_OP_NUM_DIV; } 
	  | TK_OP_NUM_MULT  { $<int_value>$ = TK_OP_NUM_MULT; } 
	  | TK_OP_NUM_MOD   { $<int_value>$ = TK_OP_NUM_MOD; } 
	  ;

unaryExp :
	  unaryExpOpPrefix primaryExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_unary( $<int_value>1, (AST_EXPRESSION *) $<ast>2, 1, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
	 | primaryExp
	 ;

unaryExpOpPrefix : TK_OP_NUM_ADD { $<int_value>$ = TK_OP_NUM_ADD; } 
	| TK_OP_NUM_SUBST	 { $<int_value>$ = TK_OP_NUM_SUBST; } 
	| TK_OP_LOGICAL_NEGATE	 { $<int_value>$ = TK_OP_LOGICAL_NEGATE; } 
  
	;


primaryExp : varRef
	   | TK_INTEGER_CONSTANT
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_INT, YYLOCPTR ); 
	  scl->val.const_value.long_value = $<long_value>1;
	  $<ast>$ = &scl->base;
	}

	   | TK_DOUBLE_CONSTANT
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_DOUBLE, YYLOCPTR ); 
	  scl->val.const_value.double_value = $<double_value>1;
	  $<ast>$ = &scl->base;
	}	
	   | TK_MULTI_PART_STRING_CONSTANT
	{
	  $<ast>$ = (AST_BASE *) AST_compile_multi_part_string( parse_context );
	  LEXER_clean_string_parts( &parse_context->lexctx );
	}
	   | TK_STRING_CONSTANT
	{
	  AST_EXPRESSION *scl;
	  STRING_PART **cur;
	  const char *sval;

	  cur = (STRING_PART **) ARRAY_at( &parse_context->lexctx.string_parts, 0 );
	  sval = (const char *) (*cur)->part_data.buf;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_STRING, & (*cur)->loc ); 
	  scl->val.const_value.string_value = strdup( sval );
	  $<ast>$ = &scl->base;

	  LEXER_clean_string_parts( &parse_context->lexctx );
	}	
           | TK_NIL 
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_NULL, YYLOCPTR ); 
	  $<ast>$ = &scl->base;
	}	
           | TK_FALSE
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_INT, YYLOCPTR ); 
	  scl->val.const_value.long_value = 0;
	  $<ast>$ = &scl->base;
	}
          | TK_TRUE
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_INT, YYLOCPTR ); 
	  scl->val.const_value.long_value = 1;
	  $<ast>$ = &scl->base;
	}
	   | TK_EMPTY_STRING
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_STRING, YYLOCPTR ); 
	  scl->val.const_value.string_value = strdup( "" );
	  $<ast>$ = &scl->base;
	}	
          | TK_PARENTHESES_OPEN expr TK_PARENTHESES_CLOSE
	{
	  $<ast>$ = $<ast>2;
	}
	   | functionCallExpr   

	   | TK_PARENTHESES_OPEN expr error
	{
	   do_yyerror( & @2, parse_context,  "Closing ) expected after expression" );
	   $<ast>$ = $<ast>2;
	   YYERROR
	}

	   ;


functionCallExpr : functionCallStmt 
	{
	  AST_EXPRESSION *scl;
	  
	  scl = AST_EXPRESSION_fcall_init( (AST_FUNC_CALL *) $<ast>1 , YYLOCPTR );

	  $<ast>$ = &scl->base;
	}

		
varRef : TK_IDENTIFIER varIndexes
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_ref( $<string_value>1, (AST_VECTOR *) $<ast>2, YYLOCPTR );

	  $<ast>$ = &scl->base;
	}	
	    | 
	TK_IDENTIFIER 	
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_ref( $<string_value>1, 0, YYLOCPTR );

	  $<ast>$ = &scl->base;
	}
	;

varIndexes : varIndexes varIndex 
	{
	  AST_VECTOR *scl;

          scl = (AST_VECTOR *) $<ast>1;

	  if ($<ast>2 != 0) {
	    AST_VECTOR_add( scl, $<ast>2 );
	  }

	  $<ast>$ = &scl->base;

	  AST_BASE_set_location( $<ast>$, YYLOCPTR );  

	}
		|
	    varIndex 
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR );
	  AST_VECTOR_add( scl, $<ast>1 );

	  $<ast>$ = &scl->base;
	}
	    ;

varIndex : 	    
	   TK_BRACE_OPEN expr TK_BRACE_CLOSE
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_HASH_INDEX, S_VAR_ANY, YYLOCPTR   ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;
	}	
	   | TK_BRACE_OPEN expr error
	{
	  AST_EXPRESSION *scl;

          do_yyerror( & @2, parse_context,  "Closing } expected after hash index" );
	  
	  scl = AST_EXPRESSION_init( S_EXPR_HASH_INDEX, S_VAR_ANY, YYLOCPTR   ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;

	  YYERROR;
	}
	
	   |  TK_BRACKET_OPEN expr TK_BRACKET_CLOSE
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_ARRAY_INDEX, S_VAR_INT, YYLOCPTR  ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;
        }	
	   |  TK_BRACKET_OPEN expr error
       	{
	  AST_EXPRESSION *scl;

          do_yyerror( & @2, parse_context,  "Closing ] expected after array index" );

	  scl = AST_EXPRESSION_init( S_EXPR_ARRAY_INDEX, S_VAR_INT, YYLOCPTR  ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;

	  YYERROR;
        }	

	     | TK_OP_DOT  TK_IDENTIFIER
       	{
	  AST_EXPRESSION *index;
	  AST_EXPRESSION *strconst;

	  strconst = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_STRING, YYLOCPTR   ); 
	  strconst->val.const_value.string_value = strdup( $<string_value>2 );

	  index = AST_EXPRESSION_init( S_EXPR_HASH_INDEX, S_VAR_ANY, YYLOCPTR  ); 
	  index->val.index_expr = (AST_EXPRESSION *) strconst ;
	  
	  $<ast>$ = &index->base;
 	}
	     | TK_OP_DOT  error
       	{
          do_yyerror( & @1, parse_context,  "Identifier expected after dot" );

	  $<ast>$ = 0;
 	}



	;

/** function calls **/

functionCallStmt : functionCallLhs TK_PARENTHESES_OPEN functionArgs TK_PARENTHESES_CLOSE        	{
	  AST_FUNC_CALL *fcall;

	  fcall = AST_FUNC_CALL_init( (AST_EXPRESSION *) $<ast>1, (AST_VECTOR *) $<ast>3, YYLOCPTR  );

	  $<ast>$ = &fcall->base;
	}   	
	;

functionCallLhs : varRef | functionCallExpr
	    ;


functionArgs : functionArgList 
	|
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR  );
	  $<ast>$ = &scl->base;
	}
	;


functionArgList  : functionArgList functionParam 	
	{
	  AST_VECTOR *scl;
          scl = (AST_VECTOR *) $<ast>1;

	  AST_VECTOR_add( scl, $<ast>2 );

	  $<ast>$ = &scl->base;

	  AST_BASE_set_location( $<ast>$, YYLOCPTR );  
	}
		 | functionParam
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR );
	  AST_VECTOR_add( scl, $<ast>1 );
	 
	  $<ast>$ = &scl->base;
	}

/*	
		 |
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR  );
	  $<ast>$ = &scl->base;
	}
 */	
		  
		 ;

functionParam : TK_PARAMETER_LABEL expr
	{
	  AST_FUNC_CALL_PARAM *param;

	  param = AST_FUNC_CALL_PARAM_init( (AST_EXPRESSION *) $<ast>2, $<string_value>1, YYLOCPTR );

	  $<ast>$ = &param->base;
	}
		 | error
	{
            do_yyerror( YYLOCPTR, parse_context,  "Missing parameter label in function call (should start with ~)" );
	    $<ast>$ = 0;
        }
		 | TK_PARAMETER_LABEL error 
	{
	     do_yyerror( &@2, parse_context,  "in function parameter expression" );
             $<ast>$ = 0;	
        }


	    ;

/** list data structure **/		 

listConstructor : TK_BRACKET_OPEN expressionList TK_BRACKET_CLOSE   
	{
	  AST_EXPRESSION *scl;
	  
	  scl = AST_EXPRESSION_init( S_EXPR_LIST_VALUES, S_VAR_LIST, YYLOCPTR );  //& $<ast>2->location ); 

	  scl->val.index_expressions = (AST_VECTOR *) $<ast>2;
	  $<ast>$ = &scl->base;
	}
		;

expressionList : expressionList TK_COMMA expr
	{
	  AST_VECTOR *sval;

	  sval = (AST_VECTOR *) $<ast>1;
	  AST_VECTOR_add( sval, $<ast>3 );
	  
	  $<ast>$ = &sval->base;
	    
	  AST_BASE_set_location( $<ast>$, YYLOCPTR );  
	}
	       | expr
	{
	  AST_VECTOR *sval;
	  
	  sval = AST_VECTOR_init( YYLOCPTR );
	  AST_VECTOR_add( sval, $<ast>1 );

	  $<ast>$ = &sval->base;
	}
	       |    
	{
	  AST_VECTOR *sval;
	  
	  sval = AST_VECTOR_init( YYLOCPTR );

	  $<ast>$ = &sval->base; 
	}
	       ;

hashConstructor : TK_BRACE_OPEN hashClauseList TK_BRACE_CLOSE
	{
	  AST_EXPRESSION *scl;
	  
	  scl = AST_EXPRESSION_init( S_EXPR_HASH_VALUES, S_VAR_HASH, YYLOCPTR ); 

	  scl->val.index_expressions = (AST_VECTOR *) $<ast>2;
	  
	  $<ast>$ = &scl->base;
	}
	       ;



hashClauseList : hashClauseList TK_COMMA hashClause
	{
	  AST_VECTOR *sval;

	  sval = (AST_VECTOR *)  $<ast>1;

	  AST_VECTOR_add( sval, $<ast>3 );

	  $<ast>$ = &sval->base;
	 
	  AST_BASE_set_location( $<ast>$, YYLOCPTR );  

	}
	       | hashClause
	{
	  AST_VECTOR *sval;
	  
	  sval = AST_VECTOR_init( YYLOCPTR );

	  AST_VECTOR_add( sval, $<ast>1 );

	  $<ast>$ = &sval->base;
	}
	       |
	{
	  AST_VECTOR *sval;
	  
	  sval = AST_VECTOR_init( YYLOCPTR );

	  $<ast>$ = &sval->base;
	}
      ;

/** hash data structure **/

hashClause : expr TK_HASH_IT expr
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( TK_HASH_IT, (AST_EXPRESSION *) $<ast>1 , (AST_EXPRESSION *) $<ast>3 );

	  $<ast>$ = &scl->base;
	}	
	   ;

%%
	

