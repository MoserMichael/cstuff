#include <vtest/vtest.h>
#include <butils/logg.h>
#include <butils/fn.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define LOG_FILE "logg.log"

void LOGG_test0()
{	
#ifndef WIN32
    char buf[512];
    char fname[512], *base;
    pid_t pid;
    
    pid = getpid();

    memset(fname, 0, sizeof(fname));
    readlink("/proc/self/exe",fname,sizeof(fname));
    base = FN_file_name( fname );

    sprintf(buf,"cat /proc/%d/maps >maps_%s.txt", pid, base);

    system(buf);
    free(base);
#endif    
}

void LOGG_test()
{
  unlink( LOG_FILE );

  MLOG_init( MLOG_LEVEL_DEBUG, MLOG_ACTION_TO_FILE, LOG_FILE);
  MLOG_alloc_option( MLOG_ALLOC_TLS_HEAP, 1024 );

  MLOG_TRACE("Not logged");
  MLOG_DEBUG("First post %d", 42 );
  MLOG_INFO("Important message %d %d %d", 42, 42, 42);
  MLOG_ERROR("ERROR ERROR ERROR");
}


