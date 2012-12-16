#ifndef _YYSTYPE_H_
#define _YYSTYPE_H_

#include <pooh-lan/poohdef.h>

#define  YYSTYPE_IS_DECLARED 

struct tagAST_BASE;

typedef union {
	char   *string_value;
	double  double_value;
	POOH_INT long_value;
	int	int_value;
	struct tagAST_BASE *ast;	

} YYSTYPE;




#endif

