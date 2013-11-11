#if !defined(__AMX_LOG_MACRO_H__)
#define __AMX_LOG_MACRO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_config/amx_config.h>

// Color schemes are defined here
#if defined(CONFIG_AMX_LOG_COLORS) || defined(CONFIG_AMX_LOG_COLORS_SIMPLE)
#define AMX_LOG_RESET	(amx_log_get_type() != amx_log_syslog)?"\033[0m":""
#define AMX_LOG_RED		(amx_log_get_type() != amx_log_syslog)?"\033[31m":""
#define AMX_LOG_LRED	(amx_log_get_type() != amx_log_syslog)?"\033[31;1m":""
#define AMX_LOG_YELLOW	(amx_log_get_type() != amx_log_syslog)?"\033[33;1m":""
#define AMX_LOG_GREEN	(amx_log_get_type() != amx_log_syslog)?"\033[32m":""
#define AMX_LOG_LGREEN	(amx_log_get_type() != amx_log_syslog)?"\033[32;1m":""
#define AMX_LOG_BLUE	(amx_log_get_type() != amx_log_syslog)?"\033[34m":""
#define AMX_LOG_LBLUE	(amx_log_get_type() != amx_log_syslog)?"\033[36m":""
#define AMX_LOG_BRIGHT	(amx_log_get_type() != amx_log_syslog)?"\033[1m":""
#else
#define AMX_LOG_RESET	""
#define AMX_LOG_RED		""
#define AMX_LOG_LRED	""
#define AMX_LOG_YELLOW	""
#define AMX_LOG_GREEN	""
#define AMX_LOG_LGREEN	""
#define AMX_LOG_BLUE	""
#define AMX_LOG_LBLUE	""
#define AMX_LOG_BRIGHT	""
#endif

#if defined(CONFIG_AMX_LOG_COLORS)
#define AMX_LOG_NEUTRAL_MARK		AMX_LOG_LBLUE,	"-",	AMX_LOG_RESET,	AMX_LOG_RESET
#define AMX_LOG_FATAL_ERROR_MARK	AMX_LOG_RED,	"X",	AMX_LOG_RESET,	AMX_LOG_RED
#define AMX_LOG_ERROR_MARK			AMX_LOG_LRED,	"x",	AMX_LOG_RESET,	AMX_LOG_LRED
#define AMX_LOG_WARNING_MARK		AMX_LOG_YELLOW,	"!",	AMX_LOG_RESET,	AMX_LOG_YELLOW
#define AMX_LOG_NOTICE_MARK			AMX_LOG_LGREEN,	"n",	AMX_LOG_RESET,	AMX_LOG_LGREEN
#define AMX_LOG_LIB_INFO_MARK		AMX_LOG_GREEN,	"I",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#define AMX_LOG_APP_INFO_MARK		AMX_LOG_GREEN,	"i",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#else
#if defined(CONFIG_AMX_LOG_COLORS_SIMPLE)
#define AMX_LOG_NEUTRAL_MARK		AMX_LOG_LBLUE,	"-",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#define AMX_LOG_FATAL_ERROR_MARK	AMX_LOG_RED,	"X",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#define AMX_LOG_ERROR_MARK			AMX_LOG_LRED,	"x",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#define AMX_LOG_WARNING_MARK		AMX_LOG_YELLOW,	"!",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#define AMX_LOG_NOTICE_MARK			AMX_LOG_LGREEN,	"n",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#define AMX_LOG_LIB_INFO_MARK		AMX_LOG_GREEN,	"I",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#define AMX_LOG_APP_INFO_MARK		AMX_LOG_GREEN,	"i",	AMX_LOG_RESET,	AMX_LOG_BRIGHT
#else
#define AMX_LOG_NEUTRAL_MARK		"", "-", "", ""
#define AMX_LOG_FATAL_ERROR_MARK	"", "X", "", ""
#define AMX_LOG_ERROR_MARK			"", "x", "", ""
#define AMX_LOG_WARNING_MARK		"", "!", "", ""
#define AMX_LOG_NOTICE_MARK			"", "n", "", ""
#define AMX_LOG_LIB_INFO_MARK		"", "I", "", ""
#define AMX_LOG_APP_INFO_MARK		"", "i", "", ""
#endif
#endif

// Logging macros
#define AMX_LOG_ZONE_FORMAT				"%s%7.7s%s - "
#define AMX_LOG_MARK_FORMAT				"%s[%s]%s%s"
#define AMX_LOG_SOURCE_FORMAT			" - %s%s(%s@%s:%d)%s"
#define AMX_LOG_DEFAULT_FORMAT(format)	AMX_LOG_ZONE_FORMAT AMX_LOG_MARK_FORMAT format AMX_LOG_SOURCE_FORMAT

#define AMX_LOG_ZONE(zone_name)			AMX_LOG_BLUE, zone_name, AMX_LOG_RESET
#define AMX_LOG_SOURCE					AMX_LOG_RESET, AMX_LOG_LBLUE, __FUNCTION__, __FILE__, __LINE__, AMX_LOG_RESET

#define AMX_LOG_NOOP

#define AMX_LOG_FATAL(format, ...) \
		amx_log(amx_log_fatal_error, \
				AMX_LOG_DEFAULT_FORMAT(format), \
				AMX_LOG_DEFAULT_MSG("", AMX_LOG_FATAL_ERROR_MARK))

#if defined(CONFIG_AMX_LOG_ENABLED)

#if !defined(CONFIG_AMX_LOG_MAX_LEVEL)
#define CONFIG_AMX_LOG_MAX_LEVEL CONFIG_AMX_LOG_DEFAULT_LEVEL
#endif

#if (CONFIG_AMX_LOG_MAX_LEVEL >= 100)
#define AMX_LOG_ERROR(format, ...) \
		amx_log(amx_log_error, \
				AMX_LOG_DEFAULT_FORMAT(format), \
				AMX_LOG_ZONE(""), AMX_LOG_ERROR_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)

#define AMX_LOG_ZONE_ERROR(zone_name, format, ...) \
		amx_log_zone(amx_log_error, zone_name, \
					 AMX_LOG_DEFAULT_FORMAT(format), \
					 AMX_LOG_ZONE(zone_name), AMX_LOG_ERROR_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)
#else
#define AMX_LOG_ERROR(format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_ERROR(zone, format, ...) AMX_LOG_NOOP
#endif

#if (CONFIG_AMX_LOG_MAX_LEVEL >= 200)
#define AMX_LOG_WARNING(format, ...) \
		amx_log(amx_log_warning, \
				AMX_LOG_DEFAULT_FORMAT(format), \
				AMX_LOG_ZONE(""), AMX_LOG_WARNING_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)

#define AMX_LOG_ZONE_WARNING(zone_name,format, ...) \
	amx_log_zone(amx_log_warning, zone_name, \
				 AMX_LOG_DEFAULT_FORMAT(format), \
				 AMX_LOG_ZONE(zone_name), AMX_LOG_WARNING_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)
#else
#define AMX_LOG_WARNING(format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_WARNING(zone,format, ...) AMX_LOG_NOOP
#endif

#if (CONFIG_AMX_LOG_MAX_LEVEL >= 300)
#define SAH_TRACE_NOTICE(format, ...) \
		amx_log(amx_log_notice, \
				AMX_LOG_DEFAULT_FORMAT(format), \
				AMX_LOG_ZONE(""), AMX_LOG_NOTICE_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)

#define SAH_TRACEZ_NOTICE(zone_name, format, ...) \
	amx_log_zone(amx_log_notice, zone_name, \
				 AMX_LOG_DEFAULT_FORMAT(format), \
				 AMX_LOG_ZONE(zone_name), AMX_LOG_NOTICE_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)
#else
#define AMX_LOG_NOTICE(format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_NOTICE(zone,format, ...) AMX_LOG_NOOP
#endif

#if (SAHTRACES_LEVEL >= 350)
#define AMX_LOG_LIB_INFO(format, ...) \
		amx_log(amx_log_lib_info, \
				AMX_LOG_DEFAULT_FORMAT(format), \
				AMX_LOG_ZONE(""), AMX_LOG_LIB_INFO_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)

#define AMX_LOG_ZONE_LIB_INFO(zone_name, format, ...) \
	amx_log_zone(amx_log_lib_info, zone_name, \
				 AMX_LOG_DEFAULT_FORMAT(format), \
				 AMX_LOG_ZONE(zone_name), AMX_LOG_LIB_INFO_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)
#else
#define AMX_LOG_LIB_INFO(format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_LIB_INFO(zone,format, ...) AMX_LOG_NOOP
#endif

#if (SAHTRACES_LEVEL >= 400)
#define AMX_LOG_APP_INFO(format, ...) \
		amx_log(amx_log_app_info, \
				AMX_LOG_DEFAULT_FORMAT(format), \
				AMX_LOG_ZONE(""), AMX_LOG_LIB_INFO_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)

#define AMX_LOG_ZONE_APP_INFO(zone_name, format, ...) \
	amx_log_zone(amx_log_app_info, zone_name, \
				 AMX_LOG_DEFAULT_FORMAT(format), \
				 AMX_LOG_ZONE(zone_name), AMX_LOG_APP_INFO_MARK, ##__VA_ARGS__, AMX_LOG_SOURCE)
#else
#define AMX_LOG_APP_INFO(format, ...) AMX_LOG_NOOP
#define AMX_LOG_APP_ZONE_INFO(zone,format, ...) AMX_LOG_NOOP
#endif

#if (SAHTRACES_LEVEL >= 500)
#define AMX_LOG_IN() \
		amx_log(amx_log_stack,"%s%-7.7s%s - >>>>>>>>>>%s - %s(%s@%s:%d)%s", \
				AMX_LOG_ZONE(""), AMX_LOG_SOURCE)

#define AMX_LOG_OUT() \
		amx_log(amx_log_stack,"%s%-7.7s%s - <<<<<<<<<<%s - %s(%s@%s:%d)%s", \
				AMX_LOG_ZONE(""), AMX_LOG_SOURCE)

#define AMX_LOG_ZONE_IN(zone_name) \
	amx_log_zone(amx_log_stack, zone_name, \
				 "%s%-7.7s%s - >>>>>>>>>>%s - %s(%s@%s:%d)%s", \
				 AMX_LOG_ZONE(zone_name), AMX_LOG_SOURCE)

#define AMX_LOG_ZONE_OUT(zone_name) \
	amx_log_zone(amx_log_stack, zone_name, \
				 "%s%-7.7s%s - <<<<<<<<<<%s - %s(%s@%s:%d)%s", \
				 AMX_LOG_ZONE(zone), AMX_LOG_SOURCE)
#else
#define SAH_TRACE_IN() AMX_LOG_NOOP
#define SAH_TRACE_OUT() AMX_LOG_NOOP
#define SAH_TRACEZ_IN(zone) AMX_LOG_NOOP
#define SAH_TRACEZ_OUT(zone) AMX_LOG_NOOP
#endif

#else

#if !defined(CONFIG_AMX_LOG_MAX_LEVEL)
#define CONFIG_AMX_LOG_MAX_LEVEL 0
#endif

#define AMX_LOG(level,format,...) AMX_LOG_NOOP
#define AMX_LOG_ERROR(format, ...) AMX_LOG_NOOP
#define AMX_LOG_WARNING(format, ...) AMX_LOG_NOOP
#define AMX_LOG_NOTICE(format, ...) AMX_LOG_NOOP
#define AMX_LOG_LIB_INFO(format, ...) AMX_LOG_NOOP
#define AMX_LOG_APP_INFO(format, ...) AMX_LOG_NOOP
#define AMX_LOG_IN() AMX_LOG_NOOP
#define AMX_LOG_OUT() AMX_LOG_NOOP

#define AMX_LOG_ZONE(level,zone_name, format,...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_ERROR(zone_name, format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_WARNING(zone_name, format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_NOTICE(format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_LIB_INFO(zone_name, format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_APP_INFO(zone_name, format, ...) AMX_LOG_NOOP
#define AMX_LOG_ZONE_IN(zone_name) AMX_LOG_NOOP
#define AMX_LOG_ZONE_OUT(zone_name) AMX_LOG_NOOP

#endif

#ifdef __cplusplus
}
#endif

#endif // __AMX_LOG_MACRO_H__
