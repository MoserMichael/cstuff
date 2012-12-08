#ifndef __LOGG_H__
#define __LOGG_H__

#include <stdlib.h>

/**
 * @defgroup logging library.
 *
 * Not so simple logging subsystem - one logging category is supported (one global log level).
 * Otherwise this is a very versatile logging subsystem.
 *
 * @{
 */

/**
 * @brief defined logging levels 
 */
typedef enum {
  MLOG_LEVEL_TURN_OFF_LOGGING,  /** setting this level as currrent level will turn off logging */
  MLOG_LEVEL_ERROR, /** logging level for errors */
  MLOG_LEVEL_WARN,  /** logging level for warnings */
  MLOG_LEVEL_INFO, /** logging level for info messages */
  MLOG_LEVEL_DEBUG, /** logging level for debug messages */
  MLOG_LEVEL_TRACE, /** logging level for trace messages */

} MLOG_LEVEL;

/**
 * @brief actions for logging stuff
 */
typedef enum {
  MLOG_ACTION_TO_FILE  = 0x1, /** log entries are dumped to file */
  MLOG_ACTION_SYSLOG_TRACE = 0x2, /** log entries are dumped to one syslog level */
  MLOG_ACTION_CONSOLE = 0x4, /** log entries are dumped to console */

} MLOG_ACTION;


/** 
 * @brief features to include in logging prefix
 */
typedef enum {
  MLOG_PREFIX_LOG_LEVEL = 0x1,    /** add log level to entry */
  MLOG_PREFIX_SOURCE_FILE = 0x2,  /** add source file / line number of log statement to entry */
  MLOG_PREFIX_TIME = 0x4,         /** add HOUR:MINUTE:SECOND:MICROSECOND to log entry */

} MLOG_PREFIX;


/**
 * @brief how to allocation the scrach pad buffer for logging
 */
typedef enum {
  MLOG_ALLOC_STACK,     /** buffer for logging allocated on stack (alloca) */
  MLOG_ALLOC_HEAP,      /** buffer for logging allocated on heap (pre log request) */
  MLOG_ALLOC_TLS_HEAP,  /** buffer for logging allocated on heap once for each operating system thread, stored in thread local storage */

} MLOG_ALLOC;


/**
 * @brief explicitly initialises logging. 
 */
int  MLOG_init( MLOG_LEVEL current, MLOG_ACTION action, void *arg);


/**
 * @brief sets level of detail added to prefix of log entry (A bitmask of MLOG_PREFIX values)
 *
 */
void MLOG_set_prefix_features( int prefix );

/**
 * @brief determines memory allocation stragegy used here
 * @param option - how to allocate buffer for logging. options are: use stack (MLOG_ALLOC_STACK), heap (MLOG_ALLOC_HEAP), heap and store in tls (MLOG_ALLOC_TLS_HEAP)
 * @param size - maximum length of a log entry allowed here.
 *  
 */
int  MLOG_alloc_option( MLOG_ALLOC option, size_t size);

/**
 * @brief set log level for which the current stack is dumped. By default this would happen for LOG_LEVEL_ERROR
 */
void MLOG_dump_stack_level( MLOG_LEVEL stack_dump_level );


extern MLOG_LEVEL m_current_log_level;

/**
 * @brief create a log entry, do not use this function directly, use MLOG_<LOG_LEVEL_NAME> macros instead
 */
int MLOG_printf( MLOG_LEVEL current, const char *file, int line,  const char *format , ... );

#define MLOG_ERROR(...) do { if (m_current_log_level >= MLOG_LEVEL_ERROR) {  MLOG_printf( MLOG_LEVEL_ERROR, __FILE__, __LINE__,  __VA_ARGS__ ); } } while(0);

#define MLOG_WARN(...)  do { if (m_current_log_level >= MLOG_LEVEL_WARN) {  MLOG_printf( MLOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__ ); } } while(0);


#ifndef NO_LOG

#define MLOG_INFO(...)  do { if (m_current_log_level >= MLOG_LEVEL_INFO) {  MLOG_printf( MLOG_LEVEL_INFO,  __FILE__, __LINE__, __VA_ARGS__ ); } } while(0);
 
#define MLOG_DEBUG(...) do { if (m_current_log_level >= MLOG_LEVEL_DEBUG) {  MLOG_printf( MLOG_LEVEL_DEBUG,  __FILE__, __LINE__, __VA_ARGS__ ); } } while(0);
 
#define MLOG_TRACE(...) do { if (m_current_log_level >= MLOG_LEVEL_TRACE) {  MLOG_printf( MLOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__ ); } } while(0);

#else

#define MLOG_INFO(...)  
 
#define MLOG_DEBUG(...) 
 
#define MLOG_TRACE(...) 

#endif

/**
 * @}
 */

#endif


