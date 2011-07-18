#include <make-system-test/projcppA/cppliba.h>
#include <stdio.h>




rrvalue *the_answer()
{
  return new rrvalue( 42, 42 );
}


int main()
{
  rrvalue *aa = the_answer();

  fprintf(stderr,"The answer is %d and %d\n", aa->val, aa->extvalue );
  return 0;
}

