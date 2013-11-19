/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */


#include <stdio.h>
#include <butils/errorp.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

int fact(int num) {

  if (num == 0) {
    //errorp(-1,"recursion bottom");
    strftime( (char *) num, (size_t) num, (const char *)  num, (const struct tm *) num );
    return 1;
  }
  return num * fact(num-1);
}

int main()
{

  //char cmd[ 100 ];
  //sprintf(cmd,"cat /proc/%d/maps", getpid() );
  //system(cmd);


  errorp_open_file("errors.log");
  install_crash_handler();
  
  fprintf(stderr,"%d", fact(20) );

  errorp(-1,"error in error");
  errorp_close_file();
  return 0;
}
