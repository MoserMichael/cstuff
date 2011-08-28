/* Copyright NDS Group - 2011 */
#define _POSIX_C_SOURCE 199309 
#include <time.h>


int my_nanosleep(const struct timespec *req, struct timespec *rem)
{
  return nanosleep( req, rem) ;
}



