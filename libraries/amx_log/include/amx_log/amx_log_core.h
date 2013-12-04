#if !defined(__AMX_LOG_CORE_H__)
#define __AMX_LOG_CORE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdarg.h>

#if !defined(NO_DOXYGEN)
#define FORMAT_PRINTF(f, a) __attribute__ ((format (printf, f, a)))
#else
/**
 @brief
 Helper macro for GCC compiler attribues, printf format checking
 */
#define FORMAT_PRINTF(format, arguments)
#endif

/**
 @file
 @brief
 Ambiorix logging core API header file
*/

/**
 @ingroup amx_logging
 @defgroup amx_log_core Log Core
*/

/**
 @ingroup amx_log_core
 @brief
 Different logging output methods.
 */
typedef enum _amx_log_type
{
	amx_log_syslog,				/**< Output to system logging facility*/
	amx_log_stdout,				/**< Output to the standard out */
	amx_log_stderr				/**< Output to the standard error */
} amx_log_type_t;

/**
 @ingroup amx_log_core
 @brief
 Different logging levels.
 */
typedef enum _amx_log_level
{
	amx_log_fatal_error = 0,	/**< Fatal error messeages, these can not be turned off and cause the application to stop */
	amx_log_error,				/**< Error messages */
	amx_log_warning,			/**< Warning messages */
	amx_log_notice,				/**< Notice messages */
	amx_log_lib_info,			/**< Library informative messages */
	amx_log_app_info,			/**< Application informative messages */
	amx_log_stack				/**< Function entry and exit messages */
} amx_log_level_t;

/**
 @ingroup amx_log_core
 @brief
 Supported time formats.

 @note
 The time format is ignored when using the system log as output method.
 */
typedef enum _amx_log_time
{
	amx_log_system_seconds,		/**< Prepend time stamp in seconds.milliseconds to each message */
	amx_log_system_time			/**< Prepend System time in human readable form (HH:MM:SS) to each message. */
} amx_log_time_t;

/**
 @ingroup amx_log_core
 @brief
 Opens logging.

 Each application should open the logging before using it.
 An identifier (mostly the application name) must be specified, as well as the output time, see @ref amx_log_type_t.

 When logging is opened, the log level is set to @ref amx_log_fatal_error by default, use @ref amx_log_set_level to
 change the log level.

 The time format is set to @ref amx_log_system_seconds by default, use @ref amx_log_set_time_format to change the time
 format.

 @note
 The logging facility can only be opened once per application. All subsequent calls to this function will have no effect

 @param identifier an identifier, can be any arbitrary string.
 @param type the output type, see @ref amx_log_type_t
 
 @return
 0 when the logging facility is opened or -1 when opening failed
*/
int amx_log_open(const char *identifier, amx_log_type_t type);

/**
 @ingroup amx_log_core
 @brief
 Closes logging.

 When the logging facilities are not needed anymore, they must be closed.
 Mostly the logging is closed right before the application exits.

 When logging was not opened, this function has no effect.
*/
void amx_log_close(void);

/**
 @ingroup amx_log_core
 @brief
 Checks that logging is opened.

 @return
 This function returns "true" when the logging is open, false if it is not.
*/
bool amx_log_is_opened(void);

/**
 @ingroup amx_log_core
 @brief
 Sets the time format used in the output.

 Each method is prepended with a time stamp, the format of the time stamp can be chosen at any time.
 For the possible time stamp formats see @ref amx_log_time_t

 The default time stamp format is @ref amx_log_system_seconds
 @note
 When the system log output method is chosen, the time stamp format is ignored.

 @param time_format the time stamp format that must be used.
*/
void amx_log_set_time_format(amx_log_time_t time_format);

/**
 @ingroup amx_log_core
 @brief
 Returns the current set time stamp format.

 @return
 The current set time stamp format, see @ref amx_log_time_t for the possible formats
*/
amx_log_time_t amx_log_get_time_format();

/**
 @ingroup amx_log_core
 @brief
 Changes the log level.

 Changes the current log level into the one specified. See @ref amx_log_level_t for the different
 supported log levels.

 @note
 The log level can only be changed when logging is opened.

 @param log_level the new log level
*/
void amx_log_set_level(amx_log_level_t log_level);

/**
 @ingroup amx_log_core
 @brief
 Get the current log level.

 @note
 When logging is not opened, this function returns @ref amx_log_fatal_error.

 @return
 The current log level.
*/
amx_log_level_t amx_log_get_level(void);

/**
 @ingroup amx_log_core
 @brief
 Get the log output type.

 @note
 The output type can not be changed after opening, to change to output type, close the logging by using
 @ref amx_log_close and re-open using @ref amx_log_open.

 @return
 The output type, see @ref amx_log_type_t for the possuble types.
*/
amx_log_type_t amx_log_get_type(void);

/**
 @ingroup amx_log_core
 @brief
 Get the log identifier.

 The identifier is always put before the log messages.

 @note
 The identifier can not be changed after opening, to change to identifier type, close the logging by using
 @ref amx_log_close and re-open using @ref amx_log_open.

 @return
 The identifier, used when opening the logging facility.
*/
const char *amx_log_get_identifier(void);

/**
 @ingroup amx_log_core
 @brief
 Prints a log message.

 Outputs a log message to the opened output stream (see @ref amx_log_open). The messages is printed only
 when the specified log level is lower or equal to the log level set.

 @param log_level the log level of the message
 @param format the format of the message (like printf)
 @param ap  argument pointer, a list of arguments
*/
void amx_log_va(amx_log_level_t log_level, const char *format, va_list ap);

/**
 @ingroup amx_log_core
 @brief
 Prints a log message.

 Outputs a log message to the opened output stream (see @ref amx_log_open). The messages is printed only
 when the zone is enabled and when the log level specified is lower or equal to the level of the zone.

 @param log_level the log level of the message
 @param zone_name the zone name for the log message
 @param format the format of the message (like printf)
 @param ap  argument pointer, a list of arguments
*/
void amx_log_zone_va(amx_log_level_t log_level, const char *zone_name, const char *format, va_list ap);

/**
 @ingroup amx_log_core
 @brief
 Prints a log message.

 Outputs a log message to the opened output stream (see @ref amx_log_open). The messages is printed only
 when the specified log level is lower or equal to the log level set.

 @param log_level the log level of the message
 @param format the format of the message (like printf)
*/
void amx_log(amx_log_level_t log_level, const char *format, ...) FORMAT_PRINTF(2, 3);

/**
 @ingroup amx_log_core
 @brief
 Prints a log message.

 Outputs a log message to the opened output stream (see @ref amx_log_open). The messages is printed only
 when the specified log level is lower or equal to the log level set.

 @param log_level the log level of the message
 @param zone_name the zone name for the log message
 @param format the format of the message (like printf)
*/
void amx_log_zone(amx_log_level_t log_level, const char *zone_name, const char *format, ...)  FORMAT_PRINTF(3, 4);

#ifdef __cplusplus
}
#endif

#endif // __AMX_LOG_CORE_H__
