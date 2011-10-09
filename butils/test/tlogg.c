#include <vtest/vtest.h>
#include <butils/logg.h>
#include <unistd.h>

#define LOG_FILE "logg.log"

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


