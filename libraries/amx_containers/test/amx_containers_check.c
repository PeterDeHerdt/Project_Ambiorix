#include "amx_containers_check.h"

int main (void)
{
	int number_failed;

	SRunner *sr = srunner_create (amx_llist_suite());
	srunner_add_suite(sr, amx_lstack_suite());
	srunner_add_suite(sr, amx_llist_it_suite());
	srunner_add_suite(sr, amx_lstack_suite());
	srunner_add_suite(sr, amx_lqueue_suite());

	srunner_run_all (sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed (sr);
	srunner_free (sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
