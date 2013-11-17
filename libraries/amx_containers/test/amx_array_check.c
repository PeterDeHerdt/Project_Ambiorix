#include "amx_containers_check.h"
#include <amx_containers/amx_array.h>

typedef struct _test_data {
	unsigned int number;
	char text[201];
} test_data_t;

START_TEST (amx_array_new_delete_null_check)
{
	amx_array_t *array = NULL;

	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_array_new(NULL, 0, 0), -1);
	ck_assert_int_eq (amx_array_new(&array, 0, 0), -1);
	ck_assert_ptr_eq (array, NULL);
	ck_assert_int_eq (amx_array_new(&array, 20, 0), -1);
	ck_assert_ptr_eq (array, NULL);
	amx_array_delete(NULL);
}
END_TEST

START_TEST (amx_array_new_delete_check)
{
	amx_array_t *array = NULL;

	// allocate array with 0 items
	ck_assert_int_eq (amx_array_new(&array, 0, sizeof(test_data_t)), 0);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_eq (array->buffer, NULL);
	ck_assert_int_eq (array->item_size, sizeof(test_data_t));
	ck_assert_int_eq (array->items, 0);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);

	// allocate array with 10 items
	ck_assert_int_eq (amx_array_new(&array, 10, sizeof(test_data_t)), 0);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_ne (array->buffer, NULL);
	ck_assert_int_eq (array->item_size, sizeof(test_data_t));
	ck_assert_int_eq (array->items, 10);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_init_clean_null_check)
{
	amx_array_t array;

	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_array_init(NULL, 0, 0), -1);
	ck_assert_int_eq (amx_array_init(&array, 0, 0), -1);
	ck_assert_int_eq (amx_array_init(&array, 20, 0), -1);
	amx_array_clean(NULL);
}
END_TEST

START_TEST (amx_array_init_clean_check)
{
	amx_array_t array;

	// initialize array with 0 items
	ck_assert_int_eq (amx_array_init(&array, 0, sizeof(test_data_t)), 0);
	ck_assert_ptr_eq (array.buffer, NULL);
	ck_assert_int_eq (array.item_size, sizeof(test_data_t));
	ck_assert_int_eq (array.items, 0);
	amx_array_clean(&array);

	// initialize array with 10 items
	ck_assert_int_eq (amx_array_init(&array, 10, sizeof(test_data_t)), 0);
	ck_assert_ptr_ne (array.buffer, NULL);
	ck_assert_int_eq (array.item_size, sizeof(test_data_t));
	ck_assert_int_eq (array.items, 10);
	amx_array_clean(&array);
}
END_TEST

START_TEST (amx_array_get_at_null_check)
{
	amx_array_t *array = NULL;
	const amx_array_it_t *it = NULL;

	it = amx_array_get_at(NULL, 5);
	ck_assert_ptr_eq((void *)it, NULL);

	ck_assert_int_eq (amx_array_new(&array, 0, sizeof(test_data_t)), 0);
	it = amx_array_get_at(array, 0);
	ck_assert_ptr_eq((void *)it, NULL);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_get_at_check)
{
	amx_array_t *array = NULL;
	const amx_array_it_t *it = NULL;

	// allocate array with 3 items
	ck_assert_int_eq (amx_array_new(&array, 3, sizeof(test_data_t)), 0);

	it = amx_array_get_at(array, 0);
	ck_assert_ptr_eq((void *)it, array->buffer);

	it = amx_array_get_at(array, 1);
	ck_assert_ptr_eq((void *)it, array->buffer + sizeof(test_data_t) + sizeof(amx_array_it_t));

	it = amx_array_get_at(array, 2);
	ck_assert_ptr_eq((void *)it, array->buffer + 2 * (sizeof(test_data_t) + sizeof(amx_array_it_t)));

	it = amx_array_get_at(array, 3);
	ck_assert_ptr_eq((void *)it, NULL);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_get_first_null_check)
{
	const amx_array_it_t *it = NULL;
	it = amx_array_get_first(NULL);
	ck_assert_ptr_eq((void *)it, NULL);
}
END_TEST

START_TEST (amx_array_get_first_check)
{
	amx_array_t *array = NULL;
	const amx_array_it_t *it = NULL;

	// allocate array with 3 items
	ck_assert_int_eq (amx_array_new(&array, 3, sizeof(test_data_t)), 0);

	it = amx_array_get_first(array);
	ck_assert_ptr_eq((void *)it, array->buffer);
	ck_assert_ptr_eq(it->array, array);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_get_last_null_check)
{
	const amx_array_it_t *it = NULL;
	it = amx_array_get_last(NULL);
	ck_assert_ptr_eq((void *)it, NULL);
}
END_TEST

START_TEST (amx_array_get_last_check)
{
	amx_array_t *array = NULL;
	const amx_array_it_t *it = NULL;

	// allocate array with 3 items
	ck_assert_int_eq (amx_array_new(&array, 3, sizeof(test_data_t)), 0);

	it = amx_array_get_last(array);
	ck_assert_ptr_eq((void *)it, (void *)amx_array_get_at(array, 2));
	ck_assert_ptr_eq(it->array, array);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_it_get_next_null_check)
{
	const amx_array_it_t *it = NULL;
	it = amx_array_it_get_next(NULL);
	ck_assert_ptr_eq((void *)it, NULL);
}
END_TEST

START_TEST (amx_array_it_get_next_check)
{
	amx_array_t *array = NULL;
	const amx_array_it_t *it = NULL;

	// allocate array with 3 items
	ck_assert_int_eq (amx_array_new(&array, 3, sizeof(test_data_t)), 0);

	it = amx_array_get_first(array);

	it = amx_array_it_get_next(it);
	ck_assert_ptr_eq((void *)it, (void *)amx_array_get_at(array, 1));
	ck_assert_ptr_eq(it->array, array);

	it = amx_array_it_get_next(it);
	ck_assert_ptr_eq((void *)it, (void *)amx_array_get_at(array, 2));
	ck_assert_ptr_eq(it->array, array);

	it = amx_array_it_get_next(it);
	ck_assert_ptr_eq((void *)it, NULL);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_it_get_previous_null_check)
{
	const amx_array_it_t *it = NULL;
	it = amx_array_it_get_previous(NULL);
	ck_assert_ptr_eq((void *)it, NULL);
}
END_TEST

START_TEST (amx_array_it_get_previous_check)
{
	amx_array_t *array = NULL;
	const amx_array_it_t *it = NULL;

	// allocate array with 3 items
	ck_assert_int_eq (amx_array_new(&array, 3, sizeof(test_data_t)), 0);

	it = amx_array_get_last(array);

	it = amx_array_it_get_previous(it);
	ck_assert_ptr_eq((void *)it, (void *)amx_array_get_at(array, 1));
	ck_assert_ptr_eq(it->array, array);

	it = amx_array_it_get_previous(it);
	ck_assert_ptr_eq((void *)it, (void *)amx_array_get_at(array, 0));
	ck_assert_ptr_eq(it->array, array);

	it = amx_array_it_get_previous(it);
	ck_assert_ptr_eq((void *)it, NULL);

	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_size_null_check)
{
	ck_assert_int_eq(amx_array_size(NULL), 0);
}
END_TEST

START_TEST (amx_array_size_check)
{
	amx_array_t *array = NULL;

	// allocate array with 0 items
	ck_assert_int_eq (amx_array_new(&array, 0, sizeof(test_data_t)), 0);
	ck_assert_int_eq(amx_array_size(array), 0);
	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);

	// allocate array with 3 items
	ck_assert_int_eq (amx_array_new(&array, 3, sizeof(test_data_t)), 0);
	ck_assert_int_eq(amx_array_size(array), 3);
	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_grow_null_check)
{
	ck_assert_int_eq(amx_array_grow(NULL, 0), -1);
}
END_TEST

START_TEST (amx_array_grow_check)
{
	amx_array_t *array = NULL;

	// allocate array with 0 items
	ck_assert_int_eq (amx_array_new(&array, 0, sizeof(test_data_t)), 0);
	ck_assert_int_eq(amx_array_size(array), 0);
	ck_assert_int_eq (amx_array_grow(array, 10), 0);
	ck_assert_int_eq(amx_array_size(array), 10);
	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);

	// allocate array with 3 items
	ck_assert_int_eq (amx_array_new(&array, 3, sizeof(test_data_t)), 0);
	ck_assert_int_eq(amx_array_size(array), 3);
	ck_assert_int_eq (amx_array_grow(array, 7), 0);
	ck_assert_int_eq(amx_array_size(array), 10);
	ck_assert_int_eq (amx_array_grow(array, 0), 0);
	ck_assert_int_eq(amx_array_size(array), 10);
	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_shrink_null_check)
{
	ck_assert_int_eq(amx_array_shrink(NULL, 0), -1);
}
END_TEST

START_TEST (amx_array_shrink_check)
{
	amx_array_t *array = NULL;

	// allocate array with 0 items
	ck_assert_int_eq (amx_array_new(&array, 0, sizeof(test_data_t)), 0);
	ck_assert_int_eq(amx_array_size(array), 0);
	ck_assert_int_eq (amx_array_shrink(array, 10), -1);
	ck_assert_int_eq(amx_array_size(array), 0);
	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);

	// allocate array with 10 items
	ck_assert_int_eq (amx_array_new(&array, 10, sizeof(test_data_t)), 0);
	ck_assert_int_eq(amx_array_size(array), 10);
	ck_assert_int_eq (amx_array_shrink(array, 7), 0);
	ck_assert_int_eq(amx_array_size(array), 3);
	ck_assert_int_eq (amx_array_shrink(array, 0), 0);
	ck_assert_int_eq(amx_array_size(array), 3);
	ck_assert_int_eq (amx_array_shrink(array, 3), 0);
	ck_assert_int_eq(amx_array_size(array), 0);
	amx_array_delete(&array);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

Suite *amx_array_suite(void)
{
	Suite *s = suite_create ("amx_array");
	TCase *tc = NULL;

	tc = tcase_create ("amx_array_new_init_delete_clean");
	tcase_add_test (tc, amx_array_new_delete_null_check);
	tcase_add_test (tc, amx_array_new_delete_check);
	tcase_add_test (tc, amx_array_init_clean_null_check);
	tcase_add_test (tc, amx_array_init_clean_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_get");
	tcase_add_test (tc, amx_array_get_at_null_check);
	tcase_add_test (tc, amx_array_get_at_check);
	tcase_add_test (tc, amx_array_get_first_null_check);
	tcase_add_test (tc, amx_array_get_first_check);
	tcase_add_test (tc, amx_array_get_last_null_check);
	tcase_add_test (tc, amx_array_get_last_check);
	tcase_add_test (tc, amx_array_it_get_next_null_check);
	tcase_add_test (tc, amx_array_it_get_next_check);
	tcase_add_test (tc, amx_array_it_get_previous_null_check);
	tcase_add_test (tc, amx_array_it_get_previous_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_size");
	tcase_add_test (tc, amx_array_size_null_check);
	tcase_add_test (tc, amx_array_size_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_grow_shrink");
	tcase_add_test (tc, amx_array_grow_null_check);
	tcase_add_test (tc, amx_array_grow_check);
	tcase_add_test (tc, amx_array_shrink_null_check);
	tcase_add_test (tc, amx_array_shrink_check);
	suite_add_tcase (s, tc);

	return s;
}
