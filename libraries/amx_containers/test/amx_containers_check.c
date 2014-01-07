#include "amx_containers_check.h"

int main (void)
{
	int number_failed;

	SRunner *sr = srunner_create (amx_llist_suite());
	srunner_add_suite(sr, amx_lstack_suite());
	srunner_add_suite(sr, amx_llist_it_suite());
	srunner_add_suite(sr, amx_lstack_suite());
	srunner_add_suite(sr, amx_lqueue_suite());
	srunner_add_suite(sr, amx_array_suite());
	srunner_add_suite(sr, amx_aqueue_suite());
	srunner_add_suite(sr, amx_astack_suite());
	srunner_add_suite(sr, amx_rbuffer_suite());
	srunner_add_suite(sr, amx_hash_suite());
	srunner_add_suite(sr, amx_htable_suite());
	srunner_add_suite(sr, amx_var_type_suite());
	srunner_add_suite(sr, amx_var_suite());
	srunner_add_suite(sr, amx_var_string_suite());
	srunner_add_suite(sr, amx_var_int8_suite());
	srunner_add_suite(sr, amx_var_int16_suite());
	srunner_add_suite(sr, amx_var_int32_suite());
	srunner_add_suite(sr, amx_var_int64_suite());
	srunner_add_suite(sr, amx_var_uint8_suite());
	srunner_add_suite(sr, amx_var_uint16_suite());
	srunner_add_suite(sr, amx_var_uint32_suite());
	srunner_add_suite(sr, amx_var_uint64_suite());
	srunner_add_suite(sr, amx_var_float_suite());
	srunner_add_suite(sr, amx_var_double_suite());

	srunner_run_all (sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed (sr);
	srunner_free (sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
