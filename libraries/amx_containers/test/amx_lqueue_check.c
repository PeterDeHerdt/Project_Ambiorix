
#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_lqueue.h>

START_TEST (amx_lqueue_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_lqueue_new(NULL), -1);
	amx_lqueue_delete(NULL, NULL);
}
END_TEST

START_TEST (amx_lqueue_new_delete_check)
{
	amx_lqueue_t *lqueue = NULL;
	ck_assert_int_eq (amx_lqueue_new(&lqueue), 0);
	amx_lqueue_delete(&lqueue, NULL);
	ck_assert_ptr_eq (lqueue, NULL);
}
END_TEST

START_TEST (amx_lqueue_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_lqueue_init(NULL), -1);
	amx_lqueue_clean(NULL, NULL);
}
END_TEST

START_TEST (amx_lqueue_init_clean_check)
{
	amx_lqueue_t lqueue;

	ck_assert_int_eq (amx_lqueue_init(&lqueue), 0);
	amx_lqueue_clean(&lqueue, NULL);
}
END_TEST

START_TEST (amx_lqueue_add_check)
{
	amx_lqueue_t lqueue;
	amx_lqueue_it_t it1;
	amx_lqueue_it_t it2;
	amx_lqueue_it_t it3;

	ck_assert_int_eq (amx_lqueue_init(&lqueue), 0);
	ck_assert_int_eq (amx_lqueue_it_init(&it1), 0);
	ck_assert_int_eq (amx_lqueue_it_init(&it2), 0);
	ck_assert_int_eq (amx_lqueue_it_init(&it3), 0);

	ck_assert_int_eq (amx_lqueue_add(&lqueue, &it1), 0);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_int_eq (amx_lqueue_add(&lqueue, &it2), 0);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, &it1);
	ck_assert_int_eq (amx_lqueue_add(&lqueue, &it3), 0);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, &it3);
	ck_assert_ptr_eq (it2.prev, &it1);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, &it2);

	amx_lqueue_clean(&lqueue, NULL);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, NULL);
}
END_TEST


START_TEST (amx_lqueue_remove_check)
{
	amx_lqueue_t lqueue;
	amx_lqueue_it_t it1;
	amx_lqueue_it_t it2;
	amx_lqueue_it_t it3;

	ck_assert_int_eq (amx_lqueue_init(&lqueue), 0);
	ck_assert_int_eq (amx_lqueue_it_init(&it1), 0);
	ck_assert_int_eq (amx_lqueue_it_init(&it2), 0);
	ck_assert_int_eq (amx_lqueue_it_init(&it3), 0);

	ck_assert_int_eq (amx_lqueue_add(&lqueue, &it1), 0);
	ck_assert_int_eq (amx_lqueue_add(&lqueue, &it2), 0);
	ck_assert_int_eq (amx_lqueue_add(&lqueue, &it3), 0);

	ck_assert_ptr_eq (amx_lqueue_remove(&lqueue), &it1);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (amx_lqueue_remove(&lqueue), &it2);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (amx_lqueue_remove(&lqueue), &it3);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, NULL);
	ck_assert_ptr_eq (amx_lqueue_remove(&lqueue), NULL);

	amx_lqueue_clean(&lqueue, NULL);
}
END_TEST

Suite *amx_lqueue_suite(void)
{
	Suite *s = suite_create ("amx_lqueue");
	TCase *tc = NULL;

	tc = tcase_create ("amx_lqueue_new_init_delete_clean");
	tcase_add_test (tc, amx_lqueue_new_delete_null_check);
	tcase_add_test (tc, amx_lqueue_new_delete_check);
	tcase_add_test (tc, amx_lqueue_init_clean_null_check);
	tcase_add_test (tc, amx_lqueue_init_clean_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_lqueue_add_pop");
	tcase_add_test (tc, amx_lqueue_add_check);
	tcase_add_test (tc, amx_lqueue_remove_check);
	suite_add_tcase (s, tc);

	return s;
}

