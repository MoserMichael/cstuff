%locations
%debug

%{
void yyerror (char const *);
 
int yylex (void);


%} 


%token TK_ERROR



%left TK_WHILE TK_FOR TK_IF TK_DO TK_ASSIGN TK_ASSIGN_REF TK_ELSE TK_ELSIF TK_END TK_RETURN TK_YIELD TK_BREAK TK_CONTINUE TK_IDENTIFIER TK_INTEGER_CONSTANT TK_STRING_CONSTANT TK_DOUBLE_CONSTANT TK_SUB TK_INCLUDE 

%token TK_OP_NUM_EQ TK_OP_NUM_NE TK_OP_NUM_LT TK_OP_NUM_GT TK_OP_NUM_LE TK_OP_NUM_GE TK_OK_DOT

%token TK_OP_STR_EQ TK_OP_STR_NE TK_OP_STR_LT TK_OP_STR_GT TK_OP_STR_LE TK_OP_STR_GE

%token TK_OP_STR_CAT TK_HASH_IT

%token TK_OP_NUM_ADD TK_OP_NUM_SUBST TK_OP_NUM_DIV TK_OP_NUM_MULT TK_OP_NUM_MOD  

%token TK_OP_NUM_AUTOINCR TK_OP_NUM_AUTODECR

%token TK_VAR_DEF TK_VAR_UNDEF TK_ARR_DEF TK_CODEREF_DEF 

%token TK_OP_STR_REGEXMATCH 

%token TK_COLON TK_SEMICOLON TK_COMMA TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE TK_BRACE_OPEN TK_BRACE_CLOSE  TK_BRACKET_OPEN  TK_BRACKET_CLOSE 


%%

prog : stmtList	
     	    {
		parse_context->my_ast_root = $<ast>1;
	    }
	|
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
	    }
	 | stmt 
	    {
		AST_BASE_LIST *slist;

		slist = AST_BASE_LIST_init( );
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

            scl = AST_FOR_LOOP_init( $<string_value>2, (AST_EXPRESSION *) $<ast>3, (AST_BASE_LIST *) $<ast>4 );
	    $<ast>$ = &scl->base;
	}
    ;     



whileStmt : TK_WHILE expr stmtList
	{
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>2, (AST_BASE_LIST *) $<ast>3, 0 );
	    $<ast>$ = &scl->base;
	}
    ;


doWhileStmt : TK_DO stmtList TK_WHILE expr
	{
	    AST_WHILE_LOOP *scl;

            scl = AST_WHILE_LOOP_init( (AST_EXPRESSION *) $<ast>4, (AST_BASE_LIST *) $<ast>2, 1 );
	    $<ast>$ = &scl->base;
	}
    ;




valueDeepCopyAssignStmt : varRef TK_ASSIGN expr 
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( ASSIGNMENT_REF_COPY, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
	    $<ast>$ = &scl->base;
	}
	;

referenceCopyAssignmentStmt : varRef TK_ASSIGN_REF expr
	{
	    AST_ASSIGNMENT *scl;

	    scl = AST_ASSIGNMENT_init( ASSIGNMENT_DEEP_COPY, (AST_EXPRESSION *) $<ast>1, (AST_EXPRESSION *) $<ast>3 );
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

	  scl = AST_COND_init( (AST_EXPRESSION *) $<ast>1, (AST_BASE_LIST *) $<ast>2 );
	  $<ast>$ = &scl->base;
	}
	    ;
	                                    

includeStmt : TK_INCLUDE TK_STRING_CONSTANT
	{
		int ret = LEXER_scan_file( $<string_value>2 );
	        free( $<string_value>2 );
		if (ret < 0) {
		  do_yyerror( &yylloc, "Can't open include file %s", $<string_value>2 );
		}
		$<ast>$ = 0;
	}
	;


functionDefStmt  :  functionPrototypeDecl stmtList TK_END  
	{
	  AST_FUNC_DECL *scl;

	  scl = (AST_FUNC_DECL *) $<ast>1;

	  AST_FUNC_DECL_set_body( scl, (AST_BASE_LIST *) $<ast>2 );

	  $<ast>$ = &scl->base;
	}
	;

functionPrototypeDecl : TK_SUB TK_IDENTIFIER TK_PARENTHESES_OPEN funcParamDecls TK_PARENTHESES_CLOSE 
	{
	  AST_FUNC_DECL *scl;
	  
	  scl = AST_FUNC_DECL_init( $<string_value>2,  (AST_VECTOR *) $<ast>4 );
	  $<ast>$ = &scl->base;
	}
		     |  TK_SUB TK_IDENTIFIER  funcParamDecls 
	{
	  AST_FUNC_DECL *scl;
	  
	      scl = AST_FUNC_DECL_init( $<string_value>2,  (AST_VECTOR *) $<ast>3 );
	      $<ast>$ = &scl->base;
	}
	;

funcParamDecls : funcParamDecls TK_COMMA TK_IDENTIFIER
	{
	  AST_VECTOR *sval;
	  AST_EXPRESSION *expr;

	  sval = (AST_VECTOR *) $<ast>1;
	  expr = AST_EXPRESSION_init_ref( $<string_value>3, 0 );
	  
	  AST_VECTOR_add( sval, &expr->base );

	  $<ast>$ = &sval->base;
	}
	    | TK_IDENTIFIER
	{
	  AST_VECTOR *scl;
	  AST_EXPRESSION *expr;

	  scl = AST_VECTOR_init( );
	  expr = AST_EXPRESSION_init_ref( $<string_value>1, 0 );
	  AST_VECTOR_add( scl, &expr->base );

	  $<ast>$ = &scl->base;
	}
	;


breakStmt : TK_BREAK
	{
	    AST_BASE *scl;

	    scl = (AST_BASE *) malloc( sizeof( AST_BASE ) );
	    AST_BASE_init( scl, S_BREAK, yyloc );
	    $<ast>$ = scl;
	}
	;

continueStmt : TK_CONTINUE
	{
	    AST_BASE *scl;

	    scl = (AST_BASE *) malloc( sizeof( AST_BASE ) );
	    AST_BASE_init( scl, S_CONTINUE, yyloc );
	    $<ast>$ = scl;
	}
	 ;

returnStmt : TK_RETURN expr
	{
	  AST_RETURN *scl;

	  scl = AST_RETURN_init( S_RETURN, (AST_EXPRESSION *) $<ast>2 );
	  $<ast>$ = &scl->base;
	}
	   | TK_RETURN
	{
	  AST_RETURN *scl;

	  scl = AST_RETURN_init( S_RETURN, 0 );
	  $<ast>$ = &scl->base;
	}
	   ;

yieldStmt  : TK_YIELD expr
	{
	  AST_RETURN *scl;

	  scl = AST_RETURN_init( S_YIELD, (AST_EXPRESSION *) $<ast>2 );
	  $<ast>$ = &scl->base;
	}
	   ;

/* *** expression clause **** */

expr : compExp
     | hashConstructor
     | listConstructor
     | anonymousFunction
     ;   

anonymousFunction : TK_SUB funcParamDecls stmtList TK_END
	{
	  AST_EXPRESSION *scl; 
	  AST_FUNC_DECL *fdecl;
	  
	  fdecl = AST_FUNC_DECL_init( 0,  (AST_VECTOR *) $<ast>2 );

	  AST_FUNC_DECL_set_body( fdecl, (AST_BASE_LIST *) $<ast>3 );

	  scl = AST_EXPRESSION_init_ref( 0, 0 );

	  scl->value_type = S_VAR_CODE;
	  scl->val.fdecl = fdecl;

	  $<ast>$ = &scl->base;
	}
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
	   primaryExp unaryExpOpPostfix 
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_unary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, 0 );
	  $<ast>$ = &scl->base;
	}
	 | unaryExpOpPrefix primaryExp
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_unary( $<int_value>2, (AST_EXPRESSION *) $<ast>1, 1 );
	  $<ast>$ = &scl->base;
	}
	 | primaryExp
	 ;

unaryExpOpPrefix : TK_OP_NUM_AUTOINCR 
	| TK_OP_NUM_AUTODECR 
	| TK_OP_NUM_ADD
	;

unaryExpOpPostfix : TK_OP_NUM_AUTOINCR 
	| TK_OP_NUM_AUTODECR
	;				  

primaryExp : varRef
	   | TK_INTEGER_CONSTANT
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_INT ); 
	  scl->val.const_value.long_value = $<long_value>1;
	  $<ast>$ = &scl->base;
	}

	   | TK_DOUBLE_CONSTANT
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_DOUBLE ); 
	  scl->val.const_value.double_value = $<double_value>1;
	  $<ast>$ = &scl->base;
	}	   
	   | TK_STRING_CONSTANT
	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_STRING ); 
	  scl->val.const_value.string_value = $<string_value>1;
	  $<ast>$ = &scl->base;
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

	  scl = AST_EXPRESSION_init_ref( $<string_value>1, (AST_VECTOR *) $<ast>2 );

	  $<ast>$ = &scl->base;
	}	       
          | TK_IDENTIFIER
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init_ref( $<string_value>1, 0 );

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

	  scl = AST_VECTOR_init( );
	  AST_VECTOR_add( scl, $<ast>1 );

	  $<ast>$ = &scl->base;
       	}
	;

oneRef :  TK_BRACE_OPEN expr TK_BRACE_CLOSE
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_ARRAY_INDEX, S_VAR_INT ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;
	}	
       |  TK_BRACKET_OPEN expr TK_BRACKET_CLOSE
       	{
	  AST_EXPRESSION *scl;

	  scl = AST_EXPRESSION_init( S_EXPR_HASH_INDEX, S_VAR_ANY ); 
	  scl->val.index_expr = (AST_EXPRESSION *) $<ast>2 ;
	  $<ast>$ = &scl->base;
	}	
      ;

/** function calls **/

functionCallStmt : TK_IDENTIFIER  TK_PARENTHESES_OPEN functionArgList TK_PARENTHESES_CLOSE
       	{
	  AST_EXPRESSION *scl;
	  AST_FUNC_CALL *fcall;

	  fcall = AST_FUNC_CALL_init( $<string_value>1, (AST_VECTOR *) $<ast>3 );

	  scl = AST_EXPRESSION_init( S_FUN_CALL, S_VAR_ANY ); 
    
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

	  scl = AST_VECTOR_init( );
	  AST_VECTOR_add( scl, $<ast>1 );
	 
	  $<ast>$ = &scl->base;
	}
		 |
	{
	  AST_VECTOR *scl;

	  scl = AST_VECTOR_init( );
	  $<ast>$ = &scl->base;
	}
		 ;

/** list data structure **/		 

listConstructor : TK_BRACKET_OPEN expressionList TK_BRACKET_CLOSE   
	{
	  AST_EXPRESSION *scl;
	  
	  scl = AST_EXPRESSION_init( S_EXPR_LIST_VALUES, S_VAR_LIST ); 

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
	  
	  sval = AST_VECTOR_init( yyloc );
	  AST_VECTOR_add( sval, $<ast>1 );

	  $<ast>$ = &sval->base;
	}
	       |    
	{
	  AST_VECTOR *sval;
	  
	  sval = AST_VECTOR_init( );

	  $<ast>$ = &sval->base; 
	}
	       ;

hashConstructor : TK_BRACE_OPEN hashClauseList TK_BRACE_CLOSE
	{
	  AST_EXPRESSION *scl;
	  
	  scl = AST_EXPRESSION_init( S_EXPR_HASH_VALUES, S_VAR_HASH ); 

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
	  
	  sval = AST_VECTOR_init( );

	  AST_VECTOR_add( sval, $<ast>1 );

	  $<ast>$ = &sval->base;
	}
	       |
	{
	  AST_VECTOR *sval;
	  
	  sval = AST_VECTOR_init( );

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
	

