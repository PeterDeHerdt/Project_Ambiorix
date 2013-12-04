/****************************************************************************
**
** - DISCLAIMER OF WARRANTY -
**
** THIS FILE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
** EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE SOURCE
** CODE IS WITH YOU. SHOULD THE SOURCE CODE PROVE DEFECTIVE, YOU
** ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
**
** - LIMITATION OF LIABILITY -
**
** IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
** WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES
** AND/OR DISTRIBUTES THE SOURCE CODE, BE LIABLE TO YOU FOR DAMAGES,
** INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
** ARISING OUT OF THE USE OR INABILITY TO USE THE SOURCE CODE
** (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED
** INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE
** OF THE SOURCE CODE TO OPERATE WITH ANY OTHER PROGRAM), EVEN IF SUCH
** HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGES.
**
****************************************************************************/

#define _BSD_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <syslog.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <amx_log.h>

struct _amx_log_zone
{
	char                 *name;
	amx_log_level_t       level;
	struct _amx_log_zone *next;
};

typedef struct _amx_log
{
	amx_log_level_t       log_level;
	amx_log_type_t        log_type;
	char                 *log_id;
	bool                  log_opened;
	amx_log_time_t        log_time_format;
	struct _amx_log_zone *log_zones;
} amx_log_t;

static amx_log_t s_amx_log;

static int amx_log_level_2_sys_log_level(amx_log_level_t level)
{
	int sys_log_level = LOG_DEBUG;
	static int amx_log_table[][2] =
	{
		{ amx_log_fatal_error, LOG_CRIT    },
		{ amx_log_error,       LOG_ERR     },
		{ amx_log_warning,     LOG_WARNING },
		{ amx_log_notice,      LOG_NOTICE  },
		{ amx_log_lib_info,    LOG_INFO    },
		{ amx_log_app_info,    LOG_INFO    },
		{ amx_log_stack,       LOG_DEBUG   }
	};
	if (level <= amx_log_stack) {
		sys_log_level = amx_log_table[level][1];
	}

	return sys_log_level;
}

static void amx_log_print_time(amx_log_time_t format, FILE *stream)
{
	struct timeval now;
	gettimeofday(&now,NULL);

	switch(format)
	{
	default:
	case amx_log_system_seconds:
	{
		fprintf(stream,"(%d.%3.3d)",(int)now.tv_sec, (int)now.tv_usec/1000);
	}
		break;
	case amx_log_system_time:
	{
		time_t ts;
		time(&ts);
		struct tm *t = localtime(&ts);
		fprintf(stream,"(%2.2d:%2.2d:%2.2d)",t->tm_hour,t->tm_min,t->tm_sec);
	}
		break;
	}
}

static void amx_log_output(amx_log_level_t log_level, amx_log_level_t max_level, const char *format, va_list vl)
{
	if (s_amx_log.log_opened &&
		max_level >= log_level)
	{
		switch(s_amx_log.log_type)
		{
		default:
		case amx_log_syslog:
		{
			int syslog_level = amx_log_level_2_sys_log_level(log_level);
			vsyslog(syslog_level, format, vl);
		}
			break;
		case amx_log_stdout:
		{
			amx_log_print_time(s_amx_log.log_time_format, stdout);
			fprintf(stdout,"[%s] - ", s_amx_log.log_id);
			vfprintf(stdout, format, vl);
			fprintf(stdout,"\n");
		}
			break;
		case amx_log_stderr:
		{
			amx_log_print_time(s_amx_log.log_time_format, stderr);
			fprintf(stderr,"[%s] - ", s_amx_log.log_id);
			vfprintf(stderr, format, vl);
			fprintf(stderr,"\n");
		}
			break;
		}
	}
}

static amx_log_level_t amx_log_parse(char *zone) {
	char *pos = strchr(zone, '=');
	amx_log_level_t level = amx_log_lib_info;
	if (pos) {
		*pos = 0x0;
		level = strtol(pos + 1, NULL, 10);
	}
	return level;
}

static void amx_log_zones_from_env(void) {
	char *env_zones = getenv("AMX_LOG_ZONES");
	amx_log_level_t level = amx_log_lib_info;

	if (env_zones) {
		char *zone = strtok(env_zones, ",");
		while(zone) {
			level = amx_log_parse(zone);
			if (strcmp("self", zone) == 0) {
				amx_log_set_level(level);
			} else {
				amx_log_enable_zone(zone, level);
			}
			zone =  strtok(NULL, ",");
		}
	}
}

static void amx_log_load_zones(void) {
	amx_log_zones_from_env();
}

int amx_log_open(const char *identifier, amx_log_type_t type)
{
	int flags = 0;
	int retval = 0;

	if (!s_amx_log.log_opened)
	{
		s_amx_log.log_id = strdup(identifier);
		s_amx_log.log_type = type;
		switch(type)
		{
		case amx_log_syslog:
			openlog(identifier, flags, LOG_USER);
			break;
		case amx_log_stdout:
		case amx_log_stderr:
			break;
		default:
			retval = -1;
			goto exit;
			break;
		}
		s_amx_log.log_opened = true;
		amx_log_load_zones();
	} else {
		retval = -1;
	}

exit:
	return retval;
}

void amx_log_close(void)
{
	if (s_amx_log.log_opened)
	{
		if (s_amx_log.log_type == amx_log_syslog)
		{
			closelog();
		}
		s_amx_log.log_opened = false;
		free(s_amx_log.log_id);
		s_amx_log.log_id = NULL;
		amx_log_disable_all_zones();
		memset(&s_amx_log, 0, sizeof(amx_log_t));
	}
}

bool amx_log_is_opened(void)
{
	return s_amx_log.log_opened;
}

void amx_log_set_time_format(amx_log_time_t time_format)
{
	s_amx_log.log_time_format = s_amx_log.log_opened?time_format:amx_log_system_seconds;
}

amx_log_time_t amx_log_get_time_format()
{
	return s_amx_log.log_time_format;
}

void amx_log_set_level(amx_log_level_t log_level)
{
	s_amx_log.log_level = s_amx_log.log_opened?log_level:amx_log_fatal_error;
}

amx_log_level_t amx_log_get_level(void)
{
	return s_amx_log.log_level;
}

amx_log_type_t amx_log_get_type(void)
{
	return s_amx_log.log_type;
}

const char *amx_log_get_identifier(void)
{
	return s_amx_log.log_id?s_amx_log.log_id:"";
}

void amx_log_enable_zone(const char *zone_name, amx_log_level_t log_level)
{
	if (!s_amx_log.log_opened)
	{
		goto exit;
	}
	amx_log_zone_t *zone = amx_log_get_zone(zone_name);
	if (!zone)
	{
		zone = malloc(sizeof(amx_log_zone_t));
		if (zone)
		{
			zone->name = strdup(zone_name);
			zone->next = s_amx_log.log_zones;
			s_amx_log.log_zones = zone;
			zone->level = log_level;
		}
	} else {
		zone->level = log_level;
	}
exit:
	return;
}

void amx_log_disable_zone(const char *zone_name)
{
	amx_log_zone_t *current = s_amx_log.log_zones;
	amx_log_zone_t *previous = NULL;

	/* go trough the zone list */
	while(current)
	{
		/* if we find the entry, remove it */
		if(strcmp(current->name, zone_name) == 0)
		{
			if(previous)
			{
				previous->next = current->next;
			}
			else
			{
				s_amx_log.log_zones = current->next; /* first element */
			}
			free(current->name);
			free(current);
			break;
		}
		previous = current;
		current = current->next;
	}
}

void amx_log_disable_all_zones(void)
{
	amx_log_zone_t *current = NULL;

	while(s_amx_log.log_zones)
	{
		/* find the last zone */
		current = s_amx_log.log_zones->next;
		free(s_amx_log.log_zones->name);
		free(s_amx_log.log_zones);
		s_amx_log.log_zones = current;
	}
}

bool amx_log_zone_is_enabled(const char *zone_name)
{
	bool retval = false;
	amx_log_zone_t *current = s_amx_log.log_zones;
	/* go trough the zone list */
	while(current)
	{
		/* if we find the entry it is enabled */
		if(strcmp(current->name, zone_name) == 0)
		{
			retval = true;
			break;
		}
		current = current->next;
	}

	return retval;
}

amx_log_zone_t *amx_log_first_zone(void)
{
	return s_amx_log.log_zones;
}

amx_log_zone_t *amx_log_next_zone(amx_log_zone_t *reference) {
	return reference?reference->next:NULL;
}

amx_log_zone_t *amx_log_get_zone(const char *zone_name)
{
	amx_log_zone_t *current = s_amx_log.log_zones;

	/* go trough the zone list */
	while(current)
	{
		/* return the level if we find the entry */
		if(strcmp(current->name, zone_name) == 0)
		{
			break;
		}
		current = current->next;
	}
	return current;
}

const char *amx_log_get_zone_name(amx_log_zone_t *zone)
{
	return zone?zone->name:NULL;
}

amx_log_level_t amx_log_get_zone_level(amx_log_zone_t *zone)
{
	return zone?zone->level:0;
}

void amx_log_set_zone_level(amx_log_zone_t *zone, amx_log_level_t log_level)
{
	if (zone)
	{
		zone->level = log_level;
	}
}

void amx_log_va(amx_log_level_t log_level, const char *format, va_list ap)
{
	amx_log_output(log_level, s_amx_log.log_level, format, ap);
}

void amx_log_zone_va(amx_log_level_t log_level, const char *zone_name, const char *format, va_list ap)
{
	amx_log_zone_t *zone = amx_log_get_zone(zone_name);
	if (zone)
	{
		amx_log_output(log_level, zone->level, format, ap);
	}
}

void amx_log(amx_log_level_t log_level, const char *format, ...)
{
	va_list args;
	va_start(args,format);
	amx_log_va(log_level, format, args);
	va_end(args);
}

void amx_log_zone(amx_log_level_t log_level, const char *zone_name, const char *format, ...)
{
	va_list args;
	va_start(args,format);
	amx_log_zone_va(log_level, zone_name, format, args);
	va_end(args);
}
