#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_lstack.h>

START_TEST (amx_lstack_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_lstack_new(NULL), -1);
	amx_lstack_delete(NULL, NULL);
}
END_TEST

START_TEST (amx_lstack_new_delete_check)
{
	amx_lstack_t *lstack = NULL;
	ck_assert_int_eq (amx_lstack_new(&lstack), 0);
	amx_lstack_delete(&lstack, NULL);
	ck_assert_ptr_eq (lstack, NULL);
}
END_TEST

START_TEST (amx_lstack_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_lstack_init(NULL), -1);
	amx_lstack_clean(NULL, NULL);
}
END_TEST

START_TEST (amx_lstack_init_clean_check)
{
	amx_lstack_t lstack;

	ck_assert_int_eq (amx_lstack_init(&lstack), 0);
	amx_lstack_clean(&lstack, NULL);
}
END_TEST

START_TEST (amx_lstack_push_check)
{
	amx_lstack_t lstack;
	amx_lstack_it_t it1;
	amx_lstack_it_t it2;
	amx_lstack_it_t it3;

	ck_assert_int_eq (amx_lstack_init(&lstack), 0);
	ck_assert_int_eq (amx_lstack_it_init(&it1), 0);
	ck_assert_int_eq (amx_lstack_it_init(&it2), 0);
	ck_assert_int_eq (amx_lstack_it_init(&it3), 0);

	ck_assert_int_eq (amx_lstack_push(&lstack, &it1), 0);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_int_eq (amx_lstack_push(&lstack, &it2), 0);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, &it1);
	ck_assert_int_eq (amx_lstack_push(&lstack, &it3), 0);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, &it3);
	ck_assert_ptr_eq (it2.prev, &it1);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, &it2);

	amx_lstack_clean(&lstack, NULL);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, NULL);
}
END_TEST


START_TEST (amx_lstack_pop_check)
{
	amx_lstack_t lstack;
	amx_lstack_it_t it1;
	amx_lstack_it_t it2;
	amx_lstack_it_t it3;

	ck_assert_int_eq (amx_lstack_init(&lstack), 0);
	ck_assert_int_eq (amx_lstack_it_init(&it1), 0);
	ck_assert_int_eq (amx_lstack_it_init(&it2), 0);
	ck_assert_int_eq (amx_lstack_it_init(&it3), 0);

	ck_assert_int_eq (amx_lstack_push(&lstack, &it1), 0);
	ck_assert_int_eq (amx_lstack_push(&lstack, &it2), 0);
	ck_assert_int_eq (amx_lstack_push(&lstack, &it3), 0);

	ck_assert_ptr_eq (amx_lstack_pop(&lstack), &it3);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, NULL);
	ck_assert_ptr_eq (amx_lstack_pop(&lstack), &it2);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (amx_lstack_pop(&lstack), &it1);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (amx_lstack_pop(&lstack), NULL);

	amx_lstack_clean(&lstack, NULL);
}
END_TEST

Suite *amx_lstack_suite(void)
{
	Suite *s = suite_create ("amx_lstack");
	TCase *tc = NULL;

	tc = tcase_create ("amx_lstack_new_init_delete_clean");
	tcase_add_test (tc, amx_lstack_new_delete_null_check);
	tcase_add_test (tc, amx_lstack_new_delete_check);
	tcase_add_test (tc, amx_lstack_init_clean_null_check);
	tcase_add_test (tc, amx_lstack_init_clean_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_lstack_push_pop");
	tcase_add_test (tc, amx_lstack_push_check);
	tcase_add_test (tc, amx_lstack_pop_check);
	suite_add_tcase (s, tc);

	return s;
}

