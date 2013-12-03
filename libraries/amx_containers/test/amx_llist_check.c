#include "amx_containers_check.h"
#include <amx_containers/amx_llist.h>

START_TEST (amx_llist_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_llist_new(NULL), -1);
	amx_llist_delete(NULL, NULL);
}
END_TEST

START_TEST (amx_llist_new_delete_check)
{
	amx_llist_t *llist = NULL;
	ck_assert_int_ne (amx_llist_new(&llist), -1);
	ck_assert_ptr_ne (llist, NULL);
	ck_assert_ptr_eq (llist->head, NULL);
	ck_assert_ptr_eq (llist->tail, NULL);
	amx_llist_delete(&llist, NULL);
	ck_assert_ptr_eq (llist, NULL);
}
END_TEST

START_TEST (amx_llist_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_llist_init(NULL), -1);
	amx_llist_clean(NULL, NULL);
}
END_TEST

START_TEST (amx_llist_init_clean_check)
{
	amx_llist_t llist;

	ck_assert_int_eq (amx_llist_init(&llist), 0);
	ck_assert_ptr_eq (llist.head, NULL);
	ck_assert_ptr_eq (llist.tail, NULL);
	amx_llist_clean(&llist, NULL);
}
END_TEST

static amx_llist_it_t s_it_func_check;

static void check_it_delete_func(amx_llist_it_t *it)
{
	ck_assert_ptr_eq(it, &s_it_func_check);
}

START_TEST (amx_llist_delete_func_check)
{
	amx_llist_t *llist = NULL;

	// create list
	ck_assert_int_ne (amx_llist_new(&llist), -1);
	ck_assert_int_eq (amx_llist_it_init(&s_it_func_check), 0);
	ck_assert_int_eq (amx_llist_append(llist, &s_it_func_check), 0);

	amx_llist_delete(&llist, check_it_delete_func);
}
END_TEST

START_TEST (amx_llist_clean_func_check)
{
	amx_llist_t *llist = NULL;

	// create list
	ck_assert_int_ne (amx_llist_new(&llist), -1);
	ck_assert_int_eq (amx_llist_it_init(&s_it_func_check), 0);
	ck_assert_int_eq (amx_llist_append(llist, &s_it_func_check), 0);

	amx_llist_clean(llist, check_it_delete_func);

	amx_llist_delete(&llist, check_it_delete_func);
}
END_TEST

START_TEST (amx_llist_delete_cb_check)
{
	amx_llist_t *llist = NULL;

	// create list
	ck_assert_int_ne (amx_llist_new(&llist), -1);
	ck_assert_int_eq (amx_llist_it_init(&s_it_func_check), 0);
	ck_assert_int_eq (amx_llist_append(llist, &s_it_func_check), 0);
	amx_llist_delete(&llist, NULL);
	ck_assert_ptr_eq (s_it_func_check.next, NULL);
	ck_assert_ptr_eq (s_it_func_check.prev, NULL);
	ck_assert_ptr_eq (s_it_func_check.llist, NULL);
}
END_TEST

START_TEST (amx_llist_clean_cb_check)
{
	amx_llist_t *llist = NULL;

	// create list
	ck_assert_int_ne (amx_llist_new(&llist), -1);
	ck_assert_int_eq (amx_llist_it_init(&s_it_func_check), 0);
	ck_assert_int_eq (amx_llist_append(llist, &s_it_func_check), 0);
	amx_llist_clean(llist, NULL);
	ck_assert_ptr_eq (s_it_func_check.next, NULL);
	ck_assert_ptr_eq (s_it_func_check.prev, NULL);
	ck_assert_ptr_eq (s_it_func_check.llist, NULL);
	amx_llist_delete(&llist, NULL);
}
END_TEST

START_TEST(amx_llist_append_null_check)
{
	amx_llist_t *llist = NULL;
	amx_llist_it_t it1;

	ck_assert_int_eq (amx_llist_new(&llist), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);

	// passing NULL pointers should not lead to segfault
	ck_assert_int_ne (amx_llist_append(NULL, NULL), 0);
	ck_assert_int_ne (amx_llist_append(llist, NULL), 0);
	ck_assert_int_ne (amx_llist_append(NULL, &it1), 0);

	amx_llist_delete(&llist, NULL);
}
END_TEST

START_TEST(amx_llist_append_check)
{
	amx_llist_t *llist = NULL;
	amx_llist_it_t it1;
	amx_llist_it_t it2;

	ck_assert_int_eq (amx_llist_new(&llist), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);
	ck_assert_int_eq (amx_llist_it_init(&it2), 0);

	// append iterator 1
	ck_assert_int_eq (amx_llist_append(llist, &it1), 0);
	ck_assert_ptr_eq (llist->head, &it1);
	ck_assert_ptr_eq (llist->tail, &it1);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it1.llist, llist);

	// append iterator 2
	ck_assert_int_eq (amx_llist_append(llist, &it2), 0);
	ck_assert_ptr_eq (llist->head, &it1);
	ck_assert_ptr_eq (llist->tail, &it2);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it2.prev, &it1);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.llist, llist);

	amx_llist_delete(&llist, NULL);
}
END_TEST


START_TEST(amx_llist_prepend_null_check)
{
	amx_llist_t *llist = NULL;
	amx_llist_it_t it1;

	ck_assert_int_eq (amx_llist_new(&llist), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);

	// passing NULL pointers should not lead to segfault
	ck_assert_int_ne (amx_llist_prepend(NULL, NULL), 0);
	ck_assert_int_ne (amx_llist_prepend(llist, NULL), 0);
	ck_assert_int_ne (amx_llist_prepend(NULL, &it1), 0);

	amx_llist_delete(&llist, NULL);
}
END_TEST

START_TEST(amx_llist_prepend_check)
{
	amx_llist_t *llist = NULL;
	amx_llist_it_t it1;
	amx_llist_it_t it2;

	ck_assert_int_eq (amx_llist_new(&llist), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);
	ck_assert_int_eq (amx_llist_it_init(&it2), 0);

	// prepend iterator 1
	ck_assert_int_eq (amx_llist_prepend(llist, &it1), 0);
	ck_assert_ptr_eq (llist->head, &it1);
	ck_assert_ptr_eq (llist->tail, &it1);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, NULL);
	ck_assert_ptr_eq (it1.llist, llist);

	// append iterator 2
	ck_assert_int_eq (amx_llist_prepend(llist, &it2), 0);
	ck_assert_ptr_eq (llist->head, &it2);
	ck_assert_ptr_eq (llist->tail, &it1);
	ck_assert_ptr_eq (it2.next, &it1);
	ck_assert_ptr_eq (it1.prev, &it2);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it2.llist, llist);

	amx_llist_delete(&llist, NULL);
}
END_TEST

static amx_llist_t *llist1 = NULL;
static amx_llist_t *llist2 = NULL;
static amx_llist_it_t it1;
static amx_llist_it_t it2;
static amx_llist_it_t it3;

static void amx_llist_setup(void)
{
	ck_assert_int_eq (amx_llist_new(&llist1), 0);
	ck_assert_int_eq (amx_llist_new(&llist2), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);
	ck_assert_int_eq (amx_llist_it_init(&it2), 0);
	ck_assert_int_eq (amx_llist_it_init(&it3), 0);
	ck_assert_int_eq (amx_llist_append(llist1, &it1), 0);
	ck_assert_int_eq (amx_llist_append(llist1, &it2), 0);
	ck_assert_int_eq (amx_llist_append(llist1, &it3), 0);
}

static void amx_llist_teardown(void)
{
	amx_llist_delete(&llist1, NULL);
	amx_llist_delete(&llist2, NULL);
}

START_TEST(amx_llist_append_move_same_list_check)
{
	// move iterator 1 from head to tail in llist1
	ck_assert_int_eq (amx_llist_append(llist1, &it1), 0);
	ck_assert_ptr_eq (llist1->head, &it2);
	ck_assert_ptr_eq (llist1->tail, &it1);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it1.prev, &it3);
	ck_assert_ptr_eq (it2.next, &it3);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (it3.next, &it1);
	ck_assert_ptr_eq (it3.prev, &it2);
	ck_assert_ptr_eq (it1.llist, llist1);

	// move iterator 3 to tail in llist1
	ck_assert_int_eq (amx_llist_append(llist1, &it3), 0);
	ck_assert_ptr_eq (llist1->head, &it2);
	ck_assert_ptr_eq (llist1->tail, &it3);
	ck_assert_ptr_eq (it1.prev, &it2);
	ck_assert_ptr_eq (it1.next, &it3);
	ck_assert_ptr_eq (it2.next, &it1);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (it3.next, NULL);
	ck_assert_ptr_eq (it3.prev, &it1);
	ck_assert_ptr_eq (it3.llist, llist1);
}
END_TEST

START_TEST(amx_llist_prepend_move_same_list_check)
{
	// move iterator 3 from tail to head in llist1
	ck_assert_int_eq (amx_llist_prepend(llist1, &it3), 0);
	ck_assert_ptr_eq (llist1->head, &it3);
	ck_assert_ptr_eq (llist1->tail, &it2);
	ck_assert_ptr_eq (it1.next, &it2);
	ck_assert_ptr_eq (it1.prev, &it3);
	ck_assert_ptr_eq (it2.next, NULL);
	ck_assert_ptr_eq (it2.prev, &it1);
	ck_assert_ptr_eq (it3.next, &it1);
	ck_assert_ptr_eq (it3.prev, NULL);
	ck_assert_ptr_eq (it3.llist, llist1);

	// move iterator 2 to tail in llist1
	ck_assert_int_eq (amx_llist_prepend(llist1, &it2), 0);
	ck_assert_ptr_eq (llist1->head, &it2);
	ck_assert_ptr_eq (llist1->tail, &it1);
	ck_assert_ptr_eq (it1.prev, &it3);
	ck_assert_ptr_eq (it1.next, NULL);
	ck_assert_ptr_eq (it2.next, &it3);
	ck_assert_ptr_eq (it2.prev, NULL);
	ck_assert_ptr_eq (it3.next, &it1);
	ck_assert_ptr_eq (it3.prev, &it2);
	ck_assert_ptr_eq (it2.llist, llist1);
}
END_TEST

START_TEST(amx_llist_is_empty_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_llist_is_empty(NULL), true);
}
END_TEST

START_TEST(amx_llist_is_empty_check)
{
	amx_llist_t *llist = NULL;
	amx_llist_it_t it1;

	ck_assert_int_eq (amx_llist_new(&llist), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);

	ck_assert_int_eq (amx_llist_is_empty(llist), true);
	ck_assert_int_eq (amx_llist_append(llist, &it1), 0);
	ck_assert_int_eq (amx_llist_is_empty(llist), false);

	amx_llist_delete(&llist, NULL);
}
END_TEST

START_TEST(amx_llist_size_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_llist_size(NULL), 0);
}
END_TEST

START_TEST(amx_llist_size_check)
{
	amx_llist_t *llist = NULL;
	amx_llist_it_t it1;
	amx_llist_it_t it2;

	ck_assert_int_eq (amx_llist_new(&llist), 0);
	ck_assert_int_eq (amx_llist_it_init(&it1), 0);
	ck_assert_int_eq (amx_llist_it_init(&it2), 0);

	ck_assert_int_eq (amx_llist_size(llist), 0);
	ck_assert_int_eq (amx_llist_append(llist, &it1), 0);
	ck_assert_int_eq (amx_llist_size(llist), 1);
	ck_assert_int_eq (amx_llist_append(llist, &it2), 0);
	ck_assert_int_eq (amx_llist_size(llist), 2);

	amx_llist_clean(llist, NULL);
	ck_assert_int_eq (amx_llist_size(llist), 0);

	amx_llist_delete(&llist, NULL);
}
END_TEST

START_TEST(amx_llist_get_at_check)
{
	ck_assert_ptr_eq (amx_llist_get_at(NULL, 10), NULL);
	ck_assert_ptr_eq (amx_llist_get_at(llist1, 0), &it1);
	ck_assert_ptr_eq (amx_llist_get_at(llist1, 1), &it2);
	ck_assert_ptr_eq (amx_llist_get_at(llist1, 2), &it3);
	ck_assert_ptr_eq (amx_llist_get_at(llist1, 3), NULL);
}
END_TEST

START_TEST(amx_llist_set_at_check)
{
	amx_llist_it_t it;
	ck_assert_int_eq (amx_llist_set_at(NULL, 10, &it1), -1);

	ck_assert_int_eq (amx_llist_it_init(&it), 0);

	ck_assert_int_eq (amx_llist_set_at(llist1, 0, &it), 0);
	ck_assert_ptr_eq (it.next, &it1);
	ck_assert_ptr_eq (it.prev, NULL);
	amx_llist_it_take(&it);

	ck_assert_int_eq (amx_llist_set_at(llist1, 1, &it), 0);
	ck_assert_ptr_eq (it.next, &it2);
	ck_assert_ptr_eq (it.prev, &it1);
	amx_llist_it_take(&it);

	ck_assert_int_eq (amx_llist_set_at(llist1, 2, &it), 0);
	ck_assert_ptr_eq (it.next, &it3);
	ck_assert_ptr_eq (it.prev, &it2);
	amx_llist_it_take(&it);

	ck_assert_int_eq (amx_llist_set_at(llist1, 3, &it), 0);
	ck_assert_ptr_eq (it.next, NULL);
	ck_assert_ptr_eq (it.prev, &it3);
	amx_llist_it_take(&it);

	ck_assert_int_eq (amx_llist_set_at(llist1, 10, &it), -1);
	ck_assert_ptr_eq (it.next, NULL);
	ck_assert_ptr_eq (it.prev, NULL);
	amx_llist_it_take(&it);
}
END_TEST

Suite *amx_llist_suite(void)
{
	Suite *s = suite_create ("amx_llist");
	TCase *tc = NULL;

	tc = tcase_create ("amx_llist_new_init_delete_clean");
	tcase_add_test (tc, amx_llist_new_delete_null_check);
	tcase_add_test (tc, amx_llist_new_delete_check);
	tcase_add_test (tc, amx_llist_init_clean_null_check);
	tcase_add_test (tc, amx_llist_init_clean_check);
	tcase_add_test (tc, amx_llist_delete_func_check);
	tcase_add_test (tc, amx_llist_clean_func_check);
	tcase_add_test (tc, amx_llist_delete_cb_check);
	tcase_add_test (tc, amx_llist_clean_cb_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_llist_append_prepend");
	tcase_add_test (tc, amx_llist_append_null_check);
	tcase_add_test (tc, amx_llist_append_check);
	tcase_add_test (tc, amx_llist_prepend_null_check);
	tcase_add_test (tc, amx_llist_prepend_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_llist_move");
	tcase_add_checked_fixture (tc, amx_llist_setup, amx_llist_teardown);
	tcase_add_test (tc, amx_llist_append_move_same_list_check);
	tcase_add_test (tc, amx_llist_prepend_move_same_list_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_llist_empty_size");
	tcase_add_test (tc, amx_llist_is_empty_null_check);
	tcase_add_test (tc, amx_llist_is_empty_check);
	tcase_add_test (tc, amx_llist_size_null_check);
	tcase_add_test (tc, amx_llist_size_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_llist_indexed_check");
	tcase_add_checked_fixture (tc, amx_llist_setup, amx_llist_teardown);
	tcase_add_test (tc, amx_llist_get_at_check);
	tcase_add_test (tc, amx_llist_set_at_check);
	suite_add_tcase (s, tc);

	return s;
}
