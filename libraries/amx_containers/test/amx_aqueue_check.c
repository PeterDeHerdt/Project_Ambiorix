
#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_aqueue.h>

START_TEST (amx_aqueue_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_aqueue_new(NULL), -1);
	amx_aqueue_delete(NULL, NULL);
}
END_TEST

START_TEST (amx_aqueue_new_delete_check)
{
	amx_aqueue_t *aqueue = NULL;
	ck_assert_int_eq (amx_aqueue_new(&aqueue), 0);
	amx_aqueue_delete(&aqueue, NULL);
	ck_assert_ptr_eq (aqueue, NULL);
}
END_TEST

START_TEST (amx_aqueue_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_aqueue_init(NULL), -1);
	amx_aqueue_clean(NULL, NULL);
}
END_TEST

START_TEST (amx_aqueue_init_clean_check)
{
	amx_aqueue_t aqueue;

	ck_assert_int_eq (amx_aqueue_init(&aqueue), 0);
	amx_aqueue_clean(&aqueue, NULL);
}
END_TEST

START_TEST (amx_aqueue_add_check)
{
	amx_aqueue_t aqueue;
	char data[] = "abcdefg";

	ck_assert_int_eq (amx_aqueue_init(&aqueue), 0);
	ck_assert_int_eq (amx_aqueue_size(&aqueue), 0);
	ck_assert_int_eq (amx_aqueue_is_empty(&aqueue), true);

	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[0]), NULL);
	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[1]), NULL);
	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[2]), NULL);
	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[3]), NULL);

	ck_assert_int_eq (amx_aqueue_size(&aqueue), 4);
	ck_assert_int_eq (amx_aqueue_is_empty(&aqueue), false);

	amx_aqueue_clean(&aqueue, NULL);
}
END_TEST


START_TEST (amx_aqueue_remove_check)
{
	amx_aqueue_t aqueue;
	char data[] = "abcdefg";

	ck_assert_int_eq (amx_aqueue_init(&aqueue), 0);

	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[0]), NULL);
	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[1]), NULL);
	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[2]), NULL);
	ck_assert_ptr_ne (amx_aqueue_add(&aqueue, &data[3]), NULL);

	ck_assert_ptr_eq (amx_aqueue_remove(&aqueue), &data[0]);
	ck_assert_ptr_eq (amx_aqueue_remove(&aqueue), &data[1]);
	ck_assert_ptr_eq (amx_aqueue_remove(&aqueue), &data[2]);
	ck_assert_ptr_eq (amx_aqueue_remove(&aqueue), &data[3]);

	ck_assert_int_eq (amx_aqueue_size(&aqueue), 0);
	ck_assert_int_eq (amx_aqueue_is_empty(&aqueue), true);

	ck_assert_ptr_eq (amx_aqueue_remove(&aqueue), NULL);

	amx_aqueue_clean(&aqueue, NULL);
}
END_TEST

Suite *amx_aqueue_suite(void)
{
	Suite *s = suite_create ("amx_aqueue");
	TCase *tc = NULL;

	tc = tcase_create ("amx_aqueue_new_init_delete_clean");
	tcase_add_test (tc, amx_aqueue_new_delete_null_check);
	tcase_add_test (tc, amx_aqueue_new_delete_check);
	tcase_add_test (tc, amx_aqueue_init_clean_null_check);
	tcase_add_test (tc, amx_aqueue_init_clean_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_aqueue_add_remove");
	tcase_add_test (tc, amx_aqueue_add_check);
	tcase_add_test (tc, amx_aqueue_remove_check);
	suite_add_tcase (s, tc);

	return s;
}
