#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_astack.h>

START_TEST (amx_astack_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_astack_new(NULL), -1);
	amx_astack_delete(NULL, NULL);
}
END_TEST

START_TEST (amx_astack_new_delete_check)
{
	amx_astack_t *astack = NULL;
	ck_assert_int_eq (amx_astack_new(&astack), 0);
	amx_astack_delete(&astack, NULL);
	ck_assert_ptr_eq (astack, NULL);
}
END_TEST

START_TEST (amx_astack_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_astack_init(NULL), -1);
	amx_astack_clean(NULL, NULL);
}
END_TEST

START_TEST (amx_astack_init_clean_check)
{
	amx_astack_t astack;

	ck_assert_int_eq (amx_astack_init(&astack), 0);
	amx_astack_clean(&astack, NULL);
}
END_TEST

START_TEST (amx_astack_push_check)
{
	amx_astack_t astack;
	char data[] = "abcdefg";

	ck_assert_int_eq (amx_astack_init(&astack), 0);
	ck_assert_int_eq (amx_astack_size(&astack), 0);
	ck_assert_int_eq (amx_astack_is_empty(&astack), true);

	ck_assert_ptr_ne (amx_astack_push(&astack, &data[0]), NULL);
	ck_assert_ptr_ne (amx_astack_push(&astack, &data[1]), NULL);
	ck_assert_ptr_ne (amx_astack_push(&astack, &data[2]), NULL);
	ck_assert_ptr_ne (amx_astack_push(&astack, &data[3]), NULL);

	ck_assert_int_eq (amx_astack_size(&astack), 4);
	ck_assert_int_eq (amx_astack_is_empty(&astack), false);

	amx_astack_clean(&astack, NULL);
}
END_TEST


START_TEST (amx_astack_pop_check)
{
	amx_astack_t astack;
	char data[] = "abcdefg";

	ck_assert_int_eq (amx_astack_init(&astack), 0);

	ck_assert_ptr_ne (amx_astack_push(&astack, &data[0]), NULL);
	ck_assert_ptr_ne (amx_astack_push(&astack, &data[1]), NULL);
	ck_assert_ptr_ne (amx_astack_push(&astack, &data[2]), NULL);
	ck_assert_ptr_ne (amx_astack_push(&astack, &data[3]), NULL);

	ck_assert_ptr_eq (amx_astack_pop(&astack), &data[3]);
	ck_assert_ptr_eq (amx_astack_pop(&astack), &data[2]);
	ck_assert_ptr_eq (amx_astack_pop(&astack), &data[1]);
	ck_assert_ptr_eq (amx_astack_pop(&astack), &data[0]);

	ck_assert_int_eq (amx_astack_size(&astack), 0);
	ck_assert_int_eq (amx_astack_is_empty(&astack), true);

	ck_assert_ptr_eq (amx_astack_pop(&astack), NULL);

	amx_astack_clean(&astack, NULL);
}
END_TEST

Suite *amx_astack_suite(void)
{
	Suite *s = suite_create ("amx_astack");
	TCase *tc = NULL;

	tc = tcase_create ("amx_astack_new_init_delete_clean");
	tcase_add_test (tc, amx_astack_new_delete_null_check);
	tcase_add_test (tc, amx_astack_new_delete_check);
	tcase_add_test (tc, amx_astack_init_clean_null_check);
	tcase_add_test (tc, amx_astack_init_clean_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_astack_push_pop");
	tcase_add_test (tc, amx_astack_push_check);
	tcase_add_test (tc, amx_astack_pop_check);
	suite_add_tcase (s, tc);

	return s;
}


