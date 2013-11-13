#include "amx_containers_check.h"
#include <amx_containers/amx_llist.h>

START_TEST (amx_llist_it_init_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_llist_it_init(NULL), -1);
}
END_TEST

START_TEST(amx_llist_it_init_check)
{
	amx_llist_it_t it;

	ck_assert_int_eq (amx_llist_it_init(&it), 0);
	ck_assert_ptr_eq (it.next, NULL);
	ck_assert_ptr_eq (it.prev, NULL);
	ck_assert_ptr_eq (it.llist, NULL);
}
END_TEST

static amx_llist_it_t s_it_func_check;

static void check_it_delete_func(amx_llist_it_t *it)
{
	ck_assert_ptr_eq(it, &s_it_func_check);
}

START_TEST (amx_llist_it_clean_null_check)
{
	amx_llist_it_t it;

	ck_assert_int_eq (amx_llist_it_init(&it), 0);

	// passing NULL pointers should not lead to segfault
	amx_llist_it_clean(NULL, check_it_delete_func);
	amx_llist_it_clean(&it, NULL);
}
END_TEST

START_TEST(amx_llist_it_clean_check)
{
	amx_llist_it_t it;

	ck_assert_int_eq (amx_llist_it_init(&it), 0);
	amx_llist_it_clean(&it, NULL);
}
END_TEST

START_TEST(amx_llist_it_clean_cb_check)
{
	ck_assert_int_eq (amx_llist_it_init(&s_it_func_check), 0);
	amx_llist_it_clean(&s_it_func_check, check_it_delete_func);
}
END_TEST

static amx_llist_t *llist = NULL;
static amx_llist_it_t it1;
static amx_llist_it_t it2;
static amx_llist_it_t it3;

static void amx_llist_it_setup(void)
{
	ck_assert_int_eq (amx_llist_new(&llist), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);
	ck_assert_int_eq (amx_llist_it_init(&it2), 0);
	ck_assert_int_eq (amx_llist_it_init(&it3), 0);
	ck_assert_int_eq (amx_llist_append(llist, &it1), 0);
	ck_assert_int_eq (amx_llist_append(llist, &it2), 0);
	ck_assert_int_eq (amx_llist_append(llist, &it3), 0);

	ck_assert_int_eq (amx_llist_it_init(&s_it_func_check), 0);
}

static void amx_llist_it_teardown(void)
{
	amx_llist_delete(&llist, NULL);
}

START_TEST(amx_llist_it_take_null_check)
{
	amx_llist_it_take(NULL);
}
END_TEST

START_TEST(amx_llist_it_take_check)
{
	amx_llist_it_take(&it2);
	ck_assert_ptr_eq (llist->head, &it1);
	ck_assert_ptr_eq (llist->tail, &it3);
	ck_assert_ptr_eq (it1.next, &it3);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, &it1);
	ck_assert_ptr_eq (it2.llist, NULL);
}
END_TEST

START_TEST(amx_llist_it_take_double_check)
{
	amx_llist_it_take(&it2);
	// remove the iterator again should not lead to segfault
	amx_llist_it_take(&it2);
}
END_TEST

START_TEST(amx_llist_it_insert_before_null_check)
{
	ck_assert_int_eq (amx_llist_it_insert_before(NULL, NULL), -1);
	ck_assert_int_eq (amx_llist_it_insert_before(&it1, NULL), -1);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it1.llist, llist);
	ck_assert_int_eq (amx_llist_it_insert_before(NULL, &it1), -1);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it1.llist, llist);
}
END_TEST

START_TEST(amx_llist_it_insert_before_check)
{
	ck_assert_int_eq (amx_llist_it_insert_before(&it2, &s_it_func_check), 0);
	ck_assert_ptr_eq (it1.next, &s_it_func_check);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, &it3);
	ck_assert_ptr_eq (it2.prev, &s_it_func_check);
	ck_assert_ptr_eq (s_it_func_check.next, &it2);
	ck_assert_ptr_eq (s_it_func_check.prev, &it1);
	ck_assert_ptr_eq (s_it_func_check.llist, llist);
}
END_TEST

START_TEST(amx_llist_it_insert_before_head_check)
{
	ck_assert_int_eq (amx_llist_it_insert_before(&it1, &s_it_func_check), 0);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, &s_it_func_check);
	ck_assert_ptr_eq (s_it_func_check.next, &it1);
	ck_assert_ptr_eq (s_it_func_check.prev, NULL);
	ck_assert_ptr_eq (s_it_func_check.llist, llist);
}
END_TEST

START_TEST(amx_llist_it_insert_before_invalid_it_check)
{
	amx_llist_it_take(&it2);

	ck_assert_int_eq (amx_llist_it_insert_before(&it2, &s_it_func_check), -1);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (s_it_func_check.next, NULL);
	ck_assert_ptr_eq (s_it_func_check.prev, NULL);
	ck_assert_ptr_eq (s_it_func_check.llist, NULL);
}
END_TEST

START_TEST(amx_llist_it_insert_after_null_check)
{
	ck_assert_int_eq (amx_llist_it_insert_after(NULL, NULL), -1);
	ck_assert_int_eq (amx_llist_it_insert_after(&it1, NULL), -1);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it1.llist, llist);
	ck_assert_int_eq (amx_llist_it_insert_after(NULL, &it1), -1);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it1.llist, llist);
}
END_TEST

START_TEST(amx_llist_it_insert_after_check)
{
	ck_assert_int_eq (amx_llist_it_insert_after(&it2, &s_it_func_check), 0);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, &s_it_func_check);
	ck_assert_ptr_eq (it2.next, &s_it_func_check);
	ck_assert_ptr_eq (it2.prev, &it1);
	ck_assert_ptr_eq (s_it_func_check.next, &it3);
	ck_assert_ptr_eq (s_it_func_check.prev, &it2);
	ck_assert_ptr_eq (s_it_func_check.llist, llist);
}
END_TEST

START_TEST(amx_llist_it_insert_after_tail_check)
{
	ck_assert_int_eq (amx_llist_it_insert_after(&it3, &s_it_func_check), 0);
	ck_assert_ptr_eq (it3.next, &s_it_func_check);
	ck_assert_ptr_eq (it3.prev, &it2);
	ck_assert_ptr_eq (s_it_func_check.next, NULL);
	ck_assert_ptr_eq (s_it_func_check.prev, &it3);
	ck_assert_ptr_eq (s_it_func_check.llist, llist);
}
END_TEST

START_TEST(amx_llist_it_insert_after_invalid_it_check)
{
	amx_llist_it_take(&it2);

	ck_assert_int_eq (amx_llist_it_insert_after(&it2, &s_it_func_check), -1);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (s_it_func_check.next, NULL);
	ck_assert_ptr_eq (s_it_func_check.prev, NULL);
	ck_assert_ptr_eq (s_it_func_check.llist, NULL);
}
END_TEST

START_TEST(amx_llist_it_index_of_check)
{
	amx_llist_it_t it;

	ck_assert_int_eq (amx_llist_it_init(&it), 0);
	ck_assert_uint_eq (amx_llist_it_index_of(NULL), AMX_LLIST_RANGE);
	ck_assert_uint_eq (amx_llist_it_index_of(&it1), 0);
	ck_assert_uint_eq (amx_llist_it_index_of(&it2), 1);
	ck_assert_uint_eq (amx_llist_it_index_of(&it3), 2);
	ck_assert_uint_eq (amx_llist_it_index_of(&it), AMX_LLIST_RANGE);
}
END_TEST

Suite *amx_llist_it_suite(void)
{
	Suite *s = suite_create ("amx_llist_it");
	TCase *tc = NULL;

	tc = tcase_create ("amx_llist_it_init_clean");
	tcase_add_test (tc, amx_llist_it_init_null_check);
	tcase_add_test (tc, amx_llist_it_init_check);
	tcase_add_test (tc, amx_llist_it_clean_null_check);
	tcase_add_test (tc, amx_llist_it_clean_check);
	tcase_add_test (tc, amx_llist_it_clean_cb_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_llist_it_take");
	tcase_add_checked_fixture (tc, amx_llist_it_setup, amx_llist_it_teardown);
	tcase_add_test (tc, amx_llist_it_take_null_check);
	tcase_add_test (tc, amx_llist_it_take_check);
	tcase_add_test (tc, amx_llist_it_take_double_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_llist_it_insert");
	tcase_add_checked_fixture (tc, amx_llist_it_setup, amx_llist_it_teardown);
	tcase_add_test (tc, amx_llist_it_insert_before_null_check);
	tcase_add_test (tc, amx_llist_it_insert_before_check);
	tcase_add_test (tc, amx_llist_it_insert_before_head_check);
	tcase_add_test (tc, amx_llist_it_insert_before_invalid_it_check);
	tcase_add_test (tc, amx_llist_it_insert_after_null_check);
	tcase_add_test (tc, amx_llist_it_insert_after_check);
	tcase_add_test (tc, amx_llist_it_insert_after_tail_check);
	tcase_add_test (tc, amx_llist_it_insert_after_invalid_it_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_llist_it_index");
	tcase_add_checked_fixture (tc, amx_llist_it_setup, amx_llist_it_teardown);
	tcase_add_test (tc, amx_llist_it_index_of_check);
	suite_add_tcase (s, tc);

	return s;
}
