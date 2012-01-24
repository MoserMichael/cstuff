%locations
%debug
%pure-parser
%parse-param { PARSECONTEXT *parse_context }
%lex-param { LEX_PARAMS }


%{
void yyerror ( YYLTYPE *loc, PARSECONTEXT *parse_context, char const *);

#ifdef IS_REENTRANT
  
  extern int yylex (YYSTYPE * yylval_param, YYLTYPE * yylloc_param, yyscan_t yyscanner);
  
  #define YYLEX_PARAM  parse_context->lexctx.yyscanner
  #define YYLOCPTR &yylloc
#else 
  #define YYLOCPTR &yyloc
#endif

%} 

/*
%left  TK_WHILE TK_UNTIL TK_FOR TK_IF TK_DO TK_ASSIGN TK_ASSIGN_REF TK_ELSE TK_ELSIF TK_END TK_RETURN TK_BREAK TK_YIELD TK_CONTINUE TK_INTEGER_CONSTANT TK_STRING_CONSTANT TK_MULTI_PART_STRING_CONSTANT TK_DOUBLE_CONSTANT TK_GOTO TK_SUB TK_INCLUDE

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

%left TK_WHILE TK_FOR TK_IF TK_DO TK_ASSIGN TK_ASSIGN_REF TK_ELSE TK_ELSIF TK_END TK_RETURN TK_YIELD TK_BREAK TK_CONTINUE TK_IDENTIFIER TK_INTEGER_CONSTANT TK_STRING_CONSTANT TK_MULTI_PART_STRING_CONSTANT TK_DOUBLE_CONSTANT TK_SUB TK_INCLUDE 


%token TK_OP_LOGICAL_AND TK_OP_LOGICAL_OR

%token TK_OP_NUM_EQ TK_OP_NUM_NE TK_OP_NUM_LT TK_OP_NUM_GT TK_OP_NUM_LE TK_OP_NUM_GE TK_OK_DOT

%token TK_OP_STR_EQ TK_OP_STR_NE TK_OP_STR_LT TK_OP_STR_GT TK_OP_STR_LE TK_OP_STR_GE

%token TK_OP_STR_CAT TK_HASH_IT

%token TK_OP_NUM_ADD TK_OP_NUM_SUBST TK_OP_NUM_DIV TK_OP_NUM_MULT TK_OP_NUM_MOD  

%token TK_OP_LOGICAL_NEGATE 

%token TK_VAR_DEF TK_VAR_UNDEF TK_ARR_DEF TK_CODEREF_DEF 

%token TK_OP_STR_REGEXMATCH 

%token TK_COLON TK_SEMICOLON TK_COMMA TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE TK_BRACE_OPEN TK_BRACE_CLOSE  


%left TK_BRACKET_OPEN  TK_BRACKET_CLOSE TK_UNDERSCORE  
 

%token TK_START_STATEMENT TK_START_EXPRESSION 

/*
%token TK_ERROR

%token TK_WHILE TK_FOR TK_IF TK_DO TK_ASSIGN TK_ASSIGN_REF TK_ELSE TK_ELSIF TK_END TK_RETURN TK_YIELD TK_BREAK TK_CONTINUE TK_INCLUDE 

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
		parse_context->my_ast_root = $<ast>2;
	    }
	    |
	TK_START_EXPRESSION expr 
	    {
		parse_context->my_ast_root = $<ast>2;
	    }
	 ;

stmtList : stmtList stmt	
	    {
		AST_BASE_LIST *slist;

		slist = (AST_BASE_LIST *) $<ast>1;
	        AST_BASE_LIST_add( slist, $<ast>2 );
	        $<ast>$ = &slist->base;
	    }
	 | stmt 
	    {
		AST_BASE_LIST *slist;

		slist = AST_BASE_LIST_init( YYLOCPTR );
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
     | returnStmt
     | yieldStmt
     | breakStmt
     | continueStmt
     ;     


forStmt : TK_FOR TK_IDENTIFIER expr stmtList TK_END
	{
	    AST_FOR_LOOP *scl;

            scl = AST_FOR_LOOP_init( $<string_value>2, (AST_EXPRESSION *) $<ast>3, (AST_BASE_LIST *) $<ast>4, YYLOCPTR );
	    $<ast>$ = &scl->base;
	}
    ;     



whileStmt : TK_WHILE expr stmtList
	{
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>2, (AST_BASE_LIST *) $<ast>3, 0, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	}
    ;


doWhileStmt : TK_DO stmtList TK_WHILE expr
	{
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>4, (AST_BASE_LIST *) $<ast>2, 1, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	}
    ;




valueDeepCopyAssignStmt : assignmentLeftHandSide TK_ASSIGN expr 
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( ASSIGNMENT_REF_COPY, $<ast>1, (AST_EXPRESSION *) $<ast>3, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	}
	;

referenceCopyAssignmentStmt : assignmentLeftHandSide TK_ASSIGN_REF expr
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( ASSIGNMENT_DEEP_COPY, $<ast>1, (AST_EXPRESSION *) $<ast>3, YYLOCPTR  );
	    $<ast>$ = &scl->base;
	}
	;
assignmentLeftHandSide : varRef 
		|
	TK_BRACKET_OPEN multiValueLeftHandSideList TK_BRACKET_CLOSE
	{
	    $<ast>$ = $<ast>2;
	}
	    ;

multiValueLeftHandSideList :  multiValueLeftHandSideList TK_COMMA multiValueLeftHandSideClause
	{
	  AST_VECTOR *sval;

	  sval = (AST_VECTOR *) $<ast>1;
	  AST_VECTOR_add( sval, $<ast>3 );
	  
	  $<ast>$ = &sval->base;
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
	    $<ast>$ = $<ast>2;
    
	    if ($<ast>3 != 0) {

	      if ($<ast>3 -> type == S_IF) {	
	        AST_COND_set_else( (AST_COND *) $<ast>2, (AST_COND *) $<ast>3 );
              } else {
	        AST_COND_set_else_block( (AST_COND *) $<ast>2, (AST_BASE_LIST *) $<ast>3 );
	      }
	    }
	}	;

elseClauses : elsifClause TK_ELSE stmtList
	{
	  AST_COND *scl;

	  scl = (AST_COND *) $<ast>1;
	
	  if  ($<ast>1 == 0) {
	      $<ast>$ = &scl->base;
	  } else {
	      AST_COND_set_else_block( scl, (AST_BASE_LIST *) $<ast>3 );
	      $<ast>$ = $<ast>1;
	  }
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
	}
	    |
	{
	    $<ast>$ = 0;	    
	}
	    ;

condClause  : expr stmtList	
	{
	  AST_COND *scl;

	  scl = AST_COND_init( (AST_EXPRESSION *) $<ast>1, (AST_BASE_LIST *) $<ast>2, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
	    ;
	                                    

includeStmt : TK_INCLUDE TK_STRING_CONSTANT
	{
		int ret = LEXER_scan_file( &parse_context->lexctx, $<string_value>2 );
	        free( $<string_value>2 );
		if (ret < 0) {
		  do_yyerror( &yylloc, parse_context,  "Can't open include file %s", $<string_value>2 );
		}
		$<ast>$ = 0;
	}
	;


functionDefStmt  :  functionPrototypeDecl stmtList TK_END  
	{
	  AST_FUNC_DECL *scl;

	  scl = (AST_FUNC_DECL *) $<ast>1;

	  AST_FUNC_DECL_set_body( scl, parse_context, (AST_BASE_LIST *) $<ast>2 );

	  $<ast>$ = &scl->base;
	}
	;

functionPrototypeDecl : TK_SUB TK_IDENTIFIER TK_PARENTHESES_OPEN funcParamDecls TK_PARENTHESES_CLOSE 
	{
	  AST_FUNC_DECL *scl;
	  
	  scl = AST_FUNC_DECL_init( $<string_value>2,  (AST_VECTOR *) $<ast>4, parse_context, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
		     |  TK_SUB TK_IDENTIFIER  funcParamDecls 
	{
	  AST_FUNC_DECL *scl;
	  
	      scl = AST_FUNC_DECL_init( $<string_value>2,  (AST_VECTOR *) $<ast>3, parse_context, YYLOCPTR );
	      $<ast>$ = &scl->base;
	}
	;

funcParamDecls : funcParamDecls TK_COMMA TK_IDENTIFIER
	{
	  AST_VECTOR *sval;
	  AST_EXPRESSION *expr;

	  sval = (AST_VECTOR *) $<ast>1;
	  expr = AST_EXPRESSION_init_ref( $<string_value>3, 0, YYLOCPTR );
	  
	  AST_VECTOR_add( sval, &expr->base );

	  $<ast>$ = &sval->base;
	}
	    | TK_IDENTIFIER
	{
	  AST_VECTOR *scl;
	  AST_EXPRESSION *expr;

	  scl = AST_VECTOR_init( YYLOCPTR );
	  expr = AST_EXPRESSION_init_ref( $<string_value>1, 0, YYLOCPTR );
	  AST_VECTOR_add( scl, &expr->base );

	  $<ast>$ = &scl->base;
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

continueStmt : TK_CONTINUE
	{
	    AST_BASE *scl;

	    scl = (AST_BASE *) malloc( sizeof( AST_BASE ) );
	    AST_BASE_init( scl, S_CONTINUE,  YYLOCPTR );
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

yieldStmt  : TK_YIELD expr
	{
	  AST_RETURN *scl;

	  scl = AST_RETURN_init( S_YIELD, (AST_EXPRESSION *) $<ast>2, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
	   ;

/* *** expression clause **** */

expr : logicalExp
     | hashConstructor
     | listConstructor
     | anonymousFunction
     ;   

anonymousFunction : TK_SUB funcParamDecls stmtList TK_END
	{
	  AST_EXPRESSION *scl; 
	  AST_FUNC_DECL *fdecl;
	  
	  fdecl = AST_FUNC_DECL_init( 0,  (AST_VECTOR *) $<ast>2, parse_context, YYLOCPTR);

	  AST_FUNC_DECL_set_body( fdecl, parse_context, (AST_BASE_LIST *) $<ast>3 );
	  scl = AST_EXPRESSION_init_ref( 0, 0, YYLOCPTR);

	  scl->value_type = S_VAR_CODE;
	  scl->val.fdecl = fdecl;

	  $<ast>$ = &scl->base;
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

logExpOp : TK_OP_LOGICAL_AND
	 | TK_OP_LOGICAL_OR
	 ;


compExp : compExp compExpOp addExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	  $<ast>$ = &scl->base;
	}
	| addExp
	;

compExpOp : TK_OP_NUM_EQ 
	  | TK_OP_NUM_NE 
	  | TK_OP_NUM_LT 
	  | TK_OP_NUM_GT 
	  | TK_OP_NUM_LE 
	  | TK_OP_NUM_GE 
  	  | TK_OP_STR_EQ 
	  | TK_OP_STR_NE 
	  | TK_OP_STR_LT 
	  | TK_OP_STR_GT 
	  | TK_OP_STR_LE 
	  | TK_OP_STR_GE             
	  ;

addExp  : addExp addExpOp multExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	  $<ast>$ = &scl->base;
	}
	| multExp
	;

addExpOp : TK_OP_NUM_SUBST 
	 | TK_OP_STR_CAT  
	 | TK_OP_NUM_ADD
	 ; 

multExp : multExp multExpOp unaryExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_binary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	  $<ast>$ = &scl->base;
	}
	| unaryExp
	;

multExpOp : TK_OP_NUM_DIV 
	  | TK_OP_NUM_MULT 
	  | TK_OP_NUM_MOD
	  ;

unaryExp :
	  unaryExpOpPrefix primaryExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_unary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, 1, YYLOCPTR );
	  $<ast>$ = &scl->base;
	}
	 | primaryExp
	 ;

unaryExpOpPrefix : TK_OP_NUM_ADD
	| TK_OP_NUM_SUBST
	| TK_OP_LOGICAL_NEGATE 
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

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_STRING, YYLOCPTR   ); 
	  scl->val.const_value.string_value = strdup( sval );
	  $<ast>$ = &scl->base;

	  LEXER_clean_string_parts( &parse_context->lexctx );
	}		   
	   | TK_PARENTHESES_OPEN expr TK_PARENTHESES_CLOSE
	{
	  $<ast>$ = $<ast>2;
	}
	   | functionCallStmt 
	   ;
		
varRef : TK_IDENTIFIER collectionRefs
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_ref( $<string_value>1, (AST_VECTOR *) $<ast>2, YYLOCPTR );

	  $<ast>$ = &scl->base;
	}	       
          | TK_IDENTIFIER
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_ref( $<string_value>1, 0, YYLOCPTR );

	  $<ast>$ = &scl->base;
	}	
	;


collectionRefs : collectionRefs oneRef  
       	{
	  AST_VECTOR *scl;

          scl = (AST_VECTOR *) $<ast>1;
	  AST_VECTOR_add( scl, $<ast>2 );
	  
	  $<ast>$ = &scl->base;
	}
	| oneRef
       	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR );
	  AST_VECTOR_add( scl, $<ast>1 );

	  $<ast>$ = &scl->base;
       	}
	;

oneRef :  TK_BRACE_OPEN expr TK_BRACE_CLOSE
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_ARRAY_INDEX, S_VAR_INT, YYLOCPTR   ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;
	}	
       |  TK_BRACKET_OPEN expr TK_BRACKET_CLOSE
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_HASH_INDEX, S_VAR_ANY, YYLOCPTR  ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;
	}	
      ;

/** function calls **/

functionCallStmt : TK_IDENTIFIER  TK_PARENTHESES_OPEN functionArgList TK_PARENTHESES_CLOSE
       	{
	  AST_EXPRESSION *scl;
	  AST_FUNC_CALL *fcall;

	  fcall = AST_FUNC_CALL_init( $<string_value>1, (AST_VECTOR *) $<ast>3, YYLOCPTR  );

	  scl = AST_EXPRESSION_init( S_FUN_CALL, S_VAR_ANY, YYLOCPTR  ); 
    
	  scl->val.fcall = fcall;
	
	  $<ast>$ = &scl->base;
	}		 
	;


functionArgList  : functionArgList TK_COMMA expr
	{
	  AST_VECTOR *scl;

          scl = (AST_VECTOR *) $<ast>1;
	  AST_VECTOR_add( scl, $<ast>3 );

	  $<ast>$ = &scl->base;
	}
		 | expr
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR );
	  AST_VECTOR_add( scl, $<ast>1 );
	 
	  $<ast>$ = &scl->base;
	}
		 |
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( YYLOCPTR  );
	  $<ast>$ = &scl->base;
	}
		 ;

/** list data structure **/		 

listConstructor : TK_BRACKET_OPEN expressionList TK_BRACKET_CLOSE   
	{
	  AST_EXPRESSION *scl;
	  
	  scl = AST_EXPRESSION_init( S_EXPR_LIST_VALUES, S_VAR_LIST, YYLOCPTR   ); 

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

	  scl = AST_EXPRESSION_init_binary( 0, (AST_EXPRESSION *) $<ast>1 , (AST_EXPRESSION *) $<ast>3 );

	  $<ast>$ = &scl->base;
	}	
	   ;

%%
	

