#define _BSD_SOURCE
#define _POSIX_SOURCE
#include <stdlib.h>
#include <check.h>
#include <amx_log.h>

#include "mock.h"
#include "mock_malloc.h"

#ifndef MOCK_MALLOC
ck_mock_declare(malloc,50)
#endif

START_TEST (log_open_syslog)
{
	// open syslog
	ck_assert_int_eq( amx_log_open("identifier", amx_log_syslog), 0);
	ck_assert_int_ne ((int)amx_log_is_opened(), 0);
	ck_assert_int_eq (amx_log_get_type(), amx_log_syslog);
	ck_assert_str_eq (amx_log_get_identifier(), "identifier");
	amx_log_close();
	ck_assert_int_eq ((int)amx_log_is_opened(), 0);
	ck_assert_str_eq (amx_log_get_identifier(), "");
}
END_TEST

START_TEST (log_open_stdout)
{
	// open stdout
	ck_assert_int_eq (amx_log_open("test", amx_log_stdout), 0);
	ck_assert_int_ne ((int)amx_log_is_opened(), 0);
	ck_assert_int_eq (amx_log_get_type(), amx_log_stdout);
	ck_assert_str_eq (amx_log_get_identifier(), "test");
	amx_log_close();
	ck_assert_int_eq ((int)amx_log_is_opened(), 0);
	ck_assert_str_eq (amx_log_get_identifier(), "");
}
END_TEST

START_TEST (log_open_stderr)
{
	// open stderr
	ck_assert_int_eq (amx_log_open("id", amx_log_stderr), 0);
	ck_assert_int_ne ((int)amx_log_is_opened(), 0);
	ck_assert_int_eq (amx_log_get_type(), amx_log_stderr);
	ck_assert_str_eq (amx_log_get_identifier(), "id");
	amx_log_close();
	ck_assert_int_eq ((int)amx_log_is_opened(), 0);
	ck_assert_str_eq (amx_log_get_identifier(), "");
}
END_TEST

START_TEST (log_open_unknown)
{
	// open stderr
	ck_assert_int_eq (amx_log_open("id", 0xffff), -1);
	ck_assert_int_eq ((int)amx_log_is_opened(), 0);
	amx_log_close();
}
END_TEST

START_TEST (log_open_double)
{
	ck_assert_int_eq (amx_log_open("double", amx_log_stdout), 0);
	amx_log_set_level(amx_log_stack);

	// openining a second time should not have any effect, the second open must fail
	ck_assert_int_eq (amx_log_open("double_second", amx_log_stderr), -1);
	ck_assert_int_ne ((int)amx_log_is_opened(), 0);
	ck_assert_int_eq (amx_log_get_type(), amx_log_stdout);
	ck_assert_str_eq (amx_log_get_identifier(), "double");
	ck_assert_int_eq (amx_log_get_level(), amx_log_stack);

	amx_log_close();
}
END_TEST

START_TEST (log_close_not_opened)
{
	amx_log_close();

	ck_assert_int_eq ((int)amx_log_is_opened(), 0);
}
END_TEST

START_TEST (log_set_time_format)
{
	// when not opened, it is not possible to change the time format
	// the default time format is returned
	ck_assert_int_eq (amx_log_get_time_format(), amx_log_system_seconds);
	amx_log_set_time_format(amx_log_system_time);
	ck_assert_int_eq (amx_log_get_time_format(), amx_log_system_seconds);

	amx_log_open("test", amx_log_syslog);
	// after open the default is set - amx_log_system_seconds
	ck_assert_int_eq (amx_log_get_time_format(), amx_log_system_seconds);
	amx_log_set_time_format(amx_log_system_time);
	ck_assert_int_eq (amx_log_get_time_format(), amx_log_system_time);
	amx_log_close();
	// after closing the time format is reset to the default
	ck_assert_int_eq (amx_log_get_time_format(), amx_log_system_seconds);
}
END_TEST

START_TEST (log_set_level)
{
	// when not opened, it is not possible to change the log level
	// the default log level is returned
	ck_assert_int_eq (amx_log_get_level(), amx_log_fatal_error);
	amx_log_set_level(amx_log_error);
	ck_assert_int_eq (amx_log_get_level(), amx_log_fatal_error);

	amx_log_open("test", amx_log_syslog);
	ck_assert_int_eq (amx_log_get_level(), amx_log_fatal_error);
	amx_log_set_level(amx_log_error);
	ck_assert_int_eq (amx_log_get_level(), amx_log_error);
	amx_log_set_level(amx_log_warning);
	ck_assert_int_eq (amx_log_get_level(), amx_log_warning);
	amx_log_set_level(amx_log_notice);
	ck_assert_int_eq (amx_log_get_level(), amx_log_notice);
	amx_log_set_level(amx_log_lib_info);
	ck_assert_int_eq (amx_log_get_level(), amx_log_lib_info);
	amx_log_set_level(amx_log_app_info);
	ck_assert_int_eq (amx_log_get_level(), amx_log_app_info);
	amx_log_set_level(amx_log_stack);
	ck_assert_int_eq (amx_log_get_level(), amx_log_stack);
	amx_log_set_level(100);
	ck_assert_int_eq (amx_log_get_level(), 100);
	amx_log_close();
	// after closing the log level is reset to the default
	ck_assert_int_eq (amx_log_get_level(), amx_log_fatal_error);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (log_zone_enable_no_memory)
{
	amx_log_open("test", amx_log_syslog);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	amx_log_enable_zone("zone1", amx_log_stack);
	ck_mock_reset(malloc);

	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone1"), 0);
	amx_log_close();

}
END_TEST
#endif

START_TEST (log_zone_enable)
{
	// when not opened, it is not possible to enable zones
	amx_log_enable_zone("zone1", amx_log_stack);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone1"), 0);

	amx_log_open("test", amx_log_syslog);
	amx_log_enable_zone("zone1", amx_log_stack);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone1"), 0);
	amx_log_enable_zone("zone2", amx_log_stack);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone2"), 0);
	amx_log_close();
	// after closing the zone is disabled again
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone2"), 0);
}
END_TEST

START_TEST (log_zone_disable)
{
	amx_log_open("test", amx_log_syslog);
	amx_log_enable_zone("zone1", amx_log_stack);
	amx_log_enable_zone("zone2", amx_log_stack);
	amx_log_enable_zone("zone3", amx_log_stack);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone2"), 0);
	amx_log_disable_zone("zone2");
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone3"), 0);
	amx_log_disable_zone("zone1");
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone3"), 0);
	amx_log_disable_zone("zone3");
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone3"), 0);
	amx_log_disable_zone("unknown");
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone3"), 0);
	amx_log_close();
}
END_TEST

START_TEST (log_zone_disable_all)
{
	amx_log_open("test", amx_log_syslog);
	amx_log_enable_zone("zone1", amx_log_stack);
	amx_log_enable_zone("zone2", amx_log_stack);
	amx_log_enable_zone("zone3", amx_log_stack);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone3"), 0);
	amx_log_disable_all_zones();
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_eq ((int)amx_log_zone_is_enabled("zone3"), 0);
	amx_log_close();
}
END_TEST

START_TEST (log_zone_iteration)
{
	amx_log_open("test", amx_log_syslog);
	amx_log_enable_zone("zone1", amx_log_stack);
	amx_log_enable_zone("zone2", amx_log_stack);
	amx_log_enable_zone("zone3", amx_log_stack);
	amx_log_zone_t *z1 = amx_log_get_zone("zone1");
	amx_log_zone_t *z2 = amx_log_get_zone("zone2");
	amx_log_zone_t *z3 = amx_log_get_zone("zone3");

	// get the next zone, using NULL pointer as reference must return NULL
	ck_assert_ptr_eq(amx_log_next_zone(NULL), NULL);

	// zones are added in front, so the last one added is the first
	amx_log_zone_t *zone = amx_log_first_zone();
	ck_assert_ptr_eq(zone, z3);
	zone = amx_log_next_zone(zone);
	ck_assert_ptr_eq(zone, z2);
	zone = amx_log_next_zone(zone);
	ck_assert_ptr_eq(zone, z1);
	amx_log_close();
}
END_TEST

START_TEST (log_zone_set_level)
{
	amx_log_open("test", amx_log_syslog);
	amx_log_enable_zone("zone1", amx_log_stack);
	amx_log_zone_t *z1 = amx_log_get_zone("zone1");
	ck_assert_int_eq (amx_log_get_zone_level(z1), amx_log_stack);
	// enable the same zone twice, should only modify the log level
	amx_log_enable_zone("zone1", amx_log_notice);
	ck_assert_int_eq (amx_log_get_zone_level(z1), amx_log_notice);
	amx_log_set_zone_level(z1, amx_log_lib_info);
	ck_assert_int_eq (amx_log_get_zone_level(z1), amx_log_lib_info);
	// get the log level of a NULL pointer must return amx_log_fatal_error
	ck_assert_int_eq (amx_log_get_zone_level(NULL), amx_log_fatal_error);
	// set the log level of a NULL pointer should not crash
	amx_log_set_zone_level(NULL, amx_log_notice);
	amx_log_close();
}
END_TEST

START_TEST (log_zone_get_name)
{
	amx_log_open("test", amx_log_syslog);
	amx_log_enable_zone("zone1", amx_log_stack);
	amx_log_zone_t *z1 = amx_log_get_zone("zone1");
	ck_assert_str_eq (amx_log_get_zone_name(z1), "zone1");
	// get the name of a NULL pointer must return NULL pointer
	const char *name = amx_log_get_zone_name(NULL);
	ck_assert_ptr_eq ((void *)name, NULL);
	amx_log_close();
}
END_TEST

void environment_setup(void)
{
	setenv("AMX_LOG_ZONES","zone1,zone2=2,zone3=4,self=5",1);
	amx_log_open("test", amx_log_syslog);
}

void environment_teardown(void)
{
	amx_log_close();
	unsetenv("AMX_LOG_ZONES");
}

START_TEST (log_load_zones)
{
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone1"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone2"), 0);
	ck_assert_int_ne ((int)amx_log_zone_is_enabled("zone3"), 0);
}
END_TEST

START_TEST (log_load_zones_level)
{
	amx_log_zone_t *zone = amx_log_get_zone("zone1");
	ck_assert_int_eq (amx_log_get_zone_level(zone), amx_log_lib_info);
	zone = amx_log_get_zone("zone2");
	ck_assert_int_eq (amx_log_get_zone_level(zone), amx_log_warning);
	zone = amx_log_get_zone("zone3");
	ck_assert_int_eq (amx_log_get_zone_level(zone), amx_log_lib_info);
	ck_assert_int_eq (amx_log_get_level(), amx_log_app_info);
}
END_TEST

START_TEST (log_output)
{
	// when logging is not opened, no messages should be outputed
	amx_log(amx_log_error,"Log message stdout");

	amx_log_open("test", amx_log_stdout);
	// no messages with log level higher then the level set should be outputed
	amx_log_set_level(amx_log_warning);
	amx_log(amx_log_notice,"Log message stdout");

	amx_log_set_level(amx_log_stack);
	amx_log(amx_log_error,"Log message stdout");
	amx_log_set_time_format(amx_log_system_time);
	amx_log(amx_log_error,"Log message stdout");
	amx_log_set_level(100);
	amx_log(100,"Log message stdout, %s level", "custom");
	AMX_LOG_ERROR("Error message, using macro");
	amx_log_close();

	amx_log_open("test", amx_log_stderr);
	amx_log_set_level(amx_log_stack);
	amx_log(amx_log_error,"Log message stderr");
	amx_log_set_time_format(amx_log_system_time);
	amx_log(amx_log_error,"Log message stderr");
	amx_log_set_level(100);
	amx_log(100,"Log message stderr, %s level", "custom");
	amx_log_close();

	amx_log_open("test", amx_log_syslog);
	amx_log_set_level(amx_log_stack);
	amx_log(amx_log_error,"Log message syslog");
	amx_log_set_time_format(amx_log_system_time);
	amx_log(amx_log_error,"Log message syslog");
	amx_log_set_level(100);
	amx_log(100,"Log message syslog, %s level", "custom");
	amx_log_close();
}
END_TEST

START_TEST (log_output_zone)
{
	amx_log_open("test", amx_log_stdout);
	amx_log_enable_zone("zone1",amx_log_stack);
	amx_log_zone(amx_log_error, "zone1", "Log message stdout zone1");
	amx_log_zone(amx_log_error, "zone2", "Log message stdout zone2");
	amx_log_close();

	amx_log_open("test", amx_log_stderr);
	amx_log_enable_zone("zone1",amx_log_stack);
	amx_log_zone(amx_log_error, "zone1", "Log message stderr zone1");
	amx_log_zone(amx_log_error, "zone2", "Log message stderr zone2");
	amx_log_close();

	amx_log_open("test", amx_log_syslog);
	amx_log_enable_zone("zone1",amx_log_stack);
	amx_log_zone(amx_log_error, "zone1", "Log message syslog zone1");
	amx_log_zone(amx_log_error, "zone2", "Log message syslog zone2");
	amx_log_close();
}
END_TEST

Suite *amx_log_suite(void)
{
	Suite *s = suite_create ("amx_log_core");

	/* Core test case */
	TCase *tc = tcase_create ("amx_log_open_close");
	tcase_add_test (tc, log_open_syslog);
	tcase_add_test (tc, log_open_stdout);
	tcase_add_test (tc, log_open_stderr);
	tcase_add_test (tc, log_open_unknown);
	tcase_add_test (tc, log_open_double);
	tcase_add_test (tc, log_close_not_opened);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_log_get_set");
	tcase_add_test (tc, log_set_time_format);
	tcase_add_test (tc, log_set_level);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_log_zones");
#ifdef MOCK_MALLOC
	tcase_add_test (tc, log_zone_enable_no_memory);
#endif
	tcase_add_test (tc, log_zone_enable);
	tcase_add_test (tc, log_zone_disable);
	tcase_add_test (tc, log_zone_disable_all);
	tcase_add_test (tc, log_zone_iteration);
	tcase_add_test (tc, log_zone_set_level);
	tcase_add_test (tc, log_zone_get_name);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_log_environment");
	tcase_add_checked_fixture (tc, environment_setup, environment_teardown);
	tcase_add_test (tc, log_load_zones);
	tcase_add_test (tc, log_load_zones_level);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_log_output");
	tcase_add_test (tc, log_output);
	tcase_add_test (tc, log_output_zone);
	suite_add_tcase (s, tc);

	return s;
}

int main (void)
{
	int number_failed;
	Suite *s = amx_log_suite ();
	SRunner *sr = srunner_create (s);
	srunner_run_all (sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed (sr);
	srunner_free (sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
