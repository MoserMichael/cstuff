%locations
%debug

%{
void yyerror (char const *);
 
int yylex (void);


%} 


%token TK_ERROR



%left  TK_WHILE TK_UNTIL TK_IF TK_DO TK_ASSIGN TK_ELSE TK_ELSIF TK_END TK_RETURN TK_BREAK TK_CONTINUE TK_ID TK_INT_CONSTANT TK_STRING_CONSTANT TK_DOUBLE_CONSTANT TK_GOTO TK_SUB TK_INCLUDE 

%token TK_OP_NUM_EQ TK_OP_NUM_NE TK_OP_NUM_LT TK_OP_NUM_GT TK_OP_NUM_LE TK_OP_NUM_GE TK_OK_DOT

%token TK_OP_STR_EQ TK_OP_STR_NE TK_OP_STR_LT TK_OP_STR_GT TK_OP_STR_LE TK_OP_STR_GE

%token TK_OP_STR_CAT

%token TK_OP_NUM_ADD TK_OP_NUM_SUBST TK_OP_NUM_DIV TK_OP_NUM_MULT TK_OP_NUM_MOD  

%token TK_OP_NUM_AUTOINCR TK_OP_NUM_AUTODECR

%token TK_OP_TOSTR TK_OP_TOINT

%token TK_VAR_DEF TK_VAR_UNDEF TK_ARR_DEF TK_CODEREF_DEF 

%token TK_OP_STR_REGEXMATCH 

%token TK_COLON TK_SEMICOLON TK_COMMA TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE TK_BRACE_OPEN TK_BRACE_CLOSE  TK_BRACKET_OPEN  TK_BRACKET_CLOSE TK_CLASS TK_INTERFACE TK_NEW



%%



/* *** expression clause **** */
expr : compExp
     ;   


compExp : compExp compExpOp addExp
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
	  | TK_OP_STR_REGEXMATCH
	  ;

addExp  : addExp addExpOp multExp
	| multExp
	;

addExpOp : TK_OP_NUM_SUBST 
	 | TK_OP_STR_CAT  
	 | TK_OP_NUM_ADD
	 ; 

multExp : multExp multExpOp  unaryExp
	| unaryExp
	;

multExpOp : TK_OP_NUM_DIV 
	  | TK_OP_NUM_MULT 
	  | TK_OP_NUM_MOD
	  ;

unaryExp :
	   primaryExp unaryExpOpPostfix 
	 | unaryExpOpPrefix primaryExp
	 | primaryExp
	 ;

unaryExpOpPrefix : TK_OP_NUM_AUTOINCR 
	| TK_OP_NUM_AUTODECR 
	| TK_OP_TOSTR 
	| TK_OP_TOINT
	| TK_OP_NUM_ADD
	;

unaryExpOpPostfix : TK_OP_NUM_AUTOINCR 
	| TK_OP_NUM_AUTODECR
	;				  

primaryExp : varRef
	   | TK_INT_CONSTANT
	   | TK_DOUBLE_CONSTANT
	   | TK_STRING_CONSTANT
	   | TK_PARENTHESES_OPEN expr TK_PARENTHESES_CLOSE
	   | functionCallStmt 
	   ;

		
varRef : TK_ID
       | TK_ID TK_BRACE_OPEN expr TK_BRACE_CLOSE
       | TK_ID TK_BRACKET_OPEN expr TK_BRACKET_CLOSE
        ;



%%
	

