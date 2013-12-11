#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_array.h>

#include "mock_malloc.h"

static int counter = 0;

static amx_array_t *array1 = NULL;
char data[] = "abcdefghij";

static void amx_array_setup(void)
{
	ck_assert_int_ne (amx_array_new(&array1, 10), -1);
	ck_assert_ptr_ne (array1, NULL);
	ck_assert_ptr_ne (array1->buffer, NULL);
	
	amx_array_set_data_at(array1, 3, &data[3]);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 3);
	
	amx_array_set_data_at(array1, 6, &data[6]);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 6);
	
	amx_array_set_data_at(array1, 9, &data[9]);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 9);
}

static void amx_array_teardown(void)
{
	amx_array_delete(&array1, NULL);
}

static void amx_check_array_it_delete(amx_array_it_t *it)
{
	counter++;
	ck_assert_ptr_ne(it->data, NULL);
}

START_TEST (amx_array_new_delete_null_check)
{
	amx_array_t *array = NULL;

	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_array_new(NULL, 0), -1);
	ck_assert_ptr_eq (array, NULL);
	amx_array_delete(NULL, NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_array_new_no_memory_check)
{
	amx_array_t *array = NULL;

	// second malloc fails
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_array_new(&array, 10);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_eq (array, NULL);

	// first malloc fails
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_array_new(&array, 10);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST
#endif

START_TEST (amx_array_new_delete_check)
{
	amx_array_t *array = NULL;

	// allocate array with 0 items
	ck_assert_int_eq (amx_array_new(&array, 0), 0);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_eq (array->buffer, NULL);
	ck_assert_int_eq (array->items, 0);

	amx_array_delete(&array, NULL);
	ck_assert_ptr_eq (array, NULL);

	// allocate array with 10 items
	ck_assert_int_eq (amx_array_new(&array, 10), 0);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_ne (array->buffer, NULL);
	ck_assert_int_eq (array->items, 10);

	amx_array_delete(&array, NULL);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_delete_cb_check)
{
	amx_array_t *array = NULL;

	// allocate array with 10 items
	ck_assert_int_eq (amx_array_new(&array, 10), 0);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_ne (array->buffer, NULL);
	ck_assert_int_eq (array->items, 10);
	counter = 0;
	amx_array_delete(&array, amx_check_array_it_delete);
	ck_assert_int_eq (counter, 0);
	ck_assert_ptr_eq (array, NULL);

	// allocate array with 10 items
	ck_assert_int_eq (amx_array_new(&array, 10), 0);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_ne (array->buffer, NULL);
	ck_assert_int_eq (array->items, 10);
	const char data[10] = "1234567890";
	for(unsigned int index = 0; index < 10; index++)
	{
		ck_assert_ptr_eq(amx_array_set_data_at(array, index, (void *)&data[index]), &array->buffer[index]);
	}
	counter = 0;
	amx_array_delete(&array, amx_check_array_it_delete);
	ck_assert_int_eq (counter, 10);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_array_init(NULL, 0), -1);
	amx_array_clean(NULL, NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_array_init_no_memory_check)
{
	amx_array_t array;

	// first malloc fails
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_array_init(&array, 10);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
}
END_TEST
#endif

START_TEST (amx_array_init_clean_check)
{
	amx_array_t array;

	// initialize array with 0 items
	ck_assert_int_eq (amx_array_init(&array, 0), 0);
	ck_assert_ptr_eq (array.buffer, NULL);
	ck_assert_int_eq (array.items, 0);
	amx_array_clean(&array, NULL);
	ck_assert_int_eq (array.items, 0);

	// initialize array with 10 items
	ck_assert_int_eq (amx_array_init(&array, 10), 0);
	ck_assert_ptr_ne (array.buffer, NULL);
	ck_assert_int_eq (array.items, 10);
	amx_array_clean(&array, NULL);
	ck_assert_int_eq (array.items, 0);
}
END_TEST

START_TEST (amx_array_clean_cb_check)
{
	amx_array_t array;

	// initialize array with 10 items
	ck_assert_int_eq (amx_array_init(&array, 10), 0);
	ck_assert_ptr_ne (array.buffer, NULL);
	ck_assert_int_eq (array.items, 10);
	counter = 0;
	amx_array_clean(&array, NULL);
	ck_assert_int_eq (counter, 0);
	ck_assert_int_eq (array.items, 0);

	// initialize array with 10 items
	ck_assert_int_eq (amx_array_init(&array, 10), 0);
	ck_assert_ptr_ne (array.buffer, amx_check_array_it_delete);
	ck_assert_int_eq (array.items, 10);
	const char data[10] = "1234567890";
	for(unsigned int index = 0; index < 10; index++)
	{
		ck_assert_ptr_eq(amx_array_set_data_at(&array, index, (void *)&data[index]), &array.buffer[index]);
	}
	counter = 0;
	amx_array_clean(&array, amx_check_array_it_delete);
	ck_assert_int_eq (array.items, 0);
	ck_assert_int_eq (counter, 10);
}
END_TEST

START_TEST (amx_array_get_at_null_check)
{
	amx_array_t *array = NULL;
	amx_array_it_t *it = NULL;

	it = amx_array_get_at(NULL, 5);
	ck_assert_ptr_eq(it, NULL);

	ck_assert_int_eq (amx_array_new(&array, 0), 0);
	it = amx_array_get_at(array, 0);
	ck_assert_ptr_eq(it, NULL);

	amx_array_delete(&array, NULL);
	ck_assert_ptr_eq (array, NULL);
}
END_TEST

START_TEST (amx_array_get_at_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_at(array1, 0);
	ck_assert_ptr_eq(it, &array1->buffer[0]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, NULL);

	it = amx_array_get_at(array1, 1);
	ck_assert_ptr_eq(it, &array1->buffer[1]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, NULL);

	it = amx_array_get_at(array1, 2);
	ck_assert_ptr_eq(it, &array1->buffer[2]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, NULL);

	it = amx_array_get_at(array1, 3);
	ck_assert_ptr_eq(it, &array1->buffer[3]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, &data[3]);

	it = amx_array_get_at(array1, 10);
	ck_assert_ptr_eq(it, NULL);
}
END_TEST

START_TEST (amx_array_get_first_null_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_first(NULL);
	ck_assert_ptr_eq(it, NULL);
}
END_TEST

START_TEST (amx_array_get_first_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_first(array1);
	ck_assert_ptr_eq(it, &array1->buffer[3]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, &data[3]);
}
END_TEST

START_TEST (amx_array_get_first_empty_check)
{
	amx_array_t *array = NULL;
	ck_assert_int_ne (amx_array_new(&array, 10), -1);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_ne (array->buffer, NULL);

	amx_array_it_t *it = NULL;
	it = amx_array_get_first(array);
	ck_assert_ptr_eq(it, NULL);

	amx_array_delete(&array, NULL);
}
END_TEST

START_TEST (amx_array_get_last_null_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_last(NULL);
	ck_assert_ptr_eq(it, NULL);
}
END_TEST

START_TEST (amx_array_get_last_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_last(array1);
	ck_assert_ptr_eq(it, &array1->buffer[9]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, &data[9]);
}
END_TEST

START_TEST (amx_array_get_last_empty_check)
{
	amx_array_t *array = NULL;
	ck_assert_int_ne (amx_array_new(&array, 10), -1);
	ck_assert_ptr_ne (array, NULL);
	ck_assert_ptr_ne (array->buffer, NULL);

	amx_array_it_t *it = NULL;
	it = amx_array_get_last(array);
	ck_assert_ptr_eq(it, NULL);

	amx_array_delete(&array, NULL);
}
END_TEST

START_TEST (amx_array_it_get_next_null_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_it_get_next(NULL);
	ck_assert_ptr_eq(it, NULL);
}
END_TEST

START_TEST (amx_array_it_get_next_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_first(array1);

	it = amx_array_it_get_next(it);
	ck_assert_ptr_eq(it, &array1->buffer[6]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, &data[6]);

	it = amx_array_it_get_next(it);
	ck_assert_ptr_eq(it, &array1->buffer[9]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, &data[9]);

	it = amx_array_it_get_next(it);
	ck_assert_ptr_eq(it, NULL);
}
END_TEST

START_TEST (amx_array_it_get_previous_null_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_it_get_previous(NULL);
	ck_assert_ptr_eq(it, NULL);
}
END_TEST

START_TEST (amx_array_it_get_previous_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_last(array1);

	it = amx_array_it_get_previous(it);
	ck_assert_ptr_eq(it, &array1->buffer[6]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, &data[6]);

	it = amx_array_it_get_previous(it);
	ck_assert_ptr_eq(it, &array1->buffer[3]);
	ck_assert_ptr_eq(it->array, array1);
	ck_assert_ptr_eq(it->data, &data[3]);

	it = amx_array_it_get_previous(it);
	ck_assert_ptr_eq(it, NULL);
}
END_TEST

START_TEST (amx_array_it_get_data_null_check)
{
	ck_assert_ptr_eq(amx_array_it_get_data(NULL), NULL);
}
END_TEST

START_TEST (amx_array_it_get_data_check)
{
	amx_array_it_t *it = NULL;
	it = amx_array_get_at(array1, 0);
	ck_assert_ptr_eq(amx_array_it_get_data(it), NULL);

	it = amx_array_get_at(array1, 1);
	ck_assert_ptr_eq(amx_array_it_get_data(it), NULL);

	it = amx_array_get_at(array1, 2);
	ck_assert_ptr_eq(amx_array_it_get_data(it), NULL);

	it = amx_array_get_at(array1, 3);
	ck_assert_ptr_eq(amx_array_it_get_data(it), &data[3]);
}
END_TEST

START_TEST (amx_array_is_empty_null_check)
{
	ck_assert_int_eq(amx_array_is_empty(NULL), true);
}
END_TEST

START_TEST (amx_array_is_empty_check)
{
	ck_assert_int_eq(amx_array_is_empty(array1), false);
	amx_array_clean(array1, NULL);
	ck_assert_int_eq(amx_array_is_empty(array1), true);
}
END_TEST

START_TEST (amx_array_size_null_check)
{
	ck_assert_int_eq(amx_array_size(NULL), 0);
}
END_TEST

START_TEST (amx_array_size_check)
{
	ck_assert_int_eq(amx_array_size(array1), 3);
}
END_TEST

START_TEST (amx_array_capacity_null_check)
{
	ck_assert_int_eq(amx_array_capacity(NULL), 0);
}
END_TEST

START_TEST (amx_array_capacity_check)
{
	ck_assert_int_eq(amx_array_capacity(array1), 10);
}
END_TEST

START_TEST (amx_array_grow_null_check)
{
	ck_assert_int_eq(amx_array_grow(NULL, 0), -1);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_array_grow_no_memory_check)
{
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_array_grow(array1, 10);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
}
END_TEST
#endif

START_TEST (amx_array_grow_check)
{
	ck_assert_int_eq (amx_array_grow(array1, 10), 0);
	ck_assert_int_eq (amx_array_capacity(array1), 20);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 9);
	
	ck_assert_int_eq (amx_array_grow(array1, 0), 0);
	ck_assert_int_eq (amx_array_capacity(array1), 20);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 9);
}
END_TEST

START_TEST (amx_array_shrink_null_check)
{
	ck_assert_int_eq(amx_array_shrink(NULL, 0, NULL), -1);
}
END_TEST

START_TEST (amx_array_shrink_check)
{
	ck_assert_int_eq (amx_array_shrink(array1, 15, NULL), -1);

	ck_assert_int_eq (amx_array_shrink(array1, 5, NULL), 0);
	ck_assert_int_eq(amx_array_capacity(array1), 5);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 3);

	ck_assert_int_eq (amx_array_shrink(array1, 3, NULL), 0);
	ck_assert_int_eq(amx_array_capacity(array1), 2);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);

	ck_assert_int_eq (amx_array_shrink(array1, 2, NULL), 0);
	ck_assert_int_eq(amx_array_capacity(array1), 0);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);
}
END_TEST

START_TEST (amx_array_shift_null_check)
{
	ck_assert_int_eq(amx_array_shift_left(NULL, 0, NULL), -1);
	ck_assert_int_eq(amx_array_shift_right(NULL, 0, NULL), -1);
}
END_TEST

START_TEST (amx_array_shift_left_check)
{
	ck_assert_int_eq (amx_array_shift_left(array1, 15, NULL), -1);
	ck_assert_int_eq (amx_array_shift_left(array1, 0, NULL), 0);

	ck_assert_int_eq (amx_array_shift_left(array1, 1, NULL), 0);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 3);
	ck_assert_int_eq (array1->first_used, 2);
	ck_assert_int_eq (array1->last_used, 8);
	ck_assert_ptr_eq (array1->buffer[2].data, &data[3]);
	ck_assert_ptr_eq (array1->buffer[3].data, NULL);
	ck_assert_ptr_eq (array1->buffer[5].data, &data[6]);
	ck_assert_ptr_eq (array1->buffer[6].data, NULL);
	ck_assert_ptr_eq (array1->buffer[8].data, &data[9]);
	ck_assert_ptr_eq (array1->buffer[9].data, NULL);

	ck_assert_int_eq (amx_array_shift_left(array1, 3, NULL), 0);
	ck_assert_int_eq(amx_array_capacity(array1), 10);
	ck_assert_int_eq(amx_array_size(array1), 2);
	ck_assert_ptr_eq (array1->buffer[2].data, &data[6]);
	ck_assert_ptr_eq (array1->buffer[5].data, &data[9]);
	ck_assert_ptr_eq (array1->buffer[8].data, NULL);
	ck_assert_int_eq (array1->first_used, 2);
	ck_assert_int_eq (array1->last_used, 5);

	ck_assert_int_eq (amx_array_shift_left(array1, 6, NULL), 0);
	ck_assert_int_eq(amx_array_capacity(array1), 10);
	ck_assert_int_eq(amx_array_size(array1), 0);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);
}
END_TEST

START_TEST (amx_array_shift_left_all_check)
{
	ck_assert_int_eq (amx_array_shift_left(array1, 10, NULL), 0);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 0);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);
	ck_assert_ptr_eq (array1->buffer[3].data, NULL);
	ck_assert_ptr_eq (array1->buffer[6].data, NULL);
	ck_assert_ptr_eq (array1->buffer[9].data, NULL);
}
END_TEST

START_TEST (amx_array_shift_right_check)
{
	ck_assert_int_eq (amx_array_shift_right(array1, 15, NULL), -1);
	ck_assert_int_eq (amx_array_shift_right(array1, 0, NULL), 0);

	ck_assert_int_eq (amx_array_shift_right(array1, 1, NULL), 0);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 2);
	ck_assert_int_eq (array1->first_used, 4);
	ck_assert_int_eq (array1->last_used, 7);
	ck_assert_ptr_eq (array1->buffer[4].data, &data[3]);
	ck_assert_ptr_eq (array1->buffer[3].data, NULL);
	ck_assert_ptr_eq (array1->buffer[7].data, &data[6]);
	ck_assert_ptr_eq (array1->buffer[6].data, NULL);
	ck_assert_ptr_eq (array1->buffer[9].data, NULL);

	ck_assert_int_eq (amx_array_shift_right(array1, 3, NULL), 0);
	ck_assert_int_eq(amx_array_capacity(array1), 10);
	ck_assert_int_eq(amx_array_size(array1), 1);
	ck_assert_ptr_eq (array1->buffer[7].data, &data[3]);
	ck_assert_ptr_eq (array1->buffer[4].data, NULL);
	ck_assert_int_eq (array1->first_used, 7);
	ck_assert_int_eq (array1->last_used, 7);

	ck_assert_int_eq (amx_array_shift_right(array1, 3, NULL), 0);
	ck_assert_int_eq(amx_array_capacity(array1), 10);
	ck_assert_int_eq(amx_array_size(array1), 0);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);
}
END_TEST

START_TEST (amx_array_shift_right_all_check)
{
	ck_assert_int_eq (amx_array_shift_right(array1, 10, NULL), 0);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 0);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);
	ck_assert_ptr_eq (array1->buffer[3].data, NULL);
	ck_assert_ptr_eq (array1->buffer[6].data, NULL);
	ck_assert_ptr_eq (array1->buffer[9].data, NULL);
}
END_TEST


START_TEST (amx_array_set_at_null_check)
{
	ck_assert_ptr_eq(amx_array_set_data_at(NULL, 0, NULL), NULL);
}
END_TEST

START_TEST (amx_array_set_at_check)
{
	ck_assert_ptr_eq (amx_array_set_data_at(array1, 2, NULL), &array1->buffer[2]);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 9);
	ck_assert_int_eq (amx_array_is_empty(array1), false);
	ck_assert_int_eq (amx_array_size(array1), 3);

	ck_assert_ptr_eq (amx_array_set_data_at(array1, 9, NULL), &array1->buffer[9]);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (array1->last_used, 6);
	ck_assert_int_eq (amx_array_is_empty(array1), false);
	ck_assert_int_eq (amx_array_size(array1), 2);

	ck_assert_ptr_eq (amx_array_set_data_at(array1, 3, NULL), &array1->buffer[3]);
	ck_assert_int_eq (array1->first_used, 6);
	ck_assert_int_eq (array1->last_used, 6);
	ck_assert_int_eq (amx_array_is_empty(array1), false);
	ck_assert_int_eq (amx_array_size(array1), 1);

	ck_assert_ptr_eq (amx_array_set_data_at(array1, 6, NULL), &array1->buffer[6]);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);
	ck_assert_int_eq (amx_array_is_empty(array1), true);
	ck_assert_int_eq (amx_array_size(array1), 0);

	ck_assert_ptr_eq (amx_array_set_data_at(array1, 7, &data[7]), &array1->buffer[7]);
	ck_assert_int_eq (array1->first_used, 7);
	ck_assert_int_eq (array1->last_used, 7);
	ck_assert_int_eq (amx_array_is_empty(array1), false);
	ck_assert_int_eq (amx_array_size(array1), 1);

	ck_assert_ptr_eq (amx_array_set_data_at(array1, 2, &data[2]), &array1->buffer[2]);
	ck_assert_int_eq (array1->first_used, 2);
	ck_assert_int_eq (array1->last_used, 7);
	ck_assert_int_eq (amx_array_is_empty(array1), false);
	ck_assert_int_eq (amx_array_size(array1), 2);

	ck_assert_ptr_eq (amx_array_set_data_at(array1, 5, &data[5]), &array1->buffer[5]);
	ck_assert_int_eq (array1->first_used, 2);
	ck_assert_int_eq (array1->last_used, 7);
	ck_assert_int_eq (amx_array_is_empty(array1), false);
	ck_assert_int_eq (amx_array_size(array1), 3);

	ck_assert_ptr_eq (amx_array_set_data_at(array1, 10, NULL), NULL);
	ck_assert_ptr_eq (amx_array_set_data_at(array1, 11, NULL), NULL);
}
END_TEST

START_TEST (amx_array_it_set_data_null_check)
{
	ck_assert_int_eq (amx_array_it_set_data(NULL, NULL), -1);
	amx_array_it_t *it = amx_array_get_at(array1, 2);
	ck_assert_int_eq (amx_array_it_set_data(it, NULL), -1);
	ck_assert_int_eq (array1->first_used, 3);
	ck_assert_int_eq (amx_array_grow(array1, 10), 0);
	it = amx_array_get_at(array1, 15);
	ck_assert_int_eq (amx_array_it_set_data(it, NULL), -1);
	ck_assert_int_eq (array1->last_used, 9);
}
END_TEST

START_TEST (amx_array_it_set_data_check)
{
	amx_array_it_t *it = NULL;

	amx_array_clean(array1, NULL);
	amx_array_init(array1, 9);

	it = amx_array_get_at(array1, 0);
	ck_assert_int_eq (amx_array_it_set_data(it, &data[0]), 0);
	ck_assert_int_eq (array1->first_used, 0);

	it = amx_array_get_at(array1, 2);
	ck_assert_int_eq (amx_array_it_set_data(it, &data[2]), 0);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 2);

	ck_assert_int_eq (amx_array_grow(array1, 10), 0);
	it = amx_array_get_at(array1, 15);
	ck_assert_int_eq (amx_array_it_set_data(it, &data[5]), 0);
	ck_assert_int_eq (array1->last_used, 15);
}
END_TEST

START_TEST (amx_array_append_data_null_check)
{
	ck_assert_ptr_eq(amx_array_append_data(NULL, NULL), NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_array_append_data_no_memory_check)
{
	amx_array_t *array = NULL;
	ck_assert_int_ne (amx_array_new(&array, 2), -1);

	amx_array_it_t *it = amx_array_append_data(array, &data[0]);
	ck_assert_ptr_ne(it, NULL);
	ck_assert_int_eq(amx_array_it_index(it), 0);
	it = amx_array_append_data(array, &data[1]);
	ck_assert_ptr_ne(it, NULL);
	ck_assert_int_eq(amx_array_it_index(it), 1);
	ck_assert_int_eq(array->last_used, 1);
	ck_assert_int_eq(array->items, 2);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	it = amx_array_append_data(array, &data[2]);
	ck_mock_reset(malloc);

	ck_assert_ptr_eq(it, NULL);
	ck_assert_int_eq (amx_array_capacity(array), 2);
	ck_assert_int_eq (amx_array_size(array), 2);
	amx_array_delete(&array, NULL);
}
END_TEST
#endif

START_TEST (amx_array_append_data_check)
{
	amx_array_it_t *it = amx_array_append_data(array1, NULL);
	ck_assert_ptr_eq (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 3);
	ck_assert_int_eq (array1->last_used, 9);

	it = amx_array_append_data(array1, &data[0]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 13);
	ck_assert_int_eq (amx_array_size(array1), 4);
	ck_assert_int_eq (array1->last_used, 10);

	it = amx_array_append_data(array1, &data[1]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 13);
	ck_assert_int_eq (amx_array_size(array1), 5);
	ck_assert_int_eq (array1->last_used, 11);
	
	amx_array_clean(array1, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 0);

	it = amx_array_append_data(array1, &data[0]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 3);
	ck_assert_int_eq (amx_array_size(array1), 1);
	ck_assert_int_eq (array1->last_used, 0);
	ck_assert_int_eq (array1->first_used, 0);

	it = amx_array_append_data(array1, &data[1]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 3);
	ck_assert_int_eq (amx_array_size(array1), 2);
	ck_assert_int_eq (array1->last_used, 1);
	ck_assert_int_eq (array1->first_used, 0);
}
END_TEST

START_TEST (amx_array_prepend_data_null_check)
{
	ck_assert_ptr_eq(amx_array_prepend_data(NULL, NULL), NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_array_prepend_data_no_memory_check)
{
	amx_array_t *array = NULL;
	ck_assert_int_ne (amx_array_new(&array, 2), -1);

	amx_array_it_t *it = amx_array_prepend_data(array, &data[0]);
	ck_assert_ptr_ne(it, NULL);
	ck_assert_int_eq(amx_array_it_index(it), 0);
	ck_assert_int_eq(array->first_used, 0);
	ck_assert_int_eq(array->items, 2);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	it = amx_array_prepend_data(array, &data[2]);
	ck_mock_reset(malloc);

	ck_assert_ptr_eq(it, NULL);
	ck_assert_int_eq (amx_array_capacity(array), 2);
	ck_assert_int_eq (amx_array_size(array), 1);
	ck_assert_int_eq(array->first_used, 0);
	ck_assert_int_eq(array->items, 2);
	amx_array_delete(&array, NULL);
}
END_TEST
#endif

START_TEST (amx_array_prepend_data_check)
{
	amx_array_it_t *it = amx_array_prepend_data(array1, NULL);
	ck_assert_ptr_eq (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 3);
	ck_assert_int_eq (array1->first_used, 3);

	it = amx_array_prepend_data(array1, &data[0]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 4);
	ck_assert_int_eq (array1->first_used, 2);

	it = amx_array_prepend_data(array1, &data[1]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 5);
	ck_assert_int_eq (array1->first_used, 1);
	
	amx_array_clean(array1, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 0);

	it = amx_array_prepend_data(array1, &data[0]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 3);
	ck_assert_int_eq (amx_array_size(array1), 1);
	ck_assert_int_eq (array1->last_used, 0);
	ck_assert_int_eq (array1->first_used, 0);

	it = amx_array_prepend_data(array1, &data[1]);
	ck_assert_ptr_ne (it, NULL);
	ck_assert_int_eq (amx_array_capacity(array1), 6);
	ck_assert_int_eq (amx_array_size(array1), 2);
	ck_assert_int_eq (array1->last_used, 3);
	ck_assert_int_eq (array1->first_used, 2);
}
END_TEST

START_TEST (amx_array_take_first_data_null_check)
{
	ck_assert_ptr_eq(amx_array_take_first_data(NULL), NULL);
}
END_TEST

START_TEST (amx_array_take_first_data_check)
{
	char *d = amx_array_take_first_data(array1);
	ck_assert_ptr_eq (d, &data[3]);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 2);
	ck_assert_int_eq (array1->first_used, 6);

	d = amx_array_take_first_data(array1);
	ck_assert_ptr_eq (d, &data[6]);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 1);
	ck_assert_int_eq (array1->first_used, 9);

	d = amx_array_take_first_data(array1);
	ck_assert_ptr_eq (d, &data[9]);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 0);
	ck_assert_int_eq (array1->first_used, 0);
}
END_TEST

START_TEST (amx_array_take_last_data_null_check)
{
	ck_assert_ptr_eq(amx_array_take_last_data(NULL), NULL);
}
END_TEST

START_TEST (amx_array_take_last_data_check)
{
	char *d = amx_array_take_last_data(array1);
	ck_assert_ptr_eq (d, &data[9]);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 2);
	ck_assert_int_eq (array1->last_used, 6);

	d = amx_array_take_last_data(array1);
	ck_assert_ptr_eq (d, &data[6]);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 1);
	ck_assert_int_eq (array1->last_used, 3);

	d = amx_array_take_last_data(array1);
	ck_assert_ptr_eq (d, &data[3]);
	ck_assert_int_eq (amx_array_capacity(array1), 10);
	ck_assert_int_eq (amx_array_size(array1), 0);
	ck_assert_int_eq (array1->first_used, 0);
}
END_TEST

START_TEST (amx_array_it_take_data_null_check)
{

	ck_assert_int_eq(amx_array_it_index(NULL), 0);
}
END_TEST

START_TEST (amx_array_it_take_data_check)
{
	ck_assert_ptr_eq(amx_array_it_take_data(NULL), NULL);
	amx_array_it_t *it = amx_array_get_at(array1, 3);
	ck_assert_ptr_eq(amx_array_it_take_data(it), &data[3]);
	ck_assert_int_eq (array1->first_used, 6);
	ck_assert_int_eq (array1->last_used, 9);

	it = amx_array_get_at(array1, 4);
	ck_assert_ptr_eq(amx_array_it_take_data(it), NULL);
	ck_assert_int_eq (array1->first_used, 6);
	ck_assert_int_eq (array1->last_used, 9);

	it = amx_array_get_at(array1, 9);
	ck_assert_ptr_eq(amx_array_it_take_data(it), &data[9]);
	ck_assert_int_eq (array1->first_used, 6);
	ck_assert_int_eq (array1->last_used, 6);

	it = amx_array_get_at(array1, 6);
	ck_assert_ptr_eq(amx_array_it_take_data(it), &data[6]);
	ck_assert_int_eq (array1->first_used, 0);
	ck_assert_int_eq (array1->last_used, 0);

	ck_assert_int_eq (amx_array_is_empty(array1), true);
}
END_TEST

START_TEST (amx_array_it_index_null_check)
{
	ck_assert_int_eq(amx_array_it_index(NULL), 0);
}
END_TEST

START_TEST (amx_array_it_index_check)
{
	for(unsigned int index = 0; index < amx_array_capacity(array1); index++)
	{
		amx_array_it_t *it = amx_array_get_at(array1, index);
		ck_assert_int_eq (amx_array_it_index(it), index);
	}
}
END_TEST

Suite *amx_array_suite(void)
{
	Suite *s = suite_create ("amx_array");
	TCase *tc = NULL;

	tc = tcase_create ("amx_array_new_init_delete_clean");
	tcase_add_test (tc, amx_array_new_delete_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_array_new_no_memory_check);
#endif
	tcase_add_test (tc, amx_array_new_delete_check);
	tcase_add_test (tc, amx_array_delete_cb_check);
	tcase_add_test (tc, amx_array_init_clean_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_array_init_no_memory_check);
#endif
	tcase_add_test (tc, amx_array_init_clean_check);
	tcase_add_test (tc, amx_array_clean_cb_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_get");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_get_at_null_check);
	tcase_add_test (tc, amx_array_get_at_check);
	tcase_add_test (tc, amx_array_get_first_null_check);
	tcase_add_test (tc, amx_array_get_first_check);
	tcase_add_test (tc, amx_array_get_first_empty_check);
	tcase_add_test (tc, amx_array_get_last_null_check);
	tcase_add_test (tc, amx_array_get_last_check);
	tcase_add_test (tc, amx_array_get_last_empty_check);
	tcase_add_test (tc, amx_array_it_get_next_null_check);
	tcase_add_test (tc, amx_array_it_get_next_check);
	tcase_add_test (tc, amx_array_it_get_previous_null_check);
	tcase_add_test (tc, amx_array_it_get_previous_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_get_data");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_it_get_data_null_check);
	tcase_add_test (tc, amx_array_it_get_data_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_size_capacity");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_is_empty_null_check);
	tcase_add_test (tc, amx_array_is_empty_check);
	tcase_add_test (tc, amx_array_size_null_check);
	tcase_add_test (tc, amx_array_size_check);
	tcase_add_test (tc, amx_array_capacity_null_check);
	tcase_add_test (tc, amx_array_capacity_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_grow_shrink");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_grow_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_array_grow_no_memory_check);
#endif
	tcase_add_test (tc, amx_array_grow_check);
	tcase_add_test (tc, amx_array_shrink_null_check);
	tcase_add_test (tc, amx_array_shrink_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_shift");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_shift_null_check);
	tcase_add_test (tc, amx_array_shift_left_check);
	tcase_add_test (tc, amx_array_shift_left_all_check);
	tcase_add_test (tc, amx_array_shift_right_check);
	tcase_add_test (tc, amx_array_shift_right_all_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_array_set");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_set_at_null_check);
	tcase_add_test (tc, amx_array_set_at_check);
	tcase_add_test (tc, amx_array_it_set_data_null_check);
	tcase_add_test (tc, amx_array_it_set_data_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_append_prepend");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_append_data_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_array_append_data_no_memory_check);
#endif
	tcase_add_test (tc, amx_array_append_data_check);
	tcase_add_test (tc, amx_array_prepend_data_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_array_prepend_data_no_memory_check);
#endif
	tcase_add_test (tc, amx_array_prepend_data_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_take_first_last_it");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_take_first_data_null_check);
	tcase_add_test (tc, amx_array_take_first_data_check);
	tcase_add_test (tc, amx_array_take_last_data_null_check);
	tcase_add_test (tc, amx_array_take_last_data_check);
	tcase_add_test (tc, amx_array_it_take_data_null_check);
	tcase_add_test (tc, amx_array_it_take_data_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_it_index");
	tcase_add_checked_fixture (tc, amx_array_setup, amx_array_teardown);
	tcase_add_test (tc, amx_array_it_index_null_check);
	tcase_add_test (tc, amx_array_it_index_check);
	suite_add_tcase (s, tc);

	return s;
}
