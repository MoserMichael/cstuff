#include <stdio.h>

main()
{
#ifndef _FORTIFY_SOURCE
printf("_FORTIFY_SOURCE not defined\n");
#else
printf("_FORTIFY_SOURCE set to %d\n", _FORTIFY_SOURCE);
#endif

#ifdef __SSP__
printf("stack protector on = %d\n", __SSP__);
#endif

#ifdef __SSP_ALL__
printf("stack protector all on = %d\n", __SSP_ALL__);
#endif

#ifdef __SSP_STRONG__
printf("stack protector strong on = %d\n", __SSP_STRONG__);
#endif

#ifdef __SANITIZE_ADDRESS__
printf("sanitize address is on\n");
#endif

return 0;
}
